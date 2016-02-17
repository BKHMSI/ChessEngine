//
//  Chess.h
//  ChessEngine
//
//  Created by Badr AlKhamissi on 1/18/16.
//  Copyright © 2016 Badr AlKhamissi. All rights reserved.
//

#ifndef Chess_h
#define Chess_h

#include <iostream>
#include <iomanip>
#include <fstream>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <set>
#include <map>

#include "Piece.h"

using namespace std;

#define vec vector<pos>
#define set set<int>
#define map map<pieces,vec>
#define boardSize 8

class Board{
private:
    Piece board[8][8];
    bool turn;
    set vis;
    vec moves;
    map boardMap;
    int material;
public:
    Board();
    void newGame();
    void findLegalMoves(pos);
    void findLegalMoves(vec&,color);
    int getValue();
    bool makeMove(pos, pos);
    bool makeMove(string from, string to){ return makeMove(convertTo2D(from), convertTo2D(to));}
    void dfsLegalMoves(int,int,int,int,color);
    bool isMoveLegal(pos, pos);
    bool isMoveLegal(string from, string to){return isMoveLegal(convertTo2D(from), convertTo2D(to));}
    void displayLegalMovesOf(pos);
    void displayLegalMovesOf(string s){displayLegalMovesOf(convertTo2D(s));}
    void display();
    void logGameToFile(string);
    string convertToChess(pos xy){return board[xy.x][xy.y].convertToChess(xy);}
    bool isCheck(color);
    bool isCheck(pos);
    bool isCheckMate(color);
    pos convertTo2D(string);
    Piece** copyBoard();
};


Board::Board(){
    newGame();
}

void Board::newGame(){
    turn = true;
    material = 0;

    for(int i = 0; i<8; i++)
        for(int j = 0; j<8; j++)
            board[i][j] = Piece(Empty,empty);
    
    for(int i = 0; i<8; i++){
        board[6][i] = Piece(Pawn,white,pos(6,i));
        boardMap[Pawn].push_back(pos(6,i));
    }
    
    for(int i = 0; i<8; i++){
        board[1][i] = Piece(Pawn,black,pos(1,i));
        boardMap[Pawn].push_back(pos(6,i)); // 8+i
    }
    
    // White
    board[7][0] = board[7][7] = Piece(Rook,white);
    boardMap[Rook].push_back(pos(7,0));
    boardMap[Rook].push_back(pos(7,7));

    board[7][1] = board[7][6] = Piece(Knight,white);
    boardMap[Knight].push_back(pos(7,1));
    boardMap[Knight].push_back(pos(7,6));

    board[7][2] = board[7][5] = Piece(Bishop,white);
    boardMap[Bishop].push_back(pos(7,5));
    boardMap[Bishop].push_back(pos(7,2));

    
    board[7][3] = Piece(Queen,white);
    boardMap[Queen].push_back(pos(7,3));

    board[7][4] = Piece(King,white);
    boardMap[King].push_back(pos(7,4));

    
    // Black
    board[0][0] = board[0][7] = Piece(Rook,black);
    boardMap[Rook].push_back(pos(0,0));
    boardMap[Rook].push_back(pos(0,7));
    
    board[0][1] = board[0][6] = Piece(Knight,black);
    boardMap[Knight].push_back(pos(0,1));
    boardMap[Knight].push_back(pos(0,6));
    
    board[0][2] = board[0][5] = Piece(Bishop,black);
    boardMap[Bishop].push_back(pos(0,5));
    boardMap[Bishop].push_back(pos(0,2));
    
    board[0][3] = Piece(Queen,black);
    boardMap[Queen].push_back(pos(0,3));

    board[0][4] = Piece(King,black);
    boardMap[King].push_back(pos(0,4));

}

bool Board::makeMove(pos from, pos to){
    if(turn == board[from.x][from.y].isWhite()){
        if (isMoveLegal(from, to)) {
            color playColor =  board[from.x][from.y].getColor();
            if(board[to.x][to.y].isOther(playColor)){
                switch (board[to.x][to.y].getPiece()) {
                    case Pawn:
                        material+=1;
                        break;
                    case Knight:
                        material+=3;
                        break;
                    case Bishop:
                        material+=3;
                        break;
                    case Rook:
                        material+=5;
                        break;
                    case Queen:
                        material+=9;
                        break;
                    case King:
                        return false;
                        break;
                    default:
                        break;
                }
                cout<<board[from.x][from.y].getPieceStr()<<" took "<<board[to.x][to.y].getPieceStr()<<endl;
            }
            board[to.x][to.y] = board[from.x][from.y];
            board[from.x][from.y] = Piece(Empty, empty);
            display();
            turn = !turn;
            return true;
        }else{
            cout<<"\n\n******** Invalid Move *********\n\n";
            return false;
        }
    }else{
        cout<<"\n\n******** Invalid Move *********\n\n";
        return false;
    }
}

int Board::getValue(){
    int whiteMoves = 0, blackMoves = 0;
    
    vec wMoves;
    findLegalMoves(wMoves,white);
    whiteMoves = (int)wMoves.size();
    
    vec bMoves;
    findLegalMoves(bMoves,black);
    blackMoves = (int)bMoves.size();
    
    return (whiteMoves-blackMoves) + 100*material;
}

bool Board::isCheck(color player){
    for (int i = 0; i<boardSize; i++)
        for (int j = 0; j<boardSize; j++)
            if(board[i][j].isOther(player)){
                findLegalMoves(pos(i,j));
                for(int k = 0; k<moves.size(); k++){
                    if(board[moves[k].x][moves[k].y].getPiece() == King){
                        return true;
                    }
                }
            }
    
    return false;
}


bool Board::isCheck(pos move){
    if(board[move.x][move.y].getPiece() == King)
        return true;
    else
        return false;
}

bool Board::isCheckMate(color player){
    if (isCheck(player)) {
        // Get Position of King
        findLegalMoves(pos());
        if(!moves.size())
            return true;
    }
    return false;
}

bool Board::isMoveLegal(pos from, pos to){
    findLegalMoves(from);
    for (int i = 0; i<moves.size(); i++) {
        if (moves[i] == to) {
            return true;
        }
    }
    return false;
}

void Board::findLegalMoves(pos xy){
    moves.clear();
    if(!board[xy.x][xy.y].isEmpty()){
        pieces piece = board[xy.x][xy.y].getPiece();
        bool isWhite = board[xy.x][xy.y].isWhite();
        color playerColor = board[xy.x][xy.y].getColor();
        
        // Diagonal Change - Bishop - Queen - King
        int dxB[4] = {-1,1,1,-1};
        int dyB[4] = {-1,1,-1,1};
        
        // Jump Change - Knight
        int dxK[8] = {-2,-2, 2,2,-1,-1,1, 1};
        int dyK[8] = {-1, 1,-1,1, 2,-2,2,-2};
        
        // Vertical & Horizontal Change - Rook - Queen - King
        int dxR[4] = {1,-1,0,0};
        int dyR[4] = {0,0,1,-1};
        
        switch (piece) {
        case Pawn:
                if(isWhite){
                    if (xy.x > 0 && board[xy.x-1][xy.y].isEmpty())
                        moves.push_back(pos(xy.x-1,xy.y));
                    
                    if (xy.x == 6 && board[xy.x-2][xy.y].isEmpty())
                        moves.push_back(pos(xy.x-2,xy.y));
                    
                    if (xy.x > 0 && xy.y > 0 && board[xy.x-1][xy.y-1].isBlack())
                        moves.push_back(pos(xy.x-1,xy.y-1));
                    
                    if (xy.x > 0 && xy.y < 7 && board[xy.x-1][xy.y+1].isBlack())
                        moves.push_back(pos(xy.x-1,xy.y+1));
                }else{
                    if (xy.x < 7 && board[xy.x+1][xy.y].isEmpty())
                        moves.push_back(pos(xy.x+1,xy.y));
                    
                    if (xy.x == 1 && board[xy.x+2][xy.y].isEmpty())
                        moves.push_back(pos(xy.x+2,xy.y));
                    
                    if (xy.x < 7 && xy.y > 0 && board[xy.x+1][xy.y-1].isWhite())
                        moves.push_back(pos(xy.x-1,xy.y-1));
                    
                    if (xy.x < 7 && xy.y < 7 && board[xy.x+1][xy.y+1].isWhite())
                        moves.push_back(pos(xy.x+1,xy.y+1));
                }
            break;
        case Knight:
                for (int i = 0; i<8; i++) {
                    int xx = dxK[i]+xy.x, yy = dyK[i]+xy.y;
                    if(xx>=0 && yy>=0 && xx<boardSize && yy<boardSize){
                        if(board[xx][yy].isOther(playerColor) || board[xx][yy].isEmpty()){
                            moves.push_back(pos(xx,yy));
                        }
                    }
                }
            break;
        case Bishop:
                for(int i = 0; i<4; i++)
                    dfsLegalMoves(xy.x, xy.y,dxB[i],dyB[i],playerColor), vis.clear();
            break;
                
        case Rook:
                for(int i = 0; i<4; i++)
                    dfsLegalMoves(xy.x, xy.y,dxR[i],dyR[i],playerColor), vis.clear();
            break;
        case Queen:
                for(int i = 0; i<4; i++)
                    dfsLegalMoves(xy.x, xy.y,dxR[i],dyR[i],playerColor);
                for(int i = 0; i<4; i++)
                    dfsLegalMoves(xy.x, xy.y,dxB[i],dyB[i],playerColor), vis.clear();
            break;
        case King:
                for(int i = 0; i<8; i++){
                    int xx = 0, yy = 0;
                    if(i<4) xx = dxR[i]+xy.x, yy = dyR[i]+xy.y;
                    else    xx = dxB[i-4]+xy.x, yy = dyB[i-4]+xy.y;
                    if(xx>=0 && yy>=0 && xx<boardSize && yy<boardSize){
                        if(board[xx][yy].isOther(playerColor) || board[xx][yy].isEmpty()){
                            moves.push_back(pos(xx,yy));
                        }
                    }
                }
            break;
        default:
            break;
        }
    }
}

void Board::findLegalMoves(vec& vMoves, color player){
    if(player == white){
        /*** White Moves ***/
        findLegalMoves(boardMap[King][0]);
        vMoves.insert(vMoves.end(), moves.begin(),moves.end());
        
        findLegalMoves(boardMap[Queen][0]);
        vMoves.insert(vMoves.end(), moves.begin(),moves.end());
        
        for(int i = 0; i<2; i++)
            findLegalMoves(boardMap[Bishop][i]), vMoves.insert(vMoves.end(), moves.begin(),moves.end());
        
        for(int i = 0; i<2; i++)
            findLegalMoves(boardMap[Knight][i]), vMoves.insert(vMoves.end(), moves.begin(),moves.end());
        
        for(int i = 0; i<2; i++)
            findLegalMoves(boardMap[Rook][i]), vMoves.insert(vMoves.end(), moves.begin(),moves.end());
        
        for(int i = 0; i<8; i++)
            findLegalMoves(boardMap[Rook][i]), vMoves.insert(vMoves.end(), moves.begin(),moves.end());
    }else if(player == black){
        /*** Black Moves ***/
        findLegalMoves(boardMap[King][1]);
        vMoves.insert(vMoves.end(), moves.begin(),moves.end());
        
        findLegalMoves(boardMap[Queen][1]);
        vMoves.insert(vMoves.end(), moves.begin(),moves.end());
        
        for(int i = 2; i<4; i++)
            findLegalMoves(boardMap[Bishop][i]), vMoves.insert(vMoves.end(), moves.begin(),moves.end());
        
        for(int i = 2; i<4; i++)
            findLegalMoves(boardMap[Knight][i]), vMoves.insert(vMoves.end(), moves.begin(),moves.end());
        
        for(int i = 2; i<4; i++)
            findLegalMoves(boardMap[Rook][i]), vMoves.insert(vMoves.end(), moves.begin(),moves.end());
        
        for(int i = 8; i<16; i++)
            findLegalMoves(boardMap[Rook][i]), vMoves.insert(vMoves.end(), moves.begin(),moves.end());
    }
}

void Board::dfsLegalMoves(int x, int y, int dx, int dy, color pcColor){
    for (int i = 0; i<4; i++) {
        int xx = x+dx, yy = y+dy;
        if(xx>=0 && yy>=0 && xx<boardSize && yy<boardSize && !vis.count(xx*boardSize+yy)){
            if(board[xx][yy].isEmpty()){
                moves.push_back(pos(xx,yy));
                vis.insert(xx*boardSize+yy);
                dfsLegalMoves(xx, yy,dx,dy,pcColor);
            }else if(board[xx][yy].isOther(pcColor)){
                moves.push_back(pos(xx,yy));
                vis.insert(xx*boardSize+yy);
            }
        }
    }
}


pos Board::convertTo2D(string input){
    if(input.length()>2) return pos();
    else{
        string alpha = "ABCDEFGH";
        return pos((8-(input[1]-'0')),(int)alpha.find(toupper(input[0])));
    }
}

Piece** Board::copyBoard(){
    Piece** p;
    p = new Piece*[8];
    for (int i = 0; i<8; i++) {
        *(p+i) = new Piece[8];
    }
    
    // Init 2D Dynamic Array
    for (int i = 0; i<8; i++) {
        for (int j = 0; j<8; j++) {
            *(*(p+i)+j) = this->board[i][j];
        }
    }
    return p;
}


void Board::displayLegalMovesOf(pos xy){
    if(!board[xy.x][xy.y].isEmpty()){
        findLegalMoves(xy);
        Piece cBoard[8][8];
        
        for (int i = 0; i<8; i++)
            for (int j = 0; j<8; j++)
                cBoard[i][j] = this->board[i][j];
        
        // Display
        for (int i = 0; i<moves.size(); i++)
            board[moves[i].x][moves[i].y].setValue(7);
        
        cout<<"\n\n******** Legal Moves of "<<cBoard[xy.x][xy.y].displayInfo(xy)<<" *********\n\n";
        for (int i = 0; i<8; i++){
            for (int j = 0; j<8; j++)
                cout<<board[i][j].displayBox()<<" ";
            cout<<endl;
        }
    }else{
        cerr<<"\n******** Postion "<<convertToChess(xy)<<" is Empty *********\n\n";
    }
}

void Board::display(){
    cout<<endl;
    string alpha = "ABCDEFGH";
    for(int i = 0; i<8; i++){
        cout<<8-i<<" | ";
        for(int j = 0; j<8; j++)
            cout<<board[i][j].displayBox()<<" ";
        cout<<endl;
    }
    cout<<setw(5);
    for(int i = 0; i<8; i++) cout<<"--";
    cout<<"\n"<<setw(5);
    for(int i = 0; i<8; i++)
        cout<<alpha[i]<<" ";
    cout<<"\n\n";
}

#endif /* Board_h */
