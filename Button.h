/* A button class with colours and a texture*/
#pragma once
#include <SDL2/SDL.h>

enum class ButtonState
{
	BUTTON_MOUSE_OUT = 0,
	BUTTON_MOUSE_OVER_MOTION = 1,
	BUTTON_MOUSE_DOWN = 2,
	BUTTON_MOUSE_UP = 3,
};

class Button
{

private:
	// Current button state
	ButtonState mCurrentState;

	// Define texture to display
	SDL_Texture* mTexture;

	// Top left position and dimensions of button and texture
	SDL_Rect mButtonRect;
	SDL_Rect mTextureRect;

	// Has the button been selected by the mouse
	bool mSelected;

protected:
	// Define button colours
	SDL_Color mMouseOutColour;
	SDL_Color mMouseOverMotionColour;
	SDL_Color mMouseDownColour;
	SDL_Color mMouseUpColour;

public:
	// Constructor
	Button();

	// Set Texture
	void setTexture(SDL_Texture* texture);

	// Set button and texture SDL_Rect
	void setButtonRect(const SDL_Rect& rect);
	void setTextureRect(const SDL_Rect& rect);

	// Center texture onto button
	void centerTextureRect(float, char);

	// Set selected
	void setSelected(const bool selected);

	// Check if mouse is inside button
	bool isMouseInside(const int x, const int y);
	
	// Get mouse event
	ButtonState getMouseEvent(const SDL_Event* event);

	// Set mouse down colour
	void setMouseDownColour(const SDL_Color& colour);

	// Render button and texture
	void renderButton(SDL_Renderer* renderer);
	void renderTexture(SDL_Renderer* renderer);

	//Display Rectangle
	void renderRectangle(SDL_Renderer* renderer, SDL_Rect Rect, bool fill);

	//Display rectangle with texts in one function call
	void renderTexts(SDL_Renderer* renderer, SDL_Rect Rect, SDL_Texture* texture, float minmz, char ch);
};

//#include "Button.h"

Button::Button()
	: mCurrentState(ButtonState::BUTTON_MOUSE_OUT),
	  mTexture(nullptr),
	  mButtonRect({ 0, 0, 0, 0 }),
	  mTextureRect({0, 0, 0, 0}),
	  mMouseOutColour({ 153, 209, 213, SDL_ALPHA_OPAQUE }), // Light Green
	  mMouseOverMotionColour({204, 255, 255, SDL_ALPHA_OPAQUE }), // Green Blue
	  mMouseDownColour({ 91, 191, 116, SDL_ALPHA_OPAQUE }), // Green
	  mMouseUpColour({ 204, 255, 255, SDL_ALPHA_OPAQUE }), // Green Blue
	  mSelected(false)
{

}

void Button::setTexture(SDL_Texture* texture)
{
	mTexture = texture;
}

void Button::setButtonRect(const SDL_Rect& rect)
{
	mButtonRect = rect;
}

void Button::setTextureRect(const SDL_Rect& rect)
{
	mTextureRect = rect;
}

void Button::centerTextureRect(float div = 1, char ch= 'y')  //if query texture gave texture huge than required use div to minimize them
{															//if no need to centarize pass char to initialize from begining of rectangle
	int textureWidth;
	int textureHeight;
	SDL_QueryTexture(mTexture, NULL, NULL, &textureWidth, &textureHeight);
	textureHeight/=div;
	textureWidth/=div;
	int textureStartRow;
	int textureStartCol;
	if (ch == 'y')
	{
		textureStartRow = mButtonRect.y + 0.5 * (mButtonRect.h - textureHeight);
		textureStartCol = mButtonRect.x + 0.5 * (mButtonRect.w - textureWidth);
	}
	else
	{
		textureStartRow = mButtonRect.y;
		textureStartCol = mButtonRect.x;
	}
	mTextureRect = { textureStartCol, textureStartRow, textureWidth, textureHeight};
}

void Button::setSelected(const bool selected)
{
	mSelected = selected;
}

bool Button::isMouseInside(const int x, const int y)
{
	// Check if mouse is in button
	bool inside = true;

	// Mouse is left of the button
	if (x < mButtonRect.x)
	{
		inside = false;
	}
	// Mouse is right of the button
	else if (x > mButtonRect.x + mButtonRect.w)
	{
		inside = false;
	}
	// Mouse above the button
	else if (y < mButtonRect.y)
	{
		inside = false;
	}
	// Mouse below the button
	else if (y > mButtonRect.y + mButtonRect.h)
	{
		inside = false;
	}

	return inside;
}

ButtonState Button::getMouseEvent(const SDL_Event* event)
{
	//If mouse event happened
	if (event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP)
	{
		// Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		// Mouse is outside button
		if (!isMouseInside(x, y))
		{
			mCurrentState = ButtonState::BUTTON_MOUSE_OUT;
		}
		// Mouse is inside button
		else
		{
			// Set mouse over State
			switch (event->type)
			{
			case SDL_MOUSEMOTION:
				mCurrentState = ButtonState::BUTTON_MOUSE_OVER_MOTION;
				break;

			case SDL_MOUSEBUTTONDOWN:
				mCurrentState = ButtonState::BUTTON_MOUSE_DOWN;
				break;

			case SDL_MOUSEBUTTONUP:
				mCurrentState = ButtonState::BUTTON_MOUSE_UP;
				break;

			}
		}
	}
	return mCurrentState;
}

void Button::setMouseDownColour(const SDL_Color& colour)
{
	mMouseDownColour = colour;
}

void Button::renderButton(SDL_Renderer* renderer)
{
	// If current button is selected
	if (mSelected)
	{
		SDL_SetRenderDrawColor(renderer, mMouseDownColour.r, mMouseDownColour.g, mMouseDownColour.b, mMouseDownColour.a);
	}
	else
	{
		// Show current button State
		switch (mCurrentState)
		{
		case ButtonState::BUTTON_MOUSE_OUT:
			SDL_SetRenderDrawColor(renderer, mMouseOutColour.r, mMouseOutColour.g, mMouseOutColour.b, mMouseOutColour.a);
			break;
		case ButtonState::BUTTON_MOUSE_OVER_MOTION:
			SDL_SetRenderDrawColor(renderer, mMouseOverMotionColour.r, mMouseOverMotionColour.g, mMouseOverMotionColour.b, mMouseOverMotionColour.a);
			break;
		case ButtonState::BUTTON_MOUSE_DOWN:
			SDL_SetRenderDrawColor(renderer, mMouseDownColour.r, mMouseDownColour.g, mMouseDownColour.b, mMouseDownColour.a);
			break;
		/*case ButtonState::BUTTON_MOUSE_UP:
			SDL_SetRenderDrawColor(renderer, mMouseUpColour.r, mMouseUpColour.g, mMouseUpColour.b, mMouseUpColour.a);
			break;*/
		}
	}

	SDL_RenderFillRect(renderer, &mButtonRect);
}

void Button::renderTexture(SDL_Renderer* renderer)
{
	// Set rendering space
	SDL_RenderCopy(renderer, mTexture, nullptr, &mTextureRect);
}

void Button::renderRectangle(SDL_Renderer* renderer, SDL_Rect Rect, bool fill)
{
	if(fill == 1)
		SDL_RenderFillRect(renderer, &Rect);
	else
		SDL_RenderDrawRect(renderer, &Rect);
}

void Button::renderTexts(SDL_Renderer* renderer, SDL_Rect Rect, SDL_Texture* texture, float minmz = 1, char ch= 'y')
{
	int textureWidth;
	int textureHeight;

    mTexture = texture;
	mButtonRect = Rect;

	SDL_QueryTexture(mTexture, NULL, NULL, &textureWidth, &textureHeight);
	textureHeight/=minmz;
	textureWidth/=minmz;

	int textureStartRow;
	int textureStartCol;
	if (ch == 'y')
	{
		textureStartRow = mButtonRect.y + 0.5 * (mButtonRect.h - textureHeight);
		textureStartCol = mButtonRect.x + 0.5 * (mButtonRect.w - textureWidth);
	}
	else
	{
		textureStartRow = mButtonRect.y;
		textureStartCol = mButtonRect.x;
	}
	mTextureRect = { textureStartCol, textureStartRow, textureWidth, textureHeight};

    // Render the texture
   SDL_RenderCopy(renderer, mTexture, nullptr, &mTextureRect);

    // Clean up the texture
   SDL_DestroyTexture(mTexture);
}