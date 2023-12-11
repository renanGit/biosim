#define PYBIND11_DETAILED_ERROR_MESSAGES
#include <map>
#include <string>
#include <algorithm>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "Config.h"
#include "Sim.h"

enum Name
{
    Activation=0,
    Grid,
    Agent,
    Genome,
    Neuron
};

std::map<const std::string, Name> nameMap =
{
    {"activation", Name::Activation},
    {"grid", Name::Grid},
    {"agent", Name::Agent},
    {"genome", Name::Genome},
    {"neuron", Name::Neuron}
};

void Setup(const pybind11::dict& configs)
{
    auto valCheck = [](std::string val, const auto& map)
    {
        if (map.find(val) == map.end())
        {
            std::string tmp = "Unhandled config: " + val;
            throw std::exception(tmp.c_str());
        }
    };

    for (const auto& config : configs)
    {
        std::string name = pybind11::cast<std::string>(config.first);
        std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c){ return std::tolower(c); });
        valCheck(name, nameMap);
        
        auto& tmp = pybind11::cast<std::map<const std::string, const std::string>>(config.second);
        switch(nameMap[name])
        {
            case Name::Activation:
                valCheck("Threshold", tmp);
                sim::Config::activationThreshold = std::stof(tmp["Threshold"]);
                break;
            case Name::Grid:
                valCheck("SizeX", tmp);
                valCheck("SizeY", tmp);
                sim::Config::gridSizeX = std::stoi(tmp["SizeX"]);
                sim::Config::gridSizeY = std::stoi(tmp["SizeY"]);
                break;
            case Name::Agent:
                valCheck("Max", tmp);
                sim::Config::maxAgents = std::stoi(tmp["Max"]);
                break;
            case Name::Neuron:
                valCheck("Max", tmp);
                sim::Config::maxNeurons = std::stoi(tmp["Max"]);
                break;
            case Name::Genome:
                valCheck("Max", tmp);
                valCheck("Min", tmp);
                sim::Config::maxGenes = std::stoi(tmp["Max"]);
                sim::Config::minGenes = std::stoi(tmp["Min"]);
                break;
        }
    }
}

PYBIND11_MODULE(biosim, handle)
{
    handle.doc() = "This is documentation";
    handle.def("cpp_add", [](float a, float b) { return a + b; });
    handle.def("Setup", &Setup);
    handle.def("PrintConfig", &sim::Config::to_string);
    pybind11::class_<sim::Sim>(handle, "Sim")
        .def(pybind11::init<>())
        .def("Init", &sim::Sim::Init)
        .def("Run", &sim::Sim::Run)
        .def("CanPollMovement", &sim::Sim::CanPollMovement)
        .def("ConsumeMovement", &sim::Sim::ConsumeMovement);
}