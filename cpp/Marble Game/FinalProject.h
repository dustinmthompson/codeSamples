#ifndef FINALPROJECT_H_INCLUDED
#define FINALPROJECT_H_INCLUDED

using namespace std;

class Square{
private:
	int marbles = 3;
public:
    int getMarbles(){
        return marbles;
    }
    void setMarbles(int newCount){
    	marbles = newCount;
    }
    void addMarble(){
    	marbles ++;
    }
};
inline
ostream& operator<<(ostream& output, Square& square){
    output << square.getMarbles();
    return output;
}

class Board{
private:
	Square squares[11];
public:
    Square& getSquare(int num){
        return squares[num];
    }
	void printBoard();
	void endGame(Board board);
};

class Player1{
public:
	void selectMove(Board board);
	void moveMarbles(Board board, int start);
};

class Computer{
public:
	void moveMarbles(Board board);
};

#endif // FINALPROJECT_H_INCLUDED
