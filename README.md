## Example uses of the procedural modeling API

### 01_Cube
Create a cube using extrusion, starting from a single vertex

### 02_Chair
Create a chair

### 03_ParametricChair
Create a program which generates chair variations based on a random seed.

### 04_Airplane
Create a complex airplane mesh, with fuselage, wings, vertical and horizontal stabilizers.

## build and install
### Clone the repo, update library (using git submodules)
`> git clone https://github.com/lsimic/AobaExamples.git`  
`> cd AobaExamples`  
`> git submodule update --remote`  

### Windows specific
Use Visual Studio Community 2019, open and build as CMake project

### LInux specific
`> cd out`  
`> cmake ..`  
`> make`  