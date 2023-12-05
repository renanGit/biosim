import yaml
import os

import biosim


if __name__ == "__main__":
    x = 5
    y = 6
    print(f"Adding {x} and {y} together.")
    z = biosim.cpp_add(x, y)
    print(f"Result is {z}")
    
    biosim.PrintConfig()
    with open(os.path.join(os.path.dirname(os.path.realpath(__file__)), 'config.yml'), 'r') as f:
        yml = yaml.safe_load(f)
        print(type(yml), yml)
        biosim.Setup(yml)
    
    sim = biosim.Sim()
    sim.Init()
    biosim.PrintConfig()