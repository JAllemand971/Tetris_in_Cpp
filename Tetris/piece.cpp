// piece.cpp

#include "game.h"       // Includes the Game class which handles the game logic
#include "board.h"      // Includes the Board class which represents the game grid
#include "piece.h"      // Includes the Piece class which represents the Tetris pieces

// A map to store the shape of each piece type (I, O, T, L, J, S, Z)
std::map<PieceType, std::vector<std::vector<int>>> Piece::pieceShapes = {
    { PieceType::I, {{1, 1, 1, 1}} }, // I piece (4 blocks in a line)
    { PieceType::O, {{1, 1}, {1, 1}} }, // O piece (2x2 square)
    { PieceType::T, {{0, 1, 0}, {1, 1, 1}} }, // T piece (T shape)
    { PieceType::L, {{0, 0, 1}, {1, 1, 1}} }, // L piece (L shape)
    { PieceType::J, {{1, 0, 0}, {1, 1, 1}} }, // J piece (reverse L shape)
    { PieceType::S, {{0, 1, 1}, {1, 1, 0}} }, // S piece (S shape)
    { PieceType::Z, {{1, 1, 0}, {0, 1, 1}} }  // Z piece (Z shape)
};

// Constructor for the Piece class
Piece::Piece(Game* g, Board* b, int x, int y, PieceType type) 
    : game(g), board(b), pieceX(x), pieceY(y), type(type), shape(pieceShapes[type]) {

    // Assign a color based on the piece type
    switch (type) {
        case PieceType::I: color = {0, 255, 255, 255}; break;
        case PieceType::O: color = {255, 255, 0, 255}; break;
        case PieceType::T: color = {128, 0, 128, 255}; break;
        case PieceType::L: color = {255, 165, 0, 255}; break;
        case PieceType::J: color = {0, 0, 255, 255}; break;
        case PieceType::S: color = {0, 255, 0, 255}; break;
        case PieceType::Z: color = {255, 0, 0, 255}; break;
    }

    updateBlocks(); // Update the blocks' positions based on the shape
}

// Destructor
Piece::~Piece() {
}

// Updates the blocks of the piece based on the current shape
void Piece::updateBlocks() {
    blocks.clear(); // Clear previous blocks
    // Iterate over the shape and create blocks where there is a 1 in the shape
    for (int i = 0; i < shape.size(); ++i) {
        for (int j = 0; j < shape[i].size(); ++j) {
            if (shape[i][j] == 1) {
                blocks.push_back({i, j}); // Store the block's relative position
            }
        }
    }
}

// Draw the piece on the screen using the provided SDL renderer
void Piece::draw(SDL_Renderer* renderer) {
    int blockSize = Board::CELL_SIZE; // Size of each block (from the board class)

    // Iterate over the blocks and draw each one
    for (const auto& block : blocks) {
        SDL_FRect rect = { 
            static_cast<float>((pieceX + block.first) * blockSize), 
            static_cast<float>((pieceY + block.second) * blockSize), 
            static_cast<float>(blockSize), 
            static_cast<float>(blockSize) 
        };
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a); // Set the color
        SDL_RenderFillRect(renderer, &rect); // Fill the rectangle with the piece's color
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set the border color
        SDL_RenderRect(renderer, &rect); // Draw the border of the block
    }
}

// Move the piece by a certain amount (dx, dy), if the move is valid (no collision)
void Piece::movePiece(int dx, int dy) {
    if (!game->checkCollision(dx, dy)) {  // Check for collisions
        pieceX += dx; // Update the piece's X position
        pieceY += dy; // Update the piece's Y position
        updateBlocks(); // Update the blocks' positions based on the new coordinates
    }
}

// Rotate the piece and check if the new position is valid
void Piece::rotatePiece() {
    // Create a new shape that is the rotated version of the current shape
    std::vector<std::vector<int>> rotatedShape(shape[0].size(), std::vector<int>(shape.size()));
    std::vector<std::pair<int, int>> newBlocks;

    // Rotate the shape (90 degrees clockwise)
    for (int i = 0; i < shape.size(); ++i) {
        for (int j = 0; j < shape[i].size(); ++j) {
            rotatedShape[shape[i].size() - 1 - j][i] = shape[i][j];
        }
    }

    // Check if the rotated piece fits within the board and doesn't collide
    for (int i = 0; i < rotatedShape.size(); ++i) {
        for (int j = 0; j < rotatedShape[i].size(); ++j) {
            if (rotatedShape[i][j] == 1) {
                int newX = pieceX + i;
                int newY = pieceY + j;
                
                // Check if the new position is valid (within bounds and not colliding)
                if (newX < 0 || newX >= board->getWidth() ||
                    newY < 0 || newY >= board->getHeight() || 
                    !board->isCellEmpty(newX, newY)) {
                    return; // If invalid, return without rotating
                }

                newBlocks.push_back({i, j}); // Store the new block's position
            }
        }
    }

    // If valid, update the shape and blocks with the rotated version
    shape = rotatedShape;
    blocks = newBlocks;
}

// Returns the blocks of the piece (used to access their positions)
const std::vector<std::pair<int, int>>& Piece::getBlock() const {
    return blocks;
}

// Returns the color of the piece
SDL_Color Piece::getColor() const {
    return color;
}

