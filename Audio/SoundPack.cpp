//
// Created by shawn on 7/26/2025.
//
#include "SoundPack.h"

std::vector<Sound *> SoundPack::addPack(const std::string& name) {
    packs[name] = std::vector<Sound*>();
    return packs[name];
}

bool SoundPack::getPack(const std::string& packName, std::vector<Sound *> *thePack) {

    if(!packs.contains(packName)) {
        return false;
    }

    *thePack = packs[packName];

    return true;
}

std::unordered_map<std::string, std::vector<Sound *>> SoundPack::getPacks() {
    return packs;
}


