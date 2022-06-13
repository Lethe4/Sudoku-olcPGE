#pragma once
#include "olcPixelGameEngine.h"
#include <random>

class SudoGrid
{
public:
	//Initialized in main.cpp using Start, then UpdateGame covers everything else

	SudoGrid(olc::PixelGameEngine* pge);  //when one of the options is selected, the size of one box row is input into the constructor

	//Function that draws everything
	void DrawGame();

	int RandNum(); //gets a random number from vec

	//Puzzle logic functions (check if a number is in a row/col/box)
	bool InCol(int num, int colnum); //col = x, row = y
	bool InRow(int num, int rownum);
	bool InBox(int num, int rowTL, int colTL); //provide x and y for top left # in target box
	bool IsUsedRC(int num, int row, int col); //check if num is used in box, col, or row - returns true if used
	bool CheckNum(int row, int col);

	//Functions that deal directly with setting up the puzzle
	bool FillBox(int rowTL, int colTL); //provide x and y for top left # in target box
	void ResetGrid(); //reset the entire grid to zeros
	void RemoveNums(int NtoRemove);
	bool SetupGrid(); //create solved sudoku using previous methods
	void FullSetup(); //using SetupGrid, create solvable sudoku with removed numbers
	void Reset(); //reset any entered numbers on current grid

	void Start();
	void ReturnToMenu();
	void BeginPuzzle();

	//Input handling and Update functions
	void HandleInput();
	void UpdateGame();

private:
	olc::PixelGameEngine* pge;

	int size = 0; //size of one row (4 or 9)
	int totalsize = 0; //size of whole puzzle (16 or 81)
	int boxsize = 0; //size of one row of one box (2 or 3)
	int NumRemoved = 0;
	int correctCounter = 0;
	int mistakesCounter = 0;
	std::vector<std::pair<int, bool>> grid; //Sudoku grid of (int, bool) pairs
	std::vector<std::pair<int, bool>> answers; //holds the numbers that were removed and whether they've been entered

	//Not the prettiest way of handling this, but functional and clear
	bool smallGrid = false; //true if user selects 4x4 grid
	bool largeGrid = false; //true if user selects 9x9 grid
	bool completed = false; //bool for puzzle completion
	bool gameOngoing = false; //check whether to draw the game to screen
	bool inStartMenu = true; //check whether to draw the start menu
	bool sizeSelected = false; //when the user chooses a size in the menu, this becomes true
	bool startPuzzle = false; //once the user has chosen a size and # to remove, this becomes true
	bool easy = false; //booleans for difficulty option
	bool medium = false;
	bool hard = false;
	std::vector<int> vec; //vector of numbers 1 through size

	int cursorPosX = 0;
	int cursorPosY = 0;

	long int seed;
	std::mt19937 rng;
	std::uniform_int_distribution<int> dist;
};