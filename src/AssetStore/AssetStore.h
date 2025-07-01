#ifndef ASSETSTORE_H
#define ASSETSTORE_H

#include <string>
#include <unordered_map>
#include <utility>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../Logger/Logger.h"

class AssetStore
{
private:
    std::unordered_map<std::string, SDL_Texture *> textures;

public:
    AssetStore();
    ~AssetStore();
    void ClearAssets();
    void AddTexture(SDL_Renderer *renderer, const std::string &assetId, const std::string &filePath);
    SDL_Texture *GetTexture(const std::string &assetId);
    std::pair<int, int> GetTextureWidthHeight(const std::string &assetId);
};

#endif
