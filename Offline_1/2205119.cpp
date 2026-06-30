#include<bits/stdc++.h>

using namespace std;

const vector<pair<int,int>> moves = {{-1,0},{1,0},{0,-1},{0,1}};


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
    pair<int,int> getPosition(int number){
        if(number<0 || number>numbers){
            return {-1,-1};
        }
        else{
            return positions[number];
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
        for(int i=0;i<arr.size();i++){
            for(int j=i+1;j<arr.size();j++){
                if(arr[i]>arr[j]){
                    inv_count++;
                }
            }
        }
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
    Board generateTargetBoard(int rows){
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
    string name;
    
};

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);


    return 0;
}
