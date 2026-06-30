package heuristics;

import board.Board;

// Abstract Heuristic class
public abstract class Heuristic {
    public String name;
    // Abstract method to calculate heuristic
    public abstract int calculate(Board currentBoard, Board targetBoard);
}
