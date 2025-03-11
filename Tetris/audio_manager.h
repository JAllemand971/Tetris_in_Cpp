#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <SDL3_mixer/SDL_mixer.h>  // Include SDL_Mixer for audio functionality
#include <string>  // Include string for managing file paths and IDs
#include <map>  // Include map to store sound/music by ID
#include <iostream>  // Include iostream for potential logging

// Forward declarations of other classes
class Game;  
class Board;
class Piece;

// The AudioManager class handles all audio-related functionalities
class AudioManager {
public:
    // Constructor - initializes the audio manager
	AudioManager();

    // Destructor - cleans up the resources when the AudioManager is destroyed
    ~AudioManager();
    
    // Initializes SDL_Mixer and audio settings
    bool init();
    
    // Loads a music file into the music map with a given ID
    void loadMusic(const std::string& id, const std::string& filepath);
    
    // Loads a sound effect file into the sound map with a given ID
    void loadSound(const std::string& id, const std::string& filepath);
    
    // Plays the music associated with the given ID. Loops indefinitely by default
    void playMusic(const std::string& id, int loops = -1);
    
    // Plays the sound effect associated with the given ID
    void playSound(const std::string& id);
    
    // Stops any currently playing music
    void stopMusic();
    
    // Cleans up all loaded music and sound resources
    void cleanUp();
    
    // Loads all predefined sounds and music (e.g., background, move, rotate)
    void loadAllSounds();

private:
    Game* game;  // Pointer to the game object for interacting with the game logic
    Board* board;  // Pointer to the board object (if needed)
    Piece* piece;  // Pointer to the piece object (if needed)
    
    // Maps to store music and sound effect resources by their unique IDs
    std::map<std::string, Mix_Music*> musicMap;
    std::map<std::string, Mix_Chunk*> soundMap;
};

#endif

