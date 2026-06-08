#include <bits/stdc++.h>
#include <cmath>
using namespace std;

const int FINDER_SIDE = 7;
const int INNER_FINDER_SIDE = 3;
const int ALIGNMENT_SIDE = 5;
const int INNER_ALIGNMENT_SIDE = 1;

void create_rect(vector<vector<int>> &grid, int value, int side,
                 int left_corner_row, int left_corner_col) {
  for (int i = left_corner_row; i < left_corner_row + side; i++)
    for (int j = left_corner_col; j < left_corner_col + side; j++)
      grid[i][j] = value;
}

void add_finder_patterns(vector<vector<int>> &grid, int module_number) {
  // Top Left Finder
  int row = 0;
  int col = 0;
  create_rect(grid, 1, FINDER_SIDE + 1, row, col);
  create_rect(grid, 0, FINDER_SIDE, row, col);
  create_rect(grid, 1, FINDER_SIDE - 2, row + 1, col + 1);
  create_rect(grid, 0, INNER_FINDER_SIDE, row + 2, col + 2);

  // Top Right Finder
  row = 0;
  col = module_number - FINDER_SIDE;
  create_rect(grid, 1, FINDER_SIDE + 1, row, col - 1);
  create_rect(grid, 0, FINDER_SIDE, row, col);
  create_rect(grid, 1, FINDER_SIDE - 2, row + 1, col + 1);
  create_rect(grid, 0, INNER_FINDER_SIDE, row + 2, col + 2);

  // Bottom Left Finder
  row = module_number - FINDER_SIDE;
  col = 0;
  create_rect(grid, 1, FINDER_SIDE + 1, row - 1, col);
  create_rect(grid, 0, FINDER_SIDE, row, col);
  create_rect(grid, 1, FINDER_SIDE - 2, row + 1, col + 1);
  create_rect(grid, 0, INNER_FINDER_SIDE, row + 2, col + 2);
}

void add_timing_pattern(vector<vector<int>> &grid, int module_number) {
  // Horizontal Timer
  bool white = true;
  for (int j = FINDER_SIDE; j < module_number - FINDER_SIDE; j++) {
    grid[FINDER_SIDE - 1][j] = int(white);
    white = !white;
  }

  // Vertical Timer
  white = true;
  for (int i = FINDER_SIDE; i < module_number - FINDER_SIDE; i++) {
    grid[i][FINDER_SIDE - 1] = int(white);
    white = !white;
  }
}

void add_format_info(vector<vector<int>> &grid,
                     vector<int> reversed_info_pattern, int module_number) {
  // Finder Pattern 1
  int k = 0;
  for (int j = 0; j < FINDER_SIDE + 2; j++) {
    if (grid[FINDER_SIDE + 1][j] == 0)
      continue;
    grid[FINDER_SIDE + 1][j] = reversed_info_pattern[k];
    k++;
  }

  for (int i = FINDER_SIDE + 1; i >= 0; i--) {
    if (grid[i][FINDER_SIDE + 1] == 0)
      continue;
    grid[i][FINDER_SIDE + 1] = reversed_info_pattern[k];
    k++;
  }

  // Finder Pattern 3
  k = 0;
  for (int i = module_number - 1; i >= module_number - FINDER_SIDE - 1; i--) {
    if (i == module_number - FINDER_SIDE - 1) {
      grid[i][FINDER_SIDE + 1] = 0;
      break;
    }
    grid[i][FINDER_SIDE + 1] = reversed_info_pattern[k];
    k++;
  }

  // Finder Pattern 2
  for (int j = module_number - FINDER_SIDE - 1; j < module_number; j++) {
    grid[FINDER_SIDE + 1][j] = reversed_info_pattern[k];
    k++;
  }
}

void add_alignment_pattern(vector<vector<int>> &grid, int left_corner_row = 16,
                           int left_corner_col = 16) {
  create_rect(grid, 0, ALIGNMENT_SIDE, left_corner_row, left_corner_col);
  create_rect(grid, 1, ALIGNMENT_SIDE - 2, left_corner_row + 1,
              left_corner_col + 1);
  create_rect(grid, 0, INNER_ALIGNMENT_SIDE, left_corner_row + 2,
              left_corner_col + 2);
}

void add_data_pattern(vector<vector<int>> &grid, string data, int n) {

  int k = 0;
  int col = n - 1;
  bool up = true;

  while (col > 0 && k < data.size()) {
    if (col == 6)
      col--; // skip timing column

    int row = up ? n - 1 : 0;

    while (row >= 0 && row < n && k < data.size()) {
      for (int c = 0; c < 2; c++) {
        int j = col - c;
        if (j < 0)
          continue;
        if (grid[row][j] == 2) {
          grid[row][j] = 7 - int(data[k] - '-');
          k++;
        }
      }
      row += up ? -1 : 1;
    }
    up = !up;
    col -= 2;
  }
}

void add_mask(vector<vector<int>> &grid, int module_number){
	for(int i=0; i<module_number; i++){
		for(int j=0; j<module_number; j++){
			if(grid[i][j] == 4 || grid[i][j] == 3)
				if((i+j) % 2 == 0)
					grid[i][j] = 7 - grid[i][j];
		}
	}
}
