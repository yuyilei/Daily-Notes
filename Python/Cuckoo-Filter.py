import numpy
import hashlib


class Bucket():

    def __init__(self):
        # 初始化每个桶有4个位置
        self.bucket = numpy.array([0,0,0,0]):


class Cuckoo():

    def __init__(self):
        # 初始化有8个桶
        self.bucket_count = 8
        self.buckets = list()
        for i in range(self.bucket_count):
            bucket = Bucket()
            self.buckets.append(bucket)
        # 设置最大挤兑次数为8
        self.kick_max = 16
        # 在最理想的情况下，能装下32个数字
        self.nums_max = 32
        # 当前过滤器中的元素个数
        self.count = 0

    @staticmethod
    def fingerprint(src):
        # 这里的fingerprint采用是md5算法
        md = hashlib.md5()
        md.update(src.encode('utf-8'))
        return md.hexdigest()


    @staticmethod
    def my_hash(src):
        # 这里的哈希算法直接采用 hash，计算完hash后再对数组长度取模
        return hash(src) % self.bucket_count


    def insert(self, data):
        if self.count >= self.nums_max:
            return False
        fp = self.fingerprint(data)
        index1 = self.my_hash(fp)
        index2 = index1 ^ self.my_hash(fp)



