/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** OBJLoader.hpp
*/

#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <SFML/Graphics.hpp>
#include "StructHeader.hpp"

class OBJLoader
{
public:
    std::vector<Vec3> vertices;
    std::vector<Vec3> uvs;
    std::vector<Tri> triangles;
    std::vector<Vec3> normals;
    std::vector<Vec3> tangents;
    std::string name;

    std::string lastError;

    sf::Image texture;
    std::string matName;
    sf::Texture specularMap;
    sf::Texture bumpMap;
    sf::Image bumpImage;
    sf::Image specularImage;
    struct MaterialGroup
    {
        std::string materialName;
        std::vector<Tri> triangles;
    };
    std::vector<MaterialGroup> materialGroups;

    void clear();

    bool loadOBJ(const std::string &path);
    bool loadMTL(const std::string &mtlPath);

private:
    [[nodiscard]] bool parseVertex(std::istringstream &iss);
    [[nodiscard]] bool parseTextureCoordinate(std::istringstream &iss);
    [[nodiscard]] bool parseFace(std::istringstream &iss, const std::string &line);
    [[nodiscard]] bool triangulateFace(const std::vector<int> &vi, const std::vector<int> &ti, const std::string &line);

    [[nodiscard]] std::string parseMTLFile(std::ifstream &in);
    [[nodiscard]] bool loadTexture(const std::string &texPath);
    [[nodiscard]] bool loadSpecularMap(const std::string &specularFile);
    [[nodiscard]] bool loadBumpMap(const std::string &bumpFile);
    void useDefaultTexture();
    [[nodiscard]] const std::string &getLastError() const;
};
