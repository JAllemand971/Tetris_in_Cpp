#ifndef PIECE_H
#define PIECE_H

#include <SDL3/SDL.h>       // Include SDL library for graphics rendering
#include <vector>           // Include vector container for grid management
#include <map>  			// <map> is included to allow the use of std::map, which stores piece types and their corresponding shapes

// Forward declarations of classes that interact with Piece
class Game;  
class Board;
class AudioManager;

// Enum for the different types of Tetris pieces
enum class PieceType { I, O, T, L, J, S, Z };

class Piece {
public:
    // Constructor: Initializes a Piece with its position, type, and associated game and board
	Piece(Game* g, Board* b, int x, int y, PieceType type);
    
    // Default constructor (not used in the code, but available)
	Piece();
    
    // Destructor
    ~Piece();
    
    // Draw the piece on the screen using SDL renderer
    void draw(SDL_Renderer* renderer);

    // Move the piece by dx and dy
    void movePiece(int dx, int dy);
    
    // Rotate the piece 90 degrees
    void rotatePiece();
    
    // Helper function to update the block positions based on the shape
    void updateBlocks();
    
    // Get the current blocks that make up the piece
	const std::vector<std::pair<int, int>>& getBlock() const;
    
    // Get the color of the piece
	SDL_Color getColor() const;

    // Set the board the piece is associated with
	void setBoard(Board* b) { board = b; }

    // Getters for piece's position
	int getPieceX() const { return pieceX; }
    int getPieceY() const { return pieceY; }
    
    // Static map to store piece shapes for each type (I, O, T, L, J, S, Z)
    static std::map<PieceType, std::vector<std::vector<int>>> pieceShapes;

private:
    // Pointer to the associated game instance
	Game* game;

    // Pointer to the associated board instance
    Board* board = nullptr;

    // Pointer to the audio manager (not used in this header, but referenced in the piece class)
    AudioManager* audio;
    
    // Piece's current position on the board
    int pieceX, pieceY;

    // The type of the piece (I, O, T, L, J, S, Z)
    PieceType type;

    // Shape of the piece, represented as a 2D vector (block structure)
    std::vector<std::vector<int>> shape;

    // The color of the piece
    SDL_Color color;

    // A list of blocks (as pairs of coordinates) that make up the piece
    std::vector<std::pair<int, int>> blocks;
};

#endif

