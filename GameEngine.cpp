#include "GameEngine.h"
#include <string>
// #include <memory>

GameEngine::GameEngine()
{
    numbPlayers = 0;  
}

GameEngine::~GameEngine()
{

}

void GameEngine::loadTheGame(std::string fileName)
{
    // Counting amount of lines the file has
    string line;
    std::ifstream fileCount(fileName);

    int lineCount = 0;
    while (getline(fileCount, line)) {
        ++lineCount;
    }
    
    if (lineCount == TWO_PLAYER_LINE_NUMB) {
        this->numbPlayers = TWO_PLAYERS;
    }
    else if (lineCount == THREE_PLAYER_LINE_NUMB)
    {
        this->numbPlayers = THREE_PLAYERS;
    }
    else if (lineCount == FOUR_PLAYER_LINE_NUMB)
    {
        this->numbPlayers = FOUR_PLAYERS;
    }

    fileCount.close();
    cin.ignore();

    // Loading up the game
    bool colour = false;
    std::string input;
    std::ifstream inStream(fileName);

    inStream >> colour;
    this->colours = colour;

    loadPlayers(inStream, input);
    loadBoard(inStream, input);
    loadTileBag(inStream, input);
    loadCurrentPlayer(inStream, input);

    cout << endl;
    cout << "\nQwirkle game loaded successfully\n" << endl;
    inStream.close();
    playTheGame();

}

void GameEngine::loadPlayers(std::istream& inStream, std::string input)
{
    for (int i = 0; i < numbPlayers; ++i) 
    {
        inStream >> input;
        std::shared_ptr<Player> newPlayer = std::make_shared<Player>(input);
        inStream >> input;
        newPlayer->setScore(stoi(input));
        inStream >> input;
        newPlayer->getHand()->setHand(input);
        this->playerList.push_back(newPlayer);
    }
}

void GameEngine::loadBoard(std::istream& inStream, std::string input)
{
    board = std::make_shared<Board>();
    inStream >> input;
    board->createBoardFromSave(input);
    getline(inStream, input);
    getline(inStream, input);
    board->setBoardFromSave(input);
}

void GameEngine::loadTileBag(std::istream& inStream, std::string input)
{
    tileBag = std::make_shared<TileBag>();
    getline(inStream, input);
    tileBag->placeTilesFromSave(input);
}

void GameEngine::loadCurrentPlayer(std::istream& inStream, std::string input)
{
    inStream >> input;
    for (int i = 0; i < numbPlayers; ++i) 
    {
        if (playerList[i]->getName() == input)
        {
            playerList[i]->setState(true);
        }
    }
}

void GameEngine::saveTheGame(std::string fileName)
{

    std::string saveAsName = fileName + SAVE_EXTENSION;
    std::ofstream outFile;
    outFile.open(saveAsName);

    outFile << colours << endl;

    for (int i = 0; i < numbPlayers; ++i)
    {
        outFile << playerList[i]->getName() << endl;
        outFile << playerList[i]->getScore() << endl;
        outFile << playerList[i]->getHand()->getHandContents() << endl;
    }
 
    outFile << board->getMaxRow() << "," << board->getMaxCol() << endl;
    outFile << board->getBoardState() << endl;
    outFile << tileBag->getBagContents() << endl;

    for (int i = 0; i < numbPlayers; ++i)
    {
        if (playerList[i]->getState())
        {
            outFile << playerList[i]->getName() << endl;
        }
    }

    outFile.close();
}

void GameEngine::newGame(int numbPlayers, bool colours)
{    
    this->numbPlayers = numbPlayers;
    this->colours = colours;
    cout << "\nStarting a New Game\n" << endl;

    for (int i = 0; i < numbPlayers; ++i) 
    {
        char playerName[] = PLACEHOLDER;
        do
        {
            cout << "Enter a name for player " << i + 1
                 << " (uppercase characters only)" << endl;

            cout << "> "; 
            cin >> playerName;
            if (std::string(playerName) == HELP) 
            {
                cout << "\nEnter a name that has no spaces (uppercase only)" 
                     << endl;
            }
            cout << "\n";
        } while (!playerNameValid(playerName) && !cin.eof());

        std::shared_ptr<Player> newPlayer = 
        std::make_shared<Player>(std::string(playerName));

        this->playerList.push_back(newPlayer);

        // If it is the first player, toggle their state
        if (i == 0) 
        {
            newPlayer->setState(true);
        }
        // If it is the last player, clear the whitespace
        else if (i == numbPlayers - 1) 
        {
            cin.ignore();
        }
    }

    // Initialise the tileBag and the board
    tileBag = std::make_shared<TileBag>();
    tileBag->shuffleTileBag();
    board = std::make_shared<Board>();
    board->setBoardDimensions(MAX_ROW, MAX_COL);

    // Initialise all the players hands
    for(int i = 0; i < numbPlayers; ++i){
        giveHand(playerList[i]);
    }

    current_round_score = 0;

    // A method used to start the game
    if (!cin.eof()) { 
        cout << "Let's Play!" << endl;
        playTheGame();

    }
    else
    {
        cout << "Goodbye" << endl;
    }

}

bool GameEngine::playerNameValid(char* name)
{
    bool flag = true;
    char notAllowed[] = "qwertyuiopasdfghjklzxcvbnm1234567890!@#$%^&*()`~-_=+[{]};:<,>.?/|'";

    /* 
     * Looping through the name and notAllowed character arrays to 
     * find if they match 
     */ 
    for (int i = 0; (name[i] != '\0'); ++i)
    {
        for (int j = 0; (notAllowed[j] != '\0'); ++j)
        {
            if (name[i] == notAllowed[j])
            {
                flag = false;
            }
        }
    }

    return flag;
}

bool GameEngine::tileIsValid(string tileString, string type)
{
    bool flag = true;
    const char* tileAsChar = tileString.c_str();
    char letter = tileAsChar[0]; 
    char notAllowed[] = "1234567890!@#$%^&*()`~-_=+[{]};:<,>.?/|'";

    // Checks if the inputted tileString is upper case 
    if (letter != toupper(letter))
    {
        flag = false;
    }

    // Checks for special characters
    for (int i = 0; i < (notAllowed[i] != '\0'); ++i)
    {
        if (letter == notAllowed[i])
        {
            flag = false;
        }
    }

    // Checking if the tile inputted by the player is in the player's hand    
    if (type == TILE_ON_HAND)
    {
        if (tileString.length() == 2)
        {
            int shape= getBoardPlacementCol(tileString);
            if (shape < 1 || shape > 6)
            {
                flag = false;
            }
        }
        else
        {
            flag = false;
        }
    }

    // If we're checking the tile coordinates on the board
    else if (type == TILE_ON_BOARD)
    {
        if (tileString.length() == 2 || tileString.length() == 3)
        {
            int col = getBoardPlacementCol(tileString);
            if (col < MIN_COL || col > MAX_COL)
            {
                flag = false;
            }
        }
        else
        {
            flag = false;
        }
    }

    else
    {
        flag = false;
    }

    return flag;
}

// Returns the integer representation of a row letter
int GameEngine::getBoardPlacementRow(string tilePlacementString)
{
    const char* tileAsChar = tilePlacementString.c_str();
    char row = tileAsChar[0];
    int rowReturn = row - ASCII;

    return rowReturn;
}

// THIS RETURNS THE NUMERIC COMPONENT OF THE TILE STRING NOT JUST THE COLS
int GameEngine::getBoardPlacementCol(string tilePlacementString)
{
    int returnCol = -1;
    bool stringIsNumeric = true;
    const char* tileAsChar = tilePlacementString.c_str();
    for (int i = 1; tileAsChar[i] != '\0'; ++i)
    {
        if (!isdigit(tileAsChar[i]))
        {
            stringIsNumeric = false;
        }
    }
    if (stringIsNumeric)
    {
        string tilePlacementNumberOnlyString = tilePlacementString.substr(1);
        returnCol = stoi(tilePlacementNumberOnlyString);    
    }

    return returnCol;
}



void GameEngine::placeOnBoard(Tile* tile, int row, int col)
{
    if (tile != nullptr && row < board->getMaxRow() && col < board->getMaxCol())
    {
        board->placeTile(tile, row, col);
    }
}


void GameEngine::updatePoints(shared_ptr<Player> player)
{
    player->incrementScore(current_round_score);
}

void GameEngine::updatePoints(shared_ptr<Player> player, int incrementPoints)
{
    player->incrementScore(incrementPoints);
}

void GameEngine::replenishHandTiles(shared_ptr<Player> player)
{
    Tile* tileToAddToHand = tileBag->drawTile();
    if (tileToAddToHand != nullptr)
    {
        player->getHand()->addTile(tileToAddToHand);
    }
    else
    {
        delete tileToAddToHand;
        tileToAddToHand = nullptr;
    }   
}
bool GameEngine::actionPlace(string command, 
                             std::shared_ptr<Player> player, bool* firstTurn)
{      
    bool success = false;

    // Gathering strings of tiles and board placement from the passed in string
    string tileString = command.substr(TILE_INDEX, TILE_LENGTH);
    string tilePlacementString = "";

    if (command.size() == PLACE_COMMAND_SINGLE_DIGIT)
    {
        tilePlacementString = command.substr(BOARD_INDEX, 
                                             BOARD_POS_SINGLE_DIGIT);
    }
    else if (command.size() == PLACE_COMMAND_DOUBLE_DIGIT)
    {
        tilePlacementString = command.substr(BOARD_INDEX, 
                                             BOARD_POS_DOUBLE_DIGIT);
    }

    int indexOfTileInHand = player->getHand()->getIndexOfTileStr(tileString);
    int row = getBoardPlacementRow(tilePlacementString);
    int col = getBoardPlacementCol(tilePlacementString);

    if(indexOfTileInHand != -1) 
    { 
        if (board->isEmpty(row, col))
        {
            // On the first turn the player can place anywhere
            if (*firstTurn)
            {
                /* 
                 * Places a tile on the board and removes it 
                 * from the players' hand
                 */
                Tile* tileToAddtoBoard = 
                new Tile(*(player->getHand()->getTile(indexOfTileInHand)));
                
                player->getHand()->removeTile(indexOfTileInHand);
                placeOnBoard(tileToAddtoBoard, row, col);

                // Refill the player's hand with tiles
                replenishHandTiles(player);

                /* 
                 * On the first turn, the player who places the tile 
                 * recieves 2 points for tile placement
                 */
                updatePoints(player, 2);

                *firstTurn = false;
                success = true;

            }

            // Otherwise the placement must be valid
            else
            {
                Tile* tileToAddtoBoard = 
                new Tile(*(player->getHand()->getTile(indexOfTileInHand)));

                if (validTilePlacement(tileToAddtoBoard, row, col, player))
                {
                    /* 
                     * Removes a tile from a players' hand and replaces it 
                     * with one from the tile bag
                     */
                    player->getHand()->removeTile(indexOfTileInHand);
                    placeOnBoard(tileToAddtoBoard, row, col);

                    // Refill the player's hand with tiles
                    replenishHandTiles(player);

                    // Updates points for each placement
                    updatePoints(player);

                    success = true;
                }
                else
                {
                    delete tileToAddtoBoard;
                    tileToAddtoBoard = nullptr;
                }
            }
        }

        else
        {
            cout << "\nTile cannot be placed at that position\n" << endl;
        }

    }
    else if (indexOfTileInHand == -1)
    {
        cout << "\nPlayer does not have tile\n" << endl;
    }

    return success;
    
}

bool GameEngine::actionReplace(string command, std::shared_ptr<Player> player)
{
    bool success = false;
    
    // Checks if player has tile in their current hand
    string tileString = command.substr(REPLACE_TILE_INDEX, TILE_LENGTH);
    int indexOfTileInHand = player->getHand()->getIndexOfTileStr(tileString);

    if (indexOfTileInHand != -1)
    {
        if (tileBag->getLength() > 0)
        {
            Tile* newTile = 
            new Tile(*(player->getHand()->getTile(indexOfTileInHand)));

            tileBag->addTile(newTile);
            player->getHand()->removeTile(indexOfTileInHand);
            // player->getHand()->addTile(tileBag->drawTile());
            replenishHandTiles(player);
            success = true;
        }
        else
        {
            cout << "\nNothing left in the tile bag, switching turns\n" << endl;
            success = true;
        }

    }
    else
    {
        cout << "\nPlayer does not have tile\n" << endl;
    }

    return success;
}

// TODO:
void GameEngine::playRound(std::shared_ptr<Player> player, bool* gameOver, bool* 
                           firstTurn, bool* quit, bool* firstEmptyHand)
{    
    printRound(player);
    string command;
    bool success = false;

    // Loops if the player enters bad input
    while (!success)
    {
        if (!cin.eof())
        {
            success = false;
            cout << "> ";
            getline(cin, command);

            // Obtaining the action command
            string delimiter = " ";
            string playerAction = command.substr(0, command.find(delimiter));

            if (playerAction == SAVE)
            {
                string fileName = "unnamed_save_file";
                if (command.size() > playerAction.size())
                {
                    fileName = command.substr(command.find(delimiter) + 1, 
                                              command.size() - 1);
                }
                saveTheGame(fileName);
            }

            if (playerAction == QUIT)
            {
                success = true;
                *quit = true;
            }

            if (playerAction == HELP)
            {
                cout << "\nPossible commands below: \n" 
                     << "- place <tile> at <board position>\n"
                     << "- replace <tile>\n" 
                     << "- quit\n"
                     << endl;
            }

            else if (command.size() <= TOTAL_COMMAND_LENGTH) 
            {   
                // If player choose to place a tile
                if (playerAction == PLACE)
                {
                    if (command.size() == TOTAL_COMMAND_LENGTH_CASE_1 || 
                        command.size() == TOTAL_COMMAND_LENGTH_CASE_2)
                    {
                        // Checks tile in hand
                        if (tileIsValid(
                            command.substr(HAND_TILE_INDEX, 
                                           TILE_STRING_LENGTH_SINGLE), 
                                           TILE_ON_HAND))
                        {
                            // Checks tile board placement
                            if (tileIsValid(
                                command.substr(TILE_STRING_INDEX, 
                                               TILE_STRING_LENGTH_SINGLE), 
                                               TILE_ON_BOARD))
                            {
                                success = actionPlace(command, 
                                                      player, firstTurn);
                            }
                            else if (tileIsValid(
                                     command.substr(TILE_STRING_INDEX, 
                                                    TILE_STRING_LENGTH_DOUBLE), 
                                                    TILE_ON_BOARD))
                            {                            
                                success = actionPlace(command, 
                                                      player, firstTurn);

                            }
                            else
                            {
                                cout << "\nInvalid placement position, " 
                                     << "try again\n" << endl;
                            }                           
                        }

                        else
                        {
                            cout << "\nInvalid tile in hand, "
                                 << "try again\n" << endl;
                        }
                    }
                    else
                    {
                        cout << "\nInvalid place command, try again\n" << endl;
                    }
                }

                else if (playerAction == REPLACE)
                {
                    if (command.size() == COMMAND_REPLACE)
                    {
                        if (tileIsValid(command.substr(REPLACE_TILE_INDEX, 
                                                       TILE_LENGTH), 
                                                       TILE_ON_HAND))
                        {
                            success = actionReplace(command, player);
                        }
                        else
                        {
                            cout << "\nInvalid replace command, "
                                 << "try again\n" << endl;
                        }
                    }
                    else
                    {
                        cout << "\nInvalid replace command, "
                             << "try again\n" << endl;
                    }
                }
                else
                {
                    if (playerAction != SAVE && !cin.eof())
                    {
                        cout << "\nInvalid Input\n" << endl;
                    }
                }   
            }

            else
            {
                if (!cin.eof())
                {
                    cout << "\nInvalid Input\n" << endl;
                }
            }
        }
        else
        {
            success = true;
            cout << "\nGoodbye\n" << endl;
            // *gameOver = true;
        }
    }
    if (!cin.eof()) 
    {

        if (!(*firstEmptyHand))
        {
            if (player->getHand()->getLength() == 0)
            {
                cout << "\n";
                cout << "The player " << player->getName() 
                     << " has cleared their hand!" << endl;

                cout << "They get +6 points" << endl;
                cout << "\n" << endl;
                updatePoints(player, 6);
                *firstEmptyHand = true;
                *gameOver = true;
            }
        }

        cout << "\n";
        setOtherPlayerTurn();  
    }
}

bool GameEngine::playersHaveEnoughTiles()
{
    bool playersHaveEnoughTiles = true;

    for (int i = 0; i < numbPlayers; ++i)
    { 
        if (playerList[i]->getHand()->getLength() != MAX_HAND 
            && tileBag->isEmpty()) 
        { 
            playersHaveEnoughTiles = true;
        } 
        else if (playerList[i]->getHand()->getLength() != MAX_HAND)
        {
            playersHaveEnoughTiles = false;
        }
    }
    if (playersHaveEnoughTiles == false)
    {
        cout << "\nNot enough tiles to distribute to players!\n" << endl;
    }

        return playersHaveEnoughTiles;
}

bool GameEngine::playerNoLongerHasTiles()
{
    bool playerNoLongerHasTiles = false;
    
    // Loops thorugh all the players and checks if any player has a empty hand
    for (int i = 0; i < numbPlayers; ++i)
    { 
        if (playerList[i]->getHand()->getLength() == 0) 
        { 
            playerNoLongerHasTiles = true;
        } 
    }

    return playerNoLongerHasTiles;
}

// TODO:
void GameEngine::playTheGame()
{  
    bool gameOver = false;
    bool firstTurn = false;
    bool firstEmptyHand = false;
    bool quit = false;

    if (board->getNumberOfTilesOnBoard() == 0)
    {
        firstTurn = true;  
    }

    // Makes sure each player starts with enough tiles equal to the maximum hand 
    if (playersHaveEnoughTiles())
    {
        // Initates game loop
        while (!gameOver && !quit && !cin.eof())
        {
            /* 
             * Loops through all the players, checks if it is their turn, if 
             * it is their turn, make them play the round
             */
            for (int i = 0; i < numbPlayers; ++i) 
            {   
                if (playerList[i]->getState() == true) 
                {
                    playRound(playerList[i], &gameOver, &firstTurn, 
                             &quit, &firstEmptyHand);
                }
            }

            if (tileBag->isEmpty() && playerNoLongerHasTiles())
            {
                gameOver = true;
            }
        }

        if (quit)
        {

            cout << "\nGoodbye\n" << endl;

        }
        if (gameOver)
        {
            gameOverPrint();
        }
    }
    
}

void GameEngine::gameOverPrint()
{
    cout << "\n";
    cout << "Game over" << endl;
    for (int i = 0; i < numbPlayers; ++i) 
    {
        cout << "Score for " << playerList[i]->getName() << " : " 
             << playerList[i]->getScore() << endl;
    }

    if (numbPlayers == TWO_PLAYERS) 
    {
        int playerOneScore = playerList[FIRST_PLAYER]->getScore();
        int playerTwoScore = playerList[SECOND_PLAYER]->getScore();

        if (playerOneScore == 0 && playerTwoScore == 0)
        { 
            std::cout << "Both players have the same score (tie)!" << std::endl;
        }
        if (playerOneScore > playerTwoScore)
        {
            cout << "Player " << playerList[FIRST_PLAYER]->getName() 
                 << " won!" << endl;
        }
        else if (playerOneScore < playerTwoScore)
        {
            cout << "Player " << playerList[SECOND_PLAYER]->getName() 
                 << " won!" << endl;
        }
    }
    else if (numbPlayers == THREE_PLAYERS)
    {
        int playerOneScore = playerList[FIRST_PLAYER]->getScore();
        int playerTwoScore = playerList[SECOND_PLAYER]->getScore();
        int playerThreeScore = playerList[THIRD_PLAYER]->getScore();

        if (playerOneScore == 0 && playerTwoScore == 0 && playerThreeScore == 0)
        { 
            std::cout << "All players have the same score (tie)!" << std::endl;
        }
        else 
        {
            /* 
             * Implemented a max heap using a priority queue to find the largest 
             * score out of the three players 
             */ 
            std::priority_queue<int> playerScores;
            for (int i = 0; i < numbPlayers; ++i) 
            { 
                playerScores.push(playerList[i]->getScore());
            }
            int highestScore = playerScores.top();

            for (int i = 0; i < numbPlayers; ++i)
            {
                if (highestScore == playerList[i]->getScore())
                {
                    cout << "Player " << playerList[i]->getName()
                         << " won!" << endl;
                }
            }
        }
    }
    else if (numbPlayers == FOUR_PLAYERS)
    {
        int playerOneScore = playerList[FIRST_PLAYER]->getScore();
        int playerTwoScore = playerList[SECOND_PLAYER]->getScore();
        int playerThreeScore = playerList[THIRD_PLAYER]->getScore();
        int playerFourScore = playerList[FOURTH_PLAYER]->getScore(); 

        
        if (playerOneScore == 0 && playerTwoScore == 0 
            && playerThreeScore == 0 && playerFourScore == 0)
        { 
            std::cout << "All players have the same score (tie)!" << std::endl;
        }
        else 
        {   
            /* 
             * Implemented a max heap using a priority queue to find the largest 
             * score out of the four players 
             */ 
            std::priority_queue<int> playerScores;
            for (int i = 0; i < numbPlayers; ++i) 
            { 
                playerScores.push(playerList[i]->getScore());
            }
            int highestScore = playerScores.top();
            for (int i = 0; i < numbPlayers; ++i)
            {
                if (highestScore == playerList[i]->getScore())
                {
                    cout << "Player " << playerList[i]->getName()
                         << " won!" << endl;
                }
            }
        }
    }

    cout << "\n";
    cout << "\nGoodbye\n" << endl;
}

// If the left board position is empty or contains a valid tile it returns true 
bool GameEngine::checkLeftIsValid(Tile* tile, int row, int col)
{
    bool leftIsValid = false;
    // Checks entire line for either same colour or shape
    if (!(board->isEmpty(row, col - 1)))
    {
        if (board->getTile(row, col - 1)->getColour() == tile->getColour())
        {
            if (sixTileLineCheck(row, col, LEFT, tile, COLOUR))
            {
                leftIsValid = true;
            }
        }
        else if (board->getTile(row, col - 1)->getShape() == tile->getShape())
        {
            if (sixTileLineCheck(row, col, LEFT, tile, SHAPE))
            {
                leftIsValid = true;
            }
        }
    } 
    else
    {
        leftIsValid = true;
    }

    return leftIsValid;
}

// If the right board position is empty or contains a valid tile it returns true 
bool GameEngine::checkRightIsValid(Tile* tile, int row, int col)
{
    bool rightIsValid = false;
    // Checks entire line for either same colour or shape
    if (!(board->isEmpty(row, col + 1)))
    {
        if (board->getTile(row, col + 1)->getColour() == tile->getColour())
        {
            if (sixTileLineCheck(row, col, RIGHT, tile, COLOUR))
            {
                rightIsValid = true;
            }
        }
        else if (board->getTile(row, col + 1)->getShape() == tile->getShape())
        {
            if (sixTileLineCheck(row, col, RIGHT, tile, SHAPE))
            {
                rightIsValid = true;
            }
        }
    } 
    else
    {
        rightIsValid = true;
    }
    return rightIsValid;
}

// If the below board position is empty or contains a valid tile it returns true 
bool GameEngine::checkDownIsValid(Tile* tile, int row, int col)
{
    bool downIsValid = false;
    // Checks entire line for either same colour or shape
    if (!(board->isEmpty(row + 1, col)))
    {
        if (board->getTile(row + 1, col)->getColour() == tile->getColour())
        {
            if (sixTileLineCheck(row, col, DOWN, tile, COLOUR))
            {
                downIsValid = true;
            }
        }
        else if (board->getTile(row + 1, col)->getShape() == tile->getShape())
        {
            if (sixTileLineCheck(row, col, DOWN, tile, SHAPE))
            {
                downIsValid = true;
            }

        }
    } 
    else
    {
        downIsValid = true;
    }    

    return downIsValid;
}

// If the above board position is empty or contains a valid tile it returns true 
bool GameEngine::checkUpIsValid(Tile* tile, int row, int col)
{
    bool upIsValid = false;
    // Checks entire line for either same colour or shape
    if (!(board->isEmpty(row - 1, col)))
    {
        if (board->getTile(row - 1, col)->getColour() == tile->getColour())
        {
            if (sixTileLineCheck(row, col, UP, tile, COLOUR))
            {
                upIsValid = true;
            }
        }
        else if (board->getTile(row - 1, col)->getShape() == tile->getShape())
        {
            if (sixTileLineCheck(row, col, UP, tile, SHAPE))
            {
                upIsValid = true;
            }
        }
    } 
    else
    {
        upIsValid = true;
    }
    return upIsValid;
}


/* 
 * Return true if line already has six tiles
 * Counts all tiles in a line depending on the direction
 * Loop stops when it reaches six tiles, has a duplicate, 
 * or reaches an empty board position
 */
bool GameEngine::sixTileLineCheck(int row, int col, char direction, 
                                  Tile* tile, string type)
{
    bool lineIsCompatible = false;
    int count = 1;
    bool stop = false;
    bool duplicateFound = false;
    bool illegalTile = false;
    if (direction == UP)
    {
        while ((count <= MAX_TILE_LINE && !stop))
        {
            if ((row - count >= board->getMaxRow() - board->getMaxRow()))
            {
                if (!board->isEmpty(row - count, col))
                {
                    // Checking duplicates on line
                    if (board->getTile(row - count, col)->isDuplicate(tile))
                    {
                        stop = true;
                        duplicateFound = true;
                    }

                    // Checking a line's tiles for the same colour or shape
                    if (type == COLOUR)
                    {
                        if (!board->getTile(row - count, col)->hasSameColour(tile))
                        {
                            illegalTile = true;
                            stop = true;
                        }
                    }

                    else if (type == SHAPE)
                    {
                        if (!board->getTile(row - count, col)->hasSameShape(tile))
                        {
                            illegalTile = true;
                            stop = true;
                        }   
                    }
                    
                    ++count;
                }
                else if (board->isEmpty(row - count, col))
                {
                    stop = true;
                }
            }
            else
            {
                stop = true;
            }
        }

    }
    else if (direction == DOWN)
    {
        while ((count <= MAX_TILE_LINE && !stop) )
        {
            if (row + count < board->getMaxRow())
            {
                if (!board->isEmpty(row + count, col))
                {

                    // Checking duplicates on line
                    if (board->getTile(row + count, col)->isDuplicate(tile))
                    {
                        stop = true;
                        duplicateFound = true;
                    }
                    if (type == COLOUR)
                    {
                        if (!board->getTile(row + count, col)->hasSameColour(tile))
                        {
                            illegalTile = true;
                            stop = true;
                        }
                    }

                    else if (type == SHAPE)
                    {
                        if (!board->getTile(row + count, col)->hasSameShape(tile))
                        {
                            illegalTile = true;
                            stop = true;
                        }   
                    }
                    
                    ++count;
                }
                else if (board->isEmpty(row + count, col))
                {
                    stop = true;
                }
            }

            else
            {
                stop = true;
            }

        }
    }
    else if (direction == LEFT)
    {
        while ((count <= MAX_TILE_LINE && !stop))
        {
            if ((col-count >= board->getMaxCol() - board->getMaxCol()))
            {
                if (!board->isEmpty(row, col - count))
                {

                    // Checking duplicates on line
                    if (board->getTile(row, col - count)->isDuplicate(tile))
                    {
                        stop = true;
                        duplicateFound = true;

                    }

                    if (type == COLOUR)
                    {
                        if (!board->getTile(row, col - count)->hasSameColour(tile))
                        {
                            illegalTile = true;
                            stop = true;
                        }
                    }

                    else if (type == SHAPE)
                    {
                        if (!board->getTile(row, col - count)->hasSameShape(tile))
                        {
                            illegalTile = true;
                            stop = true;
                        }   
                    }
                    
                    ++count;
                }
                else if(board->isEmpty(row, col - count))
                {
                    stop = true;
                }
            }
            else
            {
                stop = true;
            }
        }
    }
    else if (direction == RIGHT)
    {
        while ((count <= MAX_TILE_LINE && !stop))
        {
            if ((col+count < board->getMaxCol()))
            {
                if (!board->isEmpty(row, col + count))
                {

                    if (board->getTile(row, col + count)->isDuplicate(tile))
                    {
                        stop = true;
                        duplicateFound = true;
                    }

                    if (type == COLOUR)
                    {
                        if (!board->getTile(row, col + count)->hasSameColour(tile))
                        {
                            illegalTile = true;
                            stop = true;
                        }
                    }

                    else if (type == SHAPE)
                    {
                        if (!board->getTile(row, col + count)->hasSameShape(tile))
                        {
                            illegalTile = true;
                            stop = true;
                        }   
                    }  
                    
                    ++count;
                }
                else if (board->isEmpty(row, col + count))
                {
                    stop = true;
                }
            }
            else
            {
                stop = true;
            }

        }
    }
    
    /* 
     * If it reaches the end of the line where "stop" was
     * never called and there are 6 full tiles
     */
    if (count == MAX_TILE_LINE && !stop)
    {
        lineIsCompatible = false;
    }

    /* 
     * If "stop" was called because there's an empty spot and a 
     * duplicate was not found
     * The line should then be okay for the player to place a tile
     * If there are duplicates found, line is not compatible
     * If the rest of the line's colours and shapes are 
     * compatible then the placement is okay
     */
    if (stop && !duplicateFound && !illegalTile)
    {
        if (count == MAX_TILE_LINE)
        {
            current_round_score += count;
            cout << "\nQWIRKLE \n+6 BONUS POINTS\n" << endl;
        }
        lineIsCompatible = true;
        current_round_score += count;

    }
    return lineIsCompatible;
}

bool GameEngine::hasSurroundingTiles(int row, int col)
{
    bool hasTile = false;
    // directional check returns true if up, down, left or right is not empty
    if (!board->isEmpty(row - 1, col) || !board->isEmpty(row + 1, col) || 
        !board->isEmpty(row, col - 1) || !board->isEmpty(row, col + 1))
    {
        hasTile = true;
    }
    return hasTile;
}

/* 
 * Check that tile played share one attribute (color or shape) 
 * and that it is placed in the same line
 */
bool GameEngine::validTilePlacement(Tile* tile, int row, int col, 
                                    shared_ptr<Player> player)
{
    /* 
     * If it has found a tile that has a matching colour or shape, 
     * then isValid is FALSE
     */
    bool isValid = false;
    if (!edgeCase(row, col))
    {
        // Check that the tile placement is next to a tile
        if (hasSurroundingTiles(row, col))
        {
            // Resets score to add at start of each round
            current_round_score = 0;

            /* 
             * Makes sure the tile either has a same shape or 
             * colour and is valid
             */
            if (isSameColourOrShape(tile, row, col))
            {
                isValid = true;
            }
            //exit if conditions and return is Valid           
        }
    }
    else if (edgeCase(row, col))
    {
        current_round_score = 0;
        if (hasSameColourOrShapeEdgeCases(tile, row, col))
        {
            isValid = true;   
            //exit if conditions and rtn isValid
        }
    }

    if (!isValid)
    {
        cout << "\nInvalid placement\n" << endl;
    }
    return isValid;
}

bool GameEngine::edgeCase(int row, int col)
{
    bool flag = false;
    if (row == board->getMaxRow() - board->getMaxRow() || 
        row == board->getMaxRow() - 1 || 
        col == board->getMaxCol() - board->getMaxCol() || 
        col == board->getMaxCol() - 1)
    {
        flag = true;
    }

    return flag;
}

/* 
 * Returns true if up, down, left or right depending on 
 * the edge case has a tile of same colour or shape 
 */
bool GameEngine::hasSameColourOrShapeEdgeCases(Tile* tile, int row, int col)
{
    bool flag = false;
    // If it is Top most row, do not check up position
    if (row == board->getMaxRow()-board->getMaxRow() && !flag)
    {
        // If is in top left corner, then only check right and down position
        if(col == board->getMaxCol()-board->getMaxCol() && !flag)  
        {
            // Now we check is there is an adjacent tile to check against
            if (!board->isEmpty(row, col+1) || !board->isEmpty(row + 1, col))
            {
                if (checkRightIsValid(tile, row, col) && 
                    checkDownIsValid(tile, row, col))
                {
                    flag = true;
                }
            }

        }
        // If is in top right corner, then only check left and down position
        else if (col == board->getMaxCol() - 1 && !flag)
        {
            if (!board->isEmpty(row, col - 1) || !board->isEmpty(row + 1, col))
            {
                if (checkLeftIsValid(tile, row, col) && 
                    checkDownIsValid(tile, row, col))
                {
                    flag = true;
                }
            }

        }
        // Check left, down, right
        else if (!flag)
        {
            if (!board->isEmpty(row, col - 1) || 
                !board->isEmpty(row + 1, col) || 
                !board->isEmpty(row, col + 1))
            {
                if (checkLeftIsValid(tile, row, col) && 
                    checkDownIsValid(tile, row, col) && 
                    checkRightIsValid(tile, row, col))
                {
                    flag = true;
                }
            }

        }
    }
 
    // Bottom most row
    if (row == board->getMaxRow() - 1 && !flag)
    {
        // bottom left corner, check for up and right
        if (col == board->getMaxCol() - board->getMaxCol() && !flag)
        {
            if (!board->isEmpty(row - 1, col) || !board->isEmpty(row, col + 1))
            {
                if (checkUpIsValid(tile, row, col) && 
                    checkRightIsValid(tile, row, col))
                {
                    flag = true;
                }
            }

        }

        // bottom right corner, check up and left
        else if (col == board->getMaxCol() - 1 && !flag)
        {
            if (!board->isEmpty(row - 1, col) || !board->isEmpty(row, col - 1))
            {
                if (checkUpIsValid(tile, row, col) && 
                    checkLeftIsValid(tile, row, col))
                {
                    flag = true;
                }
            }

        }

        // do checks on left, above, right of tile
        else if (!flag)
        {
            if (!board->isEmpty(row, col - 1) || 
                !board->isEmpty(row - 1, col) || 
                !board->isEmpty(row, col + 1))
            {
                if (checkLeftIsValid(tile, row, col) && 
                    checkUpIsValid(tile, row, col) && 
                    checkRightIsValid(tile, row, col))
                {
                    flag = true;
                }
            }

        }
    }

    // Left most side
    if (col == board->getMaxCol() - board->getMaxCol() && !flag)
    {
        // top left corner, check down and right
        if (row == board->getMaxRow() - board->getMaxRow() && !flag)
        {
            if (!board->isEmpty(row + 1, col) || !board->isEmpty(row, col + 1))
            {
                if (checkDownIsValid(tile, row, col) && 
                    checkRightIsValid(tile, row, col))
                {
                    flag = true;
                }
            }
        }

        // bottom left corner, check up and right
        else if (row == board->getMaxRow() - 1 && !flag)
        {
            if (!board->isEmpty(row - 1, col) || !board->isEmpty(row, col + 1))
            {
                if (checkUpIsValid(tile, row, col) && 
                    checkRightIsValid(tile, row, col))
                {
                    flag = true;
                }
            }
        }
        
        // do checks on above, bottom, right of tile 
        else if (!flag)
        {
            if (!board->isEmpty(row - 1, col) || 
                !board->isEmpty(row + 1, col) || 
                !board->isEmpty(row, col + 1))
            {
                if (checkUpIsValid(tile, row, col) && 
                    checkRightIsValid(tile, row, col) && 
                    checkDownIsValid(tile, row, col))
                {
                    flag = true;
                }
            }

        }
    }

    // Right most side
    if (col == board->getMaxCol() - 1 && !flag)
    {
        // top right corner, check down and left
        if (row == board->getMaxRow() - board->getMaxRow()  && !flag)
        {
            if (!board->isEmpty(row + 1, col) || !board->isEmpty(row, col - 1))
            {
                if (checkDownIsValid(tile, row, col) && 
                    checkLeftIsValid(tile, row, col))
                {
                    flag = true;
                }
            }

        }

        // bottom right corner, check up and left
        else if (row == board->getMaxRow() - 1 && !flag)
        {   
            if (!board->isEmpty(row - 1, col) || !board->isEmpty(row, col - 1))
            {
                if (checkUpIsValid(tile, row, col) && 
                    checkLeftIsValid(tile, row, col))
                {
                    flag = true;
                }
            }

        }

       // do checks on left, above, bottom of tile 
        else if (!flag)
        {
            if (!board->isEmpty(row, col - 1) || 
                !board->isEmpty(row - 1, col) || 
                !board->isEmpty(row + 1, col))
            {
                if (checkLeftIsValid(tile, row, col) && 
                    checkUpIsValid(tile, row, col) && 
                    checkDownIsValid(tile, row, col))
                {
                    flag = true;
                }
            }

        }

    }

    return flag;
}

bool GameEngine::isSameColourOrShape(Tile* tile, int row, int col)
{
    bool returnFlag = false;

    if (checkUpIsValid(tile, row, col) && 
        checkDownIsValid(tile, row, col) && 
        checkLeftIsValid(tile, row, col) && 
        checkRightIsValid(tile, row, col))
    {
        returnFlag = true;
    }

    return returnFlag;
}

bool GameEngine::playerHasTile(string tileString, shared_ptr<Player> player)
{
    bool hasTile = false;

    int indexOfTileInHand = player->getHand()->getIndexOfTileStr(tileString);

    if (indexOfTileInHand != -1)
    {
        hasTile = true;
    }
    else
    {
        cout << "Player does not have tile" << endl;
    }

    return hasTile;
}

// Toggles the players turn based on the current players turn
void GameEngine::setOtherPlayerTurn()
{
    if (numbPlayers == TWO_PLAYERS) 
    { 
        if (playerList[FIRST_PLAYER]->getState()) 
        {
            playerList[FIRST_PLAYER]->setState(false);
            playerList[SECOND_PLAYER]->setState(true);
        }
        else { 
            playerList[FIRST_PLAYER]->setState(true);
            playerList[SECOND_PLAYER]->setState(false);
        }
    }
    else if (numbPlayers == THREE_PLAYERS) 
    {
        if (playerList[FIRST_PLAYER]->getState()) 
        { 
            playerList[FIRST_PLAYER]->setState(false);
            playerList[SECOND_PLAYER]->setState(true);
        }
        else if (playerList[SECOND_PLAYER]->getState())
        {
            playerList[SECOND_PLAYER]->setState(false);
            playerList[THIRD_PLAYER]->setState(true);
        }
        else 
        {
            playerList[THIRD_PLAYER]->setState(false);
            playerList[FIRST_PLAYER]->setState(true);
        }
    }
    else if (numbPlayers == FOUR_PLAYERS)
    {
        if (playerList[FIRST_PLAYER]->getState()) 
        { 
            playerList[FIRST_PLAYER]->setState(false);
            playerList[SECOND_PLAYER]->setState(true);
        }
        else if (playerList[SECOND_PLAYER]->getState())
        {
            playerList[SECOND_PLAYER]->setState(false);
            playerList[THIRD_PLAYER]->setState(true);
        }
        else if (playerList[THIRD_PLAYER]->getState())
        {
            playerList[THIRD_PLAYER]->setState(false);
            playerList[FOURTH_PLAYER]->setState(true);
        }
        else
        {
            playerList[FOURTH_PLAYER]->setState(false);
            playerList[FIRST_PLAYER]->setState(true);
        }
    }
}

void GameEngine::printRound(std::shared_ptr<Player> player)
{
    cout << player->getName() << ", it's your turn!" << endl;

    // Printing out all the players and their final score
    for (int i = 0; i < numbPlayers; ++i)
    {
        cout << "Score for " << playerList[i]->getName() << ": " 
                             << playerList[i]->getScore() << endl;
    }
    board->printBoard(colours); 
    cout << "\n";

    cout << "Your hand is" << endl;
    player->getHand()->print(colours);
    cout << "\n";
}

void GameEngine::giveHand(std::shared_ptr<Player> player)
{
    for(int i = 0; i < MAX_HAND; i++)
    {
        replenishHandTiles(player);
    }    
} 
