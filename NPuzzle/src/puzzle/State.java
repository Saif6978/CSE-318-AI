package puzzle;

import board.Board;
import heuristics.Heuristic;

import java.util.ArrayList;

public class State {
    public Board board;
    public int costSoFar;
    public State parentState;

    public State(Board board, int costSoFar, State parentState) {
        this.board = board;
        this.costSoFar = costSoFar;
        this.parentState = parentState;
    }

    public int getPriority(Heuristic heuristic, Board targetBoard){
        return costSoFar + heuristic.calculate(board, targetBoard);
    }

    public ArrayList<State> getNeighborStates(){
        ArrayList<State>neighborStates = new ArrayList<>();
        Board parentBoard = null;
        if(parentState != null){
            parentBoard = parentState.board;
        }
        ArrayList<Board>neighborBoards = board.getNeighborBoards(parentBoard);
        for(Board board: neighborBoards){
            neighborStates.add(new State(board, costSoFar+1, this));
        }
        return neighborStates;
    }

    public ArrayList<State> getAncestorStates(){
        ArrayList<State>ancestorStates = new ArrayList<>();
        State s = this.parentState;
        while(s != null){
            ancestorStates.add(s);
            s = s.parentState;
        }
        return ancestorStates;
    }
}
