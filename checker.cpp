#include "checker.h"

using namespace Game;

void checker::initGame(){
    std::cout << "initiating game ..." << std::endl;
    board Board;
    int m = 0, n= 0;
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if(!((i + j) & 1) && i < 3) {
                Board.gameBoard[i][j] = 0;
                Board.pawns[0][m].king = false;
                Board.pawns[0][m].xCoordinate = j;
                Board.pawns[0][m++].yCoordinate = i;
            } else if(!((i + j) & 1) && i > 4) {
                Board.gameBoard[i][j] = 1;
                Board.pawns[1][n].king = false;
                Board.pawns[1][n].xCoordinate = j;
                Board.pawns[1][n++].yCoordinate = i;
            } else {
                Board.gameBoard[i][j] = 4;
            }
        }
    }
    setBoard(Board);
}

std::vector <Game::move> checker::getMoves(board &Board, int currentPlayer){
    int i, j, k;
    move potentialMoves;
    pawn temp;
    std::vector<move> moveList;
    int xDestination[2], yDestination[2];
    struct {
        int x, y;
        int checked;
        bool descJump;
    } board_node[13], point;

    bool pieceJumped[8][8] = {0};
    int depth;
    int opponent = (~currentPlayer) & 1;

    potentialMoves.agentID = currentPlayer;
    for(i = 0; i < 12; i++) {
        temp = Board.pawns[currentPlayer][i];
        potentialMoves.x_route[0] = temp.xCoordinate;
        potentialMoves.y_route[0] = temp.yCoordinate;

        board_node[0].x = temp.xCoordinate;
        board_node[0].y = temp.yCoordinate;
        board_node[0].checked = 0;
        depth = 0;

        if(temp.king) {

            while(board_node[0].checked != 0xFU || depth != 0) {
                point = board_node[depth];

                // check top right
                if(!(board_node[depth].checked & 0x8U)) {
                    board_node[depth].checked |= 0x8U;

                    if(    (point.x + 2) >= 0 && (point.x + 2) <= 7 && (point.y + 2) >= 0 && (point.y + 2) <= 7
                           && Board.gameBoard[point.y + 1][point.x + 1] != 4
                           && (Board.gameBoard[point.y + 1][point.x + 1] & 1) == opponent // jump enemy
                           && !pieceJumped[point.y + 1][point.x + 1]
                           && (Board.gameBoard[point.y + 2][point.x + 2] == 4
                               || (point.x + 2 == board_node[0].x && point.y + 2 == board_node[0].y))) {


                        pieceJumped[point.y + 1][point.x + 1] = true; //  jumped
                        depth++;
                        board_node[depth].x = point.x + 2;
                        board_node[depth].y = point.y + 2;
                        board_node[depth].checked = 0;
                        board_node[depth].descJump = false;
                    }
                }

                    // check top left
                else if(!(board_node[depth].checked & 0x4U)) {
                    board_node[depth].checked |= 0x4U;

                    if(    (point.x - 2) >= 0 && (point.x - 2) <= 7 && (point.y + 2) >= 0 && (point.y + 2) <= 7
                           && Board.gameBoard[point.y + 1][point.x - 1] != 4
                           && (Board.gameBoard[point.y + 1][point.x - 1] & 1) == opponent
                           && !pieceJumped[point.y + 1][point.x - 1]
                           && (Board.gameBoard[point.y + 2][point.x - 2] == 4
                               || (point.x - 2 == board_node[0].x && point.y + 2 == board_node[0].y))) {
                        pieceJumped[point.y + 1][point.x - 1] = true; // we have jumped that piece
                        depth++;
                        board_node[depth].x = point.x - 2;
                        board_node[depth].y = point.y + 2;
                        board_node[depth].checked = 0;
                        board_node[depth].descJump = false;
                    }
                }

                    // check bottom left
                else if(!(board_node[depth].checked & 0x2U)) {
                    board_node[depth].checked |= 0x2U;

                    if(    (point.x - 2) >= 0 && (point.x - 2) <= 7 && (point.y - 2) >= 0 && (point.y - 2) <= 7
                           && Board.gameBoard[point.y - 1][point.x - 1] != 4
                           && (Board.gameBoard[point.y - 1][point.x - 1] & 1) == opponent
                           && !pieceJumped[point.y - 1][point.x - 1]
                           && (Board.gameBoard[point.y - 2][point.x - 2] == 4
                               || (point.x - 2 == board_node[0].x && point.y - 2 == board_node[0].y))) {

                        pieceJumped[point.y - 1][point.x - 1] = true; // we have jumped that piece
                        depth++;
                        board_node[depth].x = point.x - 2;
                        board_node[depth].y = point.y - 2;
                        board_node[depth].checked = 0;
                        board_node[depth].descJump = false;
                    }
                }

                    // check bottom right
                else if(!(board_node[depth].checked & 0x1U)) {
                    board_node[depth].checked |= 0x1U;

                    if(    (point.x + 2) >= 0 && (point.x + 2) <= 7 && (point.y - 2) >= 0 && (point.y - 2) <= 7
                           && Board.gameBoard[point.y - 1][point.x + 1] != 4              // jump square not empty
                           && (Board.gameBoard[point.y - 1][point.x + 1] & 1) == opponent // jump square holds enemy piece
                           && !pieceJumped[point.y - 1][point.x + 1]                    // piece wasn't already jumped
                           && (Board.gameBoard[point.y - 2][point.x + 2] == 4             // dest square empty OR
                               || (point.x + 2 == board_node[0].x && point.y - 2 == board_node[0].y))) { // dest square same as start square

                        pieceJumped[point.y - 1][point.x + 1] = true; // we have jumped that piece
                        depth++;
                        board_node[depth].x = point.x + 2;
                        board_node[depth].y = point.y - 2;
                        board_node[depth].checked = 0;
                        board_node[depth].descJump = false;
                    }
                } else {
                    if(!board_node[depth].descJump) {
                        for(j = 1; j <= depth; j++) {
                            potentialMoves.x_route[j] = board_node[j].x;
                            potentialMoves.y_route[j] = board_node[j].y;
                        }
                        if(j < 13) {
                            potentialMoves.x_route[j] = potentialMoves.y_route[j] = 0xFFU;
                        }
                        moveList.push_back(potentialMoves);
                    }
                    board_node[depth - 1].descJump = true;
                    pieceJumped[(point.y + board_node[depth - 1].y) / 2][(point.x + board_node[depth - 1].x) / 2] = false;
                    depth--;
                }
            }
        } else { //regular
            while(board_node[0].checked != 0xCU || depth != 0) {
                point = board_node[depth];
                if(!(board_node[depth].checked & 0x8U)) {
                    board_node[depth].checked |= 0x8U;

                    if(    (point.x + 2) >= 0 && (point.x + 2) <= 7 && (point.y + (4 * opponent - 2)) >= 0 && (point.y + (4 * opponent - 2)) <= 7 // in range
                           && Board.gameBoard[point.y + (2 * opponent - 1)][point.x + 1] != 4              // jump square not empty
                           && (Board.gameBoard[point.y + (2 * opponent - 1)][point.x + 1] & 1) == opponent // jump square holds enemy piece
                           && !pieceJumped[point.y + (2 * opponent - 1)][point.x + 1]                    // piece wasn't already jumped
                           && Board.gameBoard[point.y + (4 * opponent - 2)][point.x + 2] == 4) {           // dest square empty OR
                        pieceJumped[point.y + (2 * opponent - 1)][point.x + 1] = true;
                        depth++;
                        board_node[depth].x = point.x + 2;
                        board_node[depth].y = point.y + (4 * opponent - 2);
                        board_node[depth].checked = 0;
                        board_node[depth].descJump = false;
                    }
                } else if(!(board_node[depth].checked & 0x4U)) {
                    board_node[depth].checked |= 0x4U;

                    if(    (point.x - 2) >= 0 && (point.x - 2) <= 7 && (point.y + (4 * opponent - 2)) >= 0 && (point.y + (4 * opponent - 2)) <= 7 // in range
                           && Board.gameBoard[point.y + (2 * opponent - 1)][point.x - 1] != 4              // jump square not empty
                           && (Board.gameBoard[point.y + (2 * opponent - 1)][point.x - 1] & 1) == opponent // jump square holds enemy piece
                           && !pieceJumped[point.y + (2 * opponent - 1)][point.x - 1]                    // piece wasn't already jumped
                           && Board.gameBoard[point.y + (4 * opponent - 2)][point.x - 2] == 4) {           // dest square empty OR
                        pieceJumped[point.y + (2 * opponent - 1)][point.x - 1] = true; // we have jumped that piece
                        depth++;
                        board_node[depth].x = point.x - 2;
                        board_node[depth].y = point.y + (4 * opponent - 2);
                        board_node[depth].checked = 0;
                        board_node[depth].descJump = false;
                    }
                } else {
                    if(!board_node[depth].descJump) {
                        for(j = 1; j <= depth; j++) {
                            potentialMoves.x_route[j] = board_node[j].x;
                            potentialMoves.y_route[j] = board_node[j].y;
                        } if(j < 13) {
                            potentialMoves.x_route[j] = potentialMoves.y_route[j] = 0xFFU;
                        }
                        moveList.push_back(potentialMoves);
                    } board_node[depth - 1].descJump = true;
                    pieceJumped[(point.y + board_node[depth - 1].y) / 2][(point.x + board_node[depth - 1].x) / 2] = false;
                    depth--;
                }
            }
        }
    }
    if(!moveList.size()) {
        for(i = 0; i < 12; i++) {
            temp = Board.pawns[currentPlayer][i];
            potentialMoves.x_route[0] = temp.xCoordinate;
            potentialMoves.y_route[0] = temp.yCoordinate;
            potentialMoves.x_route[2] = potentialMoves.y_route[2] = 0xFFU;

            xDestination[0] = temp.xCoordinate - 1;
            xDestination[1] = temp.xCoordinate + 1;
            yDestination[0] = temp.yCoordinate - 1;
            yDestination[1] = temp.yCoordinate + 1;

            if(temp.king) {
                for(j = 0; j < 2; j++) {
                    for(k = 0; k < 2; k++) {
                        if(    xDestination[j] >= 0 && xDestination[j] <= 7
                               && yDestination[k] >= 0 && yDestination[k] <= 7
                               && Board.gameBoard[yDestination[k]][xDestination[j]] & 4) {
                            potentialMoves.x_route[1] = xDestination[j];
                            potentialMoves.y_route[1] = yDestination[k];
                            moveList.push_back(potentialMoves);
                        }
                    }
                }
            } else {
                for(j = 0; j < 2; j++) {
                    if(    xDestination[j] >= 0 && xDestination[j] <= 7
                           && yDestination[opponent] >= 0 && yDestination[opponent] <= 7
                           && Board.gameBoard[yDestination[opponent]][xDestination[j]] & 4) {
                        potentialMoves.x_route[1] = xDestination[j];
                        potentialMoves.y_route[1] = yDestination[opponent];
                        moveList.push_back(potentialMoves);
                    }
                }
            }
        }
    }

    return moveList;
}

void checker::printMoves(int i, move &moves){
    std::cout << "  [" << i+1 << "] ";
    for(int i = 0; i < 13; i++) {
        if(moves.x_route[i] > 7 || moves.y_route[i] > 7 || 1 & (moves.x_route[0] + moves.y_route[0])) {
            std::cout << std::endl;
            return;
        }
        std::cout << (i ? " -> " : "");
        std::cout << "( "<< columnIndex[moves.x_route[i]] << " , "<< moves.y_route[i] + 1 << " )";
    }
    std::cout << std::endl;
}

void checker::setBoard(board Board){
    this -> Board = Board;
}

board checker::getBoard(){
    return Board;
}

int checker::getCurrentPlayer(){
    return currentPlayer;
}

void checker::setMoveStartTime(time moveStartTime){
    this -> moveStartTime = moveStartTime;
}

Game::time checker::getMoveStartTime (){
    return moveStartTime;
}

void checker::startGame(bool isP1AI, bool isP2AI, int startingPlayer, double timeLimit){
    board tempBoard;
    move Move;
    players[0] = agent(this, isP1AI, timeLimit);
    players[1] = agent(this, isP2AI, timeLimit);

    endGame = false;
    currentPlayer = startingPlayer;

    while(!endGame) {
        printBoard();
        std::cout << "Player " << (currentPlayer + 1) << " 's turn ..." << std::endl;
        tempBoard = getBoard();
        setMoveStartTime(clock::now());
        Move = players[currentPlayer].play(tempBoard);

        tempBoard = checker::transition_board(Move);
        setBoard(tempBoard);
        currentPlayer = (~currentPlayer) & 1;
    }
    if((Move.x_route[0] > 7 || Move.y_route[0] > 7) ) {
        std::cout << "Game over! Player " << (currentPlayer + 1) << " wins!" << std::endl;
    }
    else {
        std::cout << "Draw!" << std::endl;
    }
}

void checker::printBoard(){
    board Board = getBoard();
    int i, j;
    std::cout << std::endl << std::setw(20);
    for(i = 0; i < 8; i++) {
        std::cout << "   " << columnIndex[i] << "   ";  // A , B , C ... 
    }
    std::cout << std::endl << std::endl;

    for(i = 7; i >= 0; i--) {
        std::cout << std::setw(22);
        for(int j = 0; j < 8; j++) {
            if((j + i) & 1) {
                std::cout << "\e[47m" << "       ";
            } else {
                std::cout << "\e[40m" << "       ";
            }
        }
        std::cout << "\e[40m" << std::endl << std::setw(13);
        std::cout << " " << (i + 1) << "   ";

        for(j = 0; j < 8; j++) { // tiles edge color
            if((j + i) & 1) {
                std::cout << "\e[47m" << "   ";
            } else {
                std::cout << "\e[40m" << "   ";
            }
            if(Board.gameBoard[i][j] && 1) {
                std::cout << "\u001b[36;1m" << pawnSymbol[Board.gameBoard[i][j]] << "\u001b[37m";
            } else {
                std::cout << "\u001b[31;1m" << pawnSymbol[Board.gameBoard[i][j]] << "\u001b[0m";
            }
            std::cout << "   ";
        }

        std::cout << "\e[40m" << std::endl << std::setw(22);
        for(j = 0; j < 8; ++j) {
            if((j + i) & 1) {
                std::cout << "\e[47m" << "       ";
            } else {
                std::cout << "\e[40m" << "       ";
            }
        }
        std::cout << "\e[40m" << std::endl;
    }
    std::cout << std::endl;
}

board checker::transition_board(move &Move){
    board Board = getBoard();
    return checker::transition_board(Move, Board);
}

board checker::transition_board(move &Move, board &Board){
    board next_board; move next_move;
    signed short start_box ,jumped_box;
    int x_dist, y_dist, i;
    int opponent;
    int x_jump, y_jump;

    if(Move.x_route[0] > 7 || Move.y_route[0] > 7 || Move.x_route[1] > 7 || Move.y_route[1] > 7
       || ((Move.x_route[0] + Move.y_route[0]) & 1) || ((Move.x_route[1] + Move.y_route[1]) & 1)
       || Board.gameBoard[Move.y_route[1]][Move.x_route[1]] != 4) {
        return Board;
    } else if(abs(Move.x_route[1] - Move.x_route[0]) == 1 && abs(Move.y_route[1] - Move.y_route[0]) == 1) {
        start_box = -1; 
        for(i = 0; i < 12; i++) {
            if(    Board.pawns[Move.agentID][i].xCoordinate == Move.x_route[0]
                   && Board.pawns[Move.agentID][i].yCoordinate == Move.y_route[0]) {
                start_box = i;
            }
        }
        if(start_box < 0) {
            return Board;
        }
        next_board = Board;
        if(!Board.pawns[Move.agentID][start_box].king && Move.y_route[1] == 7 * ((~Move.agentID) & 1)) {
            next_board.pawns[Move.agentID][start_box].king = true;
        }
        next_board.gameBoard[Move.y_route[0]][Move.x_route[0]] = 4;
        next_board.gameBoard[Move.y_route[1]][Move.x_route[1]] = (int(next_board.pawns[Move.agentID][start_box].king) << 1) | Move.agentID;
        next_board.pawns[Move.agentID][start_box].xCoordinate = Move.x_route[1];
        next_board.pawns[Move.agentID][start_box].yCoordinate = Move.y_route[1];
        return next_board;
    } else if(abs(x_dist = (Move.x_route[1] - Move.x_route[0])) == 2 && abs(y_dist = (Move.y_route[1] - Move.y_route[0])) == 2) {
        opponent = (~Move.agentID) & 1;
        start_box = -1;
        for(i = 0; i < 12; i++) {
            if(    Board.pawns[Move.agentID][i].xCoordinate == Move.x_route[0]
                   && Board.pawns[Move.agentID][i].yCoordinate == Move.y_route[0]) {
                start_box = i;
            }
        }
        if(start_box < 0) {
            return Board;
        }
        next_board = Board;
        next_move = Move;
        do {
            jumped_box = -1;
            x_jump = next_move.x_route[0] + (0 < x_dist) - (x_dist < 0);
            y_jump = next_move.y_route[0] + (0 < y_dist) - (y_dist < 0);
            for(i = 0; i < 12; i++) {
                if(    next_board.pawns[opponent][i].xCoordinate == x_jump
                       && next_board.pawns[opponent][i].yCoordinate == y_jump) {
                    jumped_box = i;
                }
            }
            if(jumped_box < 0) {
                return Board;
            }
            if(!Board.pawns[Move.agentID][start_box].king && next_move.y_route[1] == 7 * opponent) {
                next_board.pawns[Move.agentID][start_box].king = true;
            }
            next_board.gameBoard[next_move.y_route[0]][next_move.x_route[0]] = 4;
            next_board.gameBoard[y_jump][x_jump] = 4;
            next_board.gameBoard[next_move.y_route[1]][next_move.x_route[1]] = (int(next_board.pawns[Move.agentID][start_box].king) << 1) | Move.agentID;
            next_board.pawns[Move.agentID][start_box].xCoordinate = 0xFFU;
            next_board.pawns[Move.agentID][start_box].yCoordinate = 0xFFU;
            next_board.pawns[opponent][jumped_box].xCoordinate = 0xFFU;
            next_board.pawns[opponent][jumped_box].yCoordinate = 0xFFU;
            next_board.pawns[Move.agentID][start_box].xCoordinate = next_move.x_route[1];
            next_board.pawns[Move.agentID][start_box].yCoordinate = next_move.y_route[1];
            if(!Board.pawns[Move.agentID][start_box].king && next_board.pawns[Move.agentID][start_box].king) {
                return next_board;
            }
            for(i = 0; next_move.x_route[i] >= 0 && next_move.x_route[i] <= 7 && next_move.y_route[i] >= 0 && next_move.y_route[i] <= 7; i++) {
                next_move.x_route[i] = next_move.x_route[i + 1];
                next_move.y_route[i] = next_move.y_route[i + 1];
            }
        } while(!(next_move.x_route[0] > 7 || next_move.y_route[0] > 7 || next_move.x_route[1] > 7 || next_move.y_route[1] > 7
                  || ((next_move.x_route[0] + next_move.y_route[0]) & 1) || ((next_move.x_route[1] + next_move.y_route[1]) & 1)
                  || next_board.gameBoard[next_move.y_route[1]][next_move.x_route[1]] != 4)
                && (abs(x_dist = (next_move.x_route[1] - next_move.x_route[0])) == 2 && abs(y_dist = (next_move.y_route[1] - next_move.y_route[0])) == 2));
        return next_board;
    } else {
        return Board;
    }
}

void checker::saveGame(){
    std::string filePath;
    std::cout << "Please enter file path to save game... " << std::endl;;
    std::cout << ">: ";
    std::cin >> filePath;
    std::ofstream outFile(filePath.c_str());
    board Board = this->getBoard();

    if(!outFile.is_open()) {
        std::cout << "ERROR: Cannot open file " << filePath.c_str() << ", game save unseccessful!" << std::endl;
    } else {
        for(int i = 7; i >= 0; i--) {
            for(int j = 0; j < 8; j++) {
                outFile << int(Board.gameBoard[i][j]) << (j < 7 ? " " : "");
            }
            outFile << std::endl;
        }
        outFile.close();
        std::cout << "Game successfully saved to '" << filePath << "'!" << std::endl;
    }
}

void checker::loadGame(std::string &filePath){
    std::ifstream inFile(filePath.c_str());
    int i, j;
    int k[2] = {0};
    int square;
    int player;

    if(!inFile.is_open()) {
        std::cout << "Warning: Game could not be loaded!" << std::endl;
        std::cout << "         Failed to open '" << filePath << "' for reading." << std::endl;
        exit(-1);
    } else {
        board Board{};

        for(i = 0; i < 8; i++) {
            for(j = 0; j < 8; j++) {
                Board.gameBoard[i][j] = 4;
            }
        }
        for(i = 0; i < 2; i++) {
            for(j = 0; j < 12; j++) {
                Board.pawns[i][j].king = false;
                Board.pawns[i][j].xCoordinate = 0xFFU;
                Board.pawns[i][j].yCoordinate = 0xFFU;
            }
        }

        // read in board and pawns
        for(i = 7; i >= 0; i--) {
            for(j = 0; j < 8; j++) {
                inFile >> square;
                Board.gameBoard[i][j] = static_cast<int>(square);

                if(square < 4) {
                    player = static_cast<int>(square & 1);
                    Board.pawns[player][k[player]].king = static_cast<bool>(square & 2);
                    Board.pawns[player][k[player]].xCoordinate = j;
                    Board.pawns[player][k[player]].yCoordinate = i;
                    k[player]++;
                }
            }
        }

        inFile.close();
        std::cout << "Game successfully loaded from '" << filePath << "'!" << std::endl;
        this->setBoard(Board);
    }
}

void checker::gameOver(){
    endGame = true;
}






