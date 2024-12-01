#!/Library/Frameworks/Python.framework/Versions/3.11/bin/python3
def jendltofloat(str):
    jj = 0
    while (str[jj] != '+' and str[jj] != '-'):
        jj = jj + 1
    return float(str[0:jj]+'E'+str[jj+1:])

import numpy as np
import periodic_table as pt

with open("jendl_092_U_238_SF.txt","r") as infile:
    lines = infile.readlines()

ll = 0
nll = len(lines)
while ("8454    3" not in lines[ll]):
    ll = ll + 1
nn = 0
www = np.zeros(nll)
xxx = np.zeros(nll)
yyy = np.zeros(nll)
zzz = np.zeros(nll)
nnn = []
lll = 0
while ("99999" not in lines[ll]):
    line1 = lines[ll]
    line2 = lines[ll+1]
    s1 = line1[0:11]
    s2 = line1[12:22]
    s3 = line1[23:33]
    s4 = line1[34:44]
    s1v = int(jendltofloat(s1))
    s2v = jendltofloat(s2)
    s3v = jendltofloat(s3)
    s4v = jendltofloat(s4)
    s5 = line1[45:55]
    s6 = line1[56:66]
    s7 = line2[0:11]
    s8 = line2[12:22]
    s5v = int(jendltofloat(s5))
    s6v = jendltofloat(s6)
    s7v = jendltofloat(s7)
    s8v = jendltofloat(s8)
    s9 = line2[23:33]
    s10 = line2[34:44]
    s11 = line2[45:55]
    s12 = line2[56:66]
    s9v = int(jendltofloat(s9))
    s10v = jendltofloat(s10)
    s11v = jendltofloat(s11)
    s12v = jendltofloat(s12)
    www[lll] = s1v
    xxx[lll] = s2v
    yyy[lll] = s3v
    zzz[lll] = s4v
    www[lll+1] = s5v
    xxx[lll+1] = s6v
    yyy[lll+1] = s7v
    zzz[lll+1] = s8v
    www[lll+2] = s9v
    xxx[lll+2] = s10v
    yyy[lll+2] = s11v
    zzz[lll+2] = s12v
    if (int(s1v) % 1000 < 10):
      nnn.append("  {0}".format(int(s1v) % 1000)+pt.PT_array[int(s1v/1000)-1])
    elif (int(s1v) % 1000 < 100):
      nnn.append(" {0}".format(int(s1v) % 1000)+pt.PT_array[int(s1v/1000)-1])
    else:
      nnn.append("{0}".format(int(s1v) % 1000)+pt.PT_array[int(s1v/1000)-1])
    if (int(s5v) % 1000 < 10):
      nnn.append("  {0}".format(int(s5v) % 1000)+pt.PT_array[int(s5v/1000)-1])
    elif (int(s5v) % 1000 < 100):
      nnn.append(" {0}".format(int(s5v) % 1000)+pt.PT_array[int(s5v/1000)-1])
    else:
      nnn.append("{0}".format(int(s5v) % 1000)+pt.PT_array[int(s5v/1000)-1])
    if (int(s9v) % 1000 < 10):
      nnn.append("  {0}".format(int(s9v) % 1000)+pt.PT_array[int(s9v/1000)-1])
    elif (int(s9v) % 1000 < 100):
      nnn.append(" {0}".format(int(s9v) % 1000)+pt.PT_array[int(s9v/1000)-1])
    else:
      nnn.append("{0}".format(int(s9v) % 1000)+pt.PT_array[int(s9v/1000)-1])
    if (yyy[lll] > 1e6): 
       print("{0} {1:3.1f} {2:.6e}".format(nnn[lll],xxx[lll],yyy[lll]))
    if (yyy[lll+1] > 1e6):        
       print("{0} {1:3.1f} {2:.6e}".format(nnn[lll+1],xxx[lll+1],yyy[lll+1]))
    if (yyy[lll+2] > 1e6):   
       print("{0} {1:3.1f} {2:.6e}".format(nnn[lll+2],xxx[lll+2],yyy[lll+2]))
    lll = lll + 3
    ll = ll + 2
    nn = nn + 1

import matplotlib.pyplot as plt

plt.semilogy(www,xxx,"ko")
#plt.semilogy(www,yyy,"ro")
#plt.semilogy(www,zzz,"bo")
plt.show()
