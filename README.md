# Robot Cleaning Simulator

## Project Description 

This project allows users to simulate autonomous floor cleaning in a grid-based environment. 

By testing different combinations of starting positions and navigation algorithms, users can analyse performance metrics and determine the most efficient path for a given floor plan. 

The simulator also supports map validation, step-by-step robot movement, and simulation history tracking.

--------------------------------------------------------------------------------------------------

## Objectives

The main objectives of this project are to:

- simulate robotic cleaning behaviour in a grid-based environment
- demonstrate the use of object-oriented programming concepts in C++
- implement multiple cleaning strategies for comparison
- validate user-provided map files and handle input errors robustly
- track simulation performance through energy usage and cleaning history

--------------------------------------------------------------------------------------------------

## Basic Usage

1) Launch the program.
2) Load a valid map file.
3) Choose whether to show the layout or run a simulation.
4) Select a dock position, cleaning algorithm, and buffer time.
5) View history or save the simulation report if needed.

--------------------------------------------------------------------------------------------------

## Example Map

`##########` <br>
`#   O    #` <br>
`# XX O   #` <br>
`#   O    #` <br>
`##########` <br>

Legend: <br>
- `#` = wall
- `X` = obstacle
- `O` = dirt
- [space] = empty walkable cell

- map cannot be empty
- map must be rectangular
- map cannot contain empty lines
- map must only contain valid symbols
- map must have at least one walkable cell

--------------------------------------------------------------------------------------------------

## How to Compile and Run

### Compile
Compile all source files together using a C++ compiler such as `g++`.

(Copy into Terminal)
g++ Main.cpp Simulator.cpp Map.cpp BCDStrategy.cpp FrontierStrategy.cpp NearestCellStrategy.cpp -o simulator

### Run
(Copy into Terminal)
./simulator

--------------------------------------------------------------------------------------------------

## Energy Usage by Robot Mode

The simulator differentiates energy usage by assigning a specific energy cost to each robot mode. <br> **Default Mode** is used for normal movement and cleaning on non-dirty cells. <br>
**High Power Mode** is used when cleaning dirt cells. <br>
**Navigation Mode** is used when the robot is travelling, repositioning, or returning to the dock. <br>

**Navigation Mode** : 1 Unit of Energy <br>
**Default Mode**    : 2 Units of Energy <br>
**High Power Mode** : 3 Units of Energy <br>
 
Since each mode consumes a different amount of energy, the total energy used in a run reflects not just the distance travelled, but also the type of actions performed during cleaning. 

This makes the simulation more realistic and allows users to compare the efficiency of different algorithms and starting positions more meaningfully.


### Performance Comparison

The efficiency of each simulation run can be differentiated using the recorded performance metrics. The primary metric is **energy used**, where lower energy consumption indicates a more efficient run. 

Users can compare this together with the **starting dock position**, **selected algorithm**, and **dirt cleaned** to evaluate which combination performs best on a given map. 

The simulator further supports this by identifying the **best efficiency by map** in the simulation history report.

--------------------------------------------------------------------------------------------------

## Limitations

Although the simulator meets the project requirements, it has some limitations:

- it is console-based and does not provide a graphical interface
- movement is limited to four directions
- only three cleaning algorithms are implemented
- the environment is static, with no moving obstacles
- performance may become slower for very large maps

--------------------------------------------------------------------------------------------------

## Authors 

(Name / Student ID)

1) TEO RUI ZHE JOEL / 2500886
2) IAN LIM YU ZE / 2501023
3) TORDESILLAS JOHN REIU FEDERICO / 2503429
4) GOH CHENG HAO / 2500214
5) SHAWN TAN SI AN / 2500956 
6) EUGENE LEE ZONG JING / 2501310
