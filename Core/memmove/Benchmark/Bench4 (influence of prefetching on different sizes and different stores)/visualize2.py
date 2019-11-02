import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np

def mean(numbers):
    return float(sum(numbers)) / max(len(numbers), 1)

m = 10000000000
ind = 0

print("Daten\n")
a = []
x = np.arange(1,258,1)
for i in range(0,60):
    str = input("")
    a.append([float(l) for l in str.split(' ')])
    if m>mean(a[i]):
        m = mean(a[i])
        ind = i


for i in range(60,90):
    str = input("")
    a.append([float(l) for l in str.split(' ')])
    if m>mean(a[i]):
        m = mean(a[i])
        ind = i  
    
for i in range(90,120):
    str = input("")
    a.append([float(l) for l in str.split(' ')])
    if m>mean(a[i]):
        m = mean(a[i])
        ind = i   

for i in range(0,60):
    plt.plot(x,a[i],'blue')
    print(i)
for i in range(60,90):
    plt.plot(x,a[i],'red')
    print(i)
for i in range(90,120):  
    plt.plot(x,a[i],'yellow')
    print(i)
    

plt.show()
print(ind)
