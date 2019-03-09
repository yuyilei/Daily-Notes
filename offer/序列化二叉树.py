class TreeNode:
    def __init__(self, x):
        self.val = x
        self.left = None
        self.right = None

class Solution:
    def __init__(self):
        self.index = -1

    def Serialize(self, root):
        if not root:
            return "#."
        return str(root.val)+"." + self.Serialize(root.left) + self.Serialize(root.right)

    def Deserialize(self, s):
        self.index = -1
        vals = s.split(".")[:-1]
        print(vals)
        return self.deserialize(vals)

    def deserialize(self, vals):
        self.index += 1
        if self.index >= len(vals):
            return None
        if vals[self.index] == "#":
            return None
        root = TreeNode(int(vals[self.index]))
        root.left = self.deserialize(vals)
        root.right = self.deserialize(vals)
        return root


if __name__ == '__main__':
    s = Solution()
    s.Deserialize("1.2.3.")
