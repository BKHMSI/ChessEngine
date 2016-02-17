//
//  main.cpp
//  ChessEngine
//
//  Created by Badr AlKhamissi on 1/18/16.
//  Copyright © 2016 Badr AlKhamissi. All rights reserved.
//

#include <iostream>
#include <fstream>

#include "Chess.h"
#include "AI.h"


void createUCIEngineThread();
void uciSetAuthor();
void listenForUCIInput();

int main(int argc,  char * const argv[]) {
    
    cout.setf(ios::unitbuf);
    string line; //Read Command given by the GUI
    bool flag = false;
    Board game;
    
    game.display();
    
    string from, to;
    cout<<"White Player: ";
    cin>>from>>to;
    while(from != "zzzz"){
        if(game.makeMove(from, to)) flag = !flag;
        if(!flag) cout<<"White Player: ";
        else  cout<<"Black Player: ";
        cin>>from>>to;
    }
    
    return 0;
}

//    while( getline( cin, line ) ) {
//        cout<<line<<endl;
//        if ( line == "uci" ) {
//            cout << "id name Demo_engine" << endl;
//            cout << "id author XXX" << endl;
//            cout << "uciok" << endl;
//        } else if ( line == "quit" ) {
//            cout << "Bye Bye" << endl;
//            break;
//        } else if ( line == "isready" ) {
//            cout << "readyok" << endl;
//        } else if ( line == "ucinewgame" ) {
//            ; // nothing to do
//        }
//
//        if ( line.substr(0,23) == "position startpos moves " ) {
//            ; // nothing to do
//        } else if ( line == "stop" ) {
//            ; // nothing to do
//        } else if ( line.substr( 0, 3 ) == "go " ) {
//            // Received a command like: "go wtime 300000 btime 300000 winc 0 binc 0"
//            cout << "bestmove " << char(105-flag) << "7" << char(105-flag) << "5" << endl;
//            //Output like: "bestmove h7h5"
//            flag++; //increase flag to move other pawn on next turn
//        }
//    }


