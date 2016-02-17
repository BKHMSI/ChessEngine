//
//  AI.h
//  ChessEngine
//
//  Created by Badr AlKhamissi on 1/26/16.
//  Copyright © 2016 Badr AlKhamissi. All rights reserved.
//

#ifndef AI_h
#define AI_h

#include <iostream>
#include <vector>
#include <stdio.h>
#include "Chess.h"

using namespace std;

#define vec vector<pos>

class AI{
private:
    int search(int,int,int,Board&);
public:
    pos makeMove();
};

int AI::search(int alpha, int beta, int level, Board& board){
    if (level == 0)
        return board.getValue(); // We are at leaf, just return the static evaluation.
    
    vec moves;
    board.findLegalMoves(moves,black);
    
    int bestVal = -999; // Assume the worst
    
    // Search through all legal moves
    for (int i = 0; i<moves.size(); ++i)
    {
        if (bestVal >= beta)
        {
            // This is the alpha-beta pruning.
            // Stop searching, if we already have found a "killer" move.
            break;
        }
        if (bestVal > alpha)
        {
            // This is part of the alpha-beta pruning too.
            // Tighten the search window.
            alpha = bestVal;
        }
        
        const pos& move = moves[i];
        if (board.isCheck(move)){
            return 900 + level; // Opponent's king can be captured. That means he is check-mated.
        }
        
        // Do a recursive search
        //board.makeMove(move);
        int num = -search(-beta, -alpha, level-1, board);
        //board.undo_move(move);
        
        if (num > bestVal){
            // Store the best value so far.
            bestVal = num;
        }
    }
    
    return bestVal;
} // end of int search

/***************************************************************
 * find_best_move
 *
 * This is the main AI.
 * It returns what it considers to be the best legal move in the
 * current position.
 ***************************************************************/
//pos AI::makeMove(){
//    // Make a list of all legal moves
//    vec moves;
//    m_board.find_legal_moves(moves);
//    
//    cout << "info string " << moves.size() << " legal moves." << std::endl;
//    
//    CMoveList best_moves; // Array of the (one or more) best moves so far
//    int best_val = -999;
//    
//    // Search through all legal moves
//    for (unsigned int i=0; i<moves.size(); ++i)
//    {
//        CMove move = moves[i];
//        
//        // Get value of current move
//        m_board.make_move(move);
//        int val = -search(-999, 999, 4);
//        m_board.undo_move(move);
//        
//        cout << "info string " << val << " : " << move << std::endl;
//        
//        if (val > best_val)
//        {
//            best_val = val;
//            best_moves.clear();
//            best_moves.push_back(move);
//        }
//        else if (val == best_val)
//        {
//            best_moves.push_back(move);
//        }
//    }
//    
//    if (best_moves.size() == 0)
//    {
//        scout << "BUG: No legal moves!" << std::endl;
//        exit(1);
//    }
//    
//    return best_moves[rand() % best_moves.size()];
//} // end of find_best_move
//

#endif /* AI_h */
