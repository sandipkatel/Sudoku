//fileHandling.h
/* handleFile slass*/
#pragma once
#include <string>
#include <fstream>
#include <limits> //to use numeric_limit
namespace BackWork
{
    class handleFile
    {
        private:
            int mTime;
            std::string mLevel;
            std::string mBestTime;
            std::string mPlayerName;
            int entryCount = 0; // Counter for number of entries read

        public:
            void setData(int timeElapshed, std::string bestTime, std::string username, Buttons Level);
            int getData(std::string (&myData)[], Buttons home);
            void getData(Buttons Level, std::string &bestTime, std::string &bestPleyer, int &Time);
            void writeToFile(handleFile hf);
    };
}

//Write things to file
void BackWork::handleFile::writeToFile(handleFile hf)
{
    // Open the file in read mode to read existing data
    std::ifstream inputFile("myData.txt");
    if (!inputFile)
    {
        std::cerr << "Failed to open file for reading." << std::endl;
        return;
    }

    // Create a temporary file to write modified data
    std::ofstream tempFile("tempData.txt");
    if (!tempFile)
    {
        std::cerr << "Failed to open temp file for writing." << std::endl;
        inputFile.close();
        return;
    }

    std::string line;
    bool found = false;

    // Copy data to temp file, updating Level: Hard data
    while (std::getline(inputFile, line))
    {
        tempFile << line << std::endl;
        if (line == ("Level: " + hf.mLevel))
        {
            found = true;
            tempFile << "Shortest Time Elapsed: " << hf.mBestTime  << std::endl;
            tempFile << "By: " << hf.mPlayerName << std::endl;
            tempFile << hf.mTime << std::endl;
            tempFile << "        "; //to avoid Could not create TTF SDL_Surface! Error: Text has zero width
            for(int i = 0; i< 3; i++)
                inputFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore 3 lines
        }
        if (found && line.empty())
        {
            break; // Stop after updating the Level: Hard data
        }
    }

    // Copy the remaining data
    while (std::getline(inputFile, line))
    {
        tempFile << line << std::endl;
    }

    // Close the input and temporary files
    inputFile.close();
    tempFile.close();

    // Rename the temporary file to the original file name
    std::remove("myData.txt");
    std::rename("tempData.txt", "myData.txt");
}

//Set Datas if new best score
void BackWork::handleFile::setData(int timeElapshed, std::string bestTime, std::string username, Buttons Level)
{
    mTime = timeElapshed;
    mBestTime = bestTime;
    mPlayerName = username;
    if(Level == Buttons::HARD)
        mLevel = "Hard";
    else if(Level == Buttons::MEDIUM)
        mLevel = "Medium";
    else if(Level == Buttons::EASY)
        mLevel = "Easy";
    writeToFile(*this);
}

int BackWork::handleFile::getData(std::string (&myData)[], Buttons home)
{
    // Open the file
    std::ifstream scoreBoard("myData.txt", std::ios::in | std::ios::binary);
    if (!scoreBoard)
    {
        std::cerr << "Failed to open file for reading." << std::endl;
        return -1;
    }
    std::string findString[2];
    if(home == Buttons::BEST)
    {
        findString[0] = "BEST";
        findString[1] = "//end of best";
    }
    else if(home == Buttons::ABOUT)
    {
        findString[0] = "ABOUT";
        findString[1] = "//end of about";
    }
    else if(home == Buttons::HELP)
    {
        findString[0] = "HELP";
        findString[1] = "//end of help";
    }

    std::string line;
    entryCount = 0;
    int lineCount = 0;
    bool readingSection = false;
    while(std::getline(scoreBoard, line))
    {
        lineCount++;
        if (line.find(findString[0]) != std::string::npos)
        {
                readingSection = true;
                continue;
        } 
        else if (line.find(findString[1]) != std::string::npos)
        {
            readingSection = false;
            break; // Exit loop after reaching end of best section
        }
        if (readingSection && !line.empty() && home != Buttons::BEST)
        {
            myData[entryCount] = line;
            // Pop last element i.e newline character else it will be displayed
            myData[entryCount].pop_back();
            entryCount++;
        }
        else if(readingSection && !line.empty() && home == Buttons::BEST)
        {
            //Ignore integer time
            if ((lineCount%5) != 0)
            {
                myData[entryCount] = line;
                //Pop last element i.e newline character else it will be displayed
                myData[entryCount].pop_back();
                entryCount++;
            }
        }
    }
    //Close the file
    scoreBoard.close();
    return entryCount;
}

void BackWork::handleFile::getData(Buttons Level, std::string &bestTime, std::string &bestPleyer, int &Time)
{
    if (Level == Buttons::HARD)
        mLevel = "Hard";
    else if (Level == Buttons::MEDIUM)
        mLevel = "Medium";
    else if (Level == Buttons::EASY)
        mLevel = "Easy";

    // Open the file in read mode to read existing data
    std::ifstream inputFile("myData.txt");
    if (!inputFile)
    {
        std::cerr << "Failed to open file for reading." << std::endl;
        return;
    }

    bool found = false;
    std::string line;

    // Copy data to temp file, updating Level: Hard data
    while (std::getline(inputFile, line))
    {
        if (line == ("Level: " + mLevel))
        {
            found = true;
            // Read and store lines
            std::getline(inputFile, bestTime);
            std::getline(inputFile, bestPleyer);
            inputFile >> Time;
            break;
        }
    }

    inputFile.close();
}
