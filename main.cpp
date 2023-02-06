#define NOMINMAX            // max()
#include <iostream>
#include <ios>              //used to get stream size
#include <limits>           //used to get numeric limits
#include <ctype.h>
#include <string>
#include "Game.h"
#include "checker.h"

using namespace std;

int getGameMode();
void setTimeLimit(int gameMode, Game::info &gameinfo);
void setStartingPlayer(Game::info &gameinfo);
void loadGame(Game::info &gameinfo);
void beginGame(int gameMode, Game::info &gameinfo);

// rough styling

int main(){
    cout << "\n\n\u001b[32;1m                          Welcome to\u001b[0m"<<'\n' <<
R"(             ____ ____ ___ _ ____ _ ____ _ ____ _                 
             |__| |__/  |  | |___ | |    | |__| |                 
             |  | |  \  |  | |    | |___ | |  | |___              
                                                     
        _ _  _ ___ ____ _    _    _ ____ ____ _  _ ____ ____ 
        | |\ |  |  |___ |    |    | | __ |___ |\ | |    |___ 
        | | \|  |  |___ |___ |___ | |__] |___ | \| |___ |___ 
                                                     
            ____ _  _ ____ ____ _  _ ____ ____ ____              
            |    |__| |___ |    |_/  |___ |__/ [__               
            |___ |  | |___ |___ | \_ |___ |  \ ___]              
                                                     )"<<
    '\n' << endl;

    Game::info gameinfo;

    int gameMode = getGameMode();

    //Setting time limit if an agent is needed
    if(gameMode != 4) setTimeLimit(gameMode, gameinfo);
    
    //Setting starting player
    setStartingPlayer(gameinfo);

    //Load game from a file... Or not?
    loadGame(gameinfo);
    
    beginGame(gameMode, gameinfo);
    return 0;
}

int getGameMode(){
    int option = 0;
    cout << "  \u001b[32;1m             Please choose one option below:\u001b[0m\n" << endl;
    cout << "\t   [1]   \e[0;32m AI  \e[0m \e[0;91mvs\e[0m Human, Human in \e[0;91mred\e[0m" << endl; 
    cout << "\t   [2]   \e[0;32m AI  \e[0m \e[0;91mvs\e[0m Human, Human in \e[0;96mcyan\e[0m" << endl;
    cout << "\t   [3]   \e[0;32m AI  \e[0m \e[0;91mvs\e[0m \e[0;32m  AI\e[0m" << endl;
    cout << "\t   [4]   Human \e[0;91mvs\e[0m Human" << endl;
    cout << ">: ";
    while (!option)
    {
        cin >> option;
        if(option < 1 || option > 4){
            cout << "Invalid input, please try again..." << endl;
            cout <<">: ";
            option = 0;
            cin.clear();
            cin.ignore(numeric_limits <streamsize> ::max(), '\n');
        }
    }
    cout << "\e[0;32mYou have chosen option\e[0m [" << option << "]:";
    switch (option)
    {
    case 1:
        cout << "\e[0;32m AI\e[0m \e[0;91mvs\e[0m Human player, Human in \e[0;91mred\e[0m" << endl;
        break;
    case 2:
        cout << "\e[0;32m AI\e[0m \e[0;91mvs\e[0m Human player, Human in \e[0;96mcyan\e[0m" << endl;;
        break;
    case 3:
        cout << "\e[0;32m AI\e[0m \e[0;91mvs\e[0m \e[0;32mAI\e[0m" << endl;
        break;
    case 4:
        cout << "Human \e[0;91mvs\e[0m Human" << endl;
    default:
        break;
    }
    return option;
}

void setTimeLimit(int gameMode, Game::info &gameinfo){
    double timeLimit;
    cout << endl;
    cout << "\e[0;32mEnter a time limit for agent (seconds)...\e[0m" << endl;
    cout << ">: ";
    cin >> timeLimit;
    
    gameinfo.timeLimit = timeLimit;
    cout << "Time limit set to " << gameinfo.timeLimit << " seconds." << endl;
}

void setStartingPlayer(Game::info &gameinfo){
    int startingPlayer = 0;
    cout << endl;
    cout << "\e[0;32mPlease decide the player to start the game... \e[0m(1 : \e[0;91mred\e[0m";   
        cout << " / 2 : \e[0;96mcyan\e[0m";
        cout << ")" << endl;
    cout << ">: ";

    while (!startingPlayer)
    {
        cin >> startingPlayer;
        if(startingPlayer != 1 && startingPlayer != 2){
            cout << "Invalid input, please enter '1' or '2'" << endl;
            cout <<">: ";
            startingPlayer = 0;
            cin.clear();
            cin.ignore(numeric_limits <streamsize> ::max(), '\n');
        }
    }

    gameinfo.startingPlayer = startingPlayer - 1;
    cout << "Starting player is set to ";
    switch (startingPlayer)
    {
    case 1:
        cout << "\e[0;91mred\e[0m" << endl;
        break;
    case 2:
        cout << "\e[0;96mcyan\e[0m" << endl;
        break;
    default:
        break;
    }
}

void loadGame(Game::info &gameinfo){
    char load = 0;
    string filePath;
    cout << endl;
    cout << "Would you like to load a board-state?  [\e[0;92my\e[0m/\e[0;91mn\e[0m]" << endl;
    cout << ">: ";
    
    while(!load){
        cin >> load;
        load = tolower(load);
        if(!(load == 'y' || load == 'n')){
            load = 0;
            cin.clear();
            cin.ignore(numeric_limits <streamsize> ::max(), '\n');
            cout << "Invalid input, please enter '\e[0;92my\e[0m' or '\e[0;91mn\e[0m'" << endl;
            cout << ">: ";
        }
    }
    switch (load)
    {
    case 'y':
        Game::loadFromFile = true;
        cout << "Please enter file path... " << endl;
        cout << ">: ";
        cin >> filePath;
        Game::filePath = filePath;
        break;
    default:
        break;
    }
}

void beginGame(int gameMode, Game::info &gameinfo){
    int startingPlayer = gameinfo.startingPlayer;
    double timeLimit = gameinfo.timeLimit;
    Game::checker Checker = Game::checker();

    if(Game::loadFromFile){
        Checker.loadGame(Game::filePath);
    }

    switch (gameMode)
    {
    case 1:
        Checker.startGame(false, true, startingPlayer, timeLimit);
        break;

    case 2:
        Checker.startGame(true, false, startingPlayer, timeLimit);
        break;

    case 3:
        Checker.startGame(true, true, startingPlayer, timeLimit);
        break;

    case 4:
        Checker.startGame(false, false, startingPlayer, timeLimit);
        break;
    
    default:
        break;
    }
}
