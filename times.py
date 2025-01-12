#!/bin/python
import numpy as np
import matplotlib.pyplot as plt

# collected averaged times for each day:
times = [
    (100., 54., 5.),        # day 01
    (-1., -1., 350.),       # day 02
    (-1., 150., 50.),       # day 03
    (54., -1.,  203.),      # day 04
    (-1., -1., 2854.),      # day 05
    (57., 240., 350439.),   # day 06
    (-1., -1., 171213.),    # day 07
    (70., -1., 100.),       # day 08
    (1100., 280., 42500.),  # day 09
    (40., -1., 90.),        # day 10
    (26., -1., 6250.),      # day 11
    (65., -1., 900.),       # day 12
    (-1., -1., 157.),       # day 13
    (-1., 130., 150.),      # day 14
    (100., 250., 500.),     # day 15
    (60., -1., 4630.),      # day 16
    (-1., -1., 58.),        # day 17
    (650., 218., 269.),     # day 18
    (100., -1., 74450.),    # day 19
    (100., -1., 78000.),    # day 20
    (-1., -1., 120.),       # day 21
    (-1., -1., 220000.),    # day 22
    (250., 202., 3500.),    # day 23
    (-1., 155., 492.),      # day 24
    (400., 600., -1.)       # day 25
]

x = np.arange(1, 26)
y = []
yload = []
ypart1 = []
ypart2 = []
for i, t in enumerate(times):
    ty = 0.
    for j in range(0, 3):
        ty = ty + t[j] if t[j] >= 0. else 0.
    yload.append(t[0] if t[0] >= 0. else 0.)
    ypart1.append(t[1] if t[1] >= 0. else 0.)
    ypart2.append(t[2] if t[2] >= 0. else 0.)
    y.append(ty)

plt.figure(figsize=(12,7))
plt.bar(x, yload, label='After parsing')
plt.bar(x, ypart1, bottom=yload, label='After part 1')
plt.bar(x, ypart2, bottom=np.array(yload)+np.array(ypart1), label='After part 2')
plt.legend(frameon=True)
plt.grid(True, which="both", color='black', linestyle='--', linewidth=1, alpha=0.05)
ax= plt.gca()
ax.patch.set_alpha(0.2)
ax.patch.set_facecolor("tab:blue")
plt.xlabel(r'Day')
plt.ylabel(r'Time (µs)')
plt.yscale("log")
plt.hlines(1, x[0], x[-1])
vx= 26.
plt.hlines(100, 0, vx, ls='--')
plt.hlines(1000, 0, vx, ls='--')
plt.hlines(10000, 0, vx, ls='--')
plt.hlines(100000, 0, vx, ls='--')
plt.hlines(1000000, 0, vx, ls='--')
plt.text(vx, 100, '100 µs')
plt.text(vx, 1000, '1 ms')
plt.text(vx, 10000, '10 ms')
plt.text(vx, 100000, '100 ms')
plt.tight_layout()
plt.savefig("times.pdf")
plt.savefig("times.png", facecolor="white", dpi=300)
plt.show()

