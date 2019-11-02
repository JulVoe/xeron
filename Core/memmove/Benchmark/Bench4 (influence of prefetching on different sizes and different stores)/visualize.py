import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np

def mean(numbers):
    p = 1.0
    c = 0
    for f in numbers:
        if f>0:
            p = float(p*f)
            c = c+1
    return float(p)**float(1.0/float(c))
#    return float(sum(numbers)) / max(len(numbers), 1)

m = 10000000000
ind = 0

print("Daten\n")
a = []
x = np.arange(1,256*64+1,256)
for i in range(0,60):
    str = input("")
    a.append([int(l) for l in str.split(' ')])
    if m>mean(a[i]):
        m = mean(a[i])
        ind = i
    #plt.plot(x,a[i],'blue')

for i in range(60,90):
    str = input("")
    a.append([int(l) for l in str.split(' ')])
    if m>mean(a[i]):
        m = mean(a[i])
        ind = i
    plt.plot(x,a[i],'red')    
    
for i in range(90,120):
    str = input("")
    a.append([int(l) for l in str.split(' ')])
    if m>mean(a[i]):
        m = mean(a[i])
        ind = i
    plt.plot(x,a[i],'yellow')    
    

plt.show()
print(ind)
