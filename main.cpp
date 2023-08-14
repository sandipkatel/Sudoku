/* Play Sudoku */
#include <string>
char username[] = "username: sandip"; //Dummy username
char bestscore[] = "Best time: 00:22:23"; //just a format
char bestgammer[] = "By: katel";  //just a format
#include "Sudoku.h"

int main(int argc, char* argv[])
{
	// Create Sudoku game object
	Sudoku::Sudoku S;

	// Play Sudoku game
	S.play();

	return 0;
}