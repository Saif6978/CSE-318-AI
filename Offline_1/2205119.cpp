#include<bits/stdc++.h>

using namespace std;

const vector<pair<int,int>> moves = {{-1,0},{1,0},{0,-1},{0,1}};
const int Invalid_Indicator=-1;

class InversionCounter{
    public:
    static int countInversions(vector<int>& arr){
        vector<int> temp(arr.size());
        return mergeSort(arr,temp,0,arr.size()-1);
    }
    private:
    static int mergeSort(vector<int>& arr,vector<int>&temp,int left,int right){
        int invCount=0;
        if(left<right){
            int mid=(left+right)/2;
            invCount+=mergeSort(arr,temp,left,mid);
            invCount+=mergeSort(arr,temp,mid+1,right);
            invCount+=merger(arr,temp,left,mid,right);
        }
        return invCount;
    }
    static int merger(vector<int>&arr,vector<int>&temp,int left,int mid,int right){
        int i=left;
        int j=mid+1;
        int k=left;
        int invCount=0;
        while(i<=mid && j<=right){
            if(arr[i]<=arr[j]){
                temp[k++]=arr[i++];
            }
            else{
                temp[k++]=arr[j++];
                invCount+=(mid+1-i);
            }
        }
        while(i<=mid){
            temp[k++]=arr[i++];
        }
        while(j<=right){
            temp[k++]=arr[j++];
        }
        for(int x=left;x<=right;x++){
            arr[x]=temp[x];
        }
        return invCount;
    }
};


class Board{
    public:
    vector<vector<int>> board;
    int rows;
    int numbers;
    unordered_map<int, pair<int,int>> positions;
    Board(int rows,const vector<vector<int>>& board){
        this->rows = rows;
        this->board.resize(rows, vector<int>(rows));
        numbers = rows*rows-1;
        for(int i=0;i<rows;i++){
            for(int j=0;j<rows;j++){
                positions[board[i][j]] = {i,j};
            }
        }
        for(int i=0;i<rows;i++){
            for(int j=0;j<rows;j++){
                this->board[i][j] = board[i][j];
            }
        }
    }
    pair<int,int> getPosition(int number)const{
        if(number<0 || number>numbers){
            return {-1,-1};
        }
        else{
            return positions.at(number);
        }
    }
    int getInversionCount(){
        vector<int> arr;
        for(int i=0;i<rows;i++){
            for(int j=0;j<rows;j++){
                if(board[i][j]!=0){
                    arr.push_back(board[i][j]);
                }
            }
        }

        int inv_count = 0;
        /*for(int i=0;i<arr.size();i++){
            for(int j=i+1;j<arr.size();j++){
                if(arr[i]>arr[j]){
                    inv_count++;
                }
            }
        }*/
        inv_count=InversionCounter::countInversions(arr);
        return inv_count;
    }
    bool operator==(const Board& other)const{
        if(rows!=other.rows){
            return false;
        }
        for(int i=0;i<rows;i++){
            for(int j=0;j<rows;j++){
                if(board[i][j]!=other.board[i][j]){
                    return false;
                }
            }
        }
        return true;
    }
    bool isValidPosition(int row,int col){
        return row>=0 && row<rows && col>=0 && col<rows;
    }
    vector<vector<int>> copyBoard(){
        vector<vector<int>> newBoard(rows,vector<int>(rows));
        for(int i=0;i<rows;i++){
            for(int j=0;j<rows;j++){
                newBoard[i][j] = board[i][j];
            }
        }
        return newBoard;
    }
    vector<Board> getNextBoards(Board* parentBoard=nullptr){
        vector<Board> nextBoards;
        pair<int,int> zeroPos = getPosition(0);
        for(auto move:moves){
            int newRow = zeroPos.first + move.first;
            int newCol = zeroPos.second + move.second;
            if(isValidPosition(newRow,newCol)){
                vector<vector<int>> newBoard = copyBoard();
                swap(newBoard[zeroPos.first][zeroPos.second],newBoard[newRow][newCol]);
                Board nextBoard(rows,newBoard);
                if(parentBoard!=nullptr){
                    if(!(nextBoard==*parentBoard)){
                        nextBoards.push_back(nextBoard);
                    }
                }
                else{
                    nextBoards.push_back(nextBoard);
                }
            }
        }
        return nextBoards;
    }

    void printBoard(){
        for(int i=0;i<rows;i++){
            for(int j=0;j<rows;j++){
                cout<<board[i][j]<<" ";
            }
            cout<<"\n";
        }
    }
};

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

class Heuristic{
public:
    string name;
    virtual int calculate(const Board& currentBoard,const Board& targetBoard)=0;
    virtual ~Heuristic(){};
};

class Hamming :public Heuristic{
    public:
    Hamming(){
        name="Hamming";
    }
    int calculate(const Board& currentBoard,const Board& targetBoard)override{
        int d=0;
        for(int i=0;i<currentBoard.rows;i++){
            for(int j=0;j<currentBoard.rows;j++){
                int num=currentBoard.board[i][j];
                if(num==0){
                    continue;
                }
                else if(num!=targetBoard.board[i][j]){
                    d++;
                }
            }
        }
        return d;
    }
};

class Manhattan : public Heuristic{
    public:
    Manhattan(){
        name= "Manhattan";
    }
    int calculate(const Board& currentBoard, const Board& targetBoard)override{
        int d=0;
        for(int i=0;i<currentBoard.rows;i++){
            for(int j=0;j<currentBoard.rows;j++){
                int num=currentBoard.board[i][j];
                if(num==0)continue;
                pair<int,int> position = targetBoard.getPosition(num);
                int rowdiff=abs(position.first-i);
                int coldiff=abs(position.second-j);
                d+=(rowdiff+coldiff);
            }
        }
        return d;
    }
};

class State{
    public:
    Board board;
    int cost;
    State* parentState;
    State(const Board& board,int cost,State* parentState=nullptr):board(board),cost(cost),parentState(parentState){
        
    }
    int getPriority(Heuristic* heuristic,const Board& targetBoard){
        return cost + heuristic->calculate(board,targetBoard);
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
    Board targetBoard;
    CompareState(Heuristic* heuristic,const Board& targetBoard):heuristic(heuristic),targetBoard(targetBoard){}
    bool operator()(State* a,State* b){
        return a->getPriority(heuristic,targetBoard)>
        b->getPriority(heuristic,targetBoard);
    }
};
class Solver{
    public:
    Heuristic* heuristic;
    Solver(Heuristic* heuristic){
        this->heuristic=heuristic;
    }
    bool isVisited(const Board& board,vector<State*>& visited){
        for(int i=0;i<visited.size();i++){
            if(visited[i]->board==board){
                return true;
            }
        }
        return false;
    }
    void solve(Puzzle& puzzle){
        cout << heuristic->name << " Heuristic:\n";
        cout << "-------------------------\n";
        CompareState comp(heuristic,puzzle.targetBoard);
        priority_queue<State*,vector<State*>,CompareState> openlist(comp);
        vector<State*>visited;
        State* start=new State(puzzle.initialBoard,0,nullptr);
        openlist.push(start);
        visited.push_back(start);
        int explored=1;
        int expanded=0;
        State* finalState=nullptr;
        while(!openlist.empty()){
            State* current=openlist.top();
            openlist.pop();
            expanded++;
            if(current->board==puzzle.targetBoard){
                finalState=current;
                break;
            }
            vector<State>neighbours=current->getNeighbour();
            for(int i=0;i<neighbours.size();i++){
                if(!isVisited(neighbours[i].board,visited)){
                    State* newState=new State(neighbours[i]);
                    openlist.push(newState);
                    visited.push_back(newState);
                    explored++;
                }
            }
        }
        if(finalState!=nullptr){
            vector<State*> path=finalState->getAncestors();
            reverse(path.begin(),path.end());
            cout << "\nSolution Path:\n\n";
            for(int i=0;i<path.size();i++){
                path[i]->board.printBoard();
                cout<<endl;
            }
            finalState->board.printBoard();
            cout << "\nExplored Nodes:" << explored << endl;
            cout << "\nExpanded Nodes:" << expanded << endl;
            cout << "\nSteps Needed:" << path.size() << endl;
        }
    }
};


Board takeInput();
bool isNumber(string s);

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    Board initialBoard=takeInput();
    Puzzle puzzle(initialBoard);
    if(puzzle.isSolvable()){
        cout<< "\nThe puzzle is solvable\n\n";
    }
    else{
        cout << "\nThe puzzle is not solvable\n";
        return 0;
    }
    Manhattan manhattan;
    Hamming hamming;
    Solver manhattanSolver(&manhattan);
    Solver hammingSolver(&hamming);
    manhattanSolver.solve(puzzle);
    hammingSolver.solve(puzzle);
    return 0;
}

bool isNumber(string s){
    if(s.empty()) return false;
    for(char c : s){
        if(!isdigit(c)){
            return false;
        }
    }
    return true;
}

Board takeInput(){

    int k;

    cout << "Enter value of k: ";
    cin >> k;

    if(k <= 0){
        cout << "k must be positive.\n";
        exit(0);
    }

    int n = k*k - 1;

    vector<vector<int>> input(k, vector<int>(k));

    vector<string> errors;

    cout << "Input board:\n";

    for(int i=0;i<k;i++){
        for(int j=0;j<k;j++){

            string s;
            cin >> s;

            if(s == "*"){
                input[i][j] = 0;
            }
            else if(isNumber(s)){
                int num = stoi(s);

                if(num > n){
                    errors.push_back(
                        "Out of range at row " +
                        to_string(i) +
                        " col " +
                        to_string(j)
                    );
                }

                input[i][j] = num;
            }
            else{
                input[i][j] = Invalid_Indicator;

                errors.push_back(
                    "Invalid input at row " +
                    to_string(i) +
                    " col " +
                    to_string(j)
                );
            }
        }
    }

    // validate board numbers
    vector<int> arr;

    for(int i=0;i<k;i++){
        for(int j=0;j<k;j++){
            arr.push_back(input[i][j]);
        }
    }

    sort(arr.begin(), arr.end());

    bool boardError = false;

    for(int i=0;i<k*k;i++){
        if(arr[i] != i){
            boardError = true;
            break;
        }
    }

    if(errors.size() > 0){
        cout << "Invalid Input:\n";

        for(string e : errors){
            cout << e << "\n";
        }

        exit(0);
    }

    if(boardError){
        cout << "Numbers do not form valid board.\n";
        exit(0);
    }

    return Board(k, input);
}
