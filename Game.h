#ifndef GAME_HPP
#define GAME_HPP

#include <chrono>
#include <string>
#include <vector>
#include <set>
#include <algorithm>

namespace Game{

    static char columnIndex[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    static char pawnSymbol[5] = {'o', 'O', 'K', 'K', ' '};
    static std::set<char> possibleInputs{'q', 'Q', 'S','s'};

    static bool loadFromFile;
    static std::string filePath;
    class pawn{
        public:
            int xCoordinate, yCoordinate;
            bool king;
    };
    class board{
        public:
            int gameBoard[8][8];
            pawn pawns[2][12];
    };

    typedef struct {
        int startingPlayer;
        double timeLimit;
    } info;

    typedef struct {
        int agentID;
        int x_route[13];
        int y_route[13];
    } move;

    typedef struct {
        int alpha, beta, value, move_index;
        board node_board;
        bool is_max;
        int numMoves;
        std::vector<move> node_moves;
    } tree_node;

    typedef std::chrono::steady_clock::time_point time;
    typedef std::chrono::steady_clock clock;
    typedef std::chrono::steady_clock::duration duration;

}

#endif 