//
// Created by shawn on 7/26/2025.
//
#include "SoundPack.h"

std::vector<Sound *>* SoundPack::addPack(const std::string& name) {
    packs[name] = new std::vector<Sound*>();
    return packs[name];
}

bool SoundPack::getPack(const std::string& packName, std::vector<Sound *>** thePack) {

    if(!packs.contains(packName)) {
        return false;
    }

    *thePack = packs[packName];

    return true;
}

std::unordered_map<std::string, std::vector<Sound *>*> SoundPack::getPacks() {
    return packs;
}

bool SoundPack::removePack(const std::string &name) {
    std::vector<Sound*>* sounds;
    if(!getPack(name, &sounds) || !sounds) {
        return false;
    }

    //delete each sound
    for(Sound* sound : *sounds) {
        delete sound;
    }

    //delete the vector
    delete sounds;

    //get rid of this item
    packs.erase(name);

    return true;
}

bool SoundPack::setVolume(const std::string &name, const float &newVolume) {
    std::vector<Sound*>* sounds;
    if(!getPack(name, &sounds) || !sounds) {
        return false;
    }

    for(Sound* sound : *sounds) {
        sound->setVolume(newVolume);
    }

    return true;
}





