#ifndef TILEMAP_H
#define TILEMAP_H

#include <string>
#include <vector>
#include <fstream>
#include <glm/glm.hpp>

#include "../ECS/ECS.h"
#include "../Logger/Logger.h"
#include "../AssetStore/AssetStore.h"

#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"

class TileMap
{
private:
    std::vector<Entity> entities;
    std::vector<std::vector<int>> positions;
    std::string textureName;
    int tilesize;
    int mapNumCols;
    int mapNumRows;
    double tileScale;
    Registry *registry;
    AssetStore *assetStore;

    void ReadMapFile(const std::string &mapFilePath);
    void CreateEntities(Registry *registry, AssetStore *assetStore);

public:
    TileMap(const std::string &textureName, int tilesize, double tileScale, const std::string &mapFilePath, Registry *registry, AssetStore *assetStore);
    void PrintPositions();
    ~TileMap() = default;
};

TileMap::TileMap(const std::string &textureName, int tilesize, double tileScale, const std::string &mapFilePath, Registry *registry, AssetStore *assetStore)
{
    this->tileScale = tileScale;
    this->tilesize = tilesize;
    this->textureName = textureName;
    ReadMapFile(mapFilePath);
    CreateEntities(registry, assetStore);
}

void TileMap::CreateEntities(Registry *registry, AssetStore *assetStore)
{
    auto [textureWidth, textureHeight] = assetStore->GetTextureWidthHeight(textureName);
    int tileNumCols = textureWidth / tilesize;
    int tileNumRows = textureHeight / tilesize;

    for (int y = 0; y < mapNumRows; y++)
    {
        for (int x = 0; x < mapNumCols; x++)
        {
            int srcRectX = (positions[y][x] % tileNumCols) * tilesize;
            int srcRectY = ((positions[y][x] / tileNumCols) % tileNumRows) * tilesize;
            Entity entity = registry->CreateEntity();
            entity.AddComponent<TransformComponent>(glm::vec2(x * (tileScale * tilesize), y * (tileScale * tilesize)), glm::vec2(tileScale, tileScale), 0.0);
            entity.AddComponent<SpriteComponent>("jungle-tilemap", tilesize, tilesize, srcRectX, srcRectY);
        }
    }
}

void TileMap::ReadMapFile(const std::string &mapFilePath)
{
    std::ifstream file(mapFilePath);
    if (!file.is_open())
    {
        Logger::Err("Failed to open file ", mapFilePath);
        return;
    }

    std::string line;
    int i = 0;
    while (std::getline(file, line))
    {
        positions.resize(i + 1);
        std::istringstream lineStream(line);
        std::string token;
        char delimiter = ',';
        while (std::getline(lineStream, token, delimiter))
        {
            positions[i].push_back(std::stoi(token));
        }
        i++;
    }

    this->mapNumCols = positions[0].size();
    this->mapNumRows = positions.size();

    file.close();
}

void TileMap::PrintPositions()
{
    for (int y = 0; y < mapNumRows; y++)
    {
        for (int x = 0; x < mapNumCols; x++)
        {
            std::cout << positions[y][x] << " ";
        }
        std::cout << std::endl;
    }
}

#endif