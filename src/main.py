import biosim


if __name__ == "__main__":

    x = 5
    y = 6
    print(f"Adding {x} and {y} together.")
    z = biosim.cpp_add(x, y)
    print(f"Result is {z}")
    biosim.sim_init()