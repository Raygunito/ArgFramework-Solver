# ArgFramework-Solver

## Overview

This project is used to solve Abstract Argumentation Framework (AAF). 
It computes extensions and determines argument acceptability under complete 
and stable semantics.

## Installation

1. Clone the repository or download the source code.
```bash
git clone https://github.com/Raygunito/ArgFramework-Solver.git
cd ArgFramework-Solver
```
2. Compile the code with the provided `Makefile`.
```bash
make
```
3. Run the `ArgSolver` executable with the appropriate options.

## Usage

The input file must be in the aspertix format : 

```
arg(name_argument).
```

to declare a argument, and
```
att(name_argument_1,name_argument_2).
```
to declare an attack.

### Running the solver : 
```
./ArgSolver -p <problem_type> -f <file_path> [-a <argument>]
```

Options available :

- SE-CO : Give a complete extension of the argumentation framework.

- DC-CO : Determine if \<argument> belongs to a complete extension.

- DS-CO : Determine if \<argument> belongs to all complete extensions.

- SE-ST : Give a stable extension of the argumentation framework or NO.

- DC-ST : Determine if \<argument> belongs to a stable extension.

- DS-ST : Determine if \<argument> belongs to all stable extensions.

## Example

For this input file `simple_af.apx` containing : 
```
arg(a).
arg(b).
arg(c).
att(a,b).
att(b,c).
```

To compute a complete extension : 
```
./ArgSolver -p SE-CO -f simple_af.apx

output: [a,c]
```

Or determine if argument `a` belongs to all skeptical extensions :
```
./ArgSolver -p DS-ST -f simple_af.apx

output: YES
```

## Author

[@Raygunito aka Laurent](https://github.com/Raygunito)
