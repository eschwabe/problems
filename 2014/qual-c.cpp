// Problem C. Minesweeper Master
// https://code.google.com/codejam/contest/2974486/dashboard#s=p2

#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

typedef std::vector< std::vector<int> > Puzzle;

enum CellType {
    MINE = -1
};

// read a line from stdin
std::stringstream ReadLine() {
    std::string tmp;
    std::getline(std::cin, tmp);
    return std::stringstream(tmp);
}

// count the number of adjacent free and mine cells
std::tuple<int, int> CountAdjacent(
        int row,
        int col,
        Puzzle& grid) {

    auto rows = static_cast<int>(grid.size());
    auto cols = static_cast<int>(grid[0].size());
    int mine_cells = 0;
    int free_cells = 0;

    for(int r = std::max(row-1,0); r <= std::min(row+1, rows-1); ++r) {
        for(int c = std::max(col-1,0); c <= std::min(col+1, cols-1); ++c) {
            if(r == row && c == col) {
                continue;
            } else if(grid[r][c] == MINE) {
                mine_cells++;
            } else if(grid[r][c] != MINE) {
                free_cells++;
            }
        }
    }

    return std::make_tuple(free_cells, mine_cells);
}

// place new mine on grid and update stats
void PlaceMine(
        int row,
        int col,
        Puzzle& grid) {

    auto rows = static_cast<int>(grid.size());
    auto cols = static_cast<int>(grid[0].size());

    for(int r = std::max(row-1,0); r <= std::min(row+1, rows-1); ++r) {
        for(int c = std::max(col-1,0); c <= std::min(col+1, cols-1); ++c) {
            if(r == row && c == col) {
                grid[row][col] = MINE;
            } else if(grid[r][c] != MINE) {
                grid[r][c]++;
            }
        }
    }
}

// solve minesweeper puzzle
// return true if solvable, false if not solvable
void GeneratePuzzle(int mines, Puzzle& grid) {

    auto rows = static_cast<int>(grid.size());
    auto cols = static_cast<int>(grid[0].size());

    for(int m = 0; m < mines; ++m) {

        // scan for new mine position with least impact
        int target_row = -1;
        int target_col = -1;
        auto target_stats = std::make_tuple(9,9);

        for(int row = 0; row < rows; ++row) {
            for(int col = 0; col < cols; ++col) {

                // count adjacent cells
                if(grid[row][col] != MINE) {
                    auto stats = CountAdjacent(row, col, grid);

                    // if fewer adjacent free cells or equal free cells and more adjacent mine cells
                    // set as target to mine
                    if( std::get<0>(stats) < std::get<0>(target_stats) ||
                        (std::get<0>(stats) == std::get<0>(target_stats) && 
                         std::get<1>(stats) > std::get<1>(target_stats)) ) {
                        target_stats = stats;
                        target_row = row;
                        target_col = col;
                    }
                }

            }
        }

        // set new free cell
        PlaceMine(target_row, target_col, grid);
    }
}

// check if puzzle solvable in a single click
bool TestPuzzle(int mines, Puzzle& grid) {

    bool solvable = true;
    auto rows = static_cast<int>(grid.size());
    auto cols = static_cast<int>(grid[0].size());

    // check if only a single mine
    if(rows*cols != mines+1) {

        // check each cell
        for(int row = 0; row < rows; ++row) {
            for(int col = 0; col < cols; ++col) {

                if( grid[row][col] != MINE ) {
                    solvable = false;
                    // test if cell has an adjacent cell with no surrounding mines
                    for(int r = std::max(row-1,0); r <= std::min(row+1, rows-1); ++r) {
                        for(int c = std::max(col-1,0); c <= std::min(col+1, cols-1); ++c) {
                            if(grid[r][c] == 0) {
                                solvable = true;
                            }
                        }
                    }

                    // puzzle not solvable if any cell not next to 0 free cell
                    if(!solvable) {
                        return solvable;
                    }
                }
            }
        }

    }

    return solvable;
}

// print puzzle contents
void PrintPuzzle(Puzzle& grid) {
    auto clicked = false;
    for(auto row : grid) {
        for(auto cell : row) {
            if(cell == MINE) {
                std::cout << "*";
            } else if(cell == 0 && !clicked) {
                std::cout << "c";
                clicked = true;
            }
            else {
                //std::cout << ".";
                std::cout << cell;
            }
        }
        std::cout << std::endl;
    }
}

int main()
{
    int num_tests = 0;
    std::stringstream line = ReadLine();
    line >> num_tests;

    for(int test = 1; test <= num_tests; ++test) {
        int rows, cols, mines = 0;

        line = ReadLine();
        line >> rows >> cols >> mines;

        // initialize grid with free cells and clicked cell
        auto grid = Puzzle(rows, std::vector<int>(cols, 0));

        // solve puzzle
        GeneratePuzzle(mines, grid);
        bool result = TestPuzzle(mines, grid);
        std::cout << "Case #" << test << ":" << std::endl;
        if(result) {
            std::cout << "Solved" << std::endl;
            PrintPuzzle(grid);
        } else {
            std::cout << "Impossible" << std::endl;
            PrintPuzzle(grid);
        }

    }

    return 0;
}
