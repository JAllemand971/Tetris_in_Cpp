// main.cpp

#include "game.h"   // Includes the Game class, which manages the game logic
#include "board.h"  // Includes the Board class, which represents the game board
#include "piece.h"  // Includes the Piece class, which represents the game pieces

int main() {
    // Create a Game object
    Game game;
    
    // Start the game loop
    game.start();

    // Return 0 to indicate successful execution
    return 0;
}

