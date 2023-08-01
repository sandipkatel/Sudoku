#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <stdlib.h> // for atoi

#include "Button.h"

namespace Sudoku
{
	class Cell : public Button
	{
	private:
		// Define editiability (can type number in cell)
		bool mEditable;

		// Define number currently displayed and the actual solution
		char mCharNumber;
		char mCharSolution;

	public:
		// Constructor
		Cell();

		// Set and get number
		void setNumber(const int number);
		char getNumber() const;

		// Set solution
		void setSolution(const int solution);

		// Set and get editability (setting editability changes colour mapping for button)
		void setEditable(const bool editable);
		bool isEditable() const;

		// Handle events
		void handleKeyboardEvent(const SDL_Event* event, SDL_Texture* textureCache[]);

		// Compare number with solution
		bool isCorrect() const;

	};

};
//#include "SudokuCell.h"

Sudoku::Cell::Cell()
	: mEditable(false),
	  mCharNumber(' '), 
	  mCharSolution(' ')
{

}

void Sudoku::Cell::setNumber(const int number)
{
	if (number == 0)
	{
		mCharNumber = ' ';
	}
	else
	{
		mCharNumber = '0' + number;
	}
}

char Sudoku::Cell::getNumber() const
{
	return mCharNumber;
}

void Sudoku::Cell::setSolution(const int solution)
{
	if (solution == 0)
	{
		mCharSolution = ' ';
	}
	else
	{
		mCharSolution = '0' + solution;
	}
}

void Sudoku::Cell::setEditable(const bool editable)
{
	mEditable = editable;
	if (mEditable)
	{
		mMouseOutColour = { 219, 184, 215, SDL_ALPHA_OPAQUE }; // light purple
		mMouseOverMotionColour = { 95, 89, 191, SDL_ALPHA_OPAQUE }; // blue
		mMouseDownColour = { 91, 191, 116, SDL_ALPHA_OPAQUE }; // green
		mMouseUpColour = { 95, 89, 191, SDL_ALPHA_OPAQUE }; // blue
	}
	else
	{
		mMouseOutColour = { 159, 101, 152, SDL_ALPHA_OPAQUE }; // purple
		mMouseOverMotionColour = { 159, 101, 152, SDL_ALPHA_OPAQUE }; // purple
		mMouseDownColour = { 159, 101, 152, SDL_ALPHA_OPAQUE }; // purple
		mMouseUpColour = { 159, 101, 152, SDL_ALPHA_OPAQUE }; // purple
	}
}

bool Sudoku::Cell::isEditable() const
{
	return mEditable;
}

void Sudoku::Cell::handleKeyboardEvent(const SDL_Event* event, SDL_Texture* textureCache[])
{
	// Handle backspace
	if (event->key.keysym.sym == SDLK_BACKSPACE && mCharNumber != ' ')
	{
		// Empty char
		mCharNumber = ' ';

		// Set empty texture
		setTexture(textureCache[0]);
	}
	// Handle text input
	else if (event->type == SDL_TEXTINPUT)
	{
		// Check if integer > 0
		if (atoi(event->text.text))
		{
			// Replace char
			mCharNumber = *(event->text.text);

			// Set character based on number
			setTexture(textureCache[atoi(event->text.text)]);

		}
	}
}


bool Sudoku::Cell::isCorrect() const
{
	return mCharNumber == mCharSolution;
}

