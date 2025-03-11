// board.h

#ifndef BOARD_H
#define BOARD_H

#include <SDL3/SDL.h>
#include <vector>
#include <iostream>

class Game;
class Piece;
class AudioManager;

class Board {
public:
    static const int CELL_SIZE = 30;
	
	Board(Game* g, Piece* p, AudioManager* a, int width = 10, int height = 20);
    Board();
    ~Board();
    
    void draw(SDL_Renderer* renderer);
    void setCell(int x, int y, SDL_Color color);
    bool isValid(int x, int y);
    bool isCellEmpty(int x, int y);
    bool isFullLine(int y);
    void clearFullLines();
    SDL_Color getCell(int x, int y);  
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    
private:
	int width, height;
	int linesCleared;
    std::vector<std::vector<SDL_Color>> grid;
    Game* game;
    Piece* piece;
    AudioManager* audio;
};

#endif
