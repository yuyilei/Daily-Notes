# Redis zset 

zset(有序集合) 是redis 中的一种数据结构，一方面它是一个set，保证了内部 value 的唯一性，另一方面它可以给每个 value 赋予一个 score，代表这个 value 的排序权重。 

zset 的这两种特性，通过两种结构实现，用 hashMap 实现 它的唯一性，用 sikplist 实现 随着 score排序的功能，因此，zset 是一个复合结构。 

hashMap 的实现与 哈希表一致，rehash的方式也一样，都是渐进式rehash，不多说了

## skiplist 

SkipList（跳跃表）是一种支持快速查找的数据结构，插入、查找和删除操作都仅仅只需要O(log n)对数级别的时间复杂度，它的效率甚至可以与红黑树等二叉平衡树相提并论，而且实现的难度要比红黑树简单得多。 

![](https://upload-images.jianshu.io/upload_images/4440914-84aa104bfe6e04a9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

SkipList 以空间换取时间，由多层的链表组成，每一层都是下一层的快速通道。一个skiplist的 组成如下：

[完整代码](https://github.com/yuyilei/Daily-Notes/blob/master/C%2B%2B/SkipList.cpp) 

```C++ 
// 单链表  
struct Node {
    int key; 
    int value;
    int level; 
    struct Node **next;    // 此节点在后面一层上的后即节点，是个数组  
}; 

class SkipList {
    private:
        int max_level;
        struct Node* head; 
        struct Node* tail; 
    public: 
        SkipList(int l); 
        ~SkipList(); 
        bool insertNode(int key, int value);     // 插入节点
        bool deleteNode(int key);                // 删除节点 
        struct Node* getNode(int key);           // 查找节点   
}; 
```

zset 中，skip list是个双向链表，有向前的指针，也由向后的指针:

```C++
typedef struct zskiplistNode {
    sds ele;
    double score;
    struct zskiplistNode *backward;
    struct zskiplistLevel {
        struct zskiplistNode *forward;
        unsigned long span;                 // 用于计算排名
        // span 表示 从前一个节点沿着当前层的 forward 指针跳到当前这个节点中间会跳过多少个节点
        // 要计算一个元素的排名时，只需要将「搜索路径」上的经过的所有节点的跨度 span 值进行叠加就可以算出元素的最终 rank 值 
    } level[];  
} zskiplistNode;

typedef struct zskiplist {
    struct zskiplistNode *header, *tail;
    unsigned long length;
    int level;
} zskiplist;

// zset 由 skip list 和 hash map组成 
typedef struct zset {
    dict *dict;
    zskiplist *zsl;
} zset;

```

首先是插入节点，插入节点分为三步:

1. 寻找每层上的前驱节点 

2. 制作插入的节点 

3. 随机一个层数，在相应的层数中插入节点

```C++
/* Insert a new node in the skiplist. Assumes the element does not already
 * exist (up to the caller to enforce that). The skiplist takes ownership
 * of the passed SDS string 'ele'. */
zskiplistNode *zslInsert(zskiplist *zsl, double score, sds ele) {
    zskiplistNode *update[ZSKIPLIST_MAXLEVEL], *x;   // 前驱节点的数组 
    unsigned int rank[ZSKIPLIST_MAXLEVEL];    // 计算排名 
    int i, level;

    serverAssert(!isnan(score));          
    x = zsl->header;
    //  顶层开始，逐级下降寻找，得到搜索路径，也就是前驱节点
    // 时间复杂度 为 O(log(n))
    for (i = zsl->level-1; i >= 0; i--) {
        /* store rank that is crossed to reach the insert position */
        rank[i] = i == (zsl->level-1) ? 0 : rank[i+1];
        while (x->level[i].forward &&   // 还有后继节点 
                (x->level[i].forward->score < score ||           // score大于后继节点的score
                    (x->level[i].forward->score == score &&      // score相等时，比较value 
                    sdscmp(x->level[i].forward->ele,ele) < 0)))
        {
            rank[i] += x->level[i].span;                         // 跳过此层时，加上中间的节点数 
            x = x->level[i].forward;                             // 跳到下一层的 forward 
        }
        update[i] = x;                                           // 得到搜索路径，也就是前驱节点
    }
    /* we assume the element is not already inside, since we allow duplicated
     * scores, reinserting the same element should never happen since the
     * caller of zslInsert() should test in the hash table if the element is
     * already inside or not. */
    // 生成一个随机层数 
    level = zslRandomLevel();
    // 如果是一个没有出现过的层数，要初始化 之前最大层到 这一层的信息 
    if (level > zsl->level) {
        for (i = zsl->level; i < level; i++) {
            rank[i] = 0;                         
            update[i] = zsl->header;                   // 因为是第一个插入的节点，所以前驱节点是 header 
            update[i]->level[i].span = zsl->length;       
        }
        // 更新当前最大的层数
        zsl->level = level;
    }
    
    // 制作节点 
    x = zslCreateNode(level,score,ele);
    // 在搜索路径中插入节点 
    for (i = 0; i < level; i++) {
        x->level[i].forward = update[i]->level[i].forward;
        update[i]->level[i].forward = x;

        /* update span covered by update[i] as x is inserted here */
        x->level[i].span = update[i]->level[i].span - (rank[0] - rank[i]);
        update[i]->level[i].span = (rank[0] - rank[i]) + 1;
    }

    /* increment span for untouched levels */
    // 改变插入层的跨度 
    for (i = level; i < zsl->level; i++) {
        update[i]->level[i].span++;
    }
    
    // backward是 回溯节点，因为这是双向链表   
    // update[0] == zsl->header 表示 插入在头部，回溯节点为 NULL 
    x->backward = (update[0] == zsl->header) ? NULL : update[0]; 
    // 是否为底层链表的最后一个节点 
    if (x->level[0].forward)
        x->level[0].forward->backward = x;
    else
        // 如果是底层链表的最后一个节点，设置此节点为 tail 
        zsl->tail = x;
    zsl->length++;
    return x;
}
```

删除单个节点：


```C++
int zslDelete(zskiplist *zsl, double score, sds ele, zskiplistNode **node) {
    zskiplistNode *update[ZSKIPLIST_MAXLEVEL], *x;
    int i;

    x = zsl->header;
    // 顶层开始，逐级下降寻找，得到搜索路径，也就是前驱节点 
    for (i = zsl->level-1; i >= 0; i--) {
        while (x->level[i].forward &&
                (x->level[i].forward->score < score ||
                    (x->level[i].forward->score == score &&
                     sdscmp(x->level[i].forward->ele,ele) < 0)))
        {
            x = x->level[i].forward;
        }
        update[i] = x;
    }
    /* We may have multiple elements with the same score, what we need
     * is to find the element with both the right score and object. */
    x = x->level[0].forward;
    // 检查节点的 score和元素内容是否是要删除的
    if (x && score == x->score && sdscmp(x->ele,ele) == 0) {
        zslDeleteNode(zsl, x, update);
        // 是否要返回 被删除的节点 
        if (!node)
            zslFreeNode(x);
        else
            *node = x;
        return 1;
    }
    return 0; /* not found */
}

// 在搜索路径中删除节点，被其他删除函数zslDelete, zslDeleteByScore and zslDeleteByRank调用 
void zslDeleteNode(zskiplist *zsl, zskiplistNode *x, zskiplistNode **update) {
    int i;
    for (i = 0; i < zsl->level; i++) {
        if (update[i]->level[i].forward == x) {
            update[i]->level[i].span += x->level[i].span - 1;
            update[i]->level[i].forward = x->level[i].forward;
        } else {
            update[i]->level[i].span -= 1;
        }
    }
    if (x->level[0].forward) {
        x->level[0].forward->backward = x->backward;
    } else {
        zsl->tail = x->backward;
    }
    while(zsl->level > 1 && zsl->header->level[zsl->level-1].forward == NULL)
        zsl->level--;
    zsl->length--;
}


```

同时，skiplist还能删除一定 score 范围内的所有节点  

```C++
/* Delete all the elements with rank between start and end from the skiplist.
 * Start and end are inclusive. Note that start and end need to be 1-based */
unsigned long zslDeleteRangeByRank(zskiplist *zsl, unsigned int start, unsigned int end, dict *dict) {
    zskiplistNode *update[ZSKIPLIST_MAXLEVEL], *x;
    unsigned long traversed = 0, removed = 0;
    int i;

    x = zsl->header;
    // 逐层查找搜索路径  
    // 找出第一个score大于等于 start的节点 
    for (i = zsl->level-1; i >= 0; i--) {
        while (x->level[i].forward && (traversed + x->level[i].span) < start) {
            traversed += x->level[i].span;
            x = x->level[i].forward;
        }
        update[i] = x;
    }

    traversed++;   // 下一层后继节点的 score 
    x = x->level[0].forward;
    // 删除此范围内的 节点 
    while (x && traversed <= end) {
        zskiplistNode *next = x->level[0].forward;
        zslDeleteNode(zsl,x,update);
        dictDelete(dict,x->ele);
        zslFreeNode(x);
        removed++;
        traversed++;
        x = next;
    }
    return removed;
}

```

