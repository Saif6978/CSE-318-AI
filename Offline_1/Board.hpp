#ifndef BOARD_HPP
#define BOARD_HPP

#include<bits/stdc++.h>
#include "InversionCounter.hpp"
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

    string getKey(){
        string key="";
        for(int i=0;i<rows;i++){
            for(int j=0;j<rows;j++){
                key+=to_string(board[i][j]);
                key+=",";
            }
        }
        return key;
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

#endif