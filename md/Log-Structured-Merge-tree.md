# Log-Structured-Merge Tree in LevelDB

LSM 即 **日志结构合并树**，是一种搜索的数据结构，分层组织（物理结果为按照逻辑分层的有序文件）。google在发表的“BigTable”的论文里，提出了LSM树。LevelDB采用LSM树存储引擎。

LevelDB是google开源的key-value数据库，底层由LSM树实现。

## LSM结构

LSM树的结构如下：

![](https://upload-images.jianshu.io/upload_images/4440914-8645c81bcdd6e0d2.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240) 

memtable驻留在内存中，Level0到Levelk存在磁盘上，每一层都由SSTable组成。

1. log file，磁盘中，写操作前，先记录日志，append到文件，顺序写入，在宕机时，memtable内存数据丢失，可以通过log file恢复。

2. memtable，内存中，由**跳跃链表**实现，新的数据写入到memtable。

3. immutable memtable，内存中，memtable达到容量的上限后，变为immutable memtable，不可再写入，同时产生新的memtable。

4. SSTable(Sorted String Table)，磁盘中，组成Level0到Levelk的多层结构，每层由若干个SSTable组成。Level0的SSTable由immutable memtable 直接dump得到。SSTable文件内的数据是有序的。高层Level的SSTable由低一层的Level的SSTable和本层的SSTable**归并**产生(归并排序中的归并过程)。 新的的SSTable在归并过程中通过**顺序写**生成，生成后不再改变，只可能后续的归并中被删除。 

SSTable的结构如下：

![](https://upload-images.jianshu.io/upload_images/4440914-179e556d04385c5e.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240) 

SSTable由连续的key-value组成。

SSTable Index是SSTable的索引结构，记录了每个key对应的位置。 SSTable Index驻留在内存中，读索引更快，在读操作时，也能快速判断一个key是否存在。

LSM树有以下规定：

1. SSTable全部在磁盘上，SSTable Index全部在内存中

2. 所有写操作直接写入memtable

3. 读操作先检查 memtable，在检索内存中的SSTable Index 

4. memtable周期性地dump到磁盘上成为SSTable，并产生新的memtable

5. immutable memtable和SSTable不可更新

6. 磁盘上的SSTable周期性地归并到下一层

可以看出，LSM树把所有对磁盘的写操作都限定为**顺序写**，因为**随机写磁盘**非常耗时，所以LSM把对数据的修改/写入限定在内存中，memtable达到一定容量后，将memtable中的数据批量**顺序**地写入磁盘，同时磁盘中维持一个由SStable组成的分层结构，同理上层SStable达到一定容量后，归并到下一层（也是顺序写）。

LSM树的读操作优先查看内存，memtable没有命中的话，就读取磁盘，内存中所有SStable对应的Index，可以快速定位到这个key在磁盘中的位置，但是读磁盘的操作其实还是随机读。

LSM树就是通过避免对磁盘的**随机写**，提高写入效率，同时通过memtable和SSTable Index提高读取效率。


## LevelDB

LevelDB支持 **Put**、**Get**、**Delete**。

[LevelDB的实现文档](https://github.com/google/leveldb/blob/master/doc/impl.md)

LevelDB由以下几种文件组成：

1. log file(*.log)，存储最近修改的日志，每次修改通过append附加到当前的log file。当log file达到指定容量时（一般时4MB），会重写产生一个新的SSTable，并新建一个log file

2. String Sorted Table(*.ldb)，存储按键排序的条目序列。每个条目都是键的值，或键的删除标记。 分层结构，定期向下层归并。

3. Manifest(MANIFEST)，记录组成每一层的SSTable的分布、key的范围等其他元数据。每次重新打开数据库的时候，都会创建一个新的MANIFEST。MANIFEST是日志文件，将服务状态的改变（如SSTable的添加或删除等）追加到日志后面。 （LevelDB没有将每个SSTable的Index存在内存中，而是将每个SSTable的key区间等元数据存在Manifest文件中）

4. Current(CURRENT)，一个简单的text文件记录当前MANIFEST的名称。

5. info log(LOG或LOG.old)，记录消息


### Level0

当log file的大小增长到一定大小（默认是4MB），就会创建一个新的memtable和log file（用于之后的写操作）。具体如下：

1. 将旧的memtable dump产生一个SSTable

2. 丢弃旧的memtable，删除旧的memtable和log file

3. 把新产生的SSTable加入到Level0


### Compactions

当Level L的容量超过限制时，后台线程会开始compaction。

compaction过程：

1. 在Level-L级选择一个文件，在Level-L+1级找出所有与Level-L中选择出来的文件有重合key的文件； 由于Level-0比较特殊，Level-0的SST文件由memtable在不同时间Dump而成，所以Level-0内的Key有可能重叠，所以对Level-0进行compaction时，可能会选择多个Level-0的文件。

2. 将选择出来的Level-L和Level—L+1文件进行**归并**，产生一系列新的Level+1文件。然后切换到新的Level-L+1文件，当Level-L+1级的文件容量超过限定时，也会向下产生compaction。  归并过程中删除旧的文件，新的文件被加入到服务中去。

3. compaction过程会删除被覆盖的值（之后被修改的值）。如果有一个key对应的值是删除标志（表示这个key被删除），且下一层文件中没有包含key，那么这个key也会被删除。


### 读操作

1. 生成内部查询的Key，由用户请求的UserKey加Sequence拼接而成。Sequence由用户提供或者使用当前最新的Sequence，读操作是基于特定的Sequence的，仅查询当前Sequence之前写的数据。

2. 用生成的key，查询memtable和immutale memtable，查询不到就要读磁盘。

3. 在SSTable文件查找是需要读取磁盘，Manifest中记录的每个文件的key区间，可以很方便的知道某个key是否在某个SSTable中。Level-0由immutable memtable直接dump生成，所以Level-0中的key可能会重复，所以查找Level-0时要对每个SSTable进行查找。 对于其他层次，由于归并过程保证了其互相不重叠且有序，二分查找的方式提供了更好的查询效率。

### 写操作

写操作包括 设置key-value和删除key。对于这两种，LevelDB的处理是相同的，删除key其实是向LevelDB插入一条value为删除的数据。

1. 记录日志

2. 修改memtable 

在写操作执行中，会触发 memtable --> immutable memtable --> Level0 --> 下一层Level （其中immutable memtable写入Level0和Level之间的compaction是启动一个唯一的后台compaction线程进行的）

每个Put或者Delete都会被封装为一个WriteBatch，保证WriteBatch内操作的原子性。 WriteBatch对象作为参数传入Write函数，Write函数如下：

```C++
Status DBImpl::Write(const WriteOptions& options, WriteBatch* updates) {
  Writer w(&mutex_);     // 用WriteBatch封装一个Writer，mutex_为全局锁
  w.batch = updates;   
  w.sync = options.sync;
  w.done = false;

  MutexLock l(&mutex_);       // 加锁，唯一确定writer，MutexLock对象的生命周期结束时自动释放锁。
  writers_.push_back(&w);     // writers_为全局的writer队列，将当前writer放入
  while (!w.done && &w != writers_.front()) {   // 如果当前writer没有被完成，且当前writer不在队列头部，就等待
    w.cv.Wait();
  }
  if (w.done) {               // 当前writer已经被完成就直接返回状态
    return w.status;
  }

  // May temporarily unlock and wait.
  Status status = MakeRoomForWrite(updates == nullptr);   // 当前的写入准备Memtable空间，如果没有充足空间会挂起。
  uint64_t last_sequence = versions_->LastSequence();
  Writer* last_writer = &w;
  if (status.ok() && updates != nullptr) {  // nullptr batch is for compactions
    WriteBatch* updates = BuildBatchGroup(&last_writer);         // BuildBatchGroup中会尝试将当前等待的所有其他Writer中的写入合并到当前的WriteBatch中，
    WriteBatchInternal::SetSequence(updates, last_sequence + 1);   // 设置当前的updates的sequence（序列号）
    last_sequence += WriteBatchInternal::Count(updates);       // 根据当前updates更新last_sequence

    // Add to log and apply to memtable.  We can release the lock
    // during this phase since &w is currently responsible for logging
    // and protects against concurrent loggers and concurrent writes
    // into mem_.
    {
      mutex_.Unlock();                         // 修改日志和memtable可以释放锁，因为当前writer已经防止并发修改日志和memtable了。(代码保证同一时刻只有一个线程会执行写入操作。)
      status = log_->AddRecord(WriteBatchInternal::Contents(updates));
      bool sync_error = false;
      if (status.ok() && options.sync) {
        status = logfile_->Sync();
        if (!status.ok()) {
          sync_error = true;
        }
      }
      if (status.ok()) {
        status = WriteBatchInternal::InsertInto(updates, mem_);
      }
      mutex_.Lock();                                                // 操作完，还要加锁，因为后面还有对 writers_的操作
      if (sync_error) {
        // The state of the log file is indeterminate: the log record we
        // just added may or may not show up when the DB is re-opened.
        // So we force the DB into a mode where all future writes fail.
        RecordBackgroundError(status);
      }
    }
    if (updates == tmp_batch_) tmp_batch_->Clear();     //      

    versions_->SetLastSequence(last_sequence);          // 修改介绍后再修改last_Sequence，LevelDB的读操作是基于Sequence的，所以在WriteBatch写入过程对读操作不可见，保证了原子性
  }

  while (true) {                                        // 把writers_队列中完成的writer移除
    Writer* ready = writers_.front();
    writers_.pop_front();
    if (ready != &w) {
      ready->status = status;
      ready->done = true;
      ready->cv.Signal();
    }
    if (ready == last_writer) break;                    // 相当于默认writers_第一个writer到last_writer都是已经完成的状态
  }

  // Notify new head of write queue
  if (!writers_.empty()) {          
    writers_.front()->cv.Signal();                      // 给writers_队列第一个writer发送信号   
  }

  return status;
}
```

MakeRoomForWrite函数

```C++
Status DBImpl::MakeRoomForWrite(bool force) {
  mutex_.AssertHeld();
  assert(!writers_.empty());        // 检查当前线程是否获取了互斥锁
  bool allow_delay = !force;        // 能否延迟写入
  Status s; 
  while (true) {                    // 循环，直到mem_中有空间可供写
    if (!bg_error_.ok()) {          // 后台线程出错，跳出循环
      // Yield previous error
      s = bg_error_;
      break;
    } else if (allow_delay && versions_->NumLevelFiles(0) >=
                                  config::kL0_SlowdownWritesTrigger) {     // 如果能够延迟写入，就判断Level0的文件数量是否超过限定
      // We are getting close to hitting a hard limit on the number of
      // L0 files.  Rather than delaying a single write by several
      // seconds when we hit the hard limit, start delaying each
      // individual write by 1ms to reduce latency variance.  Also,
      // this delay hands over some CPU to the compaction thread in
      // case it is sharing the same core as the writer.
      mutex_.Unlock();
      env_->SleepForMicroseconds(1000);                                    // 睡眠当前线程，等待背景线程合并完成之后再写入
      allow_delay = false;                                                 // Do not delay a single write more than once，单此写入只能sleep一次
      mutex_.Lock();
    } else if (!force &&
               (mem_->ApproximateMemoryUsage() <= options_.write_buffer_size)) {    // 检查memtable中是否有充足空间，如果有，跳出循环
      // There is room in current memtable
      break; 
    } else if (imm_ != nullptr) {                                          // 执行到这里，说明memtable没有充足空间，尝试把mem_赋值给imm_，但是要先检查imm_是否为空，如果 imm_不为空，说明之前的imm_还没被dump到磁盘，需要等待
      // We have filled up the current memtable, but the previous
      // one is still being compacted, so we wait.
      Log(options_.info_log, "Current memtable full; waiting...\n");
      background_work_finished_signal_.Wait();
    } else if (versions_->NumLevelFiles(0) >= config::kL0_StopWritesTrigger) {   // 运行到这里，说明imm_为空，但还是要检查Level0中文件个数是否超过限定，因为之前将imm_写盘完成后，Level0 中的sstable又增加了一个文件
      // There are too many level-0 files.
      Log(options_.info_log, "Too many L0 files; waiting...\n");
      background_work_finished_signal_.Wait();
    } else {                                                               // 所有条件都满足，将mem_赋值给imm_
      // Attempt to switch to a new memtable and trigger compaction of old
      assert(versions_->PrevLogNumber() == 0);
      uint64_t new_log_number = versions_->NewFileNumber();
      WritableFile* lfile = nullptr;
      s = env_->NewWritableFile(LogFileName(dbname_, new_log_number), &lfile);     // 创建新的log file，因为memtable和log file是一一对应的
      if (!s.ok()) {
        // Avoid chewing through file number space in a tight loop.
        versions_->ReuseFileNumber(new_log_number);
        break;                                                                     // 创建失败，就跳出循环
      }
      delete log_;                                                                 // 删除旧的日志     
      delete logfile_;
      logfile_ = lfile;
      logfile_number_ = new_log_number;
      log_ = new log::Writer(lfile);
      imm_ = mem_;                                                                 // 将mem_赋值给imm_
      has_imm_.store(true, std::memory_order_release);
      mem_ = new MemTable(internal_comparator_);                                   // 创建新的memtable
      mem_->Ref();
      force = false;  // Do not force another compaction if have room
      MaybeScheduleCompaction();                                                   // 启动后台compaction线程（可能后台compaction线程已经启动了，所以是maybe）
                                                                                   // 在同一时刻，LevelDB只允许一个后台compaction线程存在
                                                                                   // 此线程主要做：
                                                                                   // 1. 将imm_写入磁盘生成一个新的sstable；
                                                                                   // 2. 对各个level中的文件进行合并，避免某个level中的文件过多，以及删除掉一些过期或者已经被用户
                                                                                   // 调用delete删除的key-value
    }
  }
  return s;
}
```

可以看出，LevelDB的的写操作是将 **一次磁盘随机写** 放大为 **一次内存写+多次磁盘顺序写**，虽然有周期性的compaction（磁盘顺序写），但这是由后台compaction线程进行的，写操作实际是将修改增量添加到内存（memtable）中，所以大大提高写操作的效率。










