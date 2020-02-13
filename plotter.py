import numpy as np
from matplotlib import pyplot as plt
data = np.genfromtxt('output.csv', delimiter=',',
                      names=['speed', 'distance', 'high_speed', 'time'])

fig = plt.figure()
ax1 = fig.add_subplot(111)

ax1.set_title("Speed, Distance Covered and time")    
ax1.set_xlabel('Time')
ax1.set_ylabel('Speed/Distance')

ax1.plot(data['time'], data['speed'], color = 'b', label="Speed")
ax1.plot(data['time'], data['distance'], color = 'r', label="Distance")

plt.show()
