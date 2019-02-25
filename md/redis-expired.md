# Redis 过期 

redis的过期机制分为定时遍历策略和惰性删除策略。 

定时遍历策略：

redis将设有过期时间的key放入一个dict，然后定期（默认为1秒）遍历这个dict中的key是否过期，具体如下：

1. 从过期dict中取出20个key

2. 删除这20个key中的过期key

3. 如果步骤2中的过期key超过1/4，就重复步骤1 

因此redis会持续扫描过期字典 (循环多次)，直到过期字典中过期的 key 变得稀疏，才会停止 (循环次数明显下降)。因此可能会造成读写请求的卡顿。 

如果要在redis中加入大量的设有过期时间的key，则需要给过期时间在一个随机范围内，不能让很多的key同时过期。 

惰性删除策略：

惰性策略就是在客户端访问这个 key 的时候，redis 对 key 的过期时间进行检查，如果过期了就立即删除。 



