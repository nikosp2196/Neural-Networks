import numpy as np
import matplotlib.pyplot as plt


x = []
y = []
c = []

with open("clustered.txt") as f:
    lines = f.readlines()
    for l in lines:
        temp = l.split()
        x.append(float(temp[1]))
        y.append(float(temp[2]))
        if temp[0] == "1":
            c.append("b")
        elif temp[0] == "2":
            c.append("g")
        elif temp[0] == "3":
            c.append("r")
        elif temp[0] == "4":
            c.append("c")
        elif temp[0] == "5":
            c.append("m")
        elif temp[0] == "6":
            c.append("linen")
        elif temp[0] == "7":
            c.append("violet")
        elif temp[0] == "8":
            c.append("aqua")
        elif temp[0] == "9":
            c.append("yellow")
        elif temp[0] == "0":
            c.append("olive")
        elif temp[0] == "C":
            c.append("black")
        else:
            c.append("grey")
plt.scatter(x,y, color=c)
plt.show()
    
