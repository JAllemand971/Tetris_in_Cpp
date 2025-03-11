// audio_manager.h

#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <SDL3_mixer/SDL_mixer.h>
#include <string>
#include <map>

class Game;  
class Board;
class Piece;

class AudioManager {
public:
	AudioManager();
    ~AudioManager();
    
    bool init();
    void loadMusic(const std::string& id, const std::string& filepath);
    void loadSound(const std::string& id, const std::string& filepath);
    void playMusic(const std::string& id, int loops = -1);
    void playSound(const std::string& id);
    void stopMusic();
    void cleanUp();
    void loadAllSounds();

private:    
    Game* game;
    Board* board;
    Piece* piece;
    
    std::map<std::string, Mix_Music*> musicMap;
    std::map<std::string, Mix_Chunk*> soundMap;
};

#endif
