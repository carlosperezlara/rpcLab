import matplotlib.pyplot as plt
from derivative import get_numerical_ln_derivative, get_inflection_point
import numpy as np
import pickle as pkl

clean_arr = pkl.load(open("output.p", "rb"))

voltages = []
currents = []

for i in clean_arr:
    voltages.append(i[0])
    currents.append(i[1])


print(voltages[-1])
fig = plt.figure()

ax1 = fig.add_subplot(111)


ax1.scatter(voltages, currents)
ax1.plot(voltages, currents)

sec_points = get_numerical_ln_derivative(voltages, currents)

slopes = []
deriv_x = []

for i in range(len(sec_points)):
    deriv_x.append(sec_points[i][0])
    slopes.append(sec_points[i][1])

#ax1.scatter(deriv_x, slopes)
#ax1.plot(deriv_x, slopes)

plt.show()
