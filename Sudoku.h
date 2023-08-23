#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> 
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <iomanip>
#include <ctime>  

#include "Button.h"
#include "Cells.h"
#include "Generator.h"
#include "fileHandling.h"

namespace Sudoku
{
	class Sudoku
	{
	private:
		// Define window dimensions
		const int mWindowHeight;
		const int mWindowWidth;

		// Define Sudoku grid dimensions
		const int mGridHeight;
		const int mGridWidth;

		// Define Sudoku max grid rows and columns
		const int mGridRows;
		const int mGridCols;

		// Define window and renderer
		SDL_Window* mWindow;
		SDL_Renderer* mRenderer;

		//Define image texture and image surfaces
		SDL_Texture* mImageTexture;
		SDL_Surface* mImageSurface;

		// Texture cache to hold preloaded textures
		int mTotalTextures;
		SDL_Texture* mTextureCache[20];

		// Define true type font paramaters
		TTF_Font* mFont;
		int mFontSize;

		// Define total cells
		const int mTotalCells;

		// Define Gray, Black color
		SDL_Color GrayColor;
		SDL_Color BlackColor;

		//User name, Best Score, Best Player and time from file
		std::string mUserName;
		std::string mBestScore;
		std::string mBestPlayer;
		int mTime;

		// Define grids of cells
		Cell mGrid[81];

		//Defie the he rectangle behind the sudoku cell
		Button mRectangle;

		// Define check and new buttons
		Button mHintButton;
		Button mNewButton;

		// Timer
		Button mTimer;

		//File from Back Work
		BackWork::handleFile myfile;

		//Textures at Right
		Button mTexts[15];

		//Texture 1 to 9 input Box
		Button mInput[10];

		//Exit Button
		Button mExit;

	private:
		// Intialise SDL withh required materials
		bool initialiseSDL();

		// Load textures using SDL true type fonts
		void loadTexture(SDL_Texture*& texture, const char* text, SDL_Color& fontColour);

		// Preload textures using SDL true type fonts
		void preloadTextures();

		//Load back ground Image
		void mImage();

		//If game is completed congrate
		void mCongrate(bool winner);

		// Get index of 1D array from row and col
		inline int getIndex(int row, int col) const;

		// Create interface layout by setting button parameters
		void createInterfaceLayout();
		
		// Generate a Sudoku puzzle with a unique solution
		void generateSudoku(Buttons);

		// Free textures
		void freeTextures();
		
		//Create Home screen and Level screen Layout
		Buttons home_levels(Buttons homeLevel);

		//Create Home screen Layout
		Buttons homeScreen();

		// Play Sudoku
		Buttons play(Buttons level);

		//Create About screen and Best screen Layout
		Buttons about_best(Buttons mButton);

		//Create Username Interface
		Buttons userName();

		//Display Level
		Buttons levels();

		//Function for Exit Button
		void exitButton();

	public:
		// Constructor to intialise member variables
		Sudoku();

		//Distructor to free all
		~Sudoku();

		//Function call controller and display
		void display();

		// Close Sudoku
		void close();
	};
};

//Constructor and distructors
Sudoku::Sudoku::Sudoku():
	   //make all related starting from window height
	mWindowHeight(690), mWindowWidth(2 * mWindowHeight - 100),
	mGridHeight(mWindowHeight), mGridWidth(mWindowWidth/1.78),
	mGridRows(9), mGridCols(9),
	mWindow(nullptr), mRenderer(nullptr),
	mImageSurface(nullptr), mImageTexture(nullptr),
	mTotalTextures(14), mTextureCache{ nullptr },
	mFont(nullptr),
	mFontSize(mGridHeight/15), mTotalCells(81),
	GrayColor({ 192, 192, 192, SDL_ALPHA_OPAQUE}),
	BlackColor({ 0, 0, 0, SDL_ALPHA_TRANSPARENT}),
	mUserName("username: unknown"), mBestPlayer("By: unknown"),
	mBestScore("Shortest Time: unknown"), mTime(0)
{}

Sudoku::Sudoku::~Sudoku()
{
	freeTextures();
	close();
}


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
	mWindow = SDL_CreateWindow("Sudoku", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWindowWidth, mWindowHeight, SDL_WINDOW_SHOWN);
	if (mWindow == nullptr)
	{
		std::cout << "SDL could not create window! Error: " << SDL_GetError() << std::endl;
		success = false;
	}

	// Create renderer
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (mRenderer == nullptr)
	{
		std::cout << "SDL could not create renderer! Error: " << SDL_GetError() << std::endl;
		success = false;
	}

	//Load splash image
    mImageSurface = IMG_Load("Sudoku.bmp");
    if( mImageSurface == NULL )
    {
        std::cout<<"SDL could not load the image! Error: "<<IMG_GetError()<<std::endl;
        success = false;
    }

	// Load font for text
	mFont = TTF_OpenFont("arial.ttf", mFontSize);
	if (mFont == nullptr)
	{
		std::cout << "Failed to load arial font! Error: " << TTF_GetError() << std::endl;
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
	//use username;
	std::string user = "username: ";
	user += mUserName;

	// Load texture for empty space
	loadTexture(mTextureCache[0], " ", BlackColor);

	// Load textures for numbers from 1 to 9
	char temp[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    for (int i = 1; i < 10; i++)
    {   
        char tmp[2] = {temp[i-1], '\0'};
        loadTexture(mTextureCache[i], tmp, BlackColor);
    }

	// Load textures
	loadTexture(mTextureCache[10], "Hint", BlackColor);
	loadTexture(mTextureCache[11], "New", BlackColor);
	loadTexture(mTextureCache[12], "SUDOKU", BlackColor);
	loadTexture(mTextureCache[13], user.c_str(), BlackColor);
	loadTexture(mTextureCache[14], mBestScore.c_str(), BlackColor);
	loadTexture(mTextureCache[15], mBestPlayer.c_str(), BlackColor);
}

void Sudoku::Sudoku::mImage()
{
	// Convert image surface to texture
    mImageTexture = SDL_CreateTextureFromSurface(mRenderer, mImageSurface);

    //Define the rectangle equal to screen width and Height
    SDL_Rect rect = {0, 0, mWindowWidth, mWindowHeight};

    // Render the image texture inside the rectangle
    SDL_RenderCopy(mRenderer, mImageTexture, nullptr, &rect);
	
	// Update the screen
    SDL_RenderPresent(mRenderer);
}

void Sudoku::Sudoku::mCongrate(bool winner)
{
	//Display Rectangle
	int rectWidth = mWindowWidth/1.5;
	int rectHeight = mWindowHeight/3;
	int initialX = (mWindowWidth - rectWidth)/2;
	int initialY = (mWindowHeight - rectHeight)/2;
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE );
	SDL_Rect buttonRect = { initialX, initialY, rectWidth, rectHeight };
	mRectangle.renderRectangle(mRenderer,buttonRect, true);

	//Display Congratulation words
	SDL_Texture* texture;
	SDL_Color lightGreen = { 200, 73, 46, SDL_ALPHA_OPAQUE };
	if(winner)
		loadTexture(texture, " NEW BEST SCORE ", lightGreen);
	else
		loadTexture(texture, " YOU DID IT ", lightGreen);
	mTexts[9].renderTexts(mRenderer, buttonRect, texture, 0.5, 'y');
	
	//Redclare for new Rectangle
	rectWidth /= 2;
	rectHeight /= 12;
	initialX = (mWindowWidth - rectWidth)/2;
	initialY = (mWindowHeight - rectHeight)/2;
	buttonRect = { initialX, initialY, rectWidth, rectHeight };
	mRectangle.renderRectangle(mRenderer,buttonRect, true);

	std::string congrateName = " Congratulation! ";
	congrateName += mUserName;
	loadTexture(texture, congrateName.c_str(), BlackColor);
	mTexts[9].renderTexts(mRenderer, buttonRect, texture, 2, 'y');

	// Update the screen
    SDL_RenderPresent(mRenderer);

	// Wait for four seconds
	SDL_Delay(4000);
}


void Sudoku::Sudoku::createInterfaceLayout()
{
	// Clear the screen with light green color
	SDL_SetRenderDrawColor(mRenderer, 153, 209, 213, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(mRenderer);

	bool fill = 1, draw = 0;  // to draw or fill the rectangle
	//display the background of Sudoku white
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	SDL_Rect buttonRect = {0, 0, mGridWidth, mGridHeight};
	mRectangle.renderRectangle(mRenderer,buttonRect, fill);


	/*Create buttons for each cell*/
	// Define thick and thin borders
    const int thinBorder = 2;
    const int thickBorder = thinBorder + 6;

	// mGridHeight = 6 * thinBorder + 4 * thickBorder + 9 * buttonHeight
	int buttonWidth = (mGridWidth - 6 * thinBorder - 4 * thickBorder) / mGridCols;
	int buttonHeight = (mGridHeight - 6 * thinBorder - 4 * thickBorder) / mGridRows;

	//Define start state
	int buttonStartRow = 0; 
	int buttonStartCol = 0;

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

	//Right side interface layout
	//Redeclare button height it is no more required for cell
	buttonHeight -= thickBorder;
	const int rightSide = mGridWidth + buttonHeight;
	int rightX = rightSide;
	int rightY = buttonHeight;
	int width  = 8;
	int height = mGridHeight-2*buttonHeight;

	//Display the seperator line
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);   //black
	buttonRect = {rightX, rightY, width, height};
	mRectangle.renderRectangle(mRenderer,buttonRect, fill);

	rightX += width;
	buttonWidth = (mWindowWidth-rightX);
	buttonHeight/=1.5;
	rightY = thickBorder;

	//Display Sudoku
	buttonRect = {rightX, rightY, buttonWidth, buttonHeight};
	mTexts[1].renderTexts(mRenderer, buttonRect, mTextureCache[12], 1.5);

	//set username
	rightX += buttonHeight;
	rightY +=buttonHeight;
	buttonRect = {rightX, rightY, buttonWidth, buttonHeight};
	mTexts[2].renderTexts(mRenderer, buttonRect, mTextureCache[13], 1.5, 'n');

	//set timer rect
	rightY +=buttonHeight;
	buttonRect = {rightX, rightY, buttonWidth, buttonHeight};
	mTimer.setButtonRect(buttonRect);

	//display best score
	rightY +=buttonHeight;
	buttonRect = {rightX, rightY, buttonWidth, buttonHeight};
	mTexts[3].renderTexts(mRenderer, buttonRect, mTextureCache[14], 2, 'n');

	//display best scorer
	rightY +=buttonHeight/1.5;
	buttonRect = {rightX, rightY, buttonWidth, buttonHeight};
	mTexts[4].renderTexts(mRenderer, buttonRect, mTextureCache[15], 2);


						//Create Hint and New button
	const int numberOfOtherButtons = 2;
	mHintButton.setTexture(mTextureCache[10]);
	mNewButton.setTexture(mTextureCache[11]);
	Button* otherButtons[numberOfOtherButtons] = { &mHintButton, &mNewButton };

	// Set check, solve, and new buttons
	buttonWidth/=2;
	rightX = rightSide + ((mWindowWidth - rightSide) - buttonWidth)/2;
	for (int button = 0; button < numberOfOtherButtons; button++) // colBlock is every 3 columns of cells
	{
		rightY+=buttonHeight +thickBorder;
		// Set button position and dimensions
		buttonRect = { rightX, rightY, buttonWidth, buttonHeight};
		otherButtons[button]->setButtonRect(buttonRect);
		buttonRect = { rightX-1, rightY-1, buttonWidth+2, buttonHeight+2};
		otherButtons[button]->renderRectangle(mRenderer,buttonRect, draw);  //mantanance is required to make both draw and display
	}

							//Display InputBar Rectangle 1 - 9
	//redefine staarting point of input and hight, width
	buttonHeight*=2;
	buttonWidth = buttonHeight;
	rightX = rightSide + ((mWindowWidth - rightSide) - 3*buttonHeight)/2;
	rightY += buttonHeight;
	int xBuf = rightX;
	int n = 1;
	for(int i =0; i<3; i++)
	{
		for(int j = 0; j<3; j++)
		{
			buttonRect = { rightX, rightY, buttonWidth, buttonHeight};
			mInput[n].setButtonRect(buttonRect);
			buttonRect = { rightX-1, rightY-1, buttonWidth+2, buttonHeight+2};
			mInput[n].renderRectangle(mRenderer,buttonRect, draw);
			rightX += buttonHeight + 1;
			n++;
		}
		rightX = xBuf;
		rightY += buttonHeight + 1;				//add 1 means gap to avoid override
	}
	buttonWidth = buttonWidth*3 + 2;
	buttonHeight /= 2;
	buttonRect = { rightX, rightY, buttonWidth, buttonHeight};
	mInput[0].setButtonRect(buttonRect);
	buttonRect = { rightX-1, rightY-1, buttonWidth+2, buttonHeight+2};
	mInput[0].renderRectangle(mRenderer,buttonRect, draw);

	exitButton();
	// Update screen from backbuffer and clear backbuffer
	SDL_RenderPresent(mRenderer);
}

void Sudoku::Sudoku::exitButton()
{
	bool draw = 0;
	int buttonWidth = mWindowWidth/16;
	int buttonHeight = mWindowHeight/23;
	int x = mWindowWidth - buttonWidth;
	int y = buttonHeight;
	SDL_Texture* texture;
	loadTexture(texture, "Exit", BlackColor);
	SDL_Rect buttonRect = { x, y, buttonWidth, buttonHeight };
	mExit.renderRectangle(mRenderer, buttonRect, draw);
	mExit.renderTexts(mRenderer, buttonRect, texture, 2);
}

void Sudoku::Sudoku::generateSudoku(Buttons LEVEL)
{
	// Create empty grid to store generated Sudoku
	int generatedGrid[81] = { };

	// Create empty grid to store solution to generated Sudoku
	int solution[81] = { };

	// Instantiate a Sudoku generator object and generate Sudoku with the empty grids
	BackWork::Generator G;
	G.generate(generatedGrid, solution, LEVEL);

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
Buttons Sudoku::Sudoku::play(Buttons level)
{
	//Retrive from File
	myfile.getData(level, mBestScore, mBestPlayer, mTime);

	// Preload textures for Sudoku grid and buttons
	preloadTextures();

	// Create interface layout
	createInterfaceLayout();

	// Generate Sudoku, set textures, and editability of each cell
	generateSudoku(level);

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
	bool exit  = false;
	bool completed = false;
	bool generateNewSudoku = false;
	bool checkHint = false;
	bool incomplete = true;
	int hintCount = 0;
	int incorrectCell = 0;

	// Timer
	time_t startTimer;
	time_t currentTime = 0;
	time(&startTimer);

	struct tm* timeInfo = localtime(&startTimer);
	std::string elapshedTime;
    timeInfo->tm_hour = 0;
    timeInfo->tm_min = 0;
    timeInfo->tm_sec = 0;

	int hours;
	int minutes;
	int seconds;
	int totalTime;

	while (!stop)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
			{
				//set stop flag
				stop = true;
			}
			// Handle mouse event for "Hint" button
			if (mHintButton.getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN)
			{
				// Set check solution flag
				hintCount++;
				checkHint = true;
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
			//Handle mouse event for input Buttons
			for(int i = 0; i<= 9; i++)
			{
				mInput[i].setTexture(mTextureCache[i]);
				if(mInput[i].getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN)
					//Handle Mouse input Event
					currentCellSelected->handleDisplayedInput(mTextureCache, i);
				mInput[i].renderButton(mRenderer);
				mInput[i].centerTextureRect();
				mInput[i].renderTexture(mRenderer);
			}

			// Handle keyboard events for current cell selected
			currentCellSelected->handleKeyboardEvent(&event, mTextureCache);

			//check if exit is clicked or not
			if(mExit.getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN)
            {
                exit = stop = true;
            }
			mExit.renderEvent(mRenderer);
		}
		// If "New" button was clicked
		if (generateNewSudoku)
		{
			// Generate new sudoku
			generateSudoku(level);

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

			//Reset Hint to false
			hintCount = 0;
			checkHint = false;
			incorrectCell = 0;

			// Reset timer
			time(&startTimer);
		}
		
		for (int cell = 0; cell < mTotalCells; cell++)
		{
			//Check correctness
			if (!mGrid[cell].isCorrect()  && mGrid[cell].getTexture() != mTextureCache[0])
			{
				mGrid[cell].setIncorrect(true);
				incorrectCell = cell;
			}
			else
			{
				mGrid[cell].setIncorrect(false);
			}

			// If "Hint" button was clicked
			mHintButton.setIncorrect(false);
			if (checkHint)
			{
				if(hintCount>5)
				{
					mHintButton.setIncorrect(true);
				}
				else
				{
					currentCellSelected->findSolution(mTextureCache);
					checkHint = false;
				}
			}
			if(hintCount == 5)
			{
				mHintButton.setIncorrect(true);
			}
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
		
		incomplete = false;
		for (int cell = 0; cell < mTotalCells; cell++)
		{
			//check complition
			if(!mGrid[cell].isCorrect())
			{
				incomplete = true;
			}
		}
		//If it is complete end the loop
		if(!incomplete)
			{
				stop = true;
				completed = true;
			}

		// Render Hint button
		mHintButton.renderButton(mRenderer);
		mHintButton.centerTextureRect();
		mHintButton.renderTexture(mRenderer);

		// Render New button
		mNewButton.renderButton(mRenderer);
		mNewButton.centerTextureRect();
		mNewButton.renderTexture(mRenderer);
		
		currentTime = time(nullptr) - startTimer;
		hours = currentTime / 3600;
		minutes = (currentTime % 3600) / 60;
		seconds = currentTime % 60;

		// Render the timer
		std::stringstream timerStream;
		timerStream << std::setfill('0') << std::setw(2) << hours << ":"
					<< std::setfill('0') << std::setw(2) << minutes << ":"
					<< std::setfill('0') << std::setw(2) << seconds;
		elapshedTime = timerStream.str();
		std::string timerStr = "Time elapsed " + elapshedTime;

		// Load and render timer
		SDL_Texture* timerTexture = nullptr;
		SDL_Color fontColour = { 0, 0, 0, SDL_ALPHA_OPAQUE }; // black
		loadTexture(timerTexture, timerStr.c_str(), fontColour);
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
	// Set current cell selected to false
	currentCellSelected->setSelected(false);

	// Disable text input
	SDL_StopTextInput();


	//Calculate total time elapshed
	totalTime = hours*3600 + minutes*60 + seconds;

	if(exit)
	{
		//Reset to Black and Clear the rendarer
    	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(mRenderer);
		return Buttons::LEVEL;
	}
	else if(completed)
	{
		if(totalTime < mTime)
		{
			myfile.setData(totalTime, elapshedTime, mUserName, level);
			mCongrate(true);
		}
		else
			mCongrate(false);
		//Reset to Black and Clear the rendarer
    	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(mRenderer);
		return Buttons::LEVEL;
	}
	else
	{
		//Reset to Black and Clear the rendarer
    	SDL_SetRenderDrawColor(mRenderer, BlackColor.r, BlackColor.g, BlackColor.b, BlackColor.a);
		SDL_RenderClear(mRenderer);
		return Buttons::CLOSE;
	}
}

void Sudoku::Sudoku::close()
{
	// Destroy
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	mRenderer = nullptr;
	mWindow = nullptr;

	// Free button textures
	freeTextures();

	//free Image
	SDL_FreeSurface(mImageSurface);

	// Free font
	TTF_CloseFont(mFont);
	mFont = nullptr;

	// Quit
	SDL_Quit();
	TTF_Quit();
}
