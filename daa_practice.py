# 1. Recursive and Non Recursive Fibonacci Sequence generator.
def fib(n):
    if n==1 or n==0:
        return n
    return fib(n-1)+fib(n-2)


x = int(input("Enter number: "))
ans0=0
ans1=1
print("\nIncremental")
for _ in range(x):
    print(ans0)
    anstemp = ans0+ans1
    ans0 = ans1
    ans1 = anstemp

print("\nRecursive")
for i in range(x):
    print(fib(i))

# --------------------------------------------------------------

# 2. Huffmann encoding 
#  data - chars = ['a', 'b', 'c', 'd', 'e', 'f']
#         freqs = [ 5, 9, 12, 13, 16, 45]

class Node:
    def __init__(self, char, freq, left=None, right=None):
        self.char = char
        self.freq = freq
        self.left = left
        self.right = right
        self.huff = ''

def print_huff(node, val=''):
    new_val = val + str(node.huff)
    if node.left:
        print_huff(node.left, new_val)
    if node.right:
        print_huff(node.right, new_val)
    else:
        print(f"{node.char}==>{new_val}")

if __name__=="__main__":
    chars = list(input("Enter Characters: ").split())
    freq = list(map(int,input("Enter Frequencies: ").split()))
    Nodes = []

    for i in range(len(chars)):
        Nodes.append(Node(chars[i], freq[i]))
    
    while len(Nodes)>1:
        Nodes = sorted(Nodes, key=lambda x: x.freq)
        left = Nodes[0]
        left.huff = 0
        right = Nodes[1]
        right.huff = 1
        temp = Node(left.char+right.char, left.freq+right.freq, left, right)

        Nodes.remove(left)
        Nodes.remove(right)
        Nodes.append(temp)
        
    print_huff(Nodes[0])

# --------------------------------------------------------------

# 3. Fractional Knapsack
# data - wt = [2,3,5,7,1,4,1]
#        val = [10,5,15,7,6,18,3]
#        capacity = 15

class weight:
    def __init__(self,wt, val):
        self.wt = wt
        self.val = val
        self.wtbyval = val/wt

def solve_fracknap(wts, vals, capacity):
    Items = []

    for i in range(len(wts)):
        Items.append(weight(wts[i], vals[i]))
    Items = sorted(Items, key= lambda x: x.wtbyval, reverse=True)

    i = 0
    profit = 0
    while capacity>0:
        if capacity-Items[i].wt>0:
            profit += Items[i].val
            capacity -= Items[i].wt
        
        else:
            profit += Items[i].val * (capacity/Items[i].wt)
            capacity=0
        i+=1
    return profit


if __name__=="__main__":
    wt = list(map(int,input("Enter Weights: ").split()))
    val = list(map(int,input("Enter Profits: ").split()))
    capacity = 15

    result = round(solve_fracknap(wt,val,capacity),2)
    print(result)

# --------------------------------------------------------------

# 4. 0/1 Knapsack
# data - val = [60, 100, 120]
#        wt = [10, 20, 30]
#        W = 50

def solve01knap(wts, vals, W, N):
    T = [[0 for _ in range(W+1)] for _ in range(N+1)]

    for i in range(N+1):
        for j in range(W+1):
            if i==0 or j==0:
                T[i][j] = 0
            elif wts[i-1] <= j:
                T[i][j] = max(T[i-1][j], vals[i-1] + T[i-1][j-wts[i-1]])
            else:
                T[i][j] = T[i-1][j]
    return T[N][W]

if __name__=="__main__":
    wt = list(map(int,input("Enter Weights (In Ascending Order): ").split()))
    val = list(map(int,input("Enter Profits: ").split()))
    W = 50
    N = len(val)

    print(solve01knap(wt,val, W, N))

# --------------------------------------------------------------

# 5. N-Queen problem

class Nqueen:
    def __init__(self, x, y):
        self.right_dia = [0]*30
        self.clm = [0]*30
        self.left_dia = [0]*30
        self.x = x
        self.y = y
        
    def printsoln(self,board):
        print("Dimensions of the board: ", len(board),"x", len(board))
        print(f"Initial Coordinates: ({self.x+1},{self.y+1}")

        for row in board:
            print(" ".join(map(str,row)))
    
    def solveNQUtil(self,board,col):
        if col >= N:
            return True

        if col == self.y:
            return self.solveNQUtil(board, col+1)
        
        for i in range(N):
            if i == self.x:
                continue

            if (self.left_dia[i-col+N-1] != 1 and self.right_dia[i+col] != 1) and self.clm[i] != 1:
                board[i][col]=1
                self.left_dia[i-col+N-1] = self.right_dia[i+col] = self.clm[i] = 1

                if self.solveNQUtil(board, col+1):
                    return True
                
                board[i][col] = 0
                self.left_dia[i-col+N-1] = self.right_dia[i+col] = self.clm[i] = 0

        return False

    def solveNQ(self):
        board = [[0 for _ in range(N)] for _ in range(N)]
        board[self.x][self.y] = 1

        self.left_dia[self.x-self.y+N-1] = self.right_dia[self.x+self.y] = self.clm[self.x] = 1

        if not self.solveNQUtil(board,0):
            print("No solution present")
            return False
        self.printsoln(board)
        return True

if __name__=="__main__":
    N = int(input("Enter Dimensionality: "))
    x,y = map(int,input("Enter Initial Position of the Queen: ").split())

    NQP = Nqueen(x-1,y-1)
    NQP.solveNQ()