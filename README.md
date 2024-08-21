# Evolution Bio Sim
---
Was inspired by this video [I programmed some creatures. They Evolved](https://www.youtube.com/watch?v=N3tRFayqVtk&t) by David Miller.
And decided to attempt my own version.


# Setup
---
- Visual studios C++ installed, to be able to `pip install .` (I tried setting up mingw32 to compile but it didnt work out).
- Mingw32 installed (optional), to be able to build/run the lib via vscode task (you may change the task to use vs cl.exe).
- Create a python environment `python -m venv env`, activate venv `env/scripts/activate.ps1`, install requirements `pip install -r requirements.txt`.
- Build the lib, `pip install .`, every change made to c++ file a install needs to be ran. If a change to a header file is made it wont be picked up, unless a c++ file is touched.

# Running
---
There are two ways of running, one is to build/run c++ lib/ (you can add breakpoints), second is to `python src/main.py`

# High level design
---
CPP/lib
- Generates random edges
- Do a discovery on the graph,
  - flowing from action to sensor (removes paths that are not connected to a sensor)
  - then flowing from sensor to action (removes paths that are not connected to an action)
  - then cut out any recursive loops (graph is a normal NN/feedforward)
- Create another graph, which is a merge of edge weights
- Create a index array, which discovers nodes in the layers and assigns an index (so that its easy to reference in the feedforward)
- Place the agents on to the grid, and add to producer "queue"
- Run feedforward for the number of epochs * steps
  - Generate new generation of agents for each epoch, by a sorted survival score and extracting a part of each parent's genome
Python/src
- Package has binders to cpp lib
  - Consume from the "queue"
- UI displays each step and epoch

# Config
---
`Grid` - number of columns and rows, [SizeX, SizeY]
`Agent` - max number of agents or individuals that will exist in the grid
`Neurons` - ~max number of neurons (excluding sensors, actions). The more neurons there are the harder it is to connect the genome graph, because when selecting a random source/sink node we dont always form a complete connection from sensor to action.
`Genome` - upper/lower bound for how many genes there can be. However, when wiring the genome, we must trim the useless paths, so we cant guarantee the min size nor the desired size.
`Activation` - during feedforward on the action layer, the node action is only active if it pass the threshold.
`MateTopNPercent` - on a new generation, mate the topN best performing agents in percentage wise ("best" meaning sorted by a survival score). Distance/StepsTaken/RotationsTaken are all part of the survival score and are summed up to 1.0.
`DistanceWeight` - on a new generation, calculate the final distance of agent.
`StepsTakenWeight` - a counter for when an agent moved.
`RotationsTakenWeight` - a counter for when an agent rotated.

# Goals
---
1. Add RNN to compare performance of survival/evolution
2. Add ui genome color visual (from the grid, show how diverse)
3. Add ui graph of diversity
4. Add save/load genome snapshot
5. Add ui genome graph
6. Add more sensors/actions
7. Add ui family tree
8. Add mutations
9. Add game theory concepts
