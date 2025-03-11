// game.h

#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <vector>
#include <iostream> 
#include <cstdlib>
#include <ctime>
#include <string>

class Board;
class Piece;
class AudioManager;

class Game {
public:

    Game();
    ~Game();
    
    void start();
    void handleEvents();
    void update();
    void render(SDL_Renderer* renderer);
    void spawnPiece();
    bool checkCollision(int dx, int dy);
    void displayErrorMessage(const std::string& message);
    void displayText(SDL_Renderer* renderer, std::string t, float x, float y, SDL_Color color, int fontSize);
    void resetGame();
    void initAudio();
    void playMusic();
    void playSound(Mix_Chunk* sound);
    void stopMusic();    
    
    void fillGridAnimation(SDL_Renderer* renderer);
    
    int getScore() const { return score; }
    void setScore(int x) { score = x; }
    
    Mix_Music* backgroundMusic;
    Mix_Chunk* moveSound;
    Mix_Chunk* rotateSound;
    Mix_Chunk* clearLineSound;
    
private:
    Board* board;
    Piece* piece;
	AudioManager* audio;    
    bool gameOver;
    bool run;
    
    int score;
    int win_Width = 800;
	int win_Height = 800;
	int grid_Width = 10;
	int grid_Height = 20;
	float scoreX = grid_Width * 30 + 20;
    float scoreY = 20;
    
    int fillProgress;
	bool isFilling;
	bool once;
	
};

#endif
