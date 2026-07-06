#ifndef HEURISTIC_HPP
#define HEURISTIC_HPP

#include<bits/stdc++.h>
#include "Board.hpp"

using namespace std;


class Heuristic{
public:
    string name;
    virtual double calculate(const Board& currentBoard,const Board& targetBoard)=0;
    virtual ~Heuristic(){};
};

class Hamming :public Heuristic{
    public:
    Hamming(){
        name="Hamming";
    }
    double calculate(const Board& currentBoard,const Board& targetBoard)override{
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
        return (double)d;
    }
};

class Manhattan : public Heuristic{
    public:
    Manhattan(){
        name= "Manhattan";
    }
    double calculate(const Board& currentBoard, const Board& targetBoard)override{
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
        return (double)d;
    }
};

class Euclidean : public Heuristic{
    public:
    Euclidean(){
        name="Euclidean";

    }
    double calculate(const Board& currentBoard,const Board& targetBoard)override{
        double d=0;
        for(int i=0;i<currentBoard.rows;i++){
            for(int j=0;j<currentBoard.rows;j++){
                int num=currentBoard.board[i][j];
                if(num==0)continue;
                pair<int,int> position=targetBoard.getPosition(num);
                int rowdiff=abs(position.first-i);
                int coldiff=abs(position.second-j);
                double dis=sqrt((rowdiff*rowdiff)+(coldiff*coldiff));
                d+=dis;
            }
        }
        return d;
    }
};

class Linear_Conflict : public Heuristic{
    public:
    Linear_Conflict(){
        name="Linear_Conflict";
    }
    double calculate(const Board& currentBoard,const Board& targetBoard)override{
        int d=0;
        int conflict=0;
        Manhattan manhattan;
        d=manhattan.calculate(currentBoard,targetBoard);
        int n=currentBoard.rows;
        //conflict in row
        for(int k=0;k<n;k++){
            for(int i=0;i<n;i++){
                int first=currentBoard.board[k][i];
                if(first==0){
                    continue;
                }
                pair<int,int> p1=targetBoard.getPosition(first);
                if(p1.first!=k)continue;
                for(int j=i+1;j<n;j++){
                    int second = currentBoard.board[k][j];
                    if(second==0)continue;
                    pair<int,int> p2=targetBoard.getPosition(second);
                    if(p2.first!=k)continue;
                    if(p1.second>p2.second)conflict++;
                }
            }
        }
        //conflict in column
        for(int k=0;k<n;k++){
            for(int i=0;i<n;i++){
                int first=currentBoard.board[i][k];
                if(first==0){
                    continue;
                }
                pair<int,int> p1=targetBoard.getPosition(first);
                if(p1.second!=k)continue;
                for(int j=i+1;j<n;j++){
                    int second = currentBoard.board[j][k];
                    if(second==0)continue;
                    pair<int,int> p2=targetBoard.getPosition(second);
                    if(p2.second!=k)continue;
                    if(p1.first>p2.first)conflict++;
                }
            }
        }
        return (d+2*(double)conflict);
    }
};

class CustomHeuristic : public Heuristic{
public:
    CustomHeuristic(){
        name = "Custom";
    }

    double calculate(const Board& currentBoard,
                    const Board& targetBoard) override{

        Linear_Conflict lc;

        double h = lc.calculate(currentBoard,targetBoard);

        int cornerPenalty = 0;

        int n = currentBoard.rows;

        // check four corners
        vector<pair<int,int>> corners = {
            {0,0},
            {0,n-1},
            {n-1,0},
            {n-1,n-1}
        };

        for(auto c : corners){
            int tile = currentBoard.board[c.first][c.second];

            if(tile==0) continue;

            auto goal = targetBoard.getPosition(tile);

            if(goal != c)
                cornerPenalty++;
        }

        return h + cornerPenalty;
    }
};
#endif