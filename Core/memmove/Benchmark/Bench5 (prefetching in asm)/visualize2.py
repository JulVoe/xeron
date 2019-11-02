import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np

def mean(numbers):
    return float(sum(numbers)) / max(len(numbers), 1)

def hsv_to_rgb(h, s, v):
        if s == 0.0: return (v, v, v)
        i = int(h*6.) # XXX assume int() truncates!
        f = (h*6.)-i; p,q,t = v*(1.-s), v*(1.-s*f), v*(1.-s*(1.-f)); i%=6
        if i == 0: return (v, t, p)
        if i == 1: return (q, v, p)
        if i == 2: return (p, v, t)
        if i == 3: return (p, q, v)
        if i == 4: return (t, p, v)
        if i == 5: return (v, p, q)

m = 10000000000
ind = 0

print("Daten\n")
a = []
x = np.arange(1,129,1)
for i in range(0,28):
    str = input("")
    a.append([float(l) for l in str.split(' ')])
    if m>mean(a[i]):
        m = mean(a[i])
        ind = i
        
for i in range(0,4):
    plt.plot(x,a[i],color=hsv_to_rgb(i/3,1,i/3),label=i)

print(ind)
plt.legend()
plt.show()
