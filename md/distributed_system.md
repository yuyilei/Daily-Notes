# 分布式系统(distributed system)

## 什么是分布式系统？

分布式系统是由一组通过网络进行通信、为了完成共同的任务而**协调工作**的计算机节点组成的系统。分布式系统的出现是为了用廉价的、普通的机器完成单个计算机无法完成的计算、存储任务。其目的是**利用更多的机器，处理更多的数据**。

单机系统：一个项目所有的代码都放在一台服务器上，所有的服务都由这台服务器提供。单机系统的处理请求和存储数据的能力有限，且一旦此台服务器宕机，服务就挂了。

集群系统：当单机系统的性能达到瓶颈的时候，把单机复制几份，就构成了“集群”。集群中的每台服务器称为一个节点，每个节点提供**相同**的服务，相当于系统的处理能力提升了好几倍。 --> 如何决定由哪个节点处理请求，负载均衡服务器。(Nginx) 集群系统容易扩展，随着业务的发展，可直接向集群中添加节点即可。 

而分布式结构就是将一个完整的系统，按照业务功能，拆分成一个个独立的子系统，在分布式结构中，每个子系统就被称为“子服务”。这些子系统能够独立运行在web容器中，它们之间又能进行通信。这样做的好处很多，可以降低系统的耦合性（每个子系统独立进行开发、测试和部署）、业务的服务易于拓展。但同时，分布式系统多节点协作、各节点拓扑结构，会引入很多单机系统没有的问题，为了解决这些问题又会引入更多的机制、协议，带来更多的问题。


![](https://upload-images.jianshu.io/upload_images/4440914-47906d7c5db0041f.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)


为什么要有分布式系统：

1. 单个节点的处理能力无法满足日益增长的计算、存储任务； 

2. 硬件的提升（加内存、加磁盘、使用更好的CPU）高昂到得不偿失；

3. 应用程序无法进一步优化； 


分布式系统面临的挑战：

1. 异构的机器与网络： 

2. 普遍的节点故障： 

3. 不可靠的网络： 

总而言之，分布式的挑战来自**不确定性**。 


## CAP定理 

CAP原则又称CAP定理，指的是在一个分布式系统中，Consistency（一致性）、 Availability（可用性）、Partition tolerance（分区容错性）这三个基本需求，最多只能同时满足其中的2个。 



|选项|描述|
|  --  |  --  | 
|Consistency（一致性）|指数据在多个副本之间能够保持一致的特性（严格的一致性）| 
|Availability（可用性）|指系统提供的服务必须一直处于可用的状态，每次请求都能获取到非错的响应（不保证获取的数据为最新数据）|
|Partition tolerance（分区容错性）|分布式系统在遇到任何网络分区故障的时候，仍然能够对外提供服务，除非整个网络环境都发生了故障| 



**CA without P**

如果不要求P（不允许分区），则C（强一致性）和A（可用性）是可以保证的。但其实分区不是你想不想的问题，而是始终会存在，因此CA的系统更多的是允许分区后各子系统依然保持CA。

**CP without A** 

如果不要求A（可用），相当于每个请求都需要在Server之间强一致，而P（分区）会导致同步时间无限延长，如此CP也是可以保证的。很多传统的数据库分布式事务都属于这种模式，在网络故障时只读不写。

**AP wihtout C** 

要高可用并允许分区，则需放弃一致性。一旦分区发生，节点之间可能会失去联系，为了高可用，每个节点只能用本地数据提供服务，而这样会导致全局数据的不一致性。现在众多的NoSQL都属于此类。


### MapReduce -- 分布式运算模型


MapReduce是一种编程模型，用于大规模数据集的分布式运算。 

MapReduce分为两部分：Map和Reduce。集群中机器（worker）分为三种：Master，Mapper，和Reducer。Master负责调度Mapper和Reducer。

1）用户输入文件（提交作业）。MapReduce将用户输入的文件分成M份。

2）Master负责分配map task和reduce task到空闲的Mapper和Reducer上。

3）被分配map task的Mapper会读入用户输入的文件(已被split为M份)。Mapper的工作是对文件的内容调用用户定义的map()函数，会产生大量的中间的key/value对，并存储在内存中。

4）内存中存储的key/value对会被周期性地写到磁盘上，并被分割为R个区域。这些key/value对在磁盘上的位置会返回到Master上，再由Master分配到Reducer上。

5）一个worker被Master分配reduce task之后成为Reducer。Reducer会使用远程的进程调用读取Mapper磁盘上的key/value对的中间文件。Reducer会对读取到的中间文件中的key/value对进行排序（根据key进行排序），这样，拥有一样的key的key/value会被规约到一起。

6）Reducer遍历排序后的中间文件，对于每一个唯一的key和它们的value的集合调用用户定义的reduce()函数，产生的输出被写入到最终输出文件中。

7）所有的Mapper和Reducer都完成工作之后，就表示计算完成了，Master将程序执行权返回给用户。

在整个过程中，用户只需要提供一个map()函数和一个reduce()，其余的交给Master去调度即可。


![](https://upload-images.jianshu.io/upload_images/4440914-5e54669407edcb40.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)  


![](https://upload-images.jianshu.io/upload_images/4440914-7260aa08a1424a29.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

核心： 

1. map处理逻辑——对传进来的一行数据如何处理？输出什么信息？

2. reduce处理逻辑——对传进来的map处理结果如何处理？输出什么信息？


[MapReduce](https://github.com/yuyilei/MIT-6.824/blob/master/notes/MapReduce.md) 



### Raft -- 分布式一致性算法

Raft 是一种为了管理复制日志的一致性算法。 所谓一致性，就是多个节点对某个事情达成一致的看法，即使是在部分节点故障、网络延时、网络分割的情况下。


在Raft中，任何时候一个服务器可以扮演下面角色之一：

Leader：负责接收客户端的请求，将日志复制到其他节点并告知其他节点何时应用这些日志是安全的；

Candidate：用于选举Leader的一种角色；

Follower：负责响应来自Leader或者Candidate的请求，完全被动，不会主动发起请求。

开始时，所有的服务器都是follower，等待leader请求，在一段时间内没有收到请求，它就认定leader宕机了，自己变为candidate，开启一次选举，给自己投票，并向其他服务器发起选举请求，如果它获得了大部分服务器的选票，它就变为一个leader。

选举出一个leader之后，leader负责管理复制日志来实现一致性。leader从客户端接收日志条目，把日志条目复制到其他服务器上，并且当保证安全性的时候告诉其他的服务器应用日志条目到他们的状态机中（将日志应用到状态机就是执行日志所对应的操作）。

![](https://upload-images.jianshu.io/upload_images/4440914-b40fe47d0f8118a9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240) 



流程如下：

1. client向leader发送请求，如（set V = 3），此时leader接收到数据是是uncommitted状态；

2. leader给所有follower发送请求，将数据（set V = 3）复制到follower上，leader等待follower的接收响应；

3. leader收到超过超过半数的接收响应后，leader返回一个ACK给client，确认数据已接收；

4. client收到leader的ACK之后，表明此数据是已提交（committed）状态，leader再向follower发通知告知该 数据状态已提交，此时follower就可以应用日志条目到他们的状态机。


![](https://upload-images.jianshu.io/upload_images/4440914-2185e8c2d609b468.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240) 


状态的伪代码：

```python
while true：
    if state is follwer:  
        初始化超时时间；
        while true：
            if 接收到投票请求 and 成功投票:
                重置超时时间
            elif 接收到heartbeat and leader的任期大于自己：
                重置超时时间
            elif 接收到AppendEntry and leader的任期大于自己：
                重置超时时间 
            elif 超时：
                become a candidate 
                break 
        
    if state is candidate:
        构造requestVote请求 
        给自己投票
        初始化投票超时时间 
        对其他server广播投票请求
        while true:
            if 接收到heartbeat and leader的任期大于自己：
                become a follower 
                break
            if 接收到AppendEntry and leader的任期大于自己：
                become a follower 
                break 
            if 选举成功：
                become a leader 
                break
            if 选举失败(超时)：
                下一轮选举
                
    if state is leader:
        while state is leader:
            对其他server发送heartbeat或者appendentry
            sleep(一段时间) 
        become a follower 
```




[Raft动画](http://thesecretlivesofdata.com/raft/?utm_source=hacpai.com)








