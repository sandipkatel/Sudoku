//GameLoop.h
/*Game looping section of Sudoku class*/
#pragma once
#include "Sudoku.h"

void Sudoku::Sudoku::display()
{
    // Initialise SDL
    if (!initialiseSDL())
    {
        close();
    }
    bool stop = false;
    Buttons clicked;
    //Initially at Home Screen
    clicked = homeScreen();
    while(!stop)
    {
        switch(clicked)
        {
        case Buttons::HOME:
            clicked = homeScreen();
            break;
        case Buttons::USERNAME:
            clicked = userName();
            break;
        case Buttons::ABOUT:
            clicked = about_best(Buttons::ABOUT);
            break;
        case Buttons::BEST:
            clicked = about_best(Buttons::BEST);
            break;
        case Buttons::HELP:
            clicked = about_best(Buttons::HELP);
            break;
        case Buttons::LEVEL:
            clicked = levels();
            break;
        case Buttons::HARD:
            clicked = play(Buttons::HARD);
            break;
        case Buttons::MEDIUM:
            clicked = play(Buttons::MEDIUM);
            break;
        case Buttons::EASY:
            clicked = play(Buttons::EASY);
            break;
        case Buttons::CLOSE:
            stop = true;
            break;
        default:
            stop = true;
            break;
        }
    }
    //Distroy and Quit
    close();
}

//Homescreen Interface
Buttons Sudoku::Sudoku::homeScreen()
{
    //Bool to choose the place to go
    bool start = false,  about = false, best = false, help = false;
	mImage();

    int x, y;
    const int numOfButton = 4;
    const int buttonHeight = mGridHeight / 10;
    const int buttonWidth = mGridWidth / 2 + 10;
    const int gap = buttonHeight / 2;
    x = buttonHeight;
    y = (mGridHeight - numOfButton * buttonHeight - (numOfButton-1) * gap) / 2;
	bool draw = 0;

    //load START, BEST, ABOUT and HELP texture
    SDL_Texture* texture[numOfButton];
	loadTexture(texture[0], "START", GrayColor);
	loadTexture(texture[1], "BEST", GrayColor);
	loadTexture(texture[2], "ABOUT", GrayColor);
    loadTexture(texture[3], "HELP", GrayColor);

    //we already used the mtexts upto 5
    int n = 6;
    // Display the Buttons
    for(int i = 0; i<numOfButton; i++)
   {
        SDL_Rect buttonRect = { x, y, buttonWidth, buttonHeight };
        mTexts[n].renderTexts(mRenderer, buttonRect, texture[i]);
        mRectangle.renderRectangle(mRenderer, buttonRect, draw);
        y += buttonHeight + gap;
        n++;
    }

    SDL_Event event;
    bool stop = false;
    while (!stop) {
        // Handle events on queue
        while (SDL_PollEvent(&event) != 0) {
            // Handle quitting and completion
            if (event.type == SDL_QUIT)
            {
                // Set stop flag
                stop = true;
            }
            if(mTexts[6].getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN)
            {
                start = stop = true;
            }
            
            else if(mTexts[7].getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN)
            {
                best = stop = true;
            }

            else if(mTexts[8].getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN)
            {
                about = stop = true;
            }

            else if(mTexts[9].getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN)
            {
                help = stop = true;
            }
            
            for(int i = 0; i<numOfButton; i++)
            {
                mTexts[6+i].renderEvent(mRenderer);
            }
        }
        // Update the screen
        SDL_RenderPresent(mRenderer);
    }
    //Clear with Black clolr
    SDL_SetRenderDrawColor(mRenderer, BlackColor.r, BlackColor.g, BlackColor.b, BlackColor.a);
    SDL_RenderClear(mRenderer);

    if(start)
        return  Buttons::USERNAME;
    else if(about)
        return Buttons::ABOUT;
    else if(best)
        return Buttons::BEST;
    else if(help)
        return Buttons::HELP;
    else 
        return Buttons::CLOSE;
}

Buttons Sudoku::Sudoku::about_best(Buttons mButton)
{
    mImage();

    //Define X and Y offset
    const int offset = mWindowWidth/26;
    int x = offset;
    int y = offset;
    int buttonLength = mWindowWidth/1.8;
    int buttonHeight = mWindowHeight - 2*x;

    //Display the rectangle
    SDL_Rect Rect = { x, y, buttonLength, buttonHeight };
    mTexts[10].renderRectangle(mRenderer, Rect, 0);
    //Redefine to be back
    Rect = { x + buttonLength, y - offset , mWindowWidth - buttonLength, mWindowHeight - offset};

    SDL_Texture* texture;
    std::string texts[15];
    int totalLines = 0;

    if(mButton == Buttons::BEST)
    {
        totalLines = myfile.getData(texts, Buttons::BEST);
    }
    else if(mButton == Buttons::ABOUT)
    {
        totalLines = myfile.getData(texts, Buttons::ABOUT);
    }
    else if(mButton == Buttons::HELP)
    {
        totalLines = myfile.getData(texts, Buttons::HELP);
    }

    //Redefine for texts
    x += 10;
    y += 10;
    buttonLength -= 2*x;
    buttonHeight /=  1.25*totalLines;
    for(int i = 0; i<totalLines; i++)
    {
    loadTexture(texture, texts[i].c_str(), BlackColor);
    SDL_Rect textRect = { x, y, buttonHeight, buttonLength };
    mTexts[8].renderTexts(mRenderer, textRect, texture, 1.5, 'n');
    y += buttonHeight;
    }

    mTexts[8].setButtonRect(Rect);
    SDL_Event event;
    bool stop = false;
    bool userClicked = false; // A flag to indicate if the user clicked outside
    while(!stop)
    {
        while(SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
                {
                    stop = true;
                }

            if(mTexts[8].getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN)
                {
                    stop = userClicked = true;
                }
        }
        // Update the screen
        SDL_RenderPresent(mRenderer);
    }
    //Reset to Black and Clear the rendarer
    SDL_SetRenderDrawColor(mRenderer, BlackColor.r, BlackColor.g, BlackColor.b, BlackColor.a);
    SDL_RenderClear(mRenderer);

    if(userClicked)
        return Buttons::HOME;
    else
    {
        return Buttons::CLOSE;
    }
}


Buttons Sudoku::Sudoku::userName()
{
    //Set Bg image
    mImage();
    //Display exit button
    exitButton();

    int buttonWidth = mWindowWidth/2.5;
    int buttonHeight = mWindowHeight/15;
    int initialX = (mWindowWidth - buttonWidth)/2;
    int initialY = (mWindowHeight - 2*buttonHeight)/2;

    //Load and Display Textures
    loadTexture(mTextureCache[16], "User Name:", BlackColor);
    SDL_Rect userRect = { initialX, initialY, buttonWidth, buttonHeight };
    mTexts[9].renderTexts(mRenderer, userRect, mTextureCache[16], 1.5, 'n');

    initialY += buttonHeight;
    loadTexture(mTextureCache[17], "User name appers Here", GrayColor);
    userRect = { initialX, initialY, buttonWidth, buttonHeight };
    //Initially display in gray color
    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
    mTexts[10].renderRectangle(mRenderer, userRect, 1);
    mTexts[10].renderTexts(mRenderer, userRect, mTextureCache[17], 1.5);

    //Store something temporarily and proceed
    SDL_Surface* temp = NULL;
    SDL_Texture* textImage = SDL_CreateTextureFromSurface(mRenderer, temp); 
    SDL_FreeSurface(temp); 

    SDL_StartTextInput();
    std::string name = " ";

    bool stop = false;
    bool exit = false;
    bool userEntered = false;  //A flaf to check if User enters the enter key or not
    while(!stop)
    {    
        //Handle events
        SDL_Event event;
        while(SDL_PollEvent(&event) != 0)
        {
            exit = stop = userEntered = false;
           //Handle Possible Events
            switch(event.type)
            {
                case SDL_QUIT:
                    stop = true;
                    break;
                //Get input texts
                case SDL_TEXTINPUT:
                    name += event.text.text;
                    if(textImage)
                    {
                        SDL_DestroyTexture(textImage);
                        textImage = NULL;
                    }

                    temp = TTF_RenderText_Solid(mFont, name.c_str(), BlackColor);
                    if(temp)
                    {
                        textImage = SDL_CreateTextureFromSurface(mRenderer, temp);
                        SDL_FreeSurface(temp);
                        temp = NULL;
                    }
                    //Render the texts
                    mTexts[10].renderRectangle(mRenderer, userRect, 1);
                    mTexts[10].renderTexts(mRenderer, userRect, textImage, 1.5);
                    break;
                //Check keyboard events Enter key and Backspace
                case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER)
                {
                    stop = userEntered = true;
                }
                //check Backspace event
                else if(event.key.keysym.sym == SDLK_BACKSPACE)
                {
                    // Check if the string is not empty
                    if (!name.empty())
                    {
                        // Remove the last character
                        name.pop_back();
                    }
                    if(textImage)
                    {
                        SDL_DestroyTexture(textImage);
                        textImage = NULL;
                    }

                    temp = TTF_RenderText_Solid(mFont, name.c_str(), BlackColor);
                    if(temp)
                    {
                        textImage = SDL_CreateTextureFromSurface(mRenderer, temp);
                        SDL_FreeSurface(temp);
                        temp = NULL;
                    }
                    //Render the texts
                    mTexts[10].renderRectangle(mRenderer, userRect, 1);
                    mTexts[10].renderTexts(mRenderer, userRect, textImage, 1.5);
                }
                    break;
                case SDL_MOUSEMOTION:
                    //Render Exit and restore back to white color
                    mExit.renderEvent(mRenderer);  
                    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
                case SDL_MOUSEBUTTONDOWN://Bug is around here
                if(mExit.getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN)
                {
                    exit = stop = true;
                }
                break;
            }
        }
        // Present the screen
        SDL_RenderPresent(mRenderer);
    }
    if(name.length() <= 1)
        name = "unknown";
    mUserName = name;
    SDL_StopTextInput();

    //Reset to Black and Clear the rendarer
    SDL_SetRenderDrawColor(mRenderer, BlackColor.r, BlackColor.g, BlackColor.b, BlackColor.a);
    SDL_RenderClear(mRenderer);

    if(userEntered)
        return Buttons::LEVEL;
    else if(exit)
        return Buttons::HOME;
    else
        return Buttons::CLOSE;
}


Buttons Sudoku::Sudoku::levels()
{
    //load bg image
    mImage();
    //Display Exit Button
   exitButton();

    //Where to go
    bool easy = false,  normal = false, hard = false, exit = false;

    int x, y;
    const int numOfButton = 3;
    const int buttonHeight = mGridHeight / 10;
    const int buttonWidth = mGridWidth / 2 + 10;
    const int gap = buttonHeight / 2;
    x = buttonHeight;
    y = (mGridHeight - numOfButton * buttonHeight - (numOfButton-1) * gap) / 2;
	bool draw = 0;

    //Load EASY, MEDIUM and HARD textures
    SDL_Texture* texture[numOfButton];
	loadTexture(texture[0], "EASY", GrayColor);
	loadTexture(texture[1], "MEDIUM", GrayColor);
	loadTexture(texture[2], "HARD", GrayColor);

    //we already used the mtexts upto 10
    int n = 11;
    // Display the Buttons
    for(int i = 0; i<numOfButton; i++)
   {
        SDL_Rect buttonRect = { x, y, buttonWidth, buttonHeight };
        mTexts[n].renderTexts(mRenderer, buttonRect, texture[i]);
        mRectangle.renderRectangle(mRenderer, buttonRect, draw);
        y += buttonHeight + gap;
        n++;
    }

    SDL_Event event;
    bool stop = false;
    while (!stop) {
        // Handle events on queue
        while (SDL_PollEvent(&event) != 0) {
            exit = false;
            // Handle quitting and completion
            if (event.type == SDL_QUIT)
            {
                // Set stop flag
                stop = true;
            }
            if(mTexts[11].getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN)
            {
                easy = stop = true;
            }
            else if(mTexts[12].getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN)
            {
                normal = stop = true;
            }
            else if(mTexts[13].getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN)
            {
                hard = stop = true;
            }
            else if(mExit.getMouseEvent(&event) == ButtonState::BUTTON_MOUSE_DOWN)
            {
                exit = stop = true;
            }
            mExit.renderEvent(mRenderer);
            for(int i = 0; i<numOfButton; i++)
            {
                mTexts[11+i].renderEvent(mRenderer);
            }
        }
        // Update the screen
        SDL_SetRenderDrawColor(mRenderer, BlackColor.r, BlackColor.g, BlackColor.b, BlackColor.a);
        SDL_RenderPresent(mRenderer);
    }
    //Clear the rendarer
    SDL_RenderClear(mRenderer);

    if(easy)
        return  Buttons::EASY;
    else if(normal)
        return Buttons::MEDIUM;
    else if(hard)
        return Buttons::HARD;
    else if(exit)
        return Buttons::HOME;
    else 
        return Buttons::CLOSE;
}
