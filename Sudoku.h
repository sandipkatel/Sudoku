#pragma once
/*---------------------------------------------A Sudoku Sudoku-------------------------------------------------*/
				// SDL Lazy Foo' Tutorials = https://lazyfoo.net/tutorials/SDL/index.php
				// SDL Documentation = https://wiki.libsdl.org/FrontPage
				// SDL True Type Fonts = https://www.libsdl.org/projects/SDL_ttf/

#include <SDL2/SDL.h> 
#include <iostream>
#include <iomanip>
#include <time.h>  

#include "Button.h"
#include "SudokuCell.h"
#include "SudokuGenerator.h"

namespace Sudoku
{
	class Sudoku
	{
	private:
		// Define window dimensions
		const int mWindowHeight;
		const int mWindowWidth;

		const int WindowHeight = 690;
		const int WindowWidth = 1280;

		// Define Sudoku grid dimensions
		const int mGridHeight;
		const int mGridWidth;

		// Define Sudoku max grid rows and columns (normally set to 9 x 9)
		const int mGridRows;
		const int mGridCols;

		// Define window and renderer
		SDL_Window* mWindow;
		SDL_Renderer* mRenderer;

		// Texture cache to hold preloaded textures
		int mTotalTextures;
		SDL_Texture* mTextureCache[30];

		// Define true type font paramaters
		TTF_Font* mFont;
		int mFontSize;

		// Define total buttons
		const int mTotalCells;

		// Define grid of cells (normally set to 9 x 9 = 81)
		Cell mGrid[81];

		//The rectangle behind the sudoku cell
		Button mRectangle;

		// Define check and new buttons
		Button mCheckButton;
		Button mNewButton;

		// Timer
		Button mTimer;

		// Define colours 
		SDL_Color mClearColour;

		//Textures at Right
		Button mTexts;

	private:
		// Intialise SDL window, renderer and true type font
		bool initialiseSDL();

		// Get index of 1D array from row and col
		inline int getIndex(int row, int col) const;

		// Load textures using SDL true type fonts
		void loadTexture(SDL_Texture*& texture, const char* text, SDL_Color& fontColour);

		// Preload textures using SDL true type fonts
		void preloadTextures();

		// Create interface layout by setting button parameters
		void createInterfaceLayout();
		
		// Generate a Sudoku puzzle with a unique solution
		void generateSudoku();

		// Free textures
		void freeTextures();

	public:
		// Constructor to intialise member variables
		Sudoku();

		// Destructor to free textures
		~Sudoku();

	public:
		// Play Sudoku
		void play();

		// Close Sudoku
		void close();

	};

};
//#include "Sudoku.h"

//----------------------------------Constructor and Destructor---------------------------------//
Sudoku::Sudoku::Sudoku()
	: //mWindowHeight(880), mWindowWidth(720),
	mWindowHeight(720), mWindowWidth(720),
	mGridHeight(720), mGridWidth(720),
	mGridRows(9), mGridCols(9),
	mWindow(nullptr), mRenderer(nullptr), 
	mTotalTextures(14), mTextureCache{ nullptr },
	mFont(nullptr), mFontSize(mGridHeight/12),
	mTotalCells(81),
	mClearColour({ 0, 0, 0, SDL_ALPHA_OPAQUE })
{

}

Sudoku::Sudoku::~Sudoku()
{
	freeTextures();
	close();
}

//--------------------------------------Private methods----------------------------------------//
bool Sudoku::Sudoku::initialiseSDL()
{
	// Set success initialisation flag
	bool success = true;

	// Initalise SDL video subsystem
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "SDL could not intialise! Error: " << SDL_GetError() << std::endl;
		success = false;
	}

	// Initialise SDL_ttf
	if (TTF_Init() == -1)
	{
		std::cout << "SDL_ttf could not initialise! Error: " << TTF_GetError() << std::endl;
		success = false;
	}

	// Create window
	mWindow = SDL_CreateWindow("Sudoku", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WindowWidth, WindowHeight, SDL_WINDOW_SHOWN);
	if (mWindow == nullptr)
	{
		std::cout << "SDL could not create window! Error: " << SDL_GetError() << std::endl;
		success = false;
	}

	// Create renderer
	mRenderer = SDL_CreateRenderer(mWindow, -1, 0);
	if (mRenderer == nullptr)
	{
		std::cout << "SDL could not create renderer! Error: " << SDL_GetError() << std::endl;
		success = false;
	}

	// Load font for text
	mFont = TTF_OpenFont("arial.ttf", mFontSize);
	if (mFont == nullptr)
	{
		std::cout << "Failed to load lazy font! Error: " << TTF_GetError() << std::endl;
		success = false;
	}

	return success;
}

inline int Sudoku::Sudoku::getIndex(int row, int col) const
{
	return row * mGridRows + col;
}

void Sudoku::Sudoku::loadTexture(SDL_Texture*& texture, const char* text, SDL_Color& fontColour)
{
	// Create text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(mFont, text, fontColour);
	if (textSurface == nullptr)
	{
		std::cout << "Could not create TTF SDL_Surface! Error: " << TTF_GetError() << std::endl;
	}
	else
	{
		// Create texture from surface pixels
		texture = SDL_CreateTextureFromSurface(mRenderer, textSurface);
		if (texture == nullptr)
		{
			std::cout << "Could not create texture from surface! Error: " << SDL_GetError() << std::endl;
		}
		SDL_FreeSurface(textSurface);
	}
}

void Sudoku::Sudoku::preloadTextures()
{
	// Choose colour of font
	SDL_Color fontColour = { 0, 0, 0, SDL_ALPHA_OPAQUE }; // black

	// Load texture for empty space
	loadTexture(mTextureCache[0], " ", fontColour);

	// Load textures for numbers from 1 to 9
	char temp[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    for (int i = 1; i < 10; i++)
    {   
        char tmp[2] = {temp[i-1], '\0'};
        loadTexture(mTextureCache[i],tmp, fontColour);
    }

	// Load textures
	loadTexture(mTextureCache[10], "Check", fontColour);
	loadTexture(mTextureCache[11], "New", fontColour);
	loadTexture(mTextureCache[12], "Wrong!", fontColour);
	loadTexture(mTextureCache[13], "Right!", fontColour);
	loadTexture(mTextureCache[14], "SUDOKU", fontColour);
	loadTexture(mTextureCache[15], "Exit", fontColour);
	loadTexture(mTextureCache[16], username, fontColour);
	loadTexture(mTextureCache[17], bestscore, fontColour);
	loadTexture(mTextureCache[18], bestgammer, fontColour);
	loadTexture(mTextureCache[19], "Hint", fontColour);
}

void Sudoku::Sudoku::createInterfaceLayout()
{
	// Set render color to light green (RGB: 153, 209, 213)
	SDL_SetRenderDrawColor(mRenderer, 153, 209, 213, SDL_ALPHA_OPAQUE);
	// Clear the screen with light green color
	SDL_RenderClear(mRenderer);

	bool fill = 1, draw = 0;  // to draw or fill the rectangle
	//display the background of Sudoku white
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	SDL_Rect buttonRect = {0, 0, mGridWidth, mGridHeight};
	mRectangle.renderRectangle(mRenderer,buttonRect, fill);


	/*----------------------------------Create buttons for each cell-------------------------------------*/
	// Define thick and thin borders
    const int thinBorder = 2;
    const int thickBorder = thinBorder + 6;

	// Define cell button dimensions
	// mGridWidth = 6 * thinBorder + 4 * thickBorder + 9 * buttonWidth (rearrange this equation)
	int buttonWidth = (mGridWidth - 6 * thinBorder - 4 * thickBorder) / mGridCols;
	int buttonHeight = buttonWidth-thickBorder;

	//Define start state
	int buttonStartRow = 0; 
	int buttonStartCol = 0;
	
	// Carry on from previous starting row
	//buttonStartRow += buttonHeight;

	// Set cell button position and dimensions
	for (int gridRow = 0; gridRow < mGridRows; gridRow++)
	{
		// Add to starting row
		if (gridRow == 0) buttonStartRow += thickBorder;
		else if (gridRow % 3 == 0) buttonStartRow += buttonHeight + thickBorder;
		else buttonStartRow += buttonHeight + thinBorder;

		// Reset starting column
		int buttonStartCol = 0;

		for (int gridCol = 0; gridCol < mGridCols; gridCol++)
		{
			// Add to starting column
			if (gridCol == 0) buttonStartCol += thickBorder;
			else if (gridCol % 3 == 0) buttonStartCol += buttonWidth + thickBorder;
			else buttonStartCol += buttonWidth + thinBorder;

			// Set button position and dimensions
			SDL_Rect buttonRect = { buttonStartCol, buttonStartRow, buttonWidth, buttonHeight };
			int index = getIndex(gridRow, gridCol);
			mGrid[index].setButtonRect(buttonRect);
		}
	}

	//Right side
	int rightX = mGridWidth + buttonHeight;
	int rightY = buttonHeight;
	int width  = 8;
	int height = mGridHeight-2*buttonHeight;

	//Display the seperator line
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	buttonRect = {rightX, rightY, width, height};
	mRectangle.renderRectangle(mRenderer,buttonRect, fill);

	rightX += width;
	buttonWidth = (WindowWidth-rightX);
	buttonHeight/=1.5;
	rightY = thickBorder;
	//Display Sudoku
	buttonRect = {rightX, rightY, buttonWidth, buttonHeight};
	mTexts.setButtonRect(buttonRect);
	mTexts.setTexture(mTextureCache[14]);
	mTexts.centerTextureRect(1.5);
	mTexts.renderTexture(mRenderer);

	//Display Sudoku
	buttonRect = {rightX + buttonWidth - buttonHeight, rightY, buttonHeight, buttonHeight/2};
	mTexts.setButtonRect(buttonRect);
	mTexts.setTexture(mTextureCache[15]);
	mTexts.centerTextureRect(2.5);
	mTexts.renderTexture(mRenderer);
	mTexts.renderRectangle(mRenderer, buttonRect, draw);

	//set username
	rightX += buttonHeight;
	rightY +=buttonHeight;
	buttonRect = {rightX, rightY, buttonWidth, buttonHeight};
	mTexts.setButtonRect(buttonRect);
	mTexts.setTexture(mTextureCache[16]);
	mTexts.centerTextureRect(1.5, 'n');
	mTexts.renderTexture(mRenderer);

	//set timer rect
	rightY +=buttonHeight;
	buttonRect = {rightX, rightY, buttonWidth, buttonHeight};
	mTimer.setButtonRect(buttonRect);

	//display best score and best scorer
	rightY +=buttonHeight;
	buttonRect = {rightX, rightY, buttonWidth, buttonHeight};
	mTexts.setButtonRect(buttonRect);
	mTexts.setTexture(mTextureCache[17]);
	mTexts.centerTextureRect(2);
	mTexts.renderTexture(mRenderer);

	rightY +=buttonHeight/1.5;
	buttonRect = {rightX, rightY, buttonWidth, buttonHeight};
	mTexts.setButtonRect(buttonRect);
	mTexts.setTexture(mTextureCache[18]);
	mTexts.centerTextureRect(2);
	mTexts.renderTexture(mRenderer);


	/*----------------------------------Create check, and new buttons-------------------------------------*/
	const int numberOfOtherButtons = 2;
	mCheckButton.setTexture(mTextureCache[10]);
	mNewButton.setTexture(mTextureCache[11]);
	Button* otherButtons[numberOfOtherButtons] = { &mCheckButton, &mNewButton };

	// Set check, solve, and new buttons (last row)
	buttonWidth/=2;
	rightX = rightX + ((WindowWidth - rightX) - buttonWidth)/2;
	for (int button = 0; button < numberOfOtherButtons; button++) // colBlock is every 3 columns of cells
	{
		rightY+=buttonHeight +thickBorder;
		// Set button position and dimensions
		SDL_Rect buttonRect = { rightX-1, rightY-1, buttonWidth+2, buttonHeight+2};
		otherButtons[button]->renderRectangle(mRenderer,buttonRect, draw);  //mantanance is required to make both draw and display
		buttonRect = { rightX, rightY, buttonWidth, buttonHeight};
		otherButtons[button]->setButtonRect(buttonRect);
	}
}

void Sudoku::Sudoku::generateSudoku()
{
	// Create empty an empty grid to store generated Sudoku
	int generatedGrid[81] = { };

	// Create empty an empty grid to store solution to generated Sudoku
	int solution[81] = { };

	// Instantiate a Sudoku generator object and generate Sudoku with the empty grids
	Generator G;
	G.generate(generatedGrid, solution);

	for (int i = 0; i < 81; i++)
	{
		// Set number and solution
		mGrid[i].setNumber(generatedGrid[i]);
		mGrid[i].setSolution(solution[i]); 

		// Set editability
		if (generatedGrid[i] == 0)
		{
			// This is a cell that can be editable
			mGrid[i].setEditable(true);
		}
		else
		{
			// This cell is fixed and cannot be edited
			mGrid[i].setEditable(false);
		}

		// Set texture (0 = ' ', 1 to 9 = '1' '2'... '9')
		mGrid[i].setTexture(mTextureCache[generatedGrid[i]]);

		// Center texture onto button
		mGrid[i].centerTextureRect();

	}
}

void Sudoku::Sudoku::freeTextures()
{
	for (int i = 0; i < mTotalTextures; i++)
	{
		// Free texture if it exists
		if (mTextureCache[i] != nullptr)
		{
			SDL_DestroyTexture(mTextureCache[i]);
			mTextureCache[i] = nullptr;
		}
	}
}

//--------------------------------------Public methods----------------------------------------//
void Sudoku::Sudoku::play()
{
	// Initialise SDL
	if (!initialiseSDL())
	{
		close();
	}
	// Preload textures for Sudoku grid and buttons
	preloadTextures();

	// Create interface layout
	createInterfaceLayout();

	// Generate Sudoku, set textures, and editability of each cell
	generateSudoku();

	// Set first current cell selected
	Cell* currentCellSelected = &mGrid[0];
	for (int cell = 0; cell < mTotalCells; cell++)
	{
		if (mGrid[cell].isEditable())
		{
			currentCellSelected = &mGrid[cell];
			currentCellSelected->setSelected(true);
			break;
		}
	}

	// Enable text input
	SDL_StartTextInput();

	// Loop variables
	SDL_Event event;
	bool stop = false;
	bool completed = false;
	bool generateNewSudoku = false;
	bool checkSolution = false;

	// Timing for check button
	bool measureTimeForCheckButton = false;
	time_t startTimeForCheckButton;

	// Timer
	time_t startTimer;
	time_t currentTime = 0;
	time(&startTimer);

	struct tm* timeInfo = localtime(&startTimer);
    timeInfo->tm_hour = 0;
    timeInfo->tm_min = 0;
    timeInfo->tm_sec = 0;

	// Game loop
	while (!stop)
	{
		// Handle events on queue
		while (SDL_PollEvent(&event) != 0)
		{
			// Handle quiting and completion
			if (event.type == SDL_QUIT)
			{
				// Set stop flag
				stop = true;
			}
			// Handle mouse event for "Check" button
			if (mCheckButton.getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN)
			{
				// Set check solution flag
				checkSolution = true;
			}
			// Handle mouse event for "New" button
			if (mNewButton.getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN)
			{
				// Set generate new Sudoku flag
				generateNewSudoku = true;
			}
			// Handle mouse event for cells
			for (int cell = 0; cell < mTotalCells; cell++)
			{
				// If editable
				if (mGrid[cell].isEditable())
				{
					// Set button state and return if mouse pressed on cell
					if (mGrid[cell].getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN)
					{
						// Set current cell selected to false
						currentCellSelected->setSelected(false);
						
						// Set new cell selected to true
						currentCellSelected = &mGrid[cell];
						currentCellSelected->setSelected(true);
					}
				}
			}
			// Handle keyboard events for current cell selected
			currentCellSelected->handleKeyboardEvent(&event, mTextureCache);
		}
		// If "New" button was clicked
		if (generateNewSudoku)
		{
			// Generate new sudoku
			generateSudoku();

			// Set current cell selected to false
			currentCellSelected->setSelected(false);

			// Find new starting cell
			for (int cell = 0; cell < mTotalCells; cell++)
			{
				if (mGrid[cell].isEditable())
				{
					currentCellSelected = &mGrid[cell];
					currentCellSelected->setSelected(true);
					break;
				}
			}

			// Reset flags
			generateNewSudoku = false;
			completed = false;

			// Reset timer
			time(&startTimer);
		}
		// If "Check" button was clicked
		if (checkSolution)
		{
			// Check if complete
			for (int cell = 0; cell < mTotalCells; cell++)
			{
				if (!mGrid[cell].isCorrect())
				{
					completed = false;
					break;
				}
				completed = true;
			}

			// Set measure time flag and starting time
			measureTimeForCheckButton = true;
			time(&startTimeForCheckButton);

			// Reset flag
			checkSolution = false;
		}
		// If currently measuring time
		if (measureTimeForCheckButton)
		{
			int seconds = 2;
			if (difftime(time(NULL), startTimeForCheckButton) < seconds && completed)
			{
				// Set colour to green
				SDL_Color colour = { 91, 191, 116, SDL_ALPHA_OPAQUE };

				// Set render colour to green
				SDL_SetRenderDrawColor(mRenderer, colour.r, colour.g, colour.b, SDL_ALPHA_OPAQUE);

				// Set texture to "Right!"
				mCheckButton.setTexture(mTextureCache[13]);

				// Set mouse down colour to green
				mCheckButton.setMouseDownColour(colour);
			}
			else if (difftime(time(NULL), startTimeForCheckButton) < seconds && !completed)
			{
				// Set colour to red
				SDL_Color colour = { 200, 73, 46, SDL_ALPHA_OPAQUE };

				// Set render colour to red
				SDL_SetRenderDrawColor(mRenderer, colour.r, colour.g, colour.b, SDL_ALPHA_OPAQUE);

				// Set texture to "Wrong!"
				mCheckButton.setTexture(mTextureCache[12]);

				// Set mouse down colour to red
				mCheckButton.setMouseDownColour(colour);
			}
			else
			{
				// Reset measure time flag
				measureTimeForCheckButton = false;
			}
		}
		else
		{
			// Set texture to "Check"
			mCheckButton.setTexture(mTextureCache[10]);

			// Set render colour to black
			SDL_SetRenderDrawColor(mRenderer, mClearColour.r, mClearColour.g, mClearColour.b, mClearColour.a);
		}

		// Render buttons and texture of each cell to backbuffer
		for (int cell = 0; cell < mTotalCells; cell++)
		{
			// Render button
			mGrid[cell].renderButton(mRenderer);

			// Re-center since diffrerent numbers have different sized textures
			mGrid[cell].centerTextureRect();

			// Render texture
			mGrid[cell].renderTexture(mRenderer);
		}

		// Render check button
		mCheckButton.renderButton(mRenderer);
		mCheckButton.centerTextureRect();
		mCheckButton.renderTexture(mRenderer);

		// Render new button
		mNewButton.renderButton(mRenderer);
		mNewButton.centerTextureRect();
		mNewButton.renderTexture(mRenderer);
		
		// Calculate timer
		/*currentTime = time(nullptr) - startTimer;
		tm formattedTime;
		localtime_s(&formattedTime, &currentTime);
		char timer[10];
		strftime(timer, sizeof(timer), "%H:%M:%S", &formattedTime);*/
		currentTime = time(nullptr) - startTimer;
		int hours = currentTime / 3600;
		int minutes = (currentTime % 3600) / 60;
		int seconds = currentTime % 60;

		// Render the timer
		std::stringstream timerStream;
		timerStream << std::setfill('0') << std::setw(2) << hours << ":"
					<< std::setfill('0') << std::setw(2) << minutes << ":"
					<< std::setfill('0') << std::setw(2) << seconds;
		std::string timerStr = "Time elapshed " + timerStream.str();

		// Load and render timer (TO DO: use preloaded textures to render timer)
		SDL_Texture* timerTexture = nullptr;
		SDL_Color fontColour = { 0, 0, 0, SDL_ALPHA_OPAQUE }; // black
		loadTexture(timerTexture, timerStr.c_str(), fontColour);
		//loadTexture(timerTexture, timer, fontColour);
		mTimer.setTexture(timerTexture);
		mTimer.renderButton(mRenderer);
		mTimer.centerTextureRect(1.5, 'n');
		mTimer.renderTexture(mRenderer);
		SDL_DestroyTexture(timerTexture);
		timerTexture = nullptr;

		// Update screen from backbuffer and clear backbuffer
		SDL_RenderPresent(mRenderer);

		// Slow down program becuase it doesn't need to run very fast
		SDL_Delay(10);
	}

	// Disable text input
	SDL_StopTextInput();

	// Free button textures
	freeTextures();

	// Destroy and quit
	close();
}

void Sudoku::Sudoku::close()
{
	// Destroy
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	mRenderer = nullptr;
	mWindow = nullptr;

	// Free font
	TTF_CloseFont(mFont);
	mFont = nullptr;

	// Quit
	SDL_Quit();
	TTF_Quit();
}
