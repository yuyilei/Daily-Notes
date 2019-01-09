# Redis pipeline transaction 

## Pipeline 

redis中，redis client将多条命令放入队列，一起发送到redis server，由redis server全部处理完成后，一起发送回 redis client，这就是pipeline。 

可以看出，pipeline类似于批处理，主要是提供了网络层面的优化，多条命令一起发送可减少中间多次的网络的延迟。 

同一个pipeline中命令不保证事务性，redis的单线程保证了单个命令本身是原子的，但多个client可以发起多个命令串行执行，也就是说对于redis server来说，可以串行执行来自多个pipeline的命令。

pipeline改变了 redis client和 redis server 中读写 buffer的顺序，这里的改变读写顺序不是指改变命令的读写顺序，pipeline不会改变命令的读写顺序。

执行一条命令时，redis client和 redis server的读写顺序如下：

1. redis client 调用write方法将消息写到操作系统内核为socket分配的发送缓冲send buffer中

2. 客户端操作系统内核将发送缓冲的内容发送到网卡，网卡硬件送到服务器的网卡

3. 服务器操作系统内核将网卡的数据读入到内核为socket分配的接收缓冲recv buffer中

4. redis server调用read方法从 recv buffer中读出数据

5. redis server处理完数据之后，将结果写入到操作系统内核为socket分配的发送缓冲send buffer中

6. 服务器操作系统内核将发送缓冲的内容发送到网卡，网卡硬件将数据送到客户端的网卡

7. 客户端操作系统内核将网卡的数据读入到内核为socket分配的接收缓冲recv buffer中

8. redis client调用read方法从 recv buffer中读出数据

可以看出 执行一个命令，对buffer的读写顺序是 写 -- 读 -- 写 -- 读，如果要执行多个命令，将会是：写 -- 读 -- 写 -- 读 -- 写 -- 读 -- 写 -- 读 -- 写 -- 读 -- 写 -- 读... 

其中，在 写 的时候，如果缓冲区满了，就会阻塞等待，在 读 的时候，如果缓冲区是空的，也回阻塞等待，这就是真正耗时的地方（比如，redis server从 缓冲区中读数据时，如果因为网络延时，没有及时写入来自客户端的数据，可能缓冲区中没有数据，造成阻塞）

使用了 pipeline了之后，多条命令就可以一起写入 缓冲区，redis server 一起将所有命令读出，完成之后，将所有结果一起写入缓冲区，然后缓冲区，然后redis client一起将执行结果读出缓冲区，因此对于pipeline来说，连续的读和写中间不会有任何堵塞，只会在刚开始写和刚开始读的时候，有等待的开销。 

## Transaction

上面说到同一个中的pipeline命令不能保证原子性，redis提供Transaction来保证多个操作的原子性。 

对应的redis 命令是 multi/exec/discard：

```
> mulit 
OK 
> command1 
QUEUED 
> command2 
QUEUED 
> command3
QUEUED 
> exec 
result 
```

multi 指示事务的开始，exec 指示事务的执行，discard 指示事务的丢弃。redis server 只用收到 exec命令，才会开始执行队列中的命令，因为 redis 的单线程特性，它不用担心自己在执行队列的时候被其它指令打搅，可以保证每一个命令能得到的「原子性」执行。

但是，redis 事务的原子性不是严格的原子性，事务的原子性是指要么事务全部成功，要么全部失败。 在redis的一个事务中，如果一个命令执行失败了，还会继续执行下面的命令。

redis事务仅仅是满足了事务的隔离性，隔离性中的串行化——当前执行的事务有着不被其它事务打断的权利。 

经常会结合 pipeline和 transaction一起使用，将一个事务中的命令放入一个管道。

另外，redis事务没有回滚，因为redis是先执行，再写日志，如果操作失败，就不会写日志，没有日志无法回滚。 

事务与watch结合可以解决在多个客户端（并发）要写同一数据的问题，如要将账户余额乘以一个倍数，只能先读出账户余额，乘以倍数后再写回，此时如果另外一个客户端要写账户余额，就会造成某一客户端改写的数据丢失。 

伪代码：

```Python
while True:
    do_watch()      # 盯住某个数据
    commands()      # 读出redis中的某些数据，进行操作
    multi()
    send_commands() # 对redis中的数据进行写
    try:
        exec()      # 自动检查关键变量是否改变，如果改变，就执行失败
        break
    except WatchError:
        continue
```

对应的是redis的watch命令。

```
> watch books
OK
> incr books  # 被修改了
(integer) 1
> multi
OK
> incr books
QUEUED
> exec  # 事务执行失败
(nil)
```

