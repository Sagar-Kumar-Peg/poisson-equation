

import csv
import sys
from mpl_toolkits.mplot3d import Axes3D  # noqa: F401
import matplotlib.pyplot as plt
import numpy as np

filename = "volt_mesh.csv"  

xs, ys, Vs = [], [], []

try:
    with open(filename, newline="") as f:
        reader = csv.reader(f)
        next(reader, None)  # skip header
        for row in reader:
            if len(row) < 3:
                continue
            xs.append(float(row[0]))
            ys.append(float(row[1]))
            Vs.append(float(row[2]))
except OSError as e:
    print("Failed to open", filename, ":", e)
    sys.exit(1)

if not xs:
    print("No data in", filename)
    sys.exit(1)

xs = np.array(xs)
ys = np.array(ys)
Vs = np.array(Vs)

m = int(xs.max() - xs.min() + 1)
n = int(ys.max() - ys.min() + 1)

X = xs.reshape((m, n))
Y = ys.reshape((m, n))
Z = Vs.reshape((m, n))

fig = plt.figure()
ax = fig.add_subplot(111, projection="3d")
surf = ax.plot_surface(X, Y, Z, cmap="viridis")

ax.set_xlabel("x")
ax.set_ylabel("y")
ax.set_zlabel("V")

fig.colorbar(surf, shrink=0.5, aspect=5)
plt.show()
