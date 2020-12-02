#include <iostream>
#include <string>
#include <cstdlib>
#include "FinalProject.h"

using namespace std;

int main(){
    Board board;
    Player1 player;

    board.getSquare(5).setMarbles(0);
    board.getSquare(10).setMarbles(0);

    player.selectMove(board);
}

void Board::printBoard(){
	// Prints board using marble count from Board
	cout << "---------------------------\t\tPositions:";
	cout << "\n| |-|   (" << squares[9] << ")     (" << squares[4] << ")   |-| |\t\t(10)     (5)";
	cout << "\n| | |     (" << squares[8] << ") (" << squares[3] << ")     | | |\t\t  (9) (4)";
	cout << "\n| |" << squares[10] << "|       (" << squares[2] << ")       |" << squares[5] << "| |\t\t    (3)";
	cout << "\n| | |     (" << squares[1] << ") (" << squares[7] << ")     | | |\t\t  (2) (8)";
	cout << "\n| |-|   (" << squares[0] << ")     (" << squares[6] << ")   |-| |\t\t(1)     (7)";
	cout << "\n---------------------------";
}

void Player1::selectMove(Board board){
    Player1 player;
	int selection;
	board.printBoard();

	// Until a valid square is selected, ask for starting square
	do {
        cin.clear();
		cout << "\nSelect a square to start: ";
		cin >> selection;
	}while((selection != 1 && selection != 2 && selection != 3 && selection != 4 && selection != 5 && selection != 7 && selection != 8 && selection != 9 && selection != 10)
           || board.getSquare(selection-1).getMarbles()==0);

    player.moveMarbles(board, selection);
}

void Player1::moveMarbles(Board board, int start){
	// Get marble count
	int marbleCount = board.getSquare(start-1).getMarbles();

	// Remove all marbles from starting square
	board.getSquare(start-1).setMarbles(0);

	bool passingRight=true;

	// Move marbles throughout the squares
	do{
		switch(start){
			case 1:
				board.getSquare(1).addMarble();
				start = 2;
				marbleCount--;
				break;
			case 2:
				board.getSquare(2).addMarble();
				start = 3;
				marbleCount--;
				passingRight=true;
				break;
			case 3:
				if (passingRight) // If going from left to right across the center
				{
					board.getSquare(3).addMarble();
					start = 4;
					marbleCount--;
					break;
				}
				else // If going from right to left across the center
				{
					board.getSquare(8).addMarble();
					start = 9;
					marbleCount--;
					break;
				}
			case 4:
				board.getSquare(4).addMarble();
				start = 5;
				marbleCount--;
				break;
			case 5:
				board.getSquare(5).addMarble();
				start = 6;
				marbleCount--;
				break;
			case 6:
				board.getSquare(6).addMarble();
				start = 7;
				marbleCount--;
				break;
			case 7:
				board.getSquare(7).addMarble();
				start = 8;
				marbleCount--;
				break;
			case 8:
				board.getSquare(2).addMarble();
				start = 3;
				marbleCount--;
				passingRight=false;
				break;
			case 9:
				board.getSquare(9).addMarble();
				start = 10;
				marbleCount--;
				break;
			case 10:
				board.getSquare(0).addMarble();
				start = 1;
				marbleCount--;
				break;
		}
	}while(marbleCount != 0);

	// If end zone has 5+, end game. Otherwise CPU turn
	if (board.getSquare(5).getMarbles() >= 5)
	{
		board.endGame(board);
	}
	// If landed in end zone, play turn again
	else if (start == 6)
	{
		Player1 player;
		player.selectMove(board);
	}
	else
	{
		Computer cpu;
		cpu.moveMarbles(board);
	}
}

void Computer::moveMarbles(Board board){
    int start;
	bool passingRight;
	char temp;
	board.printBoard();

	// Check if any start can end in CPU's end zone
	if (board.getSquare(0).getMarbles() == 10)
	{
		start = 1;
	}
	else if (board.getSquare(1).getMarbles() == 9)
	{
		start = 2;
	}
	else if ((board.getSquare(2).getMarbles() == 8) || (board.getSquare(2).getMarbles() == 3))
	{
		if (board.getSquare(2).getMarbles() == 8)
		{
			passingRight = true;
			start = 3;
		}
		else
		{
			passingRight = false;
			start = 3;
		}
	}
	else if (board.getSquare(3).getMarbles() == 7)
	{
		start = 4;
	}
	else if (board.getSquare(4).getMarbles() == 6)
	{
		start = 5;
	}
	else if (board.getSquare(6).getMarbles() == 5)
	{
		start = 7;
	}
	else if (board.getSquare(7).getMarbles() == 4)
	{
		start = 8;
	}
	else if (board.getSquare(8).getMarbles() == 3)
	{
		start = 9;
	}
	else if (board.getSquare(9).getMarbles() == 2)
	{
		start = 10;
	}
	else if (board.getSquare(9).getMarbles() == 1)
	{
		start = 11;
	}
	// None could be put in end zone, get highest marble square on the right to left path
	else
	{
		int tempMarble = 0;
		if (board.getSquare(6).getMarbles() > tempMarble)
		{
			tempMarble = board.getSquare(6).getMarbles();
			start = 7;
		}
		if (board.getSquare(7).getMarbles() > tempMarble)
		{
			tempMarble = board.getSquare(7).getMarbles();
			start = 8;
		}
		if (board.getSquare(2).getMarbles() > tempMarble)
		{
			tempMarble = board.getSquare(2).getMarbles();
			start = 3;
			passingRight = false;
		}
		if (board.getSquare(8).getMarbles() > tempMarble)
		{
			tempMarble = board.getSquare(8).getMarbles();
			start = 9;
		}
		if (board.getSquare(9).getMarbles() > tempMarble)
		{
			tempMarble = board.getSquare(9).getMarbles();
			start = 10;
		}
	}

	// Get marble count
	int marbleCount = board.getSquare(start-1).getMarbles();
	int originalStart = start;

	// Remove all marbles from starting square
	board.getSquare(start-1).setMarbles(0);

	// Move marbles throughout the squares
	do{
		switch(start){
			case 1:
				board.getSquare(1).addMarble();
				start = 2;
				marbleCount--;
				break;
			case 2:
				board.getSquare(2).addMarble();
				start = 3;
				marbleCount--;
				passingRight=true;
				break;
			case 3:
				if (passingRight)
				{
					board.getSquare(3).addMarble();
					start = 4;
					marbleCount--;
				}
				else
				{
					board.getSquare(8).addMarble();
					start = 9;
					marbleCount--;
				}
				break;
			case 4:
				board.getSquare(4).addMarble();
				start = 5;
				marbleCount--;
				break;
			case 5:
				board.getSquare(6).addMarble();
				start = 7;
				marbleCount--;
				break;
			case 7:
				board.getSquare(7).addMarble();
				start = 8;
				marbleCount--;
				break;
			case 8:
				board.getSquare(2).addMarble();
				start = 3;
				marbleCount--;
				passingRight=false;
				break;
			case 9:
				board.getSquare(9).addMarble();
				start = 10;
				marbleCount--;
				break;
			case 10:
				board.getSquare(10).addMarble();
				start = 11;
				marbleCount--;
				break;
			case 11:
				board.getSquare(0).addMarble();
				start = 1;
				marbleCount--;
				break;
		}
	}while(marbleCount != 0);

	// If end zone has 5+, end game. Otherwise CPU turn
	if (board.getSquare(10).getMarbles() >= 5)
	{
		board.endGame(board);
	}
	// If landed in end zone, play turn again
	else if (start == 11)
	{
		Computer computer;
		cout << "\n\nComputer moved from Square " << originalStart << endl;
		computer.moveMarbles(board);
	}
	else
	{
		Player1 player;
		cout << "\n\nComputer moved from Square " << originalStart << endl;
		player.selectMove(board);
	}
}

void Board::endGame(Board board){
	// Add player 1 right-side marbles
	board.getSquare(5).setMarbles( board.getSquare(3).getMarbles() + board.getSquare(4).getMarbles()
									+ board.getSquare(6).getMarbles() + board.getSquare(7).getMarbles());
	board.getSquare(3).setMarbles(0);
	board.getSquare(4).setMarbles(0);
	board.getSquare(6).setMarbles(0);
	board.getSquare(7).setMarbles(0);

	// Add Computer left-side marbles
	board.getSquare(10).setMarbles( board.getSquare(0).getMarbles() + board.getSquare(1).getMarbles()
									+ board.getSquare(8).getMarbles() + board.getSquare(9).getMarbles());
	board.getSquare(0).setMarbles(0);
	board.getSquare(1).setMarbles(0);
	board.getSquare(8).setMarbles(0);
	board.getSquare(9).setMarbles(0);

	// Print End Result
	cout << "\n";
	board.printBoard();
	if (board.getSquare(5).getMarbles() > board.getSquare(10).getMarbles())
	{
		cout << "\nYou win!";
	}
	else
		cout << "\nYou lost. Better luck next time.";
}
