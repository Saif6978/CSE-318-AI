#include<bits/stdc++.h>
#include "Heuristic.hpp"
using namespace std;


class Puzzle{
    public:
    Board initialBoard;
    Board targetBoard;
    Puzzle(const Board& initialBoard):initialBoard(initialBoard),targetBoard(generateTargetBoard(initialBoard.rows)){}
    Puzzle(const Board& initialBoard,const Board& targetBoard):initialBoard(initialBoard),targetBoard(targetBoard){}
    static Board generateTargetBoard(int rows){
        vector<vector<int>> target(rows,vector<int>(rows));
        int c=1;
        for(int i=0;i<rows;i++){
            for(int j=0;j<rows;j++){
                if(c==rows*rows){
                    target[i][j]=0;
                }
                else{
                    target[i][j]=c;
                }
                c++;
            }
        }
        return Board(rows,target);
    }
    bool isSolvable(){
        int inputInversionCount=initialBoard.getInversionCount();
        int targetInversionCount=targetBoard.getInversionCount();
        int inputInvariant,targetInvariant;
        int k=initialBoard.rows;
        if(k%2!=0){
            inputInvariant=inputInversionCount%2;
            targetInvariant=targetInversionCount%2;
        }
        else{
            int inputBlankRow=k-initialBoard.getPosition(0).first;
            int targetBlankRow=k-targetBoard.getPosition(0).first;
            inputInvariant=(inputInversionCount+inputBlankRow)%2;
            targetInvariant=(targetBlankRow+targetInversionCount)%2;
        }
        return (inputInvariant==targetInvariant);
    }
};

class State{
    public:
    Board board;
    int cost;
    State* parentState;
    State(const Board& board,int cost,State* parentState=nullptr):board(board),cost(cost),parentState(parentState){
        
    }
    double getPriority(Heuristic* heuristic,const Board& targetBoard,double weight){
        return cost + weight * heuristic->calculate(board,targetBoard);
    }
    vector<State> getNeighbour(){
        vector<State> neighbours;
        Board* parentBoard=nullptr;
        if(parentState!=nullptr){
            parentBoard= &(parentState->board);
        }
        vector<Board> neighbourBoards= board.getNextBoards(parentBoard);
        for(auto& it:neighbourBoards){
            neighbours.push_back(State(it,cost+1,this));
        }
        return neighbours;
    }
    vector<State*> getAncestors(){
        vector<State*> ancestors;
        State* s=parentState;
        while(s!=nullptr){
            ancestors.push_back(s);
            s=s->parentState;
        }
        return ancestors;
    }

};
class CompareState{
    public:
    Heuristic* heuristic;
    const Board& targetBoard;
    double weight;
    CompareState(Heuristic* heuristic,const Board& targetBoard,double weight):heuristic(heuristic),targetBoard(targetBoard),weight(weight){}
    bool operator()(State* a,State* b){
        return a->getPriority(heuristic,targetBoard,weight)>
        b->getPriority(heuristic,targetBoard,weight);
    }
};
struct Result{
    bool solved;
    int explored;
    int expanded;
    int steps;
    Result(bool solved=false,int explored=0,int expanded=0,int steps=-1){
        this->expanded=expanded;
        this->explored=explored;
        this->solved=solved;
        this->steps=steps;
    }
};
class Solver{
    public:
    Heuristic* heuristic;
    double weight;
    Solver(Heuristic* heuristic,double weight=1.0){
        this->heuristic=heuristic;
        this->weight=weight;
    }
    Result solve(Puzzle& puzzle,bool printPath=false){
        if(printPath){
            cout << heuristic->name << " Heuristic:\n";
            cout << "-------------------------\n";
        }
        CompareState comp(heuristic,puzzle.targetBoard,weight);
        priority_queue<State*,vector<State*>,CompareState> openlist(comp);
        unordered_map<string,int> gScore;
        State* start=new State(puzzle.initialBoard,0,nullptr);
        openlist.push(start);
        gScore[start->board.getKey()]=0;
        int explored=1;
        int expanded=0;
        State* finalState=nullptr;
        while(!openlist.empty()){
            State* current=openlist.top();
            openlist.pop();
            
            string currentKey=current->board.getKey();
            if(current->cost>gScore.at(currentKey))continue;
            expanded++;
            if(current->board==puzzle.targetBoard){
                finalState=current;
                break;
            }
            vector<State>neighbours=current->getNeighbour();
            for(auto& next:neighbours){
                string nextKey=next.board.getKey();
                int tentativeG=current->cost+1;
                if(!gScore.count(nextKey)||tentativeG<gScore.at(nextKey)){
                    gScore[nextKey]=tentativeG;
                    State* child =new State(next.board,tentativeG,current);
                    openlist.push(child);
                    explored++;
                }
            }
        }
        if(finalState!=nullptr){
            vector<State*> path=finalState->getAncestors();
            reverse(path.begin(),path.end());
            if(printPath){
                cout<<"\nSolution Path:\n\n";
                for(State* s:path){
                    s->board.printBoard();
                    cout<<"\n";
                }
                finalState->board.printBoard();
                cout<<"\nExplored Nodes: "<<explored;
                cout<<"\nExpanded Nodes: "<<expanded;
                cout<<"\nSteps Needed: "<<path.size()<<endl;
            }
            return Result(true,explored,expanded,path.size());
        }
        return Result(false,explored,expanded,-1);
    }
};


Board takeInput();
void runBenchmark(string inputFile,
                string outputFile,
                Heuristic* heuristic);

int main() {
    double weights[] ={1.0,1.2,2.0,5.0};
    cout <<"1. Solve a puzzle\n";
    cout <<"2. Benchmark\n";
    int choice;
    cin >> choice;
    if(choice==1){
        Board initialBoard = takeInput();
        Puzzle puzzle(initialBoard);
        if(!puzzle.isSolvable()){
            cout << "\nThe puzzle is NOT solvable.\n";
            return 0;
        }
        cout << "\nThe puzzle is solvable.\n\n";
        Hamming hamming;
        Manhattan manhattan;
        Euclidean euclidean;
        Linear_Conflict linearConflict;
        CustomHeuristic custom;
        cout << "Choose Heuristic\n";
        cout << "1. Hamming\n";
        cout << "2. Manhattan\n";
        cout << "3. Euclidean\n";
        cout << "4. Linear Conflict\n";
        cout << "5. Custom\n";
        cout << "Choice: ";
        int hChoice;
        cin >> hChoice;
        Heuristic* heuristic = nullptr;
        switch(hChoice){
            case 1:
                heuristic = &hamming;
                break;
            case 2:
                heuristic = &manhattan;
                break;
            case 3:
                heuristic = &euclidean;
                break;
            case 4:
                heuristic = &linearConflict;
                break;
            case 5:
                heuristic = &custom;
                break;
            default:
                cout << "Invalid choice.\n";
                return 0;
        }
        double weights[] = {1.0, 1.2, 2.0, 5.0};
        for(double w : weights){
            cout << "\n========================================\n";
            cout << "Weight = " << w << "\n";
            cout << "========================================\n\n";
            Solver solver(heuristic, w);
            solver.solve(puzzle, true);
        }
    }
    else{
        Linear_Conflict heuristic;
        runBenchmark("input.txt","output.txt",&heuristic);
    }
    
    return 0;
}

Board takeInput() {
    int k;
    while (true) {
        cout << "Enter board size: ";
        if (!(cin >> k)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid board size.\n";
            continue;
        }
        if (k < 2) {
            cout << "Board size must be at least 2.\n";
            continue;
        }
        break;
    }
    vector<vector<int>> board(k, vector<int>(k));
    vector<bool> used(k*k,false);
    cout << "Enter puzzle:\n";
    for(int i=0;i<k;i++){
        for(int j=0;j<k;j++){
            int x;
            while(true){
                if(!(cin>>x)){
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(),'\n');
                    cout<<"Invalid input. Enter again : ";
                    continue;
                }
                if(x<0 || x>=k*k){
                    cout<<"Number out of range. Enter again : ";
                    continue;
                }
                if(used[x]){
                    cout<<"Duplicate value. Enter again : ";
                    continue;
                }
                used[x]=true;
                board[i][j]=x;
                break;
            }
        }
    }
    return Board(k,board);
}
void runBenchmark(string inputFile,
                string outputFile,
                Heuristic* heuristic)
{
    ifstream fin(inputFile);
    if(!fin){
        cout<<"Cannot open input file.\n";
        return;
    }
    ofstream fout(outputFile);
    if(!fout){
        cout<<"Cannot create output file.\n";
        return;
    }
    vector<double> weights={1.0,1.2,2.0,5.0};
    int puzzleNo=1;
    while(true){
        int n;
        if(!(fin>>n))
            break;
        vector<vector<int>> board(n,
            vector<int>(n));
        for(int i=0;i<n;i++)
            for(int j=0;j<n;j++)
                fin>>board[i][j];
        Board b(n,board);
        Puzzle puzzle(b);
        fout<<"=========================================\n";
        fout<<"Puzzle "<<puzzleNo++<<"\n";
        fout<<"=========================================\n\n";
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++)
                fout<<board[i][j]<<" ";
            fout<<"\n";
        }
        fout<<"\n";
        if(!puzzle.isSolvable()){
            fout<<"Puzzle is NOT solvable.\n\n";
            continue;
        }
        fout<<"Puzzle is solvable.\n\n";
        fout<<left
            <<setw(10)<<"Weight"
            <<setw(15)<<"Steps"
            <<setw(18)<<"Explored"
            <<setw(18)<<"Expanded"
            <<"\n";
        fout<<string(60,'-')<<"\n";
        for(double w:weights){
            Solver solver(heuristic,w);
            Result r=solver.solve(puzzle,false);
            fout<<left
                <<setw(10)<<w
                <<setw(15)<<r.steps
                <<setw(18)<<r.explored
                <<setw(18)<<r.expanded
                <<"\n";
        }
        fout<<"\n\n";
    }
    fin.close();
    fout.close();
    cout<<"Benchmark completed.\n";
}