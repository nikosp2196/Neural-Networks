import numpy as np
import matplotlib.pyplot as plt


x = []
y = []
c = []

with open("groups.txt") as f:
    lines = f.readlines()
    for l in lines:
        temp = l.split()
        x.append(float(temp[1]))
        y.append(float(temp[2]))
        if temp[0] == "T1":
            c.append("b")
        elif temp[0] == "T2":
            c.append("g")
        elif temp[0] == "T3":
            c.append("r")
        elif temp[0] == "T4":
            c.append("c")
        elif temp[0] == "T5":
            c.append("m")
        else:
            c.append("y")
plt.scatter(x,y, color=c)
plt.show()
    
