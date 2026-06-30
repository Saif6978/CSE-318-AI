package puzzle;

import board.Board;
import heuristics.Heuristic;

import java.util.*;

public class Solver {
    public Heuristic heuristic;

    public Solver(Heuristic heuristic) {
        this.heuristic = heuristic;
    }

    public void solve(Puzzle puzzle){
        System.out.println(heuristic.name + " Heuristic: ");
        System.out.println("---------------------------------\n");

        Boolean useClosedMap = true;

        Map<Board,Boolean> closedList = new HashMap<>();

        // define the open-list as
        // a priority queue
        PriorityQueue<State> openList = new PriorityQueue<>(
                Comparator.comparingInt(s -> s.getPriority(heuristic, puzzle.targetBoard))
        );

        State initialState = new State(puzzle.inputBoard, 0, null);
        openList.add(initialState);

        if(useClosedMap) {
            closedList.put(initialState.board, true);
        }

        int explored = 1;
        int expanded = 0;

        State finalState = null;
        while (!openList.isEmpty()){
            State s = openList.poll();
            closedList.put(s.board, true);
            expanded++;
            if(s.board.equals(puzzle.targetBoard)){
                finalState = s;
                break;
            }
            ArrayList<State>neighborStates = s.getNeighborStates();
            for(State neighborState: neighborStates){
                if(useClosedMap){
                    if(!closedList.containsKey(neighborState.board)){
                        openList.add(neighborState);
                        explored++;
                    }
                }
                else {
                    openList.add(neighborState);
                    explored++;
                }
            }
        }

        if(finalState != null){
            ArrayList<State>ancestorStates = finalState.getAncestorStates();
            System.out.println("\nSteps:\n");
            ArrayList<State>solutionPath = new ArrayList<>(ancestorStates);
            Collections.reverse(solutionPath);
            solutionPath.add(finalState);
            for(State s: solutionPath){
                System.out.println(s.board+ "\n");
            }
            System.out.println("Number of Explored Nodes = " + explored);
            System.out.println("Number of Expanded Nodes = " + expanded);
            System.out.println("Number of steps needed = " + ancestorStates.size());
            System.out.println("\n\n");
        }
    }
}
