#pragma once

#include <map>
#include <string>

enum Name
{
    Activation=0,
    Grid,
    Agent,
    Genome,
    Neuron,
    MateTopNPercent,
    MinDominantCutPercent,
    DistanceWeight,
    StepsTakenWeight
};

const std::map<const std::string, Name> nameMap =
{
    {"activation", Name::Activation},
    {"grid", Name::Grid},
    {"agent", Name::Agent},
    {"genome", Name::Genome},
    {"neuron", Name::Neuron},
    {"matetopnpercent", Name::MateTopNPercent},
    {"mindominantcutpercent", Name::MinDominantCutPercent},
    {"distanceweight", Name::DistanceWeight},
    {"stepstakenweight", Name::StepsTakenWeight}
};