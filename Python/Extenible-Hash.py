

#每个桶中最多装100个元素
BUCKET_SIZE = 100

class Bucket():
    """
    Bucket
    map: 每个桶中的元素
    ld: 每个桶的local depth 
    """
    def __init__(self): 
        self.map = {} 
        self.ld = 0 
    
    def isfull(self):
        return len(self.map) >= BUCKET_SIZE 
    
    def insert(self,key,value): 
        self.map[key] = value 

    def get(self,key): 
        return self.map.get(key) 
        # 不能用 map[key]，没有的时候会报错
        # get()没有时返回 NoneType 

class ExtenibleHash():
    """
    ExtenibleHash:
    gd: 哈希表的global depth 
    bs: 桶的列表  
    插入时，先根据 key经过hash的值h在后 gd 位上的1的个数来查找桶，
    找到桶后，
        1）如果这个桶满了
                a) 如果，桶的深度 == gd（这时候要extend了），就把桶的列表乘2，gd+1 
            插入后，然后重新配分item  
        2) 如果这个桶没满，直接插入 
    """ 
    def __init__(self): 
        self.gd = 0 
        bucket = Bucket() 
        self.bs = [bucket] 

    def get_bucket(self,key): 
        h = hash(key) 
        return self.bs[ h & (( 1 << self.gd) - 1 )]

    def insert_to_bucket(self,key,value): 
        b = self.get_bucket(key) 

        # 如果桶还没满，直接插入，然后返回就行了. 
        if not b.isfull(): 
            b.insert(key,value) 
            return 

        # 下面都是桶满时候的情况 
        # 注意 gd >= ld 永远成立，不可能有 gd < ld 的情况 

        # 相等表示overflow了，需要多一位来表示 key的信息，所以把bucket的数量乘以二, gd+1  
        if b.ld == self.gd: 
            self.bs *= 2 
            self.gd += 1 

        # 重新分配item
        b.insert(key,value) 
        b1 = Bucket() 
        b2 = Bucket() 
        for k, v in b.map.items():
            h = hash(k)
            h = h & (( 1 << self.gd ) - 1 ) 
            # h：后gd里有几个 1 
            if ( h >> b.ld ) & 1 == 1:    # 右边第ld位是1  
                b2.insert(k,v) 
            else:                         # 右边第ld位是0 
                b1.insert(k,v) 

        # 把新的两个bucket插入bucketlist，代替旧的bucket 
        for index, item in enumerate(self.bs):
            if item == b: 
                # 根据index的右边第ld位数插入.. 
                if ( index >> b.ld ) & 1 == 1:
                    self.bs[index] = b2 
                else:
                    self.bs[index] = b1  
        
        b1.ld = b2.ld = b.ld + 1 


    def get(self,key):
        b = self.get_bucket(key) 
        return b.get(key) 


if __name__ == "__main__":
    eh = ExtenibleHash()
    N = 23333

    for x in list(range(N)):
        eh.insert_to_bucket(x,x)

    for i in range(N):
        print(eh.get(i)) 
