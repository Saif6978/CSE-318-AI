package board;

import puzzle.State;
import util.InversionCounter;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

public class Board {
    public int k, n;
    public int[][] grid;   // 2D array containing the board with blank as 0
    public Map<Integer, int[]> numToRowColMap = new HashMap<>();

    public Board(int k, int[][] input) {
        this.k = k;
        this.n = k*k-1;
        this.grid = new int[k][k];
        int c = 0;
        for(int i=0; i<k; i++){
            for(int j=0; j<k; j++){
                this.grid[i][j] = input[i][j];
                numToRowColMap.put(grid[i][j], new int[]{i, j});
                c++;
            }
        }
    }

    public int[] getRowCol(int num){
        if(numToRowColMap.containsKey(num)){
            return numToRowColMap.get(num);
        }
        return new int[]{-1,-1};
    }

    public int getInvCount(){
        int[] a = new int[n];
        int c = 0;
        for(int i=0; i<k; i++){
            for(int j=0; j<k; j++){
                int x = grid[i][j];
                if(x != 0){
                    a[c++] = x;
                }
            }
        }
        return InversionCounter.countInversions(a);
    }

    @Override
    public int hashCode() {
        return Arrays.deepHashCode(grid);
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (obj == null || getClass() != obj.getClass()) {
            return false;
        }
        Board other = (Board) obj;
        if (this.k != other.k) {
            return false;
        }
        return Arrays.deepEquals(this.grid, other.grid);
    }

    private boolean isValid(int row, int col) {
        return row >= 0 && row < k && col >= 0 && col < k;
    }

    private int[][] copyOwnGrid() {
        int[][] newGrid = new int[k][k];
        for(int i=0; i<k; i++){
            for(int j=0; j<k; j++){
                newGrid[i][j] = grid[i][j];
            }
        }
        return newGrid;
    }

    public ArrayList<Board> getNeighborBoards(Board parentBoard){
        ArrayList<Board>neighborBoards = new ArrayList<>();
        int[] blankRowCol = getRowCol(0);

        int blankRow = blankRowCol[0];
        int blankCol = blankRowCol[1];

        int[][] moves = {
                {1, 0}, // DOWN
                {-1, 0}, // UP
                {0, 1}, // RIGHT
                {0, -1}  // LEFT
        };

        for (int[] move : moves) {
            int newRow = blankRow + move[0];
            int newCol = blankCol + move[1];

            if (isValid(newRow, newCol)) {
                int[][] newGrid = copyOwnGrid();
                newGrid[blankRow][blankCol] = newGrid[newRow][newCol];
                newGrid[newRow][newCol] = 0;

                Board newBoard = new Board(k, newGrid);

                if (parentBoard != null) {
                    if (!newBoard.equals(parentBoard)) {
                        neighborBoards.add(newBoard);
                    }
                } else {
                    neighborBoards.add(newBoard);
                }
            }
        }
        return neighborBoards;
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        for (int[] row : grid) {
            for (int cell : row) {
                if (cell == 0) {
                    sb.append(" * ");
                } else {
                    sb.append(String.format("%2d ", cell));
                }
            }
            sb.append('\n');
        }
        return sb.toString();
    }
}

