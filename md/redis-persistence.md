# Redis 持久化

redis有RDB和AOF两种持久化方式。

RDB是快照文件的方式，redis通过执行`SAVE/BGSAVE`命令，执行数据的备份，将redis当前的数据保存到`*.rdb`文件中，文件保存了所有的数据集合。

AOF是服务器通过读取配置，在指定的时间里，追加redis写操作的命令到`*.aof`文件中，是一种增量的持久化方式。

一般通过 AOF + RDB 的方式进行redis持久化。设置定时任务，每隔一段时间用RDB备份(以时间作为标记，这样可以找到各个时段的备份)，同时，定时删除较久之前的备份文件，避免备份文件过多；同时，将AOF的时间设置得短一点，这样，即使redis奔溃，也不会损失很多数据； 

## RDB (Redis DataBase)

分为 save 和 bgsave 两种，由于redis 是单线程，在执行 save 命令时，会阻塞服务进程；而 bgsave 则是 fork 一个子进程，在子进程中执行 save 命令，不会影响服务进程，但是 bgsave 耗时时间较长，不够实时，可能会导致在停机的时候丢失大量数据。

执行 save 时：

```C++ 
void saveCommand(client *c) {
    // 当前有BGSAVE正在执行，不操作 
    if (server.rdb_child_pid != -1) {
        addReplyError(c,"Background save already in progress");
        return;
    }
    rdbSaveInfo rsi, *rsiptr;
    // 持久化 RDB 文件中的 复制性质，与  redis 主从同步有关 
    rsiptr = rdbPopulateSaveInfo(&rsi);
    // 调用 rdbSave 进行备份，会阻塞当前进程
    if (rdbSave(server.rdb_filename,rsiptr) == C_OK) {
        addReply(c,shared.ok);
    } else {
        addReply(c,shared.err);
    }
}
``` 

其中 rdbSave 函数：

```C
/* Save the DB on disk. Return C_ERR on error, C_OK on success. */ 
int rdbSave(char *filename, rdbSaveInfo *rsi) {
    char tmpfile[256];
    char cwd[MAXPATHLEN]; /* Current working dir path for error messages. */
    FILE *fp;
    rio rdb;                          // Robust I/O，有缓冲区的读写 
    int error = 0;

    snprintf(tmpfile,256,"temp-%d.rdb", (int) getpid());
    fp = fopen(tmpfile,"w");
    if (!fp) {
        char *cwdp = getcwd(cwd,MAXPATHLEN);
        serverLog(LL_WARNING,
            "Failed opening the RDB file %s (in server root dir %s) "
            "for saving: %s",
            filename,
            cwdp ? cwdp : "unknown",
            strerror(errno));
        return C_ERR;
    }

    // 将 fp 绑定到 rdb 结构体中，初始化条件  
    rioInitWithFile(&rdb,fp);

    // fsync incrementally while rdb saving
    // fsync 是系统调用，让内核强制将缓存数据写入到磁盘中 
    if (server.rdb_save_incremental_fsync)
        rioSetAutoSync(&rdb,REDIS_AUTOSYNC_BYTES);

    // Produces a dump of the database in RDB format sending it to the specified Redis I/O channel 
    if (rdbSaveRio(&rdb,&error,RDB_SAVE_NONE,rsi) == C_ERR) {
        errno = error;
        goto werr;
    }

    /* Make sure data will not remain on the OS's output buffers */
    // 确保数据不在操作系统的输出缓存中  
    if (fflush(fp) == EOF) goto werr;
    if (fsync(fileno(fp)) == -1) goto werr;
    if (fclose(fp) == EOF) goto werr;

    // 先将 RDB写入一个 tmp 文件中，如果中间有任何错误，save 就会失败
    // save 成功之后，再将 tmp 文件改名为 真正的 filename，防止 save失败，却留下一个被写了的 文件 
    /* Use RENAME to make sure the DB file is changed atomically only
     * if the generate DB file is ok. */
    // 将tmp 文件名 重命名 
    if (rename(tmpfile,filename) == -1) {
        char *cwdp = getcwd(cwd,MAXPATHLEN);
        serverLog(LL_WARNING,
            "Error moving temp DB file %s on the final "
            "destination %s (in server root dir %s): %s",
            tmpfile,
            filename,
            cwdp ? cwdp : "unknown",
            strerror(errno));
        unlink(tmpfile);
        return C_ERR;
    }

    // 快照成功，记录日志文件，dirty为0，记录当前时间 
    serverLog(LL_NOTICE,"DB saved on disk");
    server.dirty = 0;
    server.lastsave = time(NULL);
    server.lastbgsave_status = C_OK;
    return C_OK;

werr:
    serverLog(LL_WARNING,"Write error saving DB on disk: %s", strerror(errno));
    fclose(fp);
    unlink(tmpfile);
    return C_ERR;
}
```

bgsave 命令

```C
//  BGSAVE 命令实现可选参数 schedule 
void bgsaveCommand(client *c) {
    int schedule = 0;

    if (c->argc > 1) {
        // 参数只能有一个，且参数只能是"schedule"
        if (c->argc == 2 && !strcasecmp(c->argv[1]->ptr,"schedule")) {
            schedule = 1;
        } else {
            addReply(c,shared.syntaxerr);
            return;
        }
    }

    // 当前有BGSAVE正在执行，不操作
    if (server.rdb_child_pid != -1) {
        addReplyError(c,"Background save already in progress");
    } else if (server.aof_child_pid != -1) {
        // aof正在执行，且schedule==1
        if (schedule) {
            // 当条件允许的时候，执行 bgsave 
            server.rdb_bgsave_scheduled = 1;  /* BGSAVE when possible if true. */
            addReplyStatus(c,"Background saving scheduled");
        } else {
            // schedule != 1时，报错，不允许在执行 AOF 时，执行 bgsave  
            addReplyError(c,
            "An AOF log rewriting in progress: can't BGSAVE right now. "
            "Use BGSAVE SCHEDULE in order to schedule a BGSAVE whenever "
            "possible.");
        }
    } else if (rdbSaveBackground(server.rdb_filename,NULL) == C_OK) { // 调用rdbSaveBackground执行备份操作
        // 备份成功 
        addReplyStatus(c,"Background saving started");
    } else {
        // 备份失败 
        addReply(c,shared.err);
    }
}

// 在后台备份 
int rdbSaveBackground(char *filename, rdbSaveInfo *rsi) {
    pid_t childpid;
    long long start;
    
    // 如果正在 aof，或者正在 rdb 就返回错误 
    if (server.aof_child_pid != -1 || server.rdb_child_pid != -1) return C_ERR;

    // 记录一下状态 
    server.dirty_before_bgsave = server.dirty;
    server.lastbgsave_try = time(NULL);
    // Open a child-parent channel used in order to move information about the
    // RDB / AOF saving process from the child to the parent (for instance
    // the amount of copy on write memory used)
    // 父子进程之间共享一些数据 
    openChildInfoPipe();

    start = ustime();
    if ((childpid = fork()) == 0) {
        int retval;

        /* Child */

        // 关闭正在监听的socket 
        closeListeningSockets(0);
        redisSetProcTitle("redis-rdb-bgsave");
        // 调用 rdbSave 函数 
        retval = rdbSave(filename,rsi);
        if (retval == C_OK) {
            // 获取某个进程 smap bytes 的大小
            // 当参数为-1时，表示当前进程，计算 /proc/self/smaps 
            // 当参数为某一进程号时，计算 /proc/pid/smaps
            size_t private_dirty = zmalloc_get_private_dirty(-1);

            if (private_dirty) {
                serverLog(LL_NOTICE,
                    "RDB: %zu MB of memory used by copy-on-write",
                    private_dirty/(1024*1024));
            }

            server.child_info_data.cow_size = private_dirty;
            // 向channel中发送信号? 
            sendChildInfo(CHILD_INFO_TYPE_RDB);
        }
        // 根据 rdbSave的结果关闭子进程  
        exitFromChild((retval == C_OK) ? 0 : 1);
    } else {       /* Parent */
        server.stat_fork_time = ustime()-start;
        server.stat_fork_rate = (double) zmalloc_used_memory() * 1000000 / server.stat_fork_time / (1024*1024*1024); /* GB per second. */
        latencyAddSampleIfNeeded("fork",server.stat_fork_time/1000);
        if (childpid == -1) {
            closeChildInfoPipe();
            server.lastbgsave_status = C_ERR;
            serverLog(LL_WARNING,"Can't save in background: fork: %s",
                strerror(errno));
            return C_ERR;
        }
        serverLog(LL_NOTICE,"Background saving started by pid %d",childpid);
        server.rdb_save_time_start = time(NULL);
        server.rdb_child_pid = childpid;
        server.rdb_child_type = RDB_CHILD_TYPE_DISK;
        updateDictResizePolicy();
        return C_OK;
    }
    return C_OK; /* unreached */
}

/* This function is called once a background process of some kind terminates,
 * as we want to avoid resizing the hash tables when there is a child in order
 * to play well with copy-on-write (otherwise when a resize happens lots of
 * memory pages are copied). The goal of this function is to update the ability
 * for dict.c to resize the hash tables accordingly to the fact we have o not
 * running childs. */
// 避免在 aof 和 rdb 的时候 resize hash表，减少内存页面的拷贝次数 
void updateDictResizePolicy(void) {
    if (server.rdb_child_pid == -1 && server.aof_child_pid == -1)
        dictEnableResize();
    else
        dictDisableResize();
}
```


## AOF (Append Only File) 





