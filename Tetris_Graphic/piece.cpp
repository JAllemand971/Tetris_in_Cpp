// piece.cpp

#include "game.h"
#include "board.h"
#include "piece.h"

std::map<PieceType, std::vector<std::vector<int>>> Piece::pieceShapes = {
    { PieceType::I, {{1, 1, 1, 1}} },
    { PieceType::O, {{1, 1}, {1, 1}} },
    { PieceType::T, {{0, 1, 0}, {1, 1, 1}} },
    { PieceType::L, {{0, 0, 1}, {1, 1, 1}} },
    { PieceType::J, {{1, 0, 0}, {1, 1, 1}} },
    { PieceType::S, {{0, 1, 1}, {1, 1, 0}} },
    { PieceType::Z, {{1, 1, 0}, {0, 1, 1}} }
};

Piece::Piece(Game* g, Board* b, int x, int y, PieceType type) 
    : game(g), board(b), pieceX(x), pieceY(y), type(type), shape(pieceShapes[type]) {

    switch (type) {
        case PieceType::I: color = {0, 255, 255, 255}; break;
        case PieceType::O: color = {255, 255, 0, 255}; break;
        case PieceType::T: color = {128, 0, 128, 255}; break;
        case PieceType::L: color = {255, 165, 0, 255}; break;
        case PieceType::J: color = {0, 0, 255, 255}; break;
        case PieceType::S: color = {0, 255, 0, 255}; break;
        case PieceType::Z: color = {255, 0, 0, 255}; break;
    }

    updateBlocks();
}

Piece::~Piece() {
}

void Piece::updateBlocks() {
    blocks.clear();
    for (int i = 0; i < shape.size(); ++i) {
        for (int j = 0; j < shape[i].size(); ++j) {
            if (shape[i][j] == 1) {
                blocks.push_back({i, j});
            }
        }
    }
}

void Piece::draw(SDL_Renderer* renderer) {
    int blockSize = Board::CELL_SIZE;

    for (const auto& block : blocks) {
        SDL_FRect rect = { static_cast<float>((pieceX + block.first) * blockSize), static_cast<float>((pieceY + block.second) * blockSize), static_cast<float>(blockSize), static_cast<float>(blockSize) };
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderRect(renderer, &rect);
    }
}

void Piece::movePiece(int dx, int dy) {
    if (!game->checkCollision(dx, dy)) {  
        pieceX += dx;
        pieceY += dy;
        updateBlocks();
    }
}

void Piece::rotatePiece() {
    std::vector<std::vector<int>> rotatedShape(shape[0].size(), std::vector<int>(shape.size()));
    std::vector<std::pair<int, int>> newBlocks;
    
    for (int i = 0; i < shape.size(); ++i) {
        for (int j = 0; j < shape[i].size(); ++j) {
            rotatedShape[shape[i].size() - 1 - j][i] = shape[i][j];
        }
    }
    
    for (int i = 0; i < rotatedShape.size(); ++i) {
        for (int j = 0; j < rotatedShape[i].size(); ++j) {
            if (rotatedShape[i][j] == 1) {
                int newX = pieceX + i;
                int newY = pieceY + j;
                
                if (newX < 0 || newX >= board->getWidth() ||
					newY < 0 || newY >= board->getHeight() || 
                    !board->isCellEmpty(newX, newY)) {
                    return;
                }
                
                newBlocks.push_back({i, j});
            }
        }
    }
    
    shape = rotatedShape;
    blocks = newBlocks;
}

const std::vector<std::pair<int, int>>& Piece::getBlock() const {
    return blocks;
}

SDL_Color Piece::getColor() const {
    return color;
}

