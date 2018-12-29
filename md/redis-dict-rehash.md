# redis dict rehash 

哈希表是redis的一种数据结构。redis中哈希表是通过dict实现的; redis里面 set 的结构底层实现也是dict，只不过所有的 value 都是 NULL。

dict是一种用于保存键值对的抽象数据结构。 

```
set s "v1"                          // set 
hset student name "haha"            // hashtable 
```

## dict 结构 

dict是一个结构体，其中最关键的是两个hashtable：

```C++
typedef struct dict {
    dictType *type;                 
    void *privdata;
    dictht ht[2];                // 两个hashtable，ht[0]是正在使用的，ht[1]会在rehash时临时使用的 
    long rehashidx; /* rehashing not in progress if rehashidx == -1 */      // 正在被rehash的位置，为-1表示不在rehash
    unsigned long iterators; /* number of iterators currently running */    // 正在运行的 iterator 的数量  
} dict;
```

hashtable的结构：

```C++
typedef struct dictht {
    dictEntry **table;           // 存储节点的数组 
    unsigned long size;          // 大小
    unsigned long sizemask;      // 掩码，一般为size-1，求索引值时要 % sizemask 
    unsigned long used;          // 已经使用的节点的数量 
} dictht;
```

hashtable节点的结构：
```C++
typedef struct dictEntry {
    void *key;
    union {               // 用一个联合体存value
        void *val;        // 自定义类型 
        uint64_t u64;     // 无符号整形 
        int64_t s64;      // 有符号整形
        double d;         // 浮点型 
    } v;
    struct dictEntry *next;   // 只想下一个节点的指针 
} dictEntry;
```

可得，dict的结构如图：

![](https://upload-images.jianshu.io/upload_images/4440914-bb86374a5f2e15d6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240) 

## rehash 

当哈希表保存的键值对太多或者太少时，redis对哈希表大小进行相应的扩展和收缩，称为rehash（重新散列）。 

当哈希表存的元素越来越多时，需要扩容；反之，需要收缩。 

rehash的条件：

`hashtable元素总个数 / 字典的链个数 = 每个链平均存储的元素个数(load_factor)` 

1. load_factor 超过1，就表示哈希表中元素过多，可能需要扩容，一般：
    
    a. 如果redis正在做bgsave，为了减少内存页的过多分离 (Copy On Write)，load_factor 大于 5 时才会强制扩容；

    b. 如果redis没有正在执行bgsave，load_factor >= 1 时会引发扩容； 

2. load_factor < 0.1时，会引发 收缩  

rehash步骤如下：

1. 确定 为ht[1] 分配空间

    a. 扩容时，ht[1]的大小为第一个大于 2*h[0].used 的 2的n次幂； 

    b. 收缩时，ht[1]的大小为第一个大于 h[0].used 的 2的n次幂；  

2. 设置 rehashidx = 0，rehashidx相当于 rehash 计数器，记录rehash的进度

3. 将ht[0]中的所有 dictEntry 节点 rehash到 ht[1]中(重新计算的key的哈希值和对应的索引，放入ht[1]对应的位置上)，每次 rehashidx加一

4. 全部节点rehash完成后，ht[0] = ht[1], ht[1] = NULL, rehashidx = -1


redis的rehash代码：

```C++
// 返回1，表示rehash还没完成
// 返回0，表示rehash全部完成 
int dictRehash(dict *d, int n) {
    // rehash n 步 
    // 设置最大访问空bucket的数量，防止一直访问到空bucket，此函数一直不返回，阻塞了其他
    int empty_visits = n*10; /* Max number of empty buckets to visit. */
    // 是否正在rehash 
    if (!dictIsRehashing(d)) return 0;

    while(n-- && d->ht[0].used != 0) {
        dictEntry *de, *nextde;

        /* Note that rehashidx can't overflow as we are sure there are more
         * elements because ht[0].used != 0 */
        assert(d->ht[0].size > (unsigned long)d->rehashidx);
        // 跳过空bucket
        while(d->ht[0].table[d->rehashidx] == NULL) {
            d->rehashidx++;
            // 访问的空bucket超过最大值，直接返回  
            if (--empty_visits == 0) return 1;
        }
        // de 为非空的bucket 
        de = d->ht[0].table[d->rehashidx];
        
        /* Move all the keys in this bucket from the old to the new hash HT */
        // 将这个bucket中的所有dictEntry rehash到ht[1]
        while(de) {
            uint64_t h;

            nextde = de->next;
            /* Get the index in the new hash table */ 
            // 对key重新计算哈希值，从而得到在哈希表中的索引 
            h = dictHashKey(d, de->key) & d->ht[1].sizemask;
            // 放入ht[1]相应bucket的头部 
            de->next = d->ht[1].table[h];
            d->ht[1].table[h] = de;
            d->ht[0].used--;
            d->ht[1].used++;
            de = nextde;
        }
        // 在ht[0]中删除这个bucket 
        d->ht[0].table[d->rehashidx] = NULL;
        // rehash计数器加一 
        d->rehashidx++;
    }

    /* Check if we already rehashed the whole table... */
    // 检查rehash是否全部完成 
    if (d->ht[0].used == 0) {
        // ht[0] = ht[1], ht[1] = NULL, rehashidx = -1 
        zfree(d->ht[0].table);
        d->ht[0] = d->ht[1];
        _dictReset(&d->ht[1]);
        d->rehashidx = -1;
        return 0;
    }
    
    // 返回1，表示rehash还没完成
    // 返回0，表示rehash全部完成 
    /* More to rehash... */
    return 1;
}

```


## 渐进式rehash 

当 需要rehash的节点较多时，rehash耗时较久，由于redis是单线程，就会阻塞，所以rehash不是一次性完成的，redis采用的是 渐进式rehash。

`int dictRehash(dict *d, int n)`函数的第二个参数 n 表示 步长，每次调用 dictRehash 函数，就会rehash一定数量的节点，多次调用知道rehash全部完成。 这就是渐进式rehash。 

在渐进式rehash时，所有对字典的操作，包括：添加、查找、更新等等，程序除了执行指定的操作之外，还会将ht[0]哈希表索引的所有键值对rehash到ht[1]。

往dict中添加元素的代码：


```C++
dictEntry *dictAddRaw(dict *d, void *key, dictEntry **existing)
{
    long index;
    dictEntry *entry;
    dictht *ht;
    
    // 如果正在rehash，就往下rehash一个步长 
    if (dictIsRehashing(d)) _dictRehashStep(d);

    /* Get the index of the new element, or -1 if
     * the element already exists. */
    // 获取新元素的下标，如果已经存在，返回-1
    if ((index = _dictKeyIndex(d, key, dictHashKey(d,key), existing)) == -1)
        return NULL;

    /* Allocate the memory and store the new entry.
     * Insert the element in top, with the assumption that in a database
     * system it is more likely that recently added entries are accessed
     * more frequently. */
    // 如果正在rehash，ht就是 ht[1]，否则 ht是 ht[0] 
    ht = dictIsRehashing(d) ? &d->ht[1] : &d->ht[0];
    // 分配空间 
    entry = zmalloc(sizeof(*entry));
    // 放入bucket头部，因为接下来更容易访问新加入的元素  
    entry->next = ht->table[index];
    ht->table[index] = entry;
    ht->used++;

    /* Set the hash entry fields. */
    dictSetKey(d, entry, key); 
    return entry;
}

/*
返回能放 新元素的索引，如果元素已经存在，返回-1 
注意，正在rehash时，返回ht[1]中的索引，否则，返回t[1]中的索引;
*/
static long _dictKeyIndex(dict *d, const void *key, uint64_t hash, dictEntry **existing)
{
    unsigned long idx, table;
    dictEntry *he;
    if (existing) *existing = NULL;

    /* Expand the hash table if needed */
    // 是否需要扩容 
    if (_dictExpandIfNeeded(d) == DICT_ERR)
        return -1;
    // ht[0]和 ht[1]
    for (table = 0; table <= 1; table++) {
        // 计算索引 
        idx = hash & d->ht[table].sizemask;
        /* Search if this slot does not already contain the given key */
        he = d->ht[table].table[idx];
        // 检查元素是否已经存在 
        while(he) {
            if (key==he->key || dictCompareKeys(d, key, he->key)) {
                if (existing) *existing = he;
                return -1;
            }
            he = he->next;
        }
        // 如果不是正在rehash，就跳出循环，不计算ht[1]
        if (!dictIsRehashing(d)) break;
    }
    return idx;
}

```

渐进式的 rehash 避免了集中式rehash带来的庞大计算量和内存操作，但是需要注意的是redis在进行rehash的时候，正常的访问请求可能需要做多要访问两次hashtable（ht[0]， ht[1]），例如键值被rehash到新ht[1]，则需要先访问ht[0]，如果ht[0]中找不到，则去ht[1]中找。
 



