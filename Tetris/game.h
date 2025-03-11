// game.h

#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>             // Include SDL library for graphics and events
#include <SDL3_ttf/SDL_ttf.h>     // Include SDL_ttf library for text rendering
#include <SDL3_mixer/SDL_mixer.h> // Include SDL_mixer library for audio

#include <vector>     // Include vector for dynamic array usage
#include <iostream>   // Include input/output stream for debugging
#include <cstdlib>    // Include for random number generation
#include <ctime>      // Include for time-based functions
#include <string>     // Include for string manipulation

class Board;            // Forward declaration of Board class
class Piece;            // Forward declaration of Piece class
class AudioManager;     // Forward declaration of AudioManager class

// Game class encapsulates the main logic of the Tetris-like game
class Game {
public:
    // Constructor: Initializes the game state, pieces, board, audio, etc.
    Game();
    
    // Destructor: Cleans up dynamically allocated resources
    ~Game();

    // Starts the game loop and initializes SDL components
    void start();
    
    // Handles user input events (key presses, window events)
    void handleEvents();
    
    // Updates the game state (moves the piece, checks for collisions)
    void update();
    
    // Renders the game board, pieces, and other game elements
    void render(SDL_Renderer* renderer);
    
    // Spawns a new piece at the top of the board
    void spawnPiece();
    
    // Checks if moving the piece by dx and dy will cause a collision
    bool checkCollision(int dx, int dy);
    
    // Displays an error message box with the specified text
    void displayErrorMessage(const std::string& message);
    
    // Renders text (score, messages) on the screen at the given position
    void displayText(SDL_Renderer* renderer, std::string text, float x, float y, SDL_Color color, int fontSize);
    
    // Resets the game to its initial state
    void resetGame();
    
    // Initializes the audio components (music and sound effects)
    void initAudio();
    
    // Plays the background music for the game
    void playMusic();
    
    // Plays the given sound effect (move, rotate, etc.)
    void playSound(Mix_Chunk* sound);
    
    // Stops the background music
    void stopMusic();
    
    // Updates the game speed based on the current score
    void updateSpeed();
    
    // Animates the grid filling when the game is over (for game over screen)
    void fillGridAnimation(SDL_Renderer* renderer);

    // Getter for score
    int getScore() const { return score; }

    // Setter for score
    void setScore(int x) { score = x; }
    
private:
    // Pointers to the game board and current piece
    Board* board;
    Piece* piece;
    
    // Pointer to the audio manager
    AudioManager* audio; 
    
    // Game state variables
    bool gameOver;  // Flag indicating if the game is over
    bool run;       // Flag for the game loop

    // Game-specific variables
    int score;      // Current score
    int speed;      // Current game speed (affects how fast pieces fall)
    
    // Window dimensions and grid size
    int win_Width = 800;
    int win_Height = 800;
    int grid_Width = 10;
    int grid_Height = 20;
    
    // Score display position
    float scoreX = grid_Width * 30 + 20;
    float scoreY = 20;

    // Animation-related variables for the game-over screen
    int fillProgress;  // Progress of the grid-filling animation
    bool isFilling;    // Flag for the grid-filling animation
    bool once;         // Flag to ensure the game over sound plays only once
};

#endif

