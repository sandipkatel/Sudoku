//Solver.h
/* Sudoku solver class */
#pragma once
namespace BackWork
{
	// Struct for ignoring a number specified at row and col (for the Sudoku generating class)
	struct Ignore
	{
		int num = 0;
		int row = 0;
		int col = 0;
	};

	// Sudoku solver class
	class Solver
	{
	private:
		// Pointer to the Sudoku 1D grid and must be of size 9 x 9 = 81
		int* mGrid;

		// Set number to ignore and a modifier (for the Sudoku generator class)
		bool mGenModifier;
		Ignore mNumToIgnore;

	private:
		// Set and get element of 1D array using row and col
		inline void setElement(const int row, const int col, const int num);
		inline int getElement(const int row, const int col) const;

		// Check if element is valid for its row, col, and the block of 3 x 3 its in
		bool checkValid(const int inputRow, const int inputCol, const int num) const;

	public:
		// Constructor
		Solver();

		// Set generator modifier (for the Sudoku generator class)
		void setGenModifier(const bool input);

		// Set Sudoku array (Ignore input paramaters is for the Sudoku generator class)
		void setGrid(int* grid, const Ignore& ignoreInput = { 0, 0, 0 });

		// Solve Sudoku with backtracking
		bool solve();
	};
};


BackWork::Solver::Solver()
	: mGrid(nullptr), mGenModifier(false), mNumToIgnore({ 0, 0, 0 })
{

}


inline void BackWork::Solver::setElement(const int row, const int col, const int num)
{
	mGrid[row * 9 + col] = num;
}

inline int BackWork::Solver::getElement(const int row, const int col) const
{
	return mGrid[row * 9 + col];
}

bool BackWork::Solver::checkValid(const int inputRow, const int inputCol, const int num) const
{
	// if number is valid i.e. not found in row, col, or block then return true
	// check row
	for (int col = 0; col < 9; col++)
	{
		if (getElement(inputRow, col) == num)
		{
			return false;
		}
	}

	// check col
	for (int row = 0; row < 9; row++)
	{
		if (getElement(row, inputCol) == num)
		{
			return false;
		}
	}

	// check block
	const int startRow = (int)(inputRow / 3) * 3;
	const int startCol = (int)(inputCol / 3) * 3;

	const int endRow = startRow + 3;
	const int endCol = startCol + 3;

	for (int row = startRow; row < endRow; row++)
	{
		for (int col = startCol; col < endCol; col++)
		{
			if (getElement(row, col) == num)
			{
				return false;
			}
		}
	}
	return true;
}

void BackWork::Solver::setGenModifier(const bool input)
{
	mGenModifier = input;
}

void BackWork::Solver::setGrid(int* grid, const Ignore& ignoreInput)
{
	mNumToIgnore = ignoreInput;
	mGrid = grid;
}

bool BackWork::Solver::solve()
{
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			if (getElement(row, col) == 0)
			{
				for (int num = 1; num < 10; num++)
				{
					// If generator modifier is set to true and the row, col and number match then skip to next iteration
					if (!(mGenModifier && mNumToIgnore.row == row && mNumToIgnore.col == col && mNumToIgnore.num == num))
					{
						// if number is valid i.e. not found in row, col, or block then return true
						if (checkValid(row, col, num))
						{
							setElement(row, col, num);

							if (solve())
							{
								return true;
							}
							// set element to 0 since num did not work for further recursion call(s)
							setElement(row, col, 0);
						}
					}
				}
				// back track to previous element
				return false;
			}
		}
	}
	// Solution found
	return true;
}
