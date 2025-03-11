// board.cpp

#include "game.h"       // Includes the Game class which handles the game logic
#include "board.h"      // Includes the Board class which represents the game grid
#include "piece.h"      // Includes the Piece class which represents the Tetris pieces
#include "audio_manager.h" // Includes the AudioManager class for managing sounds and music

// Constructor: Initializes the board with a grid, game, piece, and audio manager
Board::Board(Game* g, Piece* p, AudioManager* a, int width, int height) : game(g), piece(p), audio(a), width(width), height(height) {
    // Initialize the grid to the given width and height, filled with transparent black cells
    grid.resize(height, std::vector<SDL_Color>(width, {0, 0, 0, 0}));
    linesCleared = 0;  // Initialize the cleared lines counter
}

// Destructor: Cleans up resources (empty in this case)
Board::~Board() {
}

// Draw the grid: Render each cell in the grid with its color, along with grid borders
void Board::draw(SDL_Renderer* renderer) {
    for (int y = 0; y < height; ++y) { // Loop through each row
        for (int x = 0; x < width; ++x) { // Loop through each column
            SDL_FRect rect = {static_cast<float>(x * CELL_SIZE), static_cast<float>(y * CELL_SIZE), static_cast<float>(CELL_SIZE), static_cast<float>(CELL_SIZE)};

            // If the cell is not empty (color is not black), draw it
            if (grid[y][x].r != 0 || grid[y][x].g != 0 || grid[y][x].b != 0) {
                SDL_SetRenderDrawColor(renderer, grid[y][x].r, grid[y][x].g, grid[y][x].b, 255); // Set the cell color
                SDL_RenderFillRect(renderer, &rect); // Fill the rectangle with the color
            }

            // Draw the grid border (light gray)
            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
            SDL_RenderRect(renderer, &rect);
        }
    }
}

// Check if a specific line is full (no empty cells)
bool Board::isFullLine(int y) {
    for (int x = 0; x < width; x++) {
        if (isCellEmpty(x, y)) { // If any cell is empty, return false
            return false;
        }
    }
    return true; // All cells are filled
}

// Clear full lines: If a line is full, shift all rows above it down and clear the top row
void Board::clearFullLines() {
    int lines = 0; // Variable to count the number of full lines
    for (int y = height - 1; y >= 0; y--) { // Start checking from the bottom row
        if (isFullLine(y)) { // If the line is full
            lines++; // Increment the line count
            
            // Shift all rows above this one down
            for (int row = y; row > 0; row--) {
                for (int x = 0; x < width; x++) {
                    grid[row][x] = grid[row - 1][x]; // Move the row down
                }
            }

            // Clear the top row
            for (int x = 0; x < width; x++) {
                grid[0][x] = {0, 0, 0, 0}; // Set each cell to empty (black)
            }
            y++; // Skip checking the row that was just shifted down
        }
    }

    // Update the score based on the number of lines cleared
    linesCleared += lines;
    if (lines == 1) {
        game->setScore(game->getScore() + 100);
        audio->playSound("line"); // Play a sound for clearing a single line
    }
    else if (lines == 2) {
        game->setScore(game->getScore() + 300);
        audio->playSound("line"); // Play a sound for clearing two lines
    }
    else if (lines == 3) {
        game->setScore(game->getScore() + 500);
        audio->playSound("line"); // Play a sound for clearing three lines
    } 
    else if (lines == 4) {
        game->setScore(game->getScore() + 800);
        audio->playSound("4lines"); // Play a special sound for clearing four lines
    }
}

// Check if a specific cell is empty (color is transparent black)
bool Board::isCellEmpty(int x, int y) {
    return grid[y][x].r == 0 && grid[y][x].g == 0 && grid[y][x].b == 0;
}

// Check if a given position is valid (within the grid bounds)
bool Board::isValid(int x, int y) {
    return x >= 0 && x < width && y >= 0 && y < height;
}

// Set the color of a specific cell
void Board::setCell(int x, int y, SDL_Color color) {
    if (isValid(x, y)) {
        grid[y][x] = color; // Set the cell to the given color
    }
}

// Get the color of a specific cell (returns black if invalid position)
SDL_Color Board::getCell(int x, int y) {
    if (!isValid(x, y)) {
        return {0, 0, 0, 0}; // Return black for an invalid cell
    }
    return grid[y][x]; // Return the color of the cell
}

