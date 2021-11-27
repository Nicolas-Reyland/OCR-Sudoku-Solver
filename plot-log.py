#!/usr/bin/env python3
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter
import sys

assert len(sys.argv) == 3, "Usage: show-log.py loss-log-file rights-log-file"

losses_y = np.array(list(map(lambda x: float(x.strip()), open(sys.argv[1]).readlines())))
rights_y = np.array(list(map(lambda x: float(x.strip()), open(sys.argv[2]).readlines())))
rights_y *= 100.0

losses_x = np.linspace(0.0, losses_y.max(), len(losses_y))
rights_x = np.linspace(0.0, 100.0, len(rights_y))

fig, axes = plt.subplots(nrows=2, ncols=1)
fig.canvas.manager.set_window_title('Model performance')

axes[0].plot(losses_x, losses_y, 'blue', label='loss')
axes[0].legend()

axes[1].plot(rights_x, rights_y, 'red', label='right predictions')
axes[1].legend()
axes[1].yaxis.set_major_formatter(FuncFormatter(lambda y, _: f'{y}%'))

plt.show()

