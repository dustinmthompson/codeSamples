/**
 * A room contains an 8x8 chessboard with 64 coins spread across it, either being heads or tails. Given a "magic square",
 * a prisoner must flip one coin only to communicate to another prisoner what the "magic square" is. If the second prisoner can successfully
 * find the magic square, both prisoners are freed.
 *
 * http://datagenetics.com/blog/december12014/index.html
 */

package intermediate;
import java.util.Random;

public class chessboardPuzzle {
    public static void main(String[] args) {
        Random rand = new Random();
        int[] boardState = new int[64];
        int magicSquare = rand.nextInt(64);
        
        // Fill the board state with random 0's and 1's (heads and tails)
        for (int i = 0; i < boardState.length; i++) {
            boardState[i] = rand.nextInt(2);
        }

        // Prisoner 1 flips a coin on the board based on the magic square, resulting in a new board state
        int[] flippedBoard = flip(boardState, prisoner1(boardState, magicSquare));
        
        // Prisoner 2 outputs the magic square based on the new board state alone
        System.out.println(prisoner2(flippedBoard) == magicSquare);
    }

    // Flip a coin value given the boardstate and tile position
    public static int[] flip(int[] boardState, int tilePos){
        boardState[tilePos] = 1 - boardState[tilePos];

        return boardState;
    }

    // Given the board state and magic square, output the correct coin to flip
    public static int prisoner1(int[] boardState, int magicSquare){
        // The coin to flip can be found by taking the board state divided 6 times
		// and represented as a binary number and XORing it with the selected magic square
        int parity = 0;
        for (int i = 0; i < boardState.length; i++) {
            parity ^= i*boardState[i];
        }

        return parity^magicSquare;
    }

    // Given the new board state, determine what the magic square is
    public static int prisoner2(int[] flippedState){
		// To then find the magic square, prisoner 2 must divide the board just as prisoner 1 did.
		// The parity of the board will represent which square is the magic square. This can be done
		// by calling the first function and XORing it with nothing to achieve the parity.
        return prisoner1(flippedState, 0);
    }
}
