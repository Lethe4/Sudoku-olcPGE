#include "SudoGrid.h"

SudoGrid::SudoGrid(olc::PixelGameEngine* pge)
	:
	pge(pge)

{
}

//Draw the grid and numbers to the screen using the PixelGameEngine
void SudoGrid::DrawGame()
{
	pge->Clear(olc::BLACK);

	int x = 0;
	int y = 0;
	int c = 0;
	int gridx = 0;
	int gridy = 0;
	int scrWid = pge->ScreenWidth();
	int scrHei = pge->ScreenHeight();
	int width = 16;
	
	if (gameOngoing) //if the user has chosen a grid and is playing, draw the game
	{
		//Set the starting points for drawing the grid outlineand the numbers
		x = ((scrWid / 2) - ((width * size) / 2)) + 4;
		y = ((scrHei / 2) - ((width * size) / 2)) + 4;

		gridx = ((scrWid / 2) - ((width * size) / 2));
		gridy = ((scrHei / 2) - ((width * size) / 2));

		//Draw the grid - uses FillRect to get rid of color in previously highlighted cell
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				//if the user has entered a correct answer, color the cell green
				if (answers[(size * i) + j].second)
				{
					pge->FillRect(gridx + (j * width), gridy + (i * width), width, width, olc::GREEN);
				}

				pge->DrawRect(gridx + (j * width), gridy + (i * width), width, width);

				if (j % boxsize == 0 && j != 0)
				{
					pge->DrawLine(gridx + (j * width), gridy, gridx + (j * width), gridy + (size * width), olc::RED);
				}
			}

			if (i % boxsize == 0 && i != 0)
			{
				pge->DrawLine(gridx, gridy + (i * width), gridx + (size * width), gridy + (i * width), olc::RED);
			}
		}

		//Fill in the selected cell
		pge->FillRect((gridx + (cursorPosX * width)) + 1, (gridy + (cursorPosY * width)) + 1, width - 1, width - 1, olc::YELLOW);

		for (auto& a : grid)
		{
			if (a.first != 0)
			{
				pge->DrawString(olc::vi2d(x, y), std::to_string(a.first));
			}

			x += width;
			c++;

			if (c == size)
			{
				x = ((scrWid / 2) - ((width * size) / 2)) + 4;
				y += width;
				c = 0;
			}
		}

		if (completed)
		{
			pge->DrawString(scrWid / 3, scrHei / 20, "COMPLETE!", olc::GREEN);
		}

		//Draw Reset instructions
		pge->DrawStringProp(pge->GetPixelSize().x, (scrHei - ((scrHei)-(width * size)) / 2) + 2, "'r' = reset, 'x' = new, 'm' = menu");

		//Draw mistakesCounter
		pge->DrawStringDecal(olc::vf2d(2.0f, 2.0f), "Mistakes:", olc::WHITE, olc::vf2d(0.75f, 0.75f));
		pge->DrawStringDecal(olc::vf2d(56.0f, 2.0f), std::to_string(mistakesCounter), olc::RED, olc::vf2d(0.9f, 0.9f));
	}

	else if (inStartMenu) //if the user is in the start/title menu, draw it
	{
		int startX = scrWid / 10;
		int startY = scrHei / 10;

		//Draw the "Sudoku" title thing with a box around it
		pge->DrawRect((startX * 2) - (pge->GetPixelSize().x), startY - (pge->GetPixelSize().y),
			startX * 6, startY * 6);

		pge->DrawString(startX * 2, startY, "S", olc::WHITE, 2U);
		pge->DrawString(startX * 3, startY * 2, "U", olc::WHITE, 2U);
		pge->DrawString(startX * 4, startY * 3, "D", olc::WHITE, 2U);
		pge->DrawString(startX * 5, startY * 4, "O", olc::WHITE, 2U);
		pge->DrawString(startX * 6, startY * 5, "K", olc::WHITE, 2U);
		pge->DrawString(startX * 7, startY * 6, "U", olc::WHITE, 2U);

		pge->DrawString(startX * 3, startY, "U", olc::WHITE, 2U);
		pge->DrawString(startX * 4, startY, "D", olc::WHITE, 2U);
		pge->DrawString(startX * 5, startY, "O", olc::WHITE, 2U);
		pge->DrawString(startX * 6, startY, "K", olc::WHITE, 2U);
		pge->DrawString(startX * 7, startY, "U", olc::WHITE, 2U);

		pge->DrawStringDecal(olc::vf2d((startX * 2), (startY / 10) * 69), "(yes this technically doesn't work as a sudoku)", olc::WHITE, olc::vf2d(0.3f, 0.22f));

		//Draw the selection buttons for the puzzle size
		if (!sizeSelected)
		{
			if (cursorPosX == 0)
			{
				pge->FillRect((startX * 2) - pge->GetPixelSize().x, startY * 7, (startX * 2), startY, olc::GREEN);
			}
			else if (cursorPosX == 1)
			{
				pge->FillRect((startX * 6) - pge->GetPixelSize().x, startY * 7, (startX * 2) + 1, startY, olc::GREEN);
			}
		}
		else if (smallGrid) //Draw the selected size button still highlighted after selection
		{
			pge->FillRect((startX * 2) - pge->GetPixelSize().x, startY * 7, (startX * 2), startY, olc::GREEN);
		}
		else if (largeGrid)
		{
			pge->FillRect((startX * 6) - pge->GetPixelSize().x, startY * 7, (startX * 2) + 1, startY, olc::GREEN);
		}

		pge->DrawString((scrWid / 4) - pge->GetPixelSize().x, startY * 7 + (startY / 4), "4x4", olc::DARK_RED);
		pge->DrawString((scrWid / 8) * 5, startY * 7 + (startY / 4), "9x9", olc::DARK_RED);

		//Once a size is selected, draw the difficulty options
		if (sizeSelected)
		{
			pge->DrawString(startX, (scrHei / 20) * 17, "Select Difficulty:");

			if (cursorPosX == 0)
			{
				pge->FillRect(((scrWid / 20) * 2) - pge->GetPixelSize().x, (scrHei / 20) * 18, (startX * 2), scrHei / 20, olc::GREEN);
			}
			else if (cursorPosX == 1)
			{
				pge->FillRect(((scrWid / 20) * 6) - pge->GetPixelSize().x, (scrHei / 20) * 18, startX * 3, scrHei / 20, olc::GREEN);
			}
			else if (cursorPosX == 2)
			{
				pge->FillRect(((scrWid / 20) * 12) - pge->GetPixelSize().x, (scrHei / 20) * 18, (startX * 2) + 1, scrHei / 20, olc::GREEN);
			}

			pge->DrawString((scrWid / 20) * 2, (scrHei / 20) * 18, "Easy");
			pge->DrawString((scrWid / 20) * 6, (scrHei / 20) * 18, "Medium");
			pge->DrawString((scrWid / 20) * 12, (scrHei / 20) * 18, "Hard");
		}
	}
}

int SudoGrid::RandNum()
{
	return dist(rng);
}

//Check if num is present in the column provided
bool SudoGrid::InCol(int num, int colnum)
{
	for (int i = 0; i < size; i++)
	{
		if (grid[(i*size)+colnum].first == num)
		{
			return true;
		}
	}

	return false;
}

//Check if num is present in the row provided
bool SudoGrid::InRow(int num, int rownum)
{
	for (int i = 0; i < size; i++)
	{
		if (grid[(rownum*size)+i].first == num)
		{
			return true;
		}
	}

	return false;
}

//Check if num is in given box - box is identified by the location of its top left number
bool SudoGrid::InBox(int num, int rowTL, int colTL)
{
	for (int i = 0; i < boxsize; i++)
	{
		for (int j = 0; j < boxsize; j++)
		{
			if (grid[(size * (i + rowTL)) + (j + colTL)].first == num)
			{
				return true;
			}
		}
	}
	
	return false;
}

//Check if a number is used in the row, col, and box of interest
bool SudoGrid::IsUsedRC(int num, int row, int col)
{
	return (InRow(num, row) || InCol(num, col));
}

//Check whether a number the user entered is correct by checking the answers vec
bool SudoGrid::CheckNum(int col, int row)
{
	if ((grid[(size * row) + col].first == answers[(size * row) + col].first)
		&& answers[(size * row) + col].second == false)
	{
		return true;
	}

	return false;
}

//Fill a boxsize x boxsize box while checking rows and cols - returns true if one cell takes >100 tries to fill
bool SudoGrid::FillBox(int rowTL, int colTL) //rowTL and colTL < size (so they must be < 9 for a 9x9 grid)
{
	for (int i = 0; i < boxsize; i++)
	{
		for (int j = 0; j < boxsize; j++)
		{
			int rand = RandNum();
			int counter = 0;

			while ((IsUsedRC(rand, (i + rowTL), (j + colTL)) || InBox(rand, rowTL, colTL))
				&& counter < 100)
			{
				rand = RandNum();
				counter++;
			}

			if (counter >= 100)
			{
				return true;
			}
			else 
			{
				grid[(size * (i + rowTL)) + (j + colTL)] = std::pair<int, bool>(rand, true);
			}
		}
	}

	return false;
}

//Clear all info for a puzzle
void SudoGrid::ResetGrid()
{
	grid.clear();
	answers.clear();
	vec.clear();

	grid.assign(totalsize, std::pair<int, bool>(0, false));
	answers.assign(totalsize, std::pair<int, bool>(0, false));

	correctCounter = 0;
	mistakesCounter = 0;
	completed = false;
}

//Remove NtoRemove numbers from the grid so it can be solved and save removed nums to answers vec
void SudoGrid::RemoveNums(int NtoRemove)
{
	int randX = 0;
	int randY = 0;

	for (int i = 0; i < NtoRemove; i++)
	{
		randX = RandNum();
		randY = RandNum();

		while (grid[(size * (randY - 1)) + (randX - 1)].first == 0)
		{
			randX = RandNum();
			randY = RandNum();
		}

		answers[(size * (randY - 1)) + (randX - 1)].first = grid[(size * (randY - 1)) + (randX - 1)].first;
		grid[(size * (randY - 1)) + (randX - 1)] = std::pair<int, bool>(0, false);
	}
}

//Set up the full grid - fills in top left, middle, and bottom right boxes first, then proceeds from top left
//to bottom right. Returns true if the setup failed (impossible puzzle generated)
bool SudoGrid::SetupGrid()
{
	bool impossible = false;

	for (int i = 0; i < size; i = i + boxsize) //fill the diagonal boxes (top left, (middle if 9x9), bottom right)
	{
		impossible = FillBox(i, i);
	}
	
	for (int i = 0; i < size; i = i + boxsize)
	{
		for (int j = 0; j < size; j = j + boxsize)
		{
			if (i != j)
			{
				impossible = FillBox(i, j);
			}

			if (impossible)
			{
				return true;
			}
		}
	}

	return false;
}

//Uses SetupGrid to create a puzzle that is possible to solve and removes NumRemoved numbers
void SudoGrid::FullSetup()
{
	//Reset all relevant variables and containers
	ResetGrid();

	for (int i = 0; i < size; i++)
	{
		vec.push_back(i);
	}
	
	cursorPosX = 0;
	cursorPosY = 0;

	seed = static_cast<long int>(std::time(nullptr));
	rng = std::mt19937(size_t(seed));
	dist = std::uniform_int_distribution<int>(1, size);
	
	bool impossibleGrid = SetupGrid();

	while (impossibleGrid) //if the generated grid is impossible to solve, retry
	{
		ResetGrid();
		impossibleGrid = SetupGrid();
	}

	RemoveNums(NumRemoved);
}

//Keep current puzzle but reset any user-entered numbers
void SudoGrid::Reset()
{
	for (int i = 0; i < totalsize; i++)
	{
		if (answers[i].first != 0)
		{
			grid[i] = std::pair<int, bool>(0, false);
			answers[i].second = false;
		}
	}

	correctCounter = 0;
	mistakesCounter = 0;
	completed = false;
}

//Used to initialize the game by sending the user to the start menu
void SudoGrid::Start()
{
	inStartMenu = true;
	gameOngoing = false;
}

//Return to the start menu from within a game
void SudoGrid::ReturnToMenu()
{
	smallGrid = false;
	largeGrid = false;
	startPuzzle = false;
	easy = false;
	medium = false;
	hard = false;
	sizeSelected = false;
	cursorPosX = 0;
	cursorPosY = 0;

	ResetGrid();
	Start();
}

//Using the user's choices, create a puzzle
void SudoGrid::BeginPuzzle()
{
	if (smallGrid)
	{
		size = 4;
		totalsize = 16;
		boxsize = 2;

		if (easy)
		{
			NumRemoved = 4;
		}
		else if (medium)
		{
			NumRemoved = 8;
		}
		else
		{
			NumRemoved = 10;
		}
	}
	else
	{
		size = 9;
		totalsize = 81;
		boxsize = 3;
		
		if (easy)
		{
			NumRemoved = 18;
		}
		else if (medium)
		{
			NumRemoved = 36;
		}
		else
		{
			NumRemoved = 50;
		}
	}

	FullSetup();

	inStartMenu = false;
	gameOngoing = true;
}

//Deal with user input
void SudoGrid::HandleInput()
{
	//If the user is playing the game, handle input accordingly
	if (gameOngoing)
	{
		//Handle cursor positioning
		if ((pge->GetKey(olc::Key::LEFT).bPressed || pge->GetKey(olc::Key::A).bPressed) && cursorPosX > 0)
		{
			cursorPosX--;
		}
		else if ((pge->GetKey(olc::Key::RIGHT).bPressed || pge->GetKey(olc::Key::D).bPressed) && cursorPosX < size - 1)
		{
			cursorPosX++;
		}

		if ((pge->GetKey(olc::Key::UP).bPressed || pge->GetKey(olc::Key::W).bPressed) && cursorPosY > 0)
		{
			cursorPosY--;
		}
		else if ((pge->GetKey(olc::Key::DOWN).bPressed || pge->GetKey(olc::Key::S).bPressed) && cursorPosY < size - 1)
		{
			cursorPosY++;
		}

		//Take user input of numbers if the number is not locked in place (yes, this is ugly...)
		//Is there a PGE function for getting the pressed key (GetKey requires you choose a key)?
		//FIX: I moved pKeyboardState in olcPixelGameEngine.h to be public, so now it is possible to directly
		//access the state of every key in the original HWButton array instead of going through GetKey
		/*if ((pge->GetKey(olc::Key::K1).bPressed || pge->GetKey(olc::Key::NP1).bPressed) && grid[(cursorPosY * size) + cursorPosX].second == false)
		{
			grid[(cursorPosY * size) + cursorPosX].first = 1;
		}
		else if ((pge->GetKey(olc::Key::K2).bPressed || pge->GetKey(olc::Key::NP2).bPressed) && grid[(cursorPosY * size) + cursorPosX].second == false)
		{
			grid[(cursorPosY * size) + cursorPosX].first = 2;
		}
		else if ((pge->GetKey(olc::Key::K3).bPressed || pge->GetKey(olc::Key::NP3).bPressed) && grid[(cursorPosY * size) + cursorPosX].second == false)
		{
			grid[(cursorPosY * size) + cursorPosX].first = 3;
		}
		else if ((pge->GetKey(olc::Key::K4).bPressed || pge->GetKey(olc::Key::NP4).bPressed) && grid[(cursorPosY * size) + cursorPosX].second == false)
		{
			grid[(cursorPosY * size) + cursorPosX].first = 4;
		}
		else if ((pge->GetKey(olc::Key::K5).bPressed || pge->GetKey(olc::Key::NP5).bPressed) && grid[(cursorPosY * size) + cursorPosX].second == false)
		{
			grid[(cursorPosY * size) + cursorPosX].first = 5;
		}
		else if ((pge->GetKey(olc::Key::K6).bPressed || pge->GetKey(olc::Key::NP6).bPressed) && grid[(cursorPosY * size) + cursorPosX].second == false)
		{
			grid[(cursorPosY * size) + cursorPosX].first = 6;
		}
		else if ((pge->GetKey(olc::Key::K7).bPressed || pge->GetKey(olc::Key::NP7).bPressed) && grid[(cursorPosY * size) + cursorPosX].second == false)
		{
			grid[(cursorPosY * size) + cursorPosX].first = 7;
		}
		else if ((pge->GetKey(olc::Key::K8).bPressed || pge->GetKey(olc::Key::NP8).bPressed) && grid[(cursorPosY * size) + cursorPosX].second == false)
		{
			grid[(cursorPosY * size) + cursorPosX].first = 8;
		}
		else if ((pge->GetKey(olc::Key::K9).bPressed || pge->GetKey(olc::Key::NP9).bPressed) && grid[(cursorPosY * size) + cursorPosX].second == false)
		{
			grid[(cursorPosY * size) + cursorPosX].first = 9;
		}
		else if ((pge->GetKey(olc::Key::BACK).bPressed || pge->GetKey(olc::Key::DEL).bPressed) && grid[(cursorPosY * size) + cursorPosX].second == false)
		{
			grid[(cursorPosY * size) + cursorPosX].first = 0;
		}*/

		//Handle number input directly using the pKeyboardState array instead of multiple GetKey requests
		if (smallGrid)
		{
			for (int i = 28; i < 32; i++) //If 4x4, only allow numbers 1-4 to be placed in the grid
			{
				if ((pge->pKeyboardState[i].bPressed || pge->pKeyboardState[i + 42].bPressed) && grid[(cursorPosY * size) + cursorPosX].second == false)
				{
					grid[(cursorPosY * size) + cursorPosX].first = i - 27;
				}
			}
		}
		else
		{
			for (int i = 28; i < 37; i++)
			{
				if ((pge->pKeyboardState[i].bPressed || pge->pKeyboardState[i + 42].bPressed) && grid[(cursorPosY * size) + cursorPosX].second == false)
				{
					grid[(cursorPosY * size) + cursorPosX].first = i - 27;
				}
			}
		}
		

		//If user enters a number and presses Enter, check whether that number is correct
		//Also checks whether the puzzle is complete upon a succesful answer
		if (pge->GetKey(olc::Key::ENTER).bPressed || pge->GetKey(olc::Key::RETURN).bPressed)
		{
			if (grid[(cursorPosY * size) + cursorPosX].first != 0
				&& grid[(cursorPosY * size) + cursorPosX].second == false)
			{
				if (CheckNum(cursorPosX, cursorPosY))
				{
					grid[(cursorPosY * size) + cursorPosX].second = true;
					answers[(cursorPosY * size) + cursorPosX].second = true;
					correctCounter++;

					if (correctCounter == NumRemoved)
					{
						completed = true;
					}
				}
				else
				{
					mistakesCounter++;
				}
			}
		}

		//Delete number if user presses Backspace or Delete
		else if ((pge->GetKey(olc::Key::BACK).bPressed || pge->GetKey(olc::Key::DEL).bPressed) && grid[(cursorPosY * size) + cursorPosX].second == false)
		{
			grid[(cursorPosY * size) + cursorPosX].first = 0;
		}

		//Reset the puzzle (clear any entered numbers), generate a new one, or go to menu
		else if (pge->GetKey(olc::Key::R).bPressed)
		{
			Reset();
		}
		else if (pge->GetKey(olc::Key::X).bPressed)
		{
			FullSetup();
		}
		else if (pge->GetKey(olc::Key::M).bPressed)
		{
			ReturnToMenu();
		}
	}
	
	//If the user is in the start menu, handle input accordingly
	else if (inStartMenu)
	{
		//Handle cursor positioning
		if (!sizeSelected)
		{
			if ((pge->GetKey(olc::Key::LEFT).bPressed || pge->GetKey(olc::Key::A).bPressed) && cursorPosX > 0)
			{
				cursorPosX--;
			}
			else if ((pge->GetKey(olc::Key::RIGHT).bPressed || pge->GetKey(olc::Key::D).bPressed) && cursorPosX < 1)
			{
				cursorPosX++;
			}

			//If the user presses Enter, select the size they have highlighted
			else if ((pge->GetKey(olc::Key::ENTER).bPressed || pge->GetKey(olc::Key::RETURN).bPressed))
			{
				if (cursorPosX == 0)
				{
					smallGrid = true;
					largeGrid = false;

					sizeSelected = true;

					//Reset the Enter and Return key states so they don't carry over from choosing a size
					pge->pKeyboardState[65].bPressed = false;
					pge->pKeyboardState[66].bPressed = false;
				}
				else
				{
					smallGrid = false;
					largeGrid = true;

					sizeSelected = true;

					cursorPosX = 0;

					//Reset the Enter and Return key states so it doesn't carry over from choosing a size
					pge->pKeyboardState[65].bPressed = false;
					pge->pKeyboardState[66].bPressed = false;
				}
			}
		}

		if (sizeSelected)
		{
			//If the user presses Back or Del after choosing a size, go back to size selection
			if (pge->GetKey(olc::Key::BACK).bPressed || pge->GetKey(olc::Key::DEL).bPressed)
			{
				sizeSelected = false;
			}

			if ((pge->GetKey(olc::Key::LEFT).bPressed || pge->GetKey(olc::Key::A).bPressed) && cursorPosX > 0)
			{
				cursorPosX--;
			}
			else if ((pge->GetKey(olc::Key::RIGHT).bPressed || pge->GetKey(olc::Key::D).bPressed) && cursorPosX < 2)
			{
				cursorPosX++;
			}

			else if (pge->GetKey(olc::Key::ENTER).bPressed || pge->GetKey(olc::Key::RETURN).bPressed)
			{
				if (cursorPosX == 0)
				{
					easy = true;
					medium = false;
					hard = false;
				}
				else if (cursorPosX == 1)
				{
					easy = false;
					medium = true;
					hard = false;
				}
				else if (cursorPosX == 2)
				{
					easy = false;
					medium = false;
					hard = true;
				}

				BeginPuzzle();
			}
		}
	}
}

//Used instead of individually calling DrawGame and HandleInput
void SudoGrid::UpdateGame()
{
	DrawGame();
	HandleInput();
}
