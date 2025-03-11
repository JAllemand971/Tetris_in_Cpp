// board.h

#ifndef BOARD_H
#define BOARD_H

#include <SDL3/SDL.h>         // Include SDL library for graphics rendering
#include <vector>              // Include vector container for grid management
#include <iostream>            // Include for output/logging

// Forward declarations to avoid circular dependencies
class Game;
class Piece;
class AudioManager;

class Board {
public:
    static const int CELL_SIZE = 30; // Size of each cell in the grid
    
    // Constructor: Initializes the board with a game, piece, audio manager, width, and height
    Board(Game* g, Piece* p, AudioManager* a, int width = 10, int height = 20);
    Board(); // Default constructor for when initialization doesn't require parameters
    ~Board(); // Destructor
    
    // Methods for drawing the grid, checking cell validity, and handling full lines
    void draw(SDL_Renderer* renderer);  // Renders the board on the screen
    void setCell(int x, int y, SDL_Color color); // Sets the color of a specific cell
    bool isValid(int x, int y);  // Checks if a given cell is within the grid bounds
    bool isCellEmpty(int x, int y); // Checks if a specific cell is empty
    bool isFullLine(int y);   // Checks if a given line (row) is full
    void clearFullLines();    // Clears all full lines and updates the grid
    SDL_Color getCell(int x, int y); // Gets the color of a specific cell
    
    // Getter methods for the board's width and height
    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    int width, height;               // The dimensions of the grid
    int linesCleared;                // Counter for the number of lines cleared
    std::vector<std::vector<SDL_Color>> grid; // 2D grid of SDL_Color to store each cell's color
    Game* game;                      // Pointer to the Game instance (to interact with game logic)
    Piece* piece;                    // Pointer to the current Piece instance
    AudioManager* audio;             // Pointer to the AudioManager for sound effects
};

#endif

