#include "game.h"
#include "audio_manager.h"

// Constructor: Initializes the AudioManager object.
AudioManager::AudioManager() {
}

// Destructor: Cleans up allocated audio resources.
AudioManager::~AudioManager() {
    cleanUp();
}

// Initializes the audio system using SDL_Mixer.
bool AudioManager::init() {
    SDL_AudioSpec desiredSpec;
    SDL_zero(desiredSpec);  // Clears the SDL_AudioSpec structure to zero.

    desiredSpec.freq = 44100;           // Sets the audio frequency to 44.1 kHz.
    desiredSpec.format = SDL_AUDIO_S16; // Sets the audio format to 16-bit signed.
    desiredSpec.channels = 2;           // Sets the number of channels to stereo.

    // Opens the audio device with the specified settings.
    if (!Mix_OpenAudio(0, reinterpret_cast<const SDL_AudioSpec*>(&desiredSpec))) {
        game->displayErrorMessage("Error initializing SDL_Mixer");
        return false;
    }
    return true;
}

// Loads all necessary sound effects and background music files.
void AudioManager::loadAllSounds() {
    loadMusic("background", "Sound_Effects/background.wav");
    loadSound("move", "Sound_Effects/move.wav");
    loadSound("rotate", "Sound_Effects/rotate.wav");
    loadSound("line", "Sound_Effects/line.wav");
    loadSound("4lines", "Sound_Effects/4_lines.wav");
    loadSound("pieceLanded", "Sound_Effects/piece_landed.wav");
    loadSound("gameover", "Sound_Effects/game_over.wav");
}

// Loads a music file and stores it in the music map.
void AudioManager::loadMusic(const std::string& id, const std::string& filepath) {
    Mix_Music* music = Mix_LoadMUS(filepath.c_str());  // Loads the music file.
    if (!music) {
        game->displayErrorMessage("Error loading music: " + filepath);
    } else {
        musicMap[id] = music;  // Stores the loaded music in the map.
    }
}

// Loads a sound effect file and stores it in the sound map.
void AudioManager::loadSound(const std::string& id, const std::string& filepath) {
    Mix_Chunk* sound = Mix_LoadWAV(filepath.c_str());  // Loads the sound effect file.
    if (!sound) {
        game->displayErrorMessage("Error loading sound: " + filepath);
    } else {
        soundMap[id] = sound;  // Stores the loaded sound effect in the map.
    }
}

// Plays a music track with a given ID. Can loop multiple times.
void AudioManager::playMusic(const std::string& id, int loops) {
    if (musicMap.find(id) != musicMap.end()) {
        Mix_PlayMusic(musicMap[id], loops);
    }
}

// Plays a sound effect with a given ID.
void AudioManager::playSound(const std::string& id) {
    if (soundMap.find(id) != soundMap.end()) {
        Mix_PlayChannel(-1, soundMap[id], 0);
    }
}

// Stops the currently playing music.
void AudioManager::stopMusic() {
    Mix_HaltMusic();
}

// Cleans up loaded audio resources to prevent memory leaks.
void AudioManager::cleanUp() {
    // Free all loaded music tracks.
    for (auto& pair : musicMap) {
        Mix_FreeMusic(pair.second);
    }
    // Free all loaded sound effects.
    for (auto& pair : soundMap) {
        Mix_FreeChunk(pair.second);
    }
    // Closes the audio system.
    Mix_CloseAudio();
}

