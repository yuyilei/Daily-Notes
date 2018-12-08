# LRU缓存机制 

LRU是Least Recently Used的缩写，即最近最少使用，常用于页面置换算法，是为虚拟页式存储管理服务的。 

要求设计一个LRU缓存机制，它应该支持以下操作： 获取数据 get 和 写入数据 put，要求在O(1)的时间复杂度下完成 get 和 put 操作。

获取数据 get(key) - 如果密钥 (key) 存在于缓存中，则获取密钥的值（总是正数），否则返回 -1。

写入数据 put(key, value) - 如果密钥不存在，则写入其数据值。当缓存容量达到上限时，它应该在写入新数据之前删除最近最少使用的数据值，从而为新的数据值留出空间。

## LRU 实现

最常见的是用一个链表实现LRU：

![](https://upload-images.jianshu.io/upload_images/4440914-4235ff4098b37551.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240) 

当一个数据到来时：

1）如果是新数据，则插在链表头部；
2）如果缓存命中，则将数据移动到链表头部；
3）链表中的数据超出一定限额时，删除链表尾部的数据；

问题的关键是如何使操作的时间复杂度为O(1)。链表不能随机访问，如果要访问一个节点，要通过O(n)的遍历，所以增加一个哈希表，哈希表的键是 key，值是 key,value 组成的节点（位于链表中）。 这样，就能通过 key 以O(1)的时间复杂度随机访问这个节点。 

代码：

```c++
class LRUCache {
public:
    LRUCache(int capacity) {
        cap = capacity; 
    }
    
    int get(int key) {
        auto now = m.find(key);
        if ( now == m.end() ) 
            return -1;
        // now->second是对应的list节点（迭代器）
        int res = now->second->second;  
        cache.erase(now->second);
        cache.push_front(make_pair(key,res));
        now->second = cache.begin();
        return res;
    }
    
    void put(int key, int value) {
        auto now = m.find(key);
        if ( now != m.end() ) {
            // 哈希表中有，把这个节点移动到头部
            cache.erase(now->second);
            cache.push_front(make_pair(key,value));
            m[key] = cache.begin();
        }
        else if ( cache.size() < cap ) {
            // 哈希表中没有，且链表还没满 ，就把这个节点插入到头部
            cache.push_front(make_pair(key,value));
            m[key] = cache.begin();
        }
        else {
            // 哈希表中没有，且链表满了 ，就移除链表尾部元素，把这个节点插入到头部
            auto tail = cache.end();
            tail--;
            m.erase(tail->first);
            cache.erase(tail);
            cache.push_front(make_pair(key,value));
            m[key] = cache.begin();
        }
    }
private:
    map<int, list<pair<int,int>>::iterator> m;
    list<pair<int,int>> cache;
    int cap;
};
```

list是C++中STL实现的双向链表，以O(1)的时间复杂度插入、删除节点。 

pair是C++中将2个数据组合成一组数据，相当于一个结构体，有两个元素，可以first和second访问其中元素。



## Redis中的LRU

Redis为了节省内存使用，和通常的LRU算法实现不太一样，Redis使用了采样的方法来模拟一个近似LRU算法。

如果使用 双向链表 + 哈希表 来实现 LRU，当key数量比较多时，会比较消耗内存。Redis对每个key增加一个24bit的时钟，作为淘汰的依据，以此节省内存。

Redis server中保存 server启动之后的信息，表包括全局时钟lruclock：

(以下都是Redis源码)

```c++
struct redisServer {
       ...
       unsigned lruclock:LRU_BITS; /* Clock for LRU eviction */
       ...
};
```

通过getLRUClock函数获得当前的 LRUCLock：

```c++
#define LRU_CLOCK_RESOLUTION 1000 /* LRU clock resolution in ms */
#define LRU_BITS 24
#define LRU_CLOCK_MAX ((1<<LRU_BITS)-1) /* Max value of obj->lru */
/* Return the LRU clock, based on the clock resolution. This is a time
 * in a reduced-bits format that can be used to set and check the
 * object->lru field of redisObject structures. */

  unsigned int getLRUClock(void) {
        return (mstime()/LRU_CLOCK_RESOLUTION) & LRU_CLOCK_MAX;
  }
```

可以看出getLRUClock函数是获得当前系统的unix时间戳的后24位。

Redis Server每隔一段时间（100ms左右），调用updateLRUClock函数（类似于getLRUClock）更新自己的lruclock。lrulock最大能到`(2**24-1)/3600/24` = 194天，超过这个时间，lruclock会从头开始。

同时，每个Redis Object中也包含它自己的lruclock:

```
typedef struct redisObject {
    ...
    unsigned lru:LRU_BITS; /* LRU time (relative to server.lruclock)*/
    ...
```

创建这个Object的时候，会初始化它的lru，之后每次访问这个Object，都会更新它的lru。这样在一段时间内（194天内），一个Object的lru越小，就表示它越久没被访问过。

如果一个Object很久很久（超过194天）没被访问过，那么它的lru就会比Redis Server的lruclock大，这个时候需要重新计算时间：

```C++
#define LRU_CLOCK_MAX ((1<<LRU_BITS)-1) /* Max value of obj->lru */
/* Given an object returns the min number of milliseconds the object was never
 * requested, using an approximated LRU algorithm. */
// 返回 Redis Object的lru 与 Redis Server的lruclock 的差值
unsigned long long estimateObjectIdleTime(robj *o) {
    unsigned long long lruclock = LRU_CLOCK();
    if (lruclock >= o->lru) {
        // Redis Object的lru <= Redis Server的lruclock 
        return (lruclock - o->lru) * LRU_CLOCK_RESOLUTION;
    } 
    else {
        // Redis Object的lru > Redis Server的lruclock 
        // 需要为Redis Server的lruclock 增加一个偏移量LRU_CLOCK_MAX
        return (lruclock + (LRU_CLOCK_MAX - o->lru)) * LRU_CLOCK_RESOLUTION;
    }
}
```

Redis支持的淘汰策略比较多:

1. noeviction 不会继续服务写请求 (DEL 请求可以继续服务)，读请求可以继续进行。这是默认的淘汰策略。

2. volatile-lru 尝试淘汰设置了过期时间的 key，最少使用的 key 优先被淘汰。这样可以保证需要持久化的数据不会突然丢失。

3. volatile-ttl 尝试淘汰设置了过期时间的 key，淘汰的策略是 key 的剩余寿命 ttl 的值，ttl 越小越优先被淘汰。

4. volatile-random 尝试淘汰设置了过期时间的 key，淘汰的 key 是过期 key 集合中随机的 key。

5. allkeys-lru 区别于 volatile-lru，这个策略要淘汰的 key 对象是全体的 key 集合，而不只是过期的 key 集合。

6. allkeys-random 区别于 volatile-random，这个策略要淘汰的 key 对象是全体的 key 集合中随机的key，而不只是过期的 key 集合。

涉及LRU的有：

1. volatile-lru 设置了过期时间的key参与近似的lru淘汰策略

2. allkeys-lru 所有的key均参与近似的lru淘汰策略

实现：

```
        if (server.maxmemory_policy & (MAXMEMORY_FLAG_LRU|MAXMEMORY_FLAG_LFU) ||
            server.maxmemory_policy == MAXMEMORY_VOLATILE_TTL)
        {
            // 存放可以被淘汰的key 
            struct evictionPoolEntry *pool = EvictionPoolLRU;
        
            while(bestkey == NULL) {
                unsigned long total_keys = 0, keys;

                /* We don't want to make local-db choices when expiring keys,
                 * so to start populate the eviction pool sampling keys from
                 * every DB. */
                // 遍历server中的每个db 
                for (i = 0; i < server.dbnum; i++) {
                    db = server.db+i;
                    // 根据MAXMEMORY_FLAG_ALLKEYS确定是从全部 key 里面选还是 从设置了过期时间 的key中选  
                    dict = (server.maxmemory_policy & MAXMEMORY_FLAG_ALLKEYS) ?
                            db->dict : db->expires;
                    if ((keys = dictSize(dict)) != 0) {
                        // 加入 Pool 
                        // 升序 
                        evictionPoolPopulate(i, dict, db->dict, pool);
                        total_keys += keys;
                    }
                }
                // 没有key 
                if (!total_keys) break; /* No keys to evict. */

                /* Go backward from best to worst element to evict. */
                // 从POOL中的最后一个元素开始遍历 Pool 
                for (k = EVPOOL_SIZE-1; k >= 0; k--) {
                    if (pool[k].key == NULL) continue;
                    bestdbid = pool[k].dbid;
                    
                    // 检查key是否存在相应的db中
                    if (server.maxmemory_policy & MAXMEMORY_FLAG_ALLKEYS) {
                        de = dictFind(server.db[pool[k].dbid].dict,
                            pool[k].key);
                    } else {
                        de = dictFind(server.db[pool[k].dbid].expires,
                            pool[k].key);
                    }
                    
                    // 从pool中删除 
                    /* Remove the entry from the pool. */
                    if (pool[k].key != pool[k].cached)
                        sdsfree(pool[k].key);
                    pool[k].key = NULL;
                    pool[k].idle = 0;

                    /* If the key exists, is our pick. Otherwise it is
                     * a ghost and we need to try the next element. */
                    if (de) {
                        // bestkey 就是被删除的元素 
                        // 找到bestkey 就跳出循环 
                        bestkey = dictGetKey(de);
                        break;
                    } else {
                        /* Ghost... Iterate again. */
                    }
                }
            }
        }

```


其中, evictionPoolPopulate函数在这里的作用是根据server.maxmemory_samples配置随机选取固定数目的key，
然后比较它们的lru访问时间，排序放入Pool中，maxmemory_samples的值越大，Pool中的元素越多，Redis的近似LRU算法就越接近于严格LRU算法，但是相应消耗也变高，对性能有一定影响，样本值默认为5。

