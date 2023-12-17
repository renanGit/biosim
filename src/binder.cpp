#define PYBIND11_DETAILED_ERROR_MESSAGES

#include <algorithm>
#include <functional>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "binder.h"
#include "Config.h"
#include "Sim.h"

void SetIntVal(std::string val, const std::map<const std::string, const std::string>& map, int& field)
{
    if (map.find(val) != map.end())
    {
        field = std::stoi(map.at(val));
    }
}
void SetFloatVal(std::string val, const std::map<const std::string, const std::string>& map, float& field)
{
    if (map.find(val) != map.end())
    {
        field = std::stof(map.at(val));
    }
}

void Setup(const pybind11::dict& configs)
{
    for (const auto& config : configs)
    {
        std::string name = pybind11::cast<std::string>(config.first);
        std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c){ return std::tolower(c); });
        if (nameMap.find(name) == nameMap.end())
        {
            continue;
        }

        const auto& tmp = pybind11::cast<std::map<const std::string, const std::string>>(config.second);
        switch(nameMap.at(name))
        {
            case Name::Activation:
                SetFloatVal("Threshold", tmp, sim::Config::activationThreshold);
                break;
            case Name::Grid:
                SetIntVal("SizeX", tmp, sim::Config::gridSizeX);
                SetIntVal("SizeY", tmp, sim::Config::gridSizeY);
                break;
            case Name::Agent:
                SetIntVal("Max", tmp, sim::Config::maxAgents);
                break;
            case Name::Neuron:
                SetIntVal("Max", tmp, sim::Config::maxNeurons);
                break;
            case Name::Genome:
                SetIntVal("Max", tmp, sim::Config::maxGenes);
                SetIntVal("Min", tmp, sim::Config::minGenes);
                break;
            case Name::MateTopNPercent:
                SetFloatVal("TopN", tmp, sim::Config::mateTopNPercent);
                break;
            case Name::MinDominantCutPercent:
                SetFloatVal("Min", tmp, sim::Config::minDominantCutPercent);
                break;
            case Name::DistanceWeight:
                SetFloatVal("Weight", tmp, sim::Config::distanceWeight);
                break;
            case Name::StepsTakenWeight:
                SetFloatVal("Weight", tmp, sim::Config::stepsTakenWeight);
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
    
    pybind11::enum_<sim::Direction>(handle, "Direction")
        .value("W", sim::Direction::W).value("NW", sim::Direction::NW)
        .value("N", sim::Direction::N).value("NE", sim::Direction::NE)
        .value("E", sim::Direction::E).value("SE", sim::Direction::SE)
        .value("S", sim::Direction::S).value("SW", sim::Direction::SW)
        .export_values();

    pybind11::class_<sim::Position>(handle, "Position")
        .def_readwrite("coordX", &sim::Position::coordX)
        .def_readwrite("coordY", &sim::Position::coordY)
        .def_readwrite("direction", &sim::Position::dir);
    
    pybind11::class_<sim::Sim>(handle, "Sim")
        .def(pybind11::init<>())
        .def("Init", &sim::Sim::Init)
        .def("Run", &sim::Sim::Run)
        .def("CanPollMovement", &sim::Sim::CanPollMovement)
        .def("ConsumeMovement", &sim::Sim::ConsumeMovement, pybind11::return_value_policy::reference);
}