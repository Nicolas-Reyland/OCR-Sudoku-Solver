#!/usr/bin/env python3
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter
import sys

assert len(sys.argv) == 2, "Usage: show-log.py file-infix"

avg_loss_file = f"logs/avg-loss-{sys.argv[1]}.log"
avg_right_file = f"logs/avg-right-{sys.argv[1]}.log"

losses_y = np.array(list(map(lambda x: float(x.strip()), open(avg_loss_file).readlines())))
rights_y = np.array(list(map(lambda x: float(x.strip()), open(avg_right_file).readlines())))
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

