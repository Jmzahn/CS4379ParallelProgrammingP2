import numpy as np
import matplotlib.pyplot as plt

N = 1984
procList = [1, 2, 4, 8, 16, 32]
timeDataMs = [10139.9, 5591.03, 4494.53, 2734.45, 1534.34, 1628.25]

speedUp = []

for time in timeDataMs:
    speedUp.append(timeDataMs[0]/time)

index = np.arange(len(procList))
width = 0.35#arbitrary

plt.figure()
plt.bar(index, timeDataMs, width, color='b', alpha=0.60)
plt.ylabel('Time Ms')
plt.xticks(index, procList)
plt.xlabel('Processors')
plt.title('Time in Ms vs Number of Processors for N:1984\nPthread LU col wise')
plt.show()

plt.figure()
plt.bar(index, speedUp, width, color='b', alpha=0.60)
plt.ylabel('Speed Up')
plt.xticks(index, procList)
plt.xlabel('Processors')
plt.title('Speed Up vs Number of Processors for N:1984\nPthread LU col wise')
plt.show()


timeDataMs = [16660, 13095.7, 11064.8, 6214.14, 3550.41, 2406.38]
speedUp = []

for time in timeDataMs:
    speedUp.append(timeDataMs[0]/time)

plt.figure()
plt.bar(index, timeDataMs, width, color='r', alpha=0.60)
plt.ylabel('Time Ms')
plt.xticks(index, procList)
plt.xlabel('Processors')
plt.title('Time in Ms vs Number of Processors for N:1984\nOMP Guass row wise')
plt.show()

plt.figure()
plt.bar(index, speedUp, width, color='r', alpha=0.60)
plt.ylabel('Speed Up')
plt.xticks(index, procList)
plt.xlabel('Processors')
plt.title('Speed Up vs Number of Processors for N:1984\nOMP Guass row wise')
plt.show()