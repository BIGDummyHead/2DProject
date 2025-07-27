//
// Created by shawn on 7/26/2025.
//

#ifndef SOUNDPACK_H
#define SOUNDPACK_H
#include <unordered_map>
#include <string>
#include <vector>

#include "Sound.h"

//A pack of sounds to use in the AudioManager class.
class SoundPack {
private:
    std::unordered_map<std::string, std::vector<Sound*>*> packs;

public:
    //Get a pack via a name, returns true if it exist, and thePack is set.
    bool getPack(const std::string& packName, std::vector<Sound*>** thePack);
    //Adds a new pack, throws an error if it already exist.
    std::vector<Sound*>* addPack(const std::string& name);

    std::unordered_map<std::string, std::vector<Sound*>*> getPacks();

    bool removePack(const std::string& name);

    bool setVolume(const std::string& name, const float& newVolume);
};

#endif //SOUNDPACK_H
