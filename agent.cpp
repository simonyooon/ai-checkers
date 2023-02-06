#define NOMINMAX // max() 

#include "agent.h"
#include "checker.h"

#include <climits>

using namespace Game;

move agent::play(board &Board) {
    move Move;
    std::vector<Game::move> moves;
    std::string inStr;
    bool moved;
    int i;
    if(isAI) {  // ai player                                                        
        moves = game->getMoves(Board, game->getCurrentPlayer()); // get all legal moves
        for(i = 0; i < moves.size(); i++) {
            game->printMoves(i, moves[i]); // print moves
        }
        if(moves.empty()) {
            Move.agentID = game->getCurrentPlayer();
            Move.x_route[0] = Move.y_route[0] = 0xFFU;
            moved = true;
            game->gameOver();  // no more moves --> end game
        } else if(moves.size() == 1) {
            Move = moves[0];  // play only move
            maxDepthReached = 0;
        } else {
            Move = alpha_beta_search(Board); // search
                                                                        
        }
        if(!moves.empty()){
            std::cout << "AI has chosen move: ";
            game->printMoves(-1, Move); // move selected
        }
    } else { // human
        moved = false;
        moves = game->getMoves(Board, game->getCurrentPlayer()); // get all legal moves
        if(moves.empty()) {
            Move.agentID = game->getCurrentPlayer();
            Move.x_route[0] = Move.y_route[0] = 0xFFU;
            moved = true;
            game->gameOver(); // no more moves 
        }
        while(!moved) {
            for(i = 0; i < moves.size(); i++) {
                game->printMoves(i, moves[i]); // print moves
            }
            std::cout << "  [S]  Save" << std::endl;
            std::cout << "  [Q]  Quit" << std::endl;
            std::cout << "Please select a step... " << std::endl;
            std::cout << ">: ";
            bool valid = false;
            while(!valid) {
                std::cin >> inStr;
                valid = true;
                if((possibleInputs.count(inStr.c_str()[0]) == 0) && 
                                          (std::strtoll(inStr.c_str(), nullptr, 10) < 1 || 
                                           std::strtoll(inStr.c_str(), nullptr, 10) > moves.size())){
                    valid = false;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid input, please try again..." << std::endl;
                    std::cout << ">: ";
                }
            }
            if(inStr.c_str()[0] == 'S' || inStr.c_str()[0] == 's') {
                game->saveGame();
            } else if(inStr.c_str()[0] == 'q' || inStr.c_str()[0] == 'Q') {
                Move.agentID = game->getCurrentPlayer();
                Move.x_route[0] = Move.y_route[0] = 0xFFU;
                moved = true;
                std::cout << "Quitting game ..." << std::endl;
                game->gameOver();
            } else {
                Move = moves[std::strtoll(inStr.c_str(), nullptr, 10) - 1];
                moved = true;
            }
        }
    }
    return Move;
}

move agent::alpha_beta_search(board &Board) {
    // initialize search tree
    int depth, max_depth = 1;
    tree_node tree_nodes[20];

    // agent variables
    int players[2];
    players[0] = game->getCurrentPlayer();
    players[1] = (~players[0]) & 1;

    move moveToMake = move();
    time startTime = game->getMoveStartTime();
    duration timeDiff;
    double remainingTime = timeLimit;

    int eval, bestMove;
    tree_nodes[0].node_board = Board;  //saving board into tree node
    tree_nodes[0].node_moves = checker::getMoves(Board, players[0]);   //all legal moves in a vector
    tree_nodes[0].numMoves = static_cast<int>(tree_nodes[0].node_moves.size());  //number of possible moves
    tree_nodes[0].is_max = true;


    do { //minimax alpha beta search
         // clear root                                                               
        depth = 0;
        tree_nodes[0].alpha = INT_MIN;
        tree_nodes[0].beta = INT_MAX;
        tree_nodes[0].value = INT_MIN;
        tree_nodes[0].move_index = 0;

        while(tree_nodes[0].move_index < tree_nodes[0].numMoves && remainingTime > 0.1) {
            if( tree_nodes[depth].beta <= tree_nodes[depth].alpha || 
                tree_nodes[depth].move_index >= tree_nodes[depth].numMoves) {
                if(!depth--) {
                    if(tree_nodes[1].value > tree_nodes[0].value) {
                        tree_nodes[0].value = tree_nodes[1].value;
                        bestMove = tree_nodes[0].move_index - 1;
                    }
                    if(tree_nodes[0].value > tree_nodes[0].alpha) {
                        tree_nodes[0].alpha = tree_nodes[0].value;
                    }
                    break;
                }
                if(tree_nodes[depth].is_max) {
                    if(tree_nodes[depth + 1].value > tree_nodes[depth].value) {
                        tree_nodes[depth].value = tree_nodes[depth + 1].value;
                        if(!depth) {
                            bestMove = tree_nodes[depth].move_index - 1;
                        }
                    }
                    if(tree_nodes[depth].value > tree_nodes[depth].alpha) {
                        tree_nodes[depth].alpha = tree_nodes[depth].value;
                    }
                } else {
                    if(tree_nodes[depth + 1].value < tree_nodes[depth].value) {
                        tree_nodes[depth].value = tree_nodes[depth + 1].value;
                    }
                    if(tree_nodes[depth].value < tree_nodes[depth].beta) {
                        tree_nodes[depth].beta = tree_nodes[depth].value;
                    }
                }
            } else {
                tree_nodes[depth + 1].node_board =
                        game->transition_board(tree_nodes[depth].node_moves[tree_nodes[depth].move_index],
                                                     tree_nodes[depth].node_board);
                tree_nodes[depth].move_index++;

                if(depth + 1 < max_depth) {
                    depth++;
                    tree_nodes[depth].is_max = !tree_nodes[depth - 1].is_max;
                    tree_nodes[depth].value = tree_nodes[depth].is_max? INT_MIN :INT_MAX;
                    tree_nodes[depth].beta = tree_nodes[depth - 1].beta;
                    tree_nodes[depth].alpha = tree_nodes[depth - 1].alpha;

                    tree_nodes[depth].node_moves = checker::getMoves(tree_nodes[depth].node_board,
                                                                                       players[depth & 1]);
                    tree_nodes[depth].numMoves = static_cast<int>(tree_nodes[depth].node_moves.size());
                    tree_nodes[depth].move_index = 0;
                }
                else {
                    eval = heuristic(tree_nodes[depth + 1].node_board);
                    if(tree_nodes[depth].is_max) {
                        if(eval > tree_nodes[depth].value) {
                            tree_nodes[depth].value = eval;
                            if(!depth) {
                                bestMove = tree_nodes[depth].move_index - 1;
                            }
                        }
                        if(tree_nodes[depth].value > tree_nodes[depth].alpha) {
                            tree_nodes[depth].alpha = tree_nodes[depth].value;
                        }
                    } else {
                        if(eval < tree_nodes[depth].value) {
                            tree_nodes[depth].value = eval;
                        }
                        if(tree_nodes[depth].value < tree_nodes[depth].beta) {
                            tree_nodes[depth].beta = tree_nodes[depth].value;
                        }
                    }
                }
            }

            timeDiff = clock::now() - startTime;
            remainingTime = timeLimit - double(timeDiff.count()) * duration::period::num / duration::period::den;
        }

        
        if(remainingTime > 0.1) {
            maxDepthReached = max_depth++;
            moveToMake = tree_nodes[0].node_moves[bestMove];
        
        }
        timeDiff = clock::now() - startTime;
        remainingTime = timeLimit - double(timeDiff.count()) * duration::period::num / duration::period::den;
        
    } while(remainingTime > 0.1 && max_depth < 20);
    std::cout << "Search completed to depth " << maxDepthReached << std::endl;
    std::cout << "Time taken for decision-making: " << std::setprecision(2) << std::fixed 
              << double(timeDiff.count()) * duration::period::num / duration::period::den << "s" << std::endl;
    return moveToMake;
}

int agent::heuristic(board &Board) {
    int i, j;
    int eval[2] = {0};
    int pieceCount[2] = {0};
    int currentPlayer = game->getCurrentPlayer();
    int opponent = static_cast<int>(~currentPlayer & 1);
    int square;

    pawn temp{};
    // iterate through all the pieces
    for(i = 0; i < 2; i++) {
        for(j = 0; j < 12; j++) {
            temp = Board.pawns[i][j];
            if(temp.xCoordinate <= 7 && temp.yCoordinate <= 7) {
                pieceCount[i]++;
                eval[i] += 300; 

                if (temp.king) {
                    eval[i] += 1600;
                } else {
                    eval[i] += 1000 + (50 * abs(7 * i - temp.yCoordinate)); // attempt to king
                    if(temp.yCoordinate == 7 * i) {
                        eval[i] += 300;
                    }
                
                    // try to keep friendly pawns surrounded
                    if(temp.yCoordinate + (2 * i - 1) >= 0 && temp.yCoordinate + (2 * i - 1) <= 7) {
                        if(temp.xCoordinate - 1 >= 0) {
                            square = Board.gameBoard[temp.yCoordinate + (2 * i - 1)][temp.xCoordinate - 1];
                            if(square != 4 && (square & 1) == i) {
                                eval[i] += 150;
                            }
                        }
                        if(temp.xCoordinate + 1 <= 7) {
                            square = Board.gameBoard[temp.yCoordinate + (2 * i - 1)][temp.xCoordinate + 1];
                            if(square != 4 && (square & 1) == i) {
                                eval[i] += 150;
                            }
                        }

                        // approaching opponent's pawns
                        if(temp.yCoordinate + (2 - 4 * i) >= 0 && temp.yCoordinate + (2 - 4 * i) <= 7) {
                            square = Board.gameBoard[temp.yCoordinate + (2 - 4 * i)][temp.xCoordinate];
                            if(square != 4 && (square & 1) == (~i & 1)) {
                                eval[i] += 99;
                            }
                        }
                    }
                }

                // try to keep pawns in the center
                eval[i] -= 30 * (abs(4 - temp.xCoordinate) + abs(4 - temp.yCoordinate));

            }
        }
    }

    if(!pieceCount[currentPlayer]) {
        return INT_MIN;
    }else if(!pieceCount[opponent]) {
        return INT_MAX;
    }else if(checker::getMoves(Board, currentPlayer).empty()) {
        return INT_MIN;
    }else if(checker::getMoves(Board, opponent).empty()) {
        return INT_MAX;
    }
    return eval[currentPlayer] - eval[opponent];
}

int agent::getMaxDepth() {
    return isAI ? maxDepthReached : -1;
};

void agent::setMoveTime(double moveTime) {
    moveTime = moveTime;
}

double agent::getMoveTime() {
    return moveTime;
};