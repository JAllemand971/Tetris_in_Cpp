// board.cpp

#include "game.h"
#include "board.h"
#include "piece.h"
#include "audio_manager.h"


Board::Board(Game* g, Piece* p, AudioManager* a, int width, int height) : game(g), piece(p), audio(a), width(width), height(height) {
    grid.resize(height, std::vector<SDL_Color>(width, {0, 0, 0, 0}));
    linesCleared = 0;
}


Board::~Board() {
}

void Board::draw(SDL_Renderer* renderer) {
		
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            SDL_FRect rect = {static_cast<float>(x * CELL_SIZE), static_cast<float>(y * CELL_SIZE), static_cast<float>(CELL_SIZE), static_cast<float>(CELL_SIZE)};

            if (grid[y][x].r != 0 || grid[y][x].g != 0 || grid[y][x].b != 0) {
                SDL_SetRenderDrawColor(renderer, grid[y][x].r, grid[y][x].g, grid[y][x].b, 255);
                SDL_RenderFillRect(renderer, &rect);
            }

            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
            SDL_RenderRect(renderer, &rect);
        }
    }
}

bool Board::isFullLine(int y){
    for(int x = 0; x < width; x++){
        if(isCellEmpty(x, y)){
            return false;
        }
    }
    return true;
}

void Board::clearFullLines() {
    int lines = 0;
    for(int y = height -1; y >= 0; y--){
        if(isFullLine(y)){
            lines++;
            
            for(int row = y; row > 0; row--){
                for(int x = 0; x < width; x++){
                    grid[row][x] = grid[row - 1][x];
                }
            }
            
            for(int x = 0; x < width; x++){
                grid[0][x] = {0, 0, 0, 0};
            }
            y++; 
        }
    }
    
    linesCleared += lines;
    if(lines == 1){
        game->setScore(game->getScore() + 100);
        audio->playSound("line");
    }
    else if(lines == 2) {
        game->setScore(game->getScore() + 300);
    }
    else if(lines == 3){
        game->setScore(game->getScore() + 500);
    } 
    else if(lines == 4){
        game->setScore(game->getScore() + 800);
	}
}


bool Board::isCellEmpty(int x, int y) {
    return grid[y][x].r == 0 && grid[y][x].g == 0 && grid[y][x].b == 0;
}

bool Board::isValid(int x, int y){
	return x >= 0 && x < width && y >= 0 && y < height;
}

void Board::setCell(int x, int y, SDL_Color color) {
    if (isValid(x, y)) {
        grid[y][x] = color;
    }
}

SDL_Color Board::getCell(int x, int y) {
    if (!isValid(x, y)) {
        return {0, 0, 0, 0};
    }
    return grid[y][x];
}
