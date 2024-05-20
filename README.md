# README

## Sudoku Solution Validator: Producer Consumer Problem

By Ryan Mackintosh

### Directory Structure

```
OS/
├── Assignment/                 # Directory containing all source files and headers
│   ├── Solutions/                      # Directory containing sample solution files
│   │   ├── solution
│   │   ├── solution1
│   │   ├── solution2
│   │   ├── solution3
│   │   └── solution4
│   │
│   ├── Makefile                        # Makefile for building the project
│   ├── mssv.c                          # Main source file containing the entry point and thread management
│   ├── sudoku_setup.c                  # Memory management for Sudoku Solution Validator
│   ├── sudoku_setup.h                  # Header for memory management
│   ├── sudoku_solution_file_reader.c   # File IO management for reading Sudoku solutions
│   └── sudoku_solution_file_reader.h   # Header for file IO management
│
├── docs/                       # Documentation and additional notes
│   ├── SampleData/                     # Sample data for report
│   │   ├── InvalidSolution.JPG
│   │   ├── InvalidSolutionInput.JPG
│   │   ├── ValidSolution.JPG
│   │   └── ValidSolutionInput.JPG
│   │ 
│   ├── declaration_of_originality.pdf  # Originality declaration
│   └── Report.md                       # Project report
│
└── README.md                   # Instructions on building and running the program
 ```

### Build Instructions and Usege

1. Make sure you are in `OS/Assignment` to build.
2. Run `make` to build the project.
3. Run the program with `./mssv Solutions/<solution_file> <delay>`.
   - `<solution_file>` is file name that contains the sudoku e.g. `solution2`
   - `<delay>` is the delay in seconds (must be between 1 and 10).
4. Run `make clean` to remove the compiled files.

    ```
    Run `make clean && make` for convenience and assurance of a clean build every time.
    ```
   
### File Descriptions

- **mssv.c**: Main program file that initializes threads and manages Sudoku validation. 
- **sudoku_setup.c/h**: Functions for initializing and freeing shared memory structures. 
- **sudoku_solution_file_reader.c/h**: Functions for reading Sudoku solutions from files.

### Additional Documentation

- **Report.md**: Contains the project report.
- **declaration_of_originality.pdf**

### Resources

I have used knowledge gained from UCP and OS to write the majority of this program.
Where this is not the case, I have commented the source in the code. 