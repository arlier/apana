import numpy as np
import pandas as pd
import matplotlib.pyplot as plt 
from sklearn.linear_model import LinearRegression 
def Least_squares(x,y):
    x_ = x.mean()
    y_ = y.mean()
    m = np.zeros(1)
    n = np.zeros(1)
    k = np.zeros(1)
    p = np.zeros(1)
    for i in np.arange(19):
        k = (x[i]-x_)* (y[i]-y_)
        m += k
        p = np.square( x[i]-x_ )
        n = n + p
    a = m/n
    b = y_ - a* x_
    return a,b
a=[1770,1821,1731,1795,1752,1770,1740,1782,1805,1745,1740,1765,1710,1722,1749,1817,1719,1751,1749]
b=[90,86,90,89,91,90,91,88,89,91,90,89,90,92,91,89,92,90,90]
#reg = LinearRegression().fit([a], [b])
a=np.array(a)/1.
b=np.array(b)/100.
a1,b1 = Least_squares(a,b)
print(a1,b1)
y1 = a1 * a + b1
#print("一元回归方程为:  Y = %.5fX + (%.5f)" % (reg.coef_[0][0], reg.intercept_[0]))
#print("R平方为: %s" % reg.score(a, b))
#plt.plot(a,b,'g-')
#a = np.asarray([a])
#b = np.asarray([b])
#plt.scatter(a, b,  color='black')
#plt.plot(x, reg.predict(x), color='red', linewidth=1)
plt.plot(a, b, 'ro', lw=2, markersize=6)
plt.plot(a, y1, 'r-', lw=2, markersize=6)
#plt.plot(a, reg.predict(a), color='red', linewidth=1)
plt.show()
