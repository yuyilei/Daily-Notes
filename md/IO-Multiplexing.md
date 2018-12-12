# select、poll、epoll 

I/O多路复用是指使用一个线程检查多个描述符的就绪状态，比如调用select函数，传入多个描述符，如果有一个或多个描述符准备就绪（一个或多个I/O事件发生后）就返回，否则就一直阻塞直到超时。

Linux中基于socket的通信本质就是一种I

O，使用socket函数创建的socket默认都是阻塞的，这就意味着，当socket API调用不能立即完成时，线程一直处于等待状态，直到操作完成得到结果或者操作超时出错。会引起阻塞的socket API有4中：

1. 输入操作
2. 输出操作
3. 接受连接（accept）等待对方接受连接请求，如果没有接受，线程就会进入睡眠
4. 发起连接（connect）在收到服务端的应答前不会返回。 

Linux支持I/O多路复用的系统调用有select、poll、epoll，这些调用都是内核级别的。

## select 

Linux提供的select相关函数接口如下：

```C
int select(int max_fd, fd_set *readset, fd_set *writeset, fd_set *exceptset, struct timeval *timeout) ; 
FD_ZERO(int fd, fd_set* fds) ;   //清空集合
FD_SET(int fd, fd_set* fds)   ;  //将给定的描述符加入集合
FD_ISSET(int fd, fd_set* fds)  ; //将给定的描述符从文件中删除  
FD_CLR(int fd, fd_set* fds)  ;  //判断指定描述符是否在集合中
```

1. select函数的返回值就绪描述符的数目，超时时返回0，出错返回-1。
2. 第一个参数max_fd指待测试的fd个数，它的值是待测试的最大文件描述符加1，文件描述符从0开始到max_fd-1都将被测试。
3. 中间三个参数readset、writeset和exceptset指定要让内核测试读、写和异常条件的fd集合，如果不需要测试的可以设置为NULL。
4. timeout是调用select函数阻塞的超时时间，单位毫秒；
5. fd_set 类型可以简单的理解为按 bit 位标记文件描述符的队列，例如要在某 fd_set 中标记一个值为 16 的文件描述符，则该 fd_set 的第 16 个 bit 位被标记为 1。具体的置位、验证可使用 FD_SET、FD_ISSET 等宏实现。在 select() 函数中，readfds、writefds 和 exceptfds 同时作为输入参数和输出参数。如果输入的 readfds 标记了 16 号文件描述符，则 select() 将检测 16 号文件描述符是否可读。在 select() 返回后，可以通过检查 readfds 有否标记 16 号文件描述符，来判断该“可读”事件是否发生。

select执行:

![](https://upload-images.jianshu.io/upload_images/4440914-4bdfa41a7bf62314.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

每次执行select函数时，要遍历全部的文件描述符来检查是否准备好，如果有很多文件描述符，效率较低，且系统设置select最多只能同时服务1024个文件描述符（可以修改，linux/posix_types.h头文件有这样的声明：`#define __FD_SETSIZE    1024`  表示select最多同时监听1024个fd）。所以，select不适合并发数大于1024的情况。
同时，每次调用select函数，都需要把fd集合从用户态拷贝到内核态，有较大开销。 

## poll 

poll的机制与select类似，管理多个描述符也是进行轮询，根据描述符的状态进行处理，但poll没有最大文件描述符数量的限制。

Linux提供的poll相关函数接口如下： 

```C
int poll(struct pollfd fds[], nfds_t nfds, int timeout);
typedef struct pollfd {
        int fd;          // 需要被检测或选择的文件描述符
        short events;     // 对文件描述符fd上感兴趣的事件
        short revents;    // 文件描述符fd上当前实际发生的事件 
} pollfd_t;
```

1. poll函数的返回值是就绪描述符的数目，超时时返回0，出错返回-1。
2. fds是一个struct pollfd类型的数组，用于存放需要检测其状态的socket描述符。
3. timeout是调用poll函数阻塞的超时时间，单位毫秒，超过timeout时，无论I/O是否准备好，poll都会返回。timeout指定为负数值表示无限超时，使poll()一直挂起直到一个指定事件发生；timeout为0指示poll调用立即返回并列出准备好I/O的文件描述符，但并不等待其它的事件。
4. pollfd的events域是监视该文件描述符的事件掩码，由用户来设置这个域，pollfd的revents域是文件描述符的操作结果事件掩码，内核在调用返回时设置这个域。events域中请求的任何事件都可能在revents域中返回。

可能事件如下：

|掩码|事件|
|--|--|
|POLLIN 　　|　　　　　有数据可读 |
|POLLRDNORM 　|　　　  有普通数据可读|
|POLLRDBAND　　|　　　 有优先数据可读| 
|POLLPRI　　　　|　　　有紧迫数据可读| 
|POLLOUT　　　　|　    写数据不会导致阻塞 | 
|POLLWRNORM　　 |　　  写普通数据不会导致阻塞 |
|POLLWRBAND　　|　　　 写优先数据不会导致阻塞 |
|POLLMSGSIGPOLL 　|　　消息可用 | 

revents域中还可能返回下列事件：

|掩码|事件|
|--|--|
|POLLER　　    |  指定的文件描述符发生错误 |
|POLLHUP　　   |   指定的文件描述符挂起事件 |
|POLLNVAL　　  |    指定的文件描述符非法|
　

与select相同，每次调用poll时，都需要遍历全部的文件查看是否准备好，也需要把fd的集合从用户态拷贝到内核态，但是poll的最大连接数没有限制，如果空间允许的话，可以加入文件描述符，但是过多的文件描述符还是会降低返回速度。

## epoll 

epoll是Linux中基于事件驱动的I/O多路复用。相对于select和poll来说，epoll没有描述符个数限制，使用一个描述符管理多个描述符，调用时不用遍历全部文件描述符。

Linux中提供的epoll相关函数接口如下：

```C
#include <sys/epoll.h>
int epoll_create(int size);
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int epoll_wait(int epfd, struct epoll_event * events, int maxevents, int timeout);
```

1. epoll_create函数创建一个epoll句柄，参数size表明内核要监听的描述符数量。调用成功时返回一个epoll句柄描述符，失败时返回-1。
2. epoll_ctl函数注册要监听的事件类型：
   + epfd: epoll句柄
   + op: fd的操作类型
     - EPOLL_CTL_ADD 注册新的fd到epfd中
     - EPOLL_CTL_MOD 修改已注册的fd的监听事件
     - EPOLL_CTL_DEL 从epfd中删除一个fd
   + fd: 要监听的文件描述符
   + event: 表示要监听的事件，结构如下 

```C
struct epoll_event {
    __uint32_t events;  /* Epoll events */
    epoll_data_t data;  /* User data variable */
};

typedef union epoll_data {
    void *ptr;
    int fd;
    __uint32_t u32;
    __uint64_t u64;
} epoll_data_t;
/*
events可能的参数：
EPOLLIN ：表示对应的文件描述符可以读；
EPOLLOUT：表示对应的文件描述符可以写；
EPOLLPRI：表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）；
EPOLLERR：表示对应的文件描述符发生错误；
EPOLLHUP：表示对应的文件描述符被挂断；
EPOLLET： 将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的。
EPOLLONESHOT：只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里
*/
```

3. epoll_wait函数等待事件的就绪，成功时返回就绪的事件数目，调用失败时返回 -1，等待超时返回 0。
    + epfd: epoll句柄
    + events: 就绪的事件的集合
    + maxevents: 最多event的数目
    + timeout: 等待的超时时间
    
epoll对文件描述符的操作有两种模式：LT（level trigger，水平触发）和ET（edge trigger，边缘触发）。

1. 水平触发：默认工作模式，即当epoll_wait检测到某描述符事件就绪并通知应用程序时，应用程序可以不立即处理该事件；下次调用epoll_wait时，会再次通知此事件。

2. 边缘触发：当epoll_wait检测到某描述符事件就绪并通知应用程序时，应用程序必须立即处理该事件。如果不处理，下次调用epoll_wait时，不会再次通知此事件，也就是说边缘触发只在状态由未就绪变为就绪时通知一次。

### epoll实现

epoll在Linux内核中申请一个简易的文件系统，用这个文件系统来存储要监控的事件（文件系统一般是什么结构？树！） 

1. 当某一进程调用epoll_create方法时，Linux内核会创建一个eventpoll结构体，eventpoll结构体如下所示：

```C
struct eventpoll{
    ....
    struct rb_root  rbr; // 红黑树的根节点，这颗树中存储着所有添加到epoll中的需要监控的事件
    struct list_head rdlist; // 双向链表，存放着将要通过epoll_wait返回给用户的满足条件的事件
    ....
};
```

每一个epoll对象都有一个独立的eventpoll结构体，调用epoll_ctl添加事件时，这些事件都以节点的形式会挂载在红黑树中，如此，重复添加的事件就可以通过红黑树而高效的识别出来(红黑树的插入时间效率是lg(n)，其中n为树的高度)。

所有添加到epoll中的事件都会与设备驱动程序建立回调关系，也就是说，当相应的事件发生时会调用这个回调方法。这个回调方法在内核中叫`ep_poll_callback`,它会将发生的事件添加到rdlist双链表中，表示该事件已经就绪。

在epoll中，对于每一个事件，都会建立一个epitem结构体，如下所示：

```C
struct epitem{
    struct rb_node  rbn;         //红黑树节点
    struct list_head    rdllink; //双向链表节点
    struct epoll_filefd  ffd;  //事件句柄信息
    struct eventpoll *ep;    //指向其所属的eventpoll对象
    struct epoll_event event;   //期待发生的事件
}
```

结构如下：

![](https://upload-images.jianshu.io/upload_images/4440914-4aa4a4e8ae5d1aee.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

调用epoll的三个函数，实际上发生了：

1. epoll_create: 创建一个eventpoll结构体；返回一个句柄，之后所有的使用都依靠这个句柄来标识。
2. epoll_ctl: 向epoll对象中添加、删除、修改感兴趣的事件，返回0标识成功，返回-1表示失败。(插入，删除，修改红黑树) 
3. epoll_wait: 收集收集在epoll监控中已经发生的事件。（返回双向链表中的元素）

epoll适用于总连接数大，但是任意时刻只有少量的活跃的连接（就绪的fd）的情况。

# 总结


|区别|select|poll| epoll| 
|--|--|--|--|
|操作方式|遍历|遍历|回调|
|数据结构|数组|链表|红黑树+链表|
|效率|每次调用都线性遍历，时间复杂度为O(n)|每次调用都线性遍历，时间复杂度为O(n)|事件驱动，当fd就绪，系统注册的回调函数就会被调用，将就绪fd放到链表中，时间复杂度O(1)|
|最大连接数|1024|无上限|无上限|
|fd拷贝|每次调用，都需要把fd集合从用户态拷贝到内核态|每次调用，都需要把fd集合从用户态拷贝到内核态|调用epoll_ctl时拷贝fd进内核并保存，epoll_wait不拷贝|



