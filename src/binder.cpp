#include <pybind11/pybind11.h>

#include "Sim.h"

float cpp_add(float arg1, float arg2) {
    float return_val = arg1 + arg2;
    return return_val;
}

void sim_init()
{
    sim::Sim sim;
    sim.Init();
}

PYBIND11_MODULE(biosim, handle) {
    handle.doc() = "This is documentation";
    handle.def("cpp_add", &cpp_add);
    handle.def("sim_init", &sim_init);
}