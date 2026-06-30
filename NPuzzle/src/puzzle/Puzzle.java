package puzzle;

import board.Board;
import heuristics.Heuristic;

import java.util.PriorityQueue;

public class Puzzle {
    public Board inputBoard, targetBoard;

    public Puzzle(Board inputBoard) {
        this.inputBoard = inputBoard;

        // generating default targetBoard
        int k = inputBoard.k;
        int [][] a = new int[k][k];
        int c = 1;
        for(int i=0; i<k; i++){
            for(int j=0; j<k; j++){
                if(c == k*k){
                    a[i][j] = 0;
                }
                else {
                    a[i][j] = c;
                }
                c++;
            }
        }

        this.targetBoard = new Board(k,a);
    }

    public Puzzle(Board inputBoard, Board targetBoard) {
        this.inputBoard = inputBoard;
        this.targetBoard = targetBoard;
    }

    public Boolean isSolvable(){
        int inputBoardInvCount = inputBoard.getInvCount();
        int targetBoardInvCount = targetBoard.getInvCount();

        int inputInvariant, targetInvariant;

        int k = inputBoard.k;

        if(k % 2 != 0){     // odd k
            inputInvariant = inputBoardInvCount % 2;
            targetInvariant = targetBoardInvCount % 2;
        } else {    // even k
            int inputBlankRowFromBottom = k - inputBoard.getRowCol(0)[0];
            int targetBlankRowFromBottom = k - targetBoard.getRowCol(0)[0];

            inputInvariant = (inputBoardInvCount + inputBlankRowFromBottom)%2;
            targetInvariant = (targetBoardInvCount + targetBlankRowFromBottom)%2;
        }

        return (inputInvariant == targetInvariant);
    }
}
