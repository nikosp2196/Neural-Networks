import numpy as np
import matplotlib.pyplot as plt


x = []
y = []
c = []

with open("train.txt") as f:
    lines = f.readlines()
    for l in lines:
        temp = l.split()
        x.append(float(temp[4]))
        y.append(float(temp[5]))
        if temp[3] == "C1":
            c.append("b")
        elif temp[3] == "C2":
            c.append("g")
        elif temp[3] == "C3":
            c.append("r")
        else:
            c.append("y")
plt.scatter(x,y, color=c)
plt.show()