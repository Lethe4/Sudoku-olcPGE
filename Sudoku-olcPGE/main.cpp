#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "SudoGrid.h"

//1. Draw Grid and center the numbers - Done, though the grid could use better colors
//2. Remove random (?) numbers on setup - Done
//3. Add ability for user to enter numbers - Done, but no way of confirming if a number is right
//4. Add size selection - Done
//5. Add timer and/or highlighting of boxes for speed version?


class SudokuGame : public olc::PixelGameEngine
{
public:
	SudokuGame()
	{
		sAppName = "Commit Sudoku";
	}

public:
	bool OnUserCreate() override
	{
		srand(time(NULL));
		Clear(olc::BLACK);

		sudoku.Start();
		sudoku.UpdateGame();

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		sudoku.UpdateGame();
		
		return true;
	}

protected:
	SudoGrid sudoku = SudoGrid(this);
};

int main()
{
	SudokuGame demo;
	if (demo.Construct(200, 200, 4, 4))
		demo.Start();


	return 0;
}