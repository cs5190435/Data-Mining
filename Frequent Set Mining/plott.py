import matplotlib.pyplot as plt

import sys

outp = sys.argv[1]

fptime = [0,0,0,0,0]
aptime = [0,0,0,0,0]
with open("times.txt", "r") as file:
    data = file.readlines()
    t1 = data[0].split()
    t2 = data[1].split()

    for i in range(len(t1)):
        fptime[i] = float(t1[i])
    for i in range(len(t2)):
        aptime[i] = float(t2[i])

x = [5, 10, 25, 50, 90]


plt.plot(x, fptime, color = 'r', label = "FP-Tree")
plt.plot(x, aptime, color = 'b', label = "Apriori")

plt.xlabel("Support")
plt.ylabel("Running Times in milli seconds")
plt.title("Plot of Support threshold vs running times for different algorithms")

plt.legend()

plt.savefig(outp + ".png")

