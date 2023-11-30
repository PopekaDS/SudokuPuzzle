// Sudoku Puzzle, by Al Sweigart
// The classic 9x9 number placement puzzle.
// More info at https://en.wikipedia.org/wiki/Sudoku
// This code is available at https ://nostarch.com/big-book-small-python-programming
// #73 SUDOKU PUZZLE

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream> 
#include <random>

// This game requires a sudokupuzzle.txt file that contains the puzzles.
// Download it from https://inventwithpython.com/sudokupuzzles.txt
// Here's a sample of the content in this file:
// ..3.2.6..9..3.5..1..18.64....81.29..7.......8..67.82....26.95..8..2.3..9..5.1.3..
// 2...8.3...6..7..84.3.5..2.9...1.54.8.........4.27.6...3.1..7.4.72..4..6...4.1...3
// ......9.7...42.18....7.5.261..9.4....5.....4....5.7..992.1.8....34.59...5.7......
// .3..5..4...8.1.5..46.....12.7.5.2.8....6.3....4.1.9.3.25.....98..1.2.6...8..6..2.

using namespace std;

// Set up the constants :
char EMPTY_SPACE = '.';
int GRID_LENGTH = 9;
int BOX_LENGTH = 3;
int FULL_GRID_SIZE = GRID_LENGTH * GRID_LENGTH;

class SudokuGrid {
	// originalSetup is a string of 81 characters for the puzzle
	// setup, with numbers and periods (for the blank spaces).
	// See https://inventwithpython.com/sudokupuzzles.txt

	vector<string> grid;
	vector<string> moves; // Tracks each move for the undo feature.
	string originalSetup;
public:
	SudokuGrid(string setup);
	void display();
	bool _isCompleteSetOfNumbers(string numbers);
	bool isSolved();
	bool makeMove(char column, int row, char number);
	void resetGrid(); // Set the grid state to its original setup.
	void undo();
};

SudokuGrid::SudokuGrid(string setup) {
	originalSetup = setup;

	for (int i = 0; i < GRID_LENGTH * GRID_LENGTH; ) {
		string tmp = "";
		for (int j = 0; j < GRID_LENGTH; j++, i++) {
			tmp.push_back(setup[i]);
		}
		grid.push_back(tmp);
	}
}

void SudokuGrid::display() {
	// Display the current state of the grid on the screen.
	cout << "  A B C   D E F   G H I\n"; // Display column labels.
	for (int y = 0; y < GRID_LENGTH; ++y) {
		for (int x = 0; x < GRID_LENGTH; ++x) {
			if (x == 0) {
				// Display row label:
				cout << y + 1 << " ";
			}

			cout << grid[y][x] << " ";
			if (x == 2 || x == 5) {
				// Display a vertical line:
				cout << "| ";
			}
		}

		cout << '\n'; // Print a newline.
		if (y == 2 || y == 5) {
			// Display a horizontal line:
			cout << "  ------+-------+------\n";
		}
	}
}

bool SudokuGrid::_isCompleteSetOfNumbers(string numbers) {
	// Return True if numbers contains the digits 1 through 9.
	sort(numbers.begin(), numbers.end());
	return numbers == "123456789";
}

bool SudokuGrid::isSolved() {
	// Returns True if the current grid is in a solved state.
	// Check each row:
	for (int row = 0; row < GRID_LENGTH; ++row) {
		string rowNumbers = "";
		for (int x = 0; x < GRID_LENGTH; ++x) {
			rowNumbers.push_back(grid[row][x]);
		}
		if (!(_isCompleteSetOfNumbers(rowNumbers))) {
			return false;
		}
	}

	// Check each column:
	for (int column = 0; column < GRID_LENGTH; ++column) {
		string columnNumbers = "";
		for (int y = 0; y < GRID_LENGTH; ++y) {
			columnNumbers.push_back(grid[y][column]);
		}
		if (!(_isCompleteSetOfNumbers(columnNumbers))) {
			return false;
		}
	}

	for (int boxx = 0; boxx < GRID_LENGTH; boxx += 3) {
		for (int boxy = 0; boxy < GRID_LENGTH; boxy += 3) {
			string boxNumbers = "";
			for (int x = 0; x < BOX_LENGTH; ++x) {
				for (int y = 0; y < BOX_LENGTH; ++y) {
					boxNumbers.push_back(grid[boxx + x][boxy + y]);
				}
			}
			if (!(_isCompleteSetOfNumbers(boxNumbers))) {
				return false;
			}
		}
	}

	return true;
}

int symbol(char c) {
	int x = -1;
	switch (c) {
	case 'A':
		return 0;
	case 'B':
		return 1;
	case 'C':
		return 2;
	case 'D':
		return 3;
	case 'E':
		return 4;
	case 'F':
		return 5;
	case 'G':
		return 6;
	case 'H':
		return 7;
	case 'I':
		return 8;
	default:
		return x;
	}
}

bool SudokuGrid::makeMove(char column, int row, char number) {
	// Place the number at the column (a letter from A to I) and row
	// (an integer from 1 to 9) on the grid.
	int x = -1;
	x = symbol(column);

	int y = row - 1;

	// Check if the move is being made on a "given" number:
	if (grid[y][x] != EMPTY_SPACE) {
		return false;
	}

	grid[y][x] = number; // Place this number on the grid.

	// We need to store a separate copy of the dictionary object:
	string pb = column + to_string(row) + to_string(number);
	moves.push_back(pb);

	return true;
}

void SudokuGrid::resetGrid() {
	// Reset the state of the grid, tracked by grid, to the
	// state in originalSetup.
	vector<string> gridTmp;

	for (int i = 0; i < GRID_LENGTH * GRID_LENGTH; ) {
		string tmp;

		for (int j = 0; j < GRID_LENGTH; j++, i++) {
			tmp.push_back(originalSetup[i]);
		}
		gridTmp.push_back(tmp);
	}

	grid = gridTmp;
}

void SudokuGrid::undo() {
	// Set the current grid state to the previous state in the moves list.
	if (moves.empty()) {
		return; // No states in moves, so do nothing.
	}

	string move = moves[moves.size() - 1];
	moves.pop_back(); // Remove the current state.

	if (moves.empty()) {
		resetGrid();
	}

	// set the grid to the last move.
	char one = move[0], two = move[1];
	int x = symbol(one);
	int y = int(two - '0') - 1;
	grid[y][x] = '.';
}

int main() {
	cout << "Sudoku Puzzle, by Al Sweigart al@inventwithpython.com\n\n";
	cout << "Sudoku is a number placement logic puzzle game.A Sudoku grid is a 9x9\n";
	cout << "grid of numbers.Try to place numbers in the grid such that every row,\n";
	cout << "column, and 3x3 box has the numbers 1 through 9 once and only once.\n";
	cout << "For example, here is a starting Sudoku grid and its solved form:\n";
	cout << "5 3 . | . 7 . | . . .      5 3 4 | 6 7 8 | 9 1 2\n";
	cout << "6 . . | 1 9 5 | . . .      6 7 2 | 1 9 5 | 3 4 8\n";
	cout << ". 9 8 | . . . | . 6 .      1 9 8 | 3 4 2 | 5 6 7\n";
	cout << "------+-------+------      ------+-------+------\n";
	cout << "8 . . | . 6 . | . . 3      8 5 9 | 7 6 1 | 4 2 3\n";
	cout << "4 . . | 8 . 3 | . . 1 -- > 4 2 6 | 8 5 3 | 7 9 1\n";
	cout << "7 . . | . 2 . | . . 6      7 1 3 | 9 2 4 | 8 5 6\n";
	cout << "------+-------+------      ------+-------+------\n";
	cout << ". 6 . | . . . | 2 8 .      9 6 1 | 5 3 7 | 2 8 4\n";
	cout << ". . . | 4 1 9 | . . 5      2 8 7 | 4 1 9 | 6 3 5\n";
	cout << ". . . | . 8 . | . 7 9      3 4 5 | 2 8 6 | 1 7 9\n\n\n\n\n";

	string s;
	int end = 0;

	// Load the sudokupuzzles.txt file:
	ifstream file("sudokupuzzles.txt");
	vector<string> puzzleFile;
	while (getline(file, s)) {
		puzzleFile.push_back(s);
		end++;
	}
	file.close();
	std::random_device rd;  // Seed the random number generator
	std::mt19937 gen(rd()); // Mersenne Twister PRNG engine
	std::uniform_int_distribution<int> distribution(0, end); // Define the range
	// Generate a random number
	int random_number = distribution(gen);
	SudokuGrid grid(puzzleFile[random_number]);

	while (1) { // Main game loop.
		grid.display();

		// Check if the puzzle is solved.
		if (grid.isSolved()) {
			cout << "Congratulations! You solved the puzzle!\n";
			cout << "Thanks for playing!\n";
			return 0;
		}

		string action;
		char column, row, number;
		// Get the player's action:
		while (1) { // Keep asking until the player enters a valid action.
			cout << "\n\nEnter a move, or RESET, NEW, UNDO, ORIGINAL, or QUIT:\n";
			cout << "For example, a move looks like 'B4 9'.)\n";
			cout << "> ";
			getline(cin, action);
			transform(action.begin(), action.end(), action.begin(), ::toupper);
			if (action.size() > 0 && (action[0] == 'R' || action[0] == 'N' || action[0] == 'U' || action[0] == 'O' || action[0] == 'Q')) {
				// Player entered a valid action.
				break;
			}

			if (action.size() == 4) {
				column = action[0];
				row = action[1];
				if (action[2] != ' ') {
					continue;
				}
				number = action[3];

				if (!(column == 'A' || column == 'B' || column == 'C' || column == 'D' || column == 'E' || column == 'F' || column == 'G' || column == 'H' || column == 'I')) {
					cout << "There is no column " << column << '\n';
					continue;
				}

				if (!(row >= '1' && row <= '9')) {
					cout << "There is no row" << row << '\n';
					continue;
				}

				if (!(number >= '1' && number <= '9')) {
					cout << "Select a number from 1 to 9, not " << number << '\n';
					continue;
				}
				break; // Player entered a valid move.
			}
		}
		cout << "\n\n\n";
		if (action[0] == 'R') {
			// Reset the grid:
			grid.resetGrid();
			continue;
		}

		if (action[0] == 'N') {
			// Get a new puzzle:
			std::random_device rd;  // Seed the random number generator
			std::mt19937 gen(rd()); // Mersenne Twister PRNG engine
			std::uniform_int_distribution<int> distribution(0, end); // Define the range
			random_number = distribution(gen);
			SudokuGrid gridN(puzzleFile[random_number]);
			grid = gridN;
			continue;
		}

		if (action[0] == 'U') {
			// Undo the last move:
			grid.undo();
			continue;
		}

		if (action[0] == 'O') {
			// View the original numbers:
			SudokuGrid gridO(puzzleFile[random_number]);
			cout << "\n\nThe original grid looked like this:\n";
			gridO.display();

			continue;
		}

		if (action[0] == 'Q') {
			// Quit the game.
			cout << "Thanks for playing!\n";
			return 0;
		}

		// Handle the move the player selected.
		int rowInt = int(row - '0');
		if (grid.makeMove(column, rowInt, number) == false) {
			cout << "You cannot overwrite the original grid\'s numbers.\n";
			cout << "Enter ORIGINAL to view the original grid.\n\n";
		}
	}

	return 0;
}