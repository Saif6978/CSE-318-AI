import board.Board;
import heuristics.Hamming;
import heuristics.Manhattan;
import puzzle.Puzzle;
import puzzle.Solver;

import java.util.*;

public class Main {
    private static final int INVALID_INDICATOR = -1;

    public static void main(String[] args) {
        Board inputBoard = takeInput();

        if(inputBoard == null){
            return;
        }

        Puzzle puzzle = new Puzzle(inputBoard);

        if(puzzle.isSolvable()) {
            System.out.println("\nThe puzzle is solvable.\n");
        } else {
            System.out.println("\nSorry! The puzzle is not solvable.\n");
            return;
        }

        Solver manhattanSolver = new Solver(new Manhattan());
        manhattanSolver.solve(puzzle);

        Solver hammingSolver = new Solver(new Hamming());
        hammingSolver.solve(puzzle);
    }

    public static Board takeInput(){
        Scanner scanner = new Scanner(System.in);
        int k;
        System.out.print("Enter the value of k: ");
        k = scanner.nextInt();
        if(k <= 0){
            System.out.println("Sorry! The value of k must be positive.");
            return null;
        }
        int n = (int) (Math.pow(k,2)) - 1;

        int[][] input = new int[k][k];

        ArrayList<String>errors = new ArrayList<>();

        System.out.println("Input the initial board: ");
        for(int i=0; i<k; i++) {
            for (int j = 0; j < k; j++) {
                String s = scanner.next();
                if (s.equals("*")) {
                    input[i][j] = 0;
                } else if (isNumber(s)) {
                    int num = parseNumber(s);
                    if(num > n){
                        errors.add("Out of range number inputted at row "+i+", column "+j);
                    }
                    input[i][j] = num;
                } else {
                    input[i][j] = INVALID_INDICATOR;
                    errors.add("Invalid string inputted at row "+i+", column "+j);
                }
            }
        }

        ArrayList<Integer>arr = new ArrayList<>();
        for(int i=0; i<k; i++) {
            for (int j = 0; j < k; j++) {
                arr.add(input[i][j]);
            }
        }
        Collections.sort(arr);

        Boolean boardError = false;
        for(int i=0; i<k*k; i++){
            if(arr.get(i) != i){
                boardError = true;
                break;
            }
        }

        if(errors.size() > 0){
            System.out.println("Sorry! Invalid Input.");
            for(String e: errors){
                System.out.println(e);
            }
            return null;
        }
        if (boardError){
            System.out.println("Sorry! The numbers do not form a valid board.");
            return null;
        }

        // valid input
        Board inputBoard = new Board(k,input);
        return inputBoard;
    }

    public static boolean isNumber(String str) {
        try {
            Integer.parseInt(str); // or Double.parseDouble(str) for floating-point numbers
            return true;
        } catch (NumberFormatException e) {
            return false;
        }
    }

    public static int parseNumber(String str) {
        try {
            return Integer.parseInt(str); // or Double.parseDouble(str) for floating-point number
        } catch (NumberFormatException e) {
            return -1;
        }
    }
}