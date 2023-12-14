import yaml
import os

import numpy as np
from matplotlib.animation import FuncAnimation
import matplotlib.pyplot as plt
import matplotlib.animation as animation

import biosim


if __name__ == "__main__":
    x = 5
    y = 6
    print(f"Adding {x} and {y} together.")
    z = biosim.cpp_add(x, y)
    print(f"Result is {z}")
    
    # biosim.PrintConfig()
    config = None
    with open(os.path.join(os.path.dirname(os.path.realpath(__file__)), 'config.yml'), 'r') as f:
        yml = yaml.safe_load(f)
        config = yml
        biosim.Setup(yml)

    sim = biosim.Sim()
    sim.Init()
    steps = 100
    sim.Run(steps, 1)
    # biosim.PrintConfig()
    # i = 0
    # while sim.CanPollMovement():
    #     pos = sim.ConsumeMovement()
    #     print(i, pos.coordX, pos.coordY, pos.direction)
    #     i += 1
    
    sizeX = int(config["Grid"]["SizeX"])
    sizeY = int(config["Grid"]["SizeY"])
    xdata, ydata = [], []
    def AnimationFunction(frame):
        frameComplete = False
        while sim.CanPollMovement() and not frameComplete:
            pos = sim.ConsumeMovement()
            frameComplete = pos.coordX > sizeX and pos.coordY > sizeY
            if not frameComplete:
                xdata.append(pos.coordX)
                ydata.append(pos.coordY)
        if len(xdata) > 0:
            scatter.set_offsets(np.stack([xdata, ydata]).T)
        xdata.clear()
        ydata.clear()
        return scatter,
    
    fig, ax = plt.subplots()
    scatter = ax.scatter(-1,-1, s=1)
    ax.grid()
    plt.xlim(0, sizeX)
    plt.ylim(0, sizeY)
    ani = FuncAnimation(fig, AnimationFunction, frames=steps, interval=200)
 
    writer = animation.PillowWriter(fps=5, metadata=dict(artist='Me'), bitrate=1800)
    ani.save('src/sim.gif', writer=writer)
    # plt.show()