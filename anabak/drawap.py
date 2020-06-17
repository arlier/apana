
import numpy as np
import matplotlib.pyplot as plt
apr=[]
apr1=[]
apr2=[]
apr3=[]
xaxis=[]
count=0
for line in open("/home/pmthome/aptest/Analysis_cy/restxt/allapratio.txt"):   
    #print line   
    a=line.split()
    print(a)
    if(float(a[0])<0.001):
       continue
    apr.append(float(a[0]))
    apr1.append(float(a[1]))
    apr2.append(float(a[2]))
    apr3.append(float(a[3]))
    count=count+1
    xaxis.append(count)
print(apr)
print(apr1)
print(apr2)
print(apr3)

#xaxis=np.array(xaxis)
#apr=np.array(apr)
print(xaxis)
print(apr)
fig, ax = plt.subplots()
# Using set_dashes() to modify dashing of an existing line
line0, = ax.plot(xaxis, apr,  'o',lw=0,c='green',label='average ap charge ratio')
line1, = ax.plot(xaxis, apr1, 'o',lw=0,c='yellow',label='ap charge ratio @ light intensity 1')
line2, = ax.plot(xaxis, apr2, 'o',lw=0,c='red',label='ap charge ratio @ light intensity 2')
line3, = ax.plot(xaxis, apr3, 'o',lw=0,c='blue',label='ap charge ratio @ light intensity 3')
plt.ylim(0.001,.2)
#line1.set_dashes([2, 2, 10, 2])  # 2pt line, 2pt break, 10pt line, 2pt break

# Using plot(..., dashes=...) to set the dashing when creating a line
#line2, = ax.plot(x, y - 0.2, dashes=[6, 2], label='Using the dashes parameter')

ax.legend()

plt.rcParams['savefig.dpi'] = 300

plt.xlabel(u'PMT' ) 
plt.ylabel(u'after-pulse ratio' ) 
plt.savefig('test.png')
plt.show()
