package heuristics;

import board.Board;

public class Manhattan extends Heuristic{
    public Manhattan(){
        this.name = "Manhattan";
    }
    @Override
    public int calculate(Board currentBoard, Board targetBoard) {
        int d = 0;

        for(int i=0; i<currentBoard.k; i++){
            for(int j=0; j<currentBoard.k; j++){
                int num = currentBoard.grid[i][j];

                if(num == 0) continue;

                int[] expectedRowCol = targetBoard.getRowCol(num);
                int rowdiff = Math.abs(expectedRowCol[0] - i);
                int coldiff = Math.abs(expectedRowCol[1] - j);

                d += (rowdiff+coldiff);
            }
        }

        return d;
    }
}