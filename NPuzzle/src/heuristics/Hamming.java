package heuristics;

import board.Board;

public class Hamming extends Heuristic{
    public Hamming(){
        this.name = "Hamming";
    }
    @Override
    public int calculate(Board currentBoard, Board targetBoard) {
        int d = 0;

        for(int i=0; i<currentBoard.k; i++){
            for(int j=0; j<currentBoard.k; j++){
                int num = currentBoard.grid[i][j];

                if(num == 0) continue;

                if(num != targetBoard.grid[i][j]){
                    d++;
                }
            }
        }

        return d;
    }
}
