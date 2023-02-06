#ifndef AGENT_H
#define AGENT_H

#include "Game.h"

namespace Game {

    class checker;

    class agent {
    public:
        agent(): game(nullptr), isAI(false), moveTime(0), timeLimit(0), maxDepthReached(0) {};

        agent(checker* game, bool isAI, double timeLimit) : game(game), isAI(isAI), moveTime(0), timeLimit(timeLimit), maxDepthReached(0) {};

        move play(board &Board);

        move alpha_beta_search(board &Board);
        int heuristic(board &Board);

        int getMaxDepth();
        void setMoveTime(double moveTime);
        double getMoveTime();

    private:
        friend class checker;
        checker *game;
        std::vector <move> moveList;

        double timeLimit;
        double moveTime;

        bool isAI;
        int maxDepthReached;
    };
}

#endif 
