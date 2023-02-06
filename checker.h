#ifndef CHECKER_H
#define CHECKER_H

#include <iomanip>
#include <iostream>
#include <fstream>
#include "Game.h"
#include "agent.h"

namespace Game{
    class checker{
        public:
            checker(){
                endGame = true;
                currentPlayer = 0;
                initGame();
            }

            void initGame();
            static std::vector <move> getMoves(board &Board, int currentPlayer);
            static void printMoves (int i, move &moves);

            void setBoard(board Board);
            board getBoard();
            int getCurrentPlayer();
            void setMoveStartTime(time moveStartTime);
            time getMoveStartTime();

            void startGame(bool isP1AI, bool isP2AI, int startingPlayer, double timeLimit);

            void printBoard();

            board transition_board(move &Move);
            board transition_board(move &Move, board &Board);

            void saveGame();
            void loadGame(std::string &filePath);

            void gameOver();


        private:
            board Board;
            bool endGame;
            int currentPlayer;
            agent players[2];

            time moveStartTime;
            std::vector <move> paths_list;

    };
}

#endif
