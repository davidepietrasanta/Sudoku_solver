# Sudoku_solver
A simple C ++ class for solving sudoku.

The `main.cpp` file contains the test functions performed to verify the correctness of the program.

The `Sudoku.hpp` is the library that actually contains the code for solving the sudoku.

## Simple Tutorial

``` cpp
#include "#include "Sudoku.hpp"
#include <iostream>

int main(){
  Sudoku s;
  
  s.set(value, x-coordinate, y-coordinate); //To insert the data in the table.
  //s.set(5, 1, 1) will insert the value '5' in the cell (1,1), so the first cell.
  
  std::cout << s << std::endl; //To display the unsolved sudoku.
  
  //To solve the sudoku you have 2 options s.solve(); and s.brute_force();
  //s.solve(); Try to simplify the problem first. If it can't solve it use brute force.
  //s.brute_force(); Solve using brute_force.
  
  s.solve();
  std::cout << s << std::endl; //To display the solved sudoku.
  
}
```

If you have any doubts or curiosity, look at the `main.cpp` file, or contact me.



