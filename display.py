import csv
import sys
from matplotlib import pyplot as plt

xvals = []
yvals = []
with open(sys.argv[1]) as datafile:
    reader = csv.reader(datafile)
    for row in reader:
        xvals.append(float(row[0]))
        yvals.append(float(row[1]))

plt.plot(xvals, yvals)
plt.show()