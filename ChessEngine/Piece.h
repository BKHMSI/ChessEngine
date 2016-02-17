//
//  Piece.h
//  ChessEngine
//
//  Created by Badr AlKhamissi on 1/20/16.
//  Copyright © 2016 Badr AlKhamissi. All rights reserved.
//

#ifndef Piece_h
#define Piece_h

#include <iostream>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

enum pieces{
    Empty = 0, Pawn, Knight, Bishop, Rook, Queen, King
};

enum color{
    white, black, empty
};

struct move{
    int from, to;
    move(int _s, int _e){from = _s; to = _e;}
};

struct pos{
    int x, y;
    pos(){ x = y = 0;}
    pos(int _x, int _y){x = _x, y = _y;}
    pos(const pos& rhs){x = rhs.x, y = rhs.y;}
    bool operator == (const pos& rhs) {return x == rhs.x && y == rhs.y;}
    bool operator << (const pos& rhs){ return x;}
};

class Piece{
private:
    int val;
    pieces piece;
    color side;
    pos position;
public:
    // Constructors
    Piece();
    Piece(pieces, color);
    Piece(pieces, color, pos);
    
    // Copy Constructor
    Piece(const Piece&);
    
    string displayInfo(pos xy);
    string convertToChess(pos xy);
    char displayBox();
    
    void setValue(int _v){val = _v;}
    int getValue()const{return val;}
    color getColor()const{return side;}
    pieces getPiece()const{return piece;}
    string getPieceStr()const;
    pos getPosition()const{return position;}
    bool isWhite()const{return side == white ? true:false;}
    bool isBlack()const{return side == black ? true:false;}
    bool isEmpty()const{return side == empty ? true:false;}
    bool isOther(const Piece& pce)const{return (pce.side != side && side != empty) ? true:false;}
    bool isOther(const color& c)const{return (c != side && side != empty) ? true:false;}
};

Piece::Piece(){
    val = 0, piece = Empty, side = empty;
}

Piece::Piece(pieces p, color s){
    val = (int)p, piece = p, side = s;
}

Piece::Piece(pieces p, color s, pos xy){
    piece = p, val = (int)p, side = s, position = pos(xy);
}

Piece::Piece(const Piece& rhs){
    val = rhs.val, piece = rhs.piece, side = rhs.side;
}


string Piece::displayInfo(pos xy){
    string pcs[6] = {"Pawn", "Knight", "Bishop", "Rook", "Queen", "King"};
    return pcs[val-1]+" in "+convertToChess(xy);
}

string Piece::getPieceStr()const{
    string pcs[6] = {"Pawn", "Knight", "Bishop", "Rook", "Queen", "King"};
    return pcs[piece-1];
}

string Piece::convertToChess(pos xy){
    string alpha = "ABCDEFGH";
    char c = alpha[xy.y];
    string output = "";
    output+=c;
    output+=((8-xy.x)+'0');
    return output;
}

char Piece::displayBox(){
    char pcs[8] = {'#','P', 'K', 'B', 'R', 'Q', '+', '*'};
    return pcs[val];
}


#endif /* Piece_h */
