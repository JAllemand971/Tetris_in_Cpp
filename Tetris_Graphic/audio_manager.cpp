// audio_manager.cpp

#include "audio_manager.h"
#include "game.h"

#include <iostream>


AudioManager::AudioManager() {
}

bool AudioManager::init() {
    SDL_AudioSpec desiredSpec;
    SDL_zero(desiredSpec);

    desiredSpec.freq = 44100;
    desiredSpec.format = SDL_AUDIO_S16; 
    desiredSpec.channels = 2;

    if (!Mix_OpenAudio(0, reinterpret_cast<const SDL_AudioSpec*>(&desiredSpec))) {
        game->displayErrorMessage("Error in intializing SDL_Mixer");
        return false;
    }
    return true;
}

void AudioManager::loadAllSounds() {
    loadMusic("background", "Sound_Effects/background.wav");
    loadSound("move", "Sound_Effects/move.wav");
    loadSound("rotate", "Sound_Effects/rotate.wav");
    loadSound("line", "Sound_Effects/line.wav");
    loadSound("pieceLanded", "Sound_Effects/piece_landed.wav");
    loadSound("gameover", "Sound_Effects/game_over.wav");
}

void AudioManager::loadMusic(const std::string& id, const std::string& filepath) {
    Mix_Music* music = Mix_LoadMUS(filepath.c_str());
    if (!music) {
        game->displayErrorMessage("Error in loading sound: ");
    } else {
        musicMap[id] = music;
    }
}

void AudioManager::loadSound(const std::string& id, const std::string& filepath) {
    Mix_Chunk* sound = Mix_LoadWAV(filepath.c_str());
    if (!sound) {
        game->displayErrorMessage("Error in loading sound: ");
    } else {
        soundMap[id] = sound;
    }
}

void AudioManager::playMusic(const std::string& id, int loops) {
    if (musicMap.find(id) != musicMap.end()) {
        Mix_PlayMusic(musicMap[id], loops);
    }
}

void AudioManager::playSound(const std::string& id) {
    if (soundMap.find(id) != soundMap.end()) {
        Mix_PlayChannel(-1, soundMap[id], 0);
    }
}

void AudioManager::stopMusic() {
    Mix_HaltMusic();
}

void AudioManager::cleanUp() {
    for (auto& pair : musicMap) {
        Mix_FreeMusic(pair.second);
    }
    for (auto& pair : soundMap) {
        Mix_FreeChunk(pair.second);
    }
    Mix_CloseAudio();
}

AudioManager::~AudioManager() {
    cleanUp();
}

