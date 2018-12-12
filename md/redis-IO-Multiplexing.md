# Redis与I/O多路复用

Redis 中 I/O 多路复用（mutiplexing）部分的实现非常干净和优雅，十分利于阅读与分析。

Redis将select、epoll、evport的kqueue封装为不同的子模块，提供给上层统一的接口，以供在不用的操作系统使用，如下图：

![](https://upload-images.jianshu.io/upload_images/4440914-b74ff67024941062.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

接口：

```C
static int  aeApiCreate(aeEventLoop *eventLoop)  
static int  aeApiResize(aeEventLoop *eventLoop, int setsize)
static void aeApiFree(aeEventLoop *eventLoop)
static int  aeApiAddEvent(aeEventLoop *eventLoop, int fd, int mask)
static void aeApiDelEvent(aeEventLoop *eventLoop, int fd, int mask)
static int  aeApiPoll(aeEventLoop *eventLoop, struct timeval *tvp)
```

在每一个子模块内部通过aeApiState 来存储需要的上下文信息：
 
select中：

```
typedef struct aeApiState {
    fd_set rfds, wfds;
    /* We need to have a copy of the fd sets as it's not safe to reuse
     * FD sets after select(). */
    fd_set _rfds, _wfds;
} aeApiState;

```

epoll中：

```C
typedef struct aeApiState {
    int epfd;
    struct epoll_event *events;
} aeApiState;
```

evport中：
```C
typedef struct aeApiState {
    int     portfd;                             /* event port */
    int     npending;                           /* # of pending fds */
    int     pending_fds[MAX_EVENT_BATCHSZ];     /* pending fds */
    int     pending_masks[MAX_EVENT_BATCHSZ];   /* pending fds' masks */
} aeApiState;
```

kqueue中：
```C
typedef struct aeApiState {
    int kqfd;
    struct kevent *events;
} aeApiState;
```

这些上下文信息会存储在 eventLoop 的 `void *state` 中，不会暴露到上层，只在当前子模块中使用。

## select 

首先在aeApiCreate函数中，初始化 rfds，wfds，并存入 eventLoop中state:

```C
static int aeApiCreate(aeEventLoop *eventLoop) {
    aeApiState *state = zmalloc(sizeof(aeApiState));

    if (!state) return -1;
    FD_ZERO(&state->rfds);
    FD_ZERO(&state->wfds);
    eventLoop->apidata = state;
    return 0;
}
```

aeApiAddEvent 和 aeApiDelEvent 会通过 FD_SET 和 FD_CLR 修改 fd_set 中对应 FD 的标志位。

aeApiAddEvent对每一文件描述符加入要监控的事件（可读或可写）：

```C
static int aeApiAddEvent(aeEventLoop *eventLoop, int fd, int mask){
    aeApiState *state = eventLoop->apidata;
    if (mask & AE_READABLE) FD_SET(fd,&state->rfds);
    if (mask & AE_WRITABLE) FD_SET(fd,&state->wfds);
    return 0;
}
```

aeApiAddEvent对每一文件描述符取消要监控的事件（可读或可写）：

```C
static void aeApiDelEvent(aeEventLoop *eventLoop, int fd, int mask){
    aeApiState *state = eventLoop->apidata;

    if (mask & AE_READABLE) FD_CLR(fd,&state->rfds);
    if (mask & AE_WRITABLE) FD_CLR(fd,&state->wfds);
}
```

aeApiPoll封装了select函数，获取就绪的事件，加入eventLoop->fired，并返回事件的个数：

```C
static int aeApiPoll(aeEventLoop *eventLoop, struct timeval *tvp) {
    aeApiState *state = eventLoop->apidata;
    int retval, j, numevents = 0;
    
    // 拷贝fd集合
    memcpy(&state->_rfds,&state->rfds,sizeof(fd_set));
    memcpy(&state->_wfds,&state->wfds,sizeof(fd_set));
    
    // 调用select函数
    retval = select(eventLoop->maxfd+1,
                &state->_rfds,&state->_wfds,NULL,tvp);
                
    if (retval > 0) {
        // 遍历检查  eventLoop中的事件，查看是否就绪
        for (j = 0; j <= eventLoop->maxfd; j++) {
            int mask = 0;
            aeFileEvent *fe = &eventLoop->events[j];

            if (fe->mask == AE_NONE) continue;
            if (fe->mask & AE_READABLE && FD_ISSET(j,&state->_rfds))
                mask |= AE_READABLE;
            if (fe->mask & AE_WRITABLE && FD_ISSET(j,&state->_wfds))
                mask |= AE_WRITABLE;
            // fired中存放就绪的fd
            eventLoop->fired[numevents].fd = j;
            // 修改掩码 
            eventLoop->fired[numevents].mask = mask;
            numevents++;
        }
    }
    return numevents;
}
```

## epoll

aeApiCreate函数，调用epoll_create，生成epoll描述符，初始化events，都存入 eventLoop中state：

```C
static int aeApiCreate(aeEventLoop *eventLoop) {
    aeApiState *state = zmalloc(sizeof(aeApiState));

    if (!state) return -1;
    state->events = zmalloc(sizeof(struct epoll_event)*eventLoop->setsize);
    if (!state->events) {
        zfree(state);
        return -1;
    }
    state->epfd = epoll_create(1024); /* 1024 is just a hint for the kernel */
    if (state->epfd == -1) {
        zfree(state->events);
        zfree(state);
        return -1;
    }
    eventLoop->apidata = state;
    return 0;
}
```

aeApiAddEvent函数通过调用epoll_ctl函数，加入要监控的描述符及其事件：

```C
static int aeApiAddEvent(aeEventLoop *eventLoop, int fd, int mask) {
    aeApiState *state = eventLoop->apidata;
    struct epoll_event ee = {0}; /* avoid valgrind warning */
    /* If the fd was already monitored for some event, we need a MOD
     * operation. Otherwise we need an ADD operation. */
    // 如果这个描述符号已经被修改过了，使用EPOLL_CTL_MOD(修改已经注册的fd的监控事件)，否则使用EPOLL_CTL_ADD加入
    int op = eventLoop->events[fd].mask == AE_NONE ?
            EPOLL_CTL_ADD : EPOLL_CTL_MOD;

    ee.events = 0;
    mask |= eventLoop->events[fd].mask; /* Merge old events */
    // 将mask转化为epoll对应的掩码，以此设置要监控的事件
    if (mask & AE_READABLE) ee.events |= EPOLLIN;
    if (mask & AE_WRITABLE) ee.events |= EPOLLOUT;
    ee.data.fd = fd;
    // 加入要监控的描述符及其事件
    if (epoll_ctl(state->epfd,op,fd,&ee) == -1) return -1;
    return 0;
}
```

aeApiDelEvent函数通过调用epoll_ctl函数，取消要描述符要监控的事件：

```C
static void aeApiDelEvent(aeEventLoop *eventLoop, int fd, int delmask) {
    aeApiState *state = eventLoop->apidata;
    struct epoll_event ee = {0}; /* avoid valgrind warning */
    int mask = eventLoop->events[fd].mask & (~delmask);

    ee.events = 0;
    if (mask & AE_READABLE) ee.events |= EPOLLIN;
    if (mask & AE_WRITABLE) ee.events |= EPOLLOUT;
    ee.data.fd = fd;
    if (mask != AE_NONE) {
        // 修改已经注册的fd的监控事件
        epoll_ctl(state->epfd,EPOLL_CTL_MOD,fd,&ee);
    } else {
        /* Note, Kernel < 2.6.9 requires a non null event pointer even for
         * EPOLL_CTL_DEL. */
        // 从epfd删除这个fd
        epoll_ctl(state->epfd,EPOLL_CTL_DEL,fd,&ee);
    }
}
```

aeApiPoll调用epoll_wait，获取已经就绪的事件的链表（无需遍历），加入
eventLoop->fired，并返回个数：

```C
static int aeApiPoll(aeEventLoop *eventLoop, struct timeval *tvp) {
    aeApiState *state = eventLoop->apidata;
    int retval, numevents = 0;

    
    retval = epoll_wait(state->epfd,state->events,eventLoop->setsize,tvp ? (tvp->tv_sec*1000 + tvp->tv_usec/1000) : -1);
    // 就绪链表为state->events
    // timeout参数为负数表示无限超时，是epoll_wait挂起直到有一个指定事件发生
    if (retval > 0) {
        int j;
        
        numevents = retval;
        for (j = 0; j < numevents; j++) {
            int mask = 0;
            struct epoll_event *e = state->events+j;
            // 根据就绪状态，修改掩码 
            if (e->events & EPOLLIN) mask |= AE_READABLE;
            if (e->events & EPOLLOUT) mask |= AE_WRITABLE;
            if (e->events & EPOLLERR) mask |= AE_WRITABLE;
            if (e->events & EPOLLHUP) mask |= AE_WRITABLE;
            // 加入eventLoop->fired
            eventLoop->fired[j].fd = e->data.fd;
            eventLoop->fired[j].mask = mask;
        }
    }
    return numevents;
}
```
