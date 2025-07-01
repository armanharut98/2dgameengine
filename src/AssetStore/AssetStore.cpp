#include "./AssetStore.h"

AssetStore::AssetStore()
{
    Logger::Log("AssetStore constructor called!");
}

AssetStore::~AssetStore()
{
    Logger::Log("AssetStore destructor called!");
    ClearAssets();
}

void AssetStore::ClearAssets()
{
    for (auto texture : textures)
    {
        SDL_DestroyTexture(texture.second);
    }
    textures.clear();
}

void AssetStore::AddTexture(SDL_Renderer *renderer, const std::string &assetId, const std::string &filePath)
{
    SDL_Surface *surface = IMG_Load(filePath.c_str());
    if (!surface)
    {
        Logger::Err("Error when creating a surface from file ", filePath, ": ", SDL_GetError());
        return;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
    {
        Logger::Err("Error when creating a texture from file ", filePath, ": ", SDL_GetError());
        return;
    }
    SDL_FreeSurface(surface);

    textures.emplace(assetId, texture);

    Logger::Log("New texture added to the asset store with id = ", assetId);
}

SDL_Texture *AssetStore::GetTexture(const std::string &assetId)
{
    return textures[assetId];
}

std::pair<int, int> AssetStore::GetTextureWidthHeight(const std::string &assetId)
{
    int w, h;
    SDL_QueryTexture(GetTexture(assetId), NULL, NULL, &w, &h);
    return std::make_pair(w, h);
}
