#!/Library/Frameworks/Python.framework/Versions/3.11/bin/python3      

with open("u238_e6_sf.0041","r") as infile:
    lines = infile.readlines()

import numpy as np

def NBeta(Q,Z,T,pm):
    print(Q,Z,T,pm)
    m_e = 0.511
    E = T + m_e
    p = np.sqrt(E*E-m_e*m_e)
    eta = -pm*Z/p
    F = 2*np.pi*eta/(1.0-np.exp(-2.0*np.pi*eta))
    if (T < Q):
       N_T = F*p*E*(Q-T)*(Q-T)
    else:
       N_T = 0.0
    return N_T

e_val = np.zeros(7000)
spec = np.zeros(7000)

for i in range(len(e_val)):
    e_val[i] = float(i+1)/1000.0
    spec[i] = 1e-15

for line in lines:
    if (line[0] == "G" and line[1] == " "):
        sline = line.strip().split()
        g_nucid = sline[1]
        g_E = float(sline[2])/1000.0
        g_normfact = float(sline[3])
        g_DR = float(sline[4])
        g_RI = float(sline[5])
#        g_NR = float(sline[6])
        g_BR = float(sline[6])
        g_bratio = float(sline[7])
        if (g_bratio == 0.0):
           print(g_nucid,g_E,g_normfact)
        e_idx = 0
        g_NR = 1
        while (e_val[e_idx] < g_E):
            e_idx = e_idx + 1
        e_idx = e_idx - 1
        if (g_bratio > 0.0):
          spec[e_idx] = spec[e_idx] + g_normfact*g_DR*g_RI*g_NR/g_bratio
#    if (line[0] == "B" and line[1] == "-"):
#        sline = line.strip().split()
#        bp_nucid = sline[1]
#        bp_Z = int(sline[2])
#        bp_E = float(sline[3])/1000.0
#        bp_Q = float(sline[4])/1000.0
#        bp_normfact = float(sline[5])
#        bp_DR = float(sline[6])
#        bp_IB = float(sline[7])/100.0
#        bp_BR = float(sline[8])
#        bp_bratio = float(sline[9])
#        e_idx = 0
#        for b_idx in range(len(e_val)-1):
#           spec[b_idx] = spec[b_idx] + 0.01*NBeta(bp_Q,bp_Z,0.5*(e_val[b_idx]+e_val[b_idx+1]),-1.0)*bp_normfact*bp_DR*bp_IB/bp_bratio


import matplotlib.pyplot as plt

plt.step(e_val[0:2800],spec[0:2800],"k-")
plt.yscale("log");
plt.xlabel("Energy (keV)")
plt.ylabel("Relative Intensity")
plt.show()
