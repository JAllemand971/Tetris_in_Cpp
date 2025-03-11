// piece.h

#ifndef PIECE_H
#define PIECE_H

#include <SDL3/SDL.h>
#include <vector>
#include <map>

class Game;  
class Board;
class AudioManager;

enum class PieceType { I, O, T, L, J, S, Z };

class Piece {
public:
	Piece(Game* g, Board* b, int x, int y, PieceType type);
	Piece();
    ~Piece();
    
    void draw(SDL_Renderer* renderer);
    void movePiece(int dx, int dy);
    void rotatePiece();
	const std::vector<std::pair<int, int>>& getBlock() const;
	SDL_Color getColor() const;

	void setBoard(Board* b) { board = b; } // Sets the board pointer (to interact with the game board)
	
	int getPieceX() const { return pieceX; }
    int getPieceY() const { return pieceY; }
    
    static std::map<PieceType, std::vector<std::vector<int>>> pieceShapes;

private:
	Game* game;
    Board* board = nullptr;
    AudioManager* audio;
    
    int pieceX, pieceY;
    PieceType type;
    std::vector<std::vector<int>> shape;
    SDL_Color color;
    std::vector<std::pair<int, int>> blocks;
    
    void updateBlocks();
    
};

#endif

