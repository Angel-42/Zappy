/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** MeshFactory.hpp
*/

#pragma once

#include "Mesh.hpp"
#include <memory>
#include <string>

/**
 * @class MeshFactory
 * @brief Utility class for creating Mesh objects from OBJ files.
 *
 * MeshFactory provides a static method to load a mesh from an OBJ file
 * and return it as a unique pointer.
 */
class MeshFactory
{
public:
    /**
     * @brief Creates a Mesh object from an OBJ file.
     *
     * Attempts to load a mesh from the specified OBJ file path.
     * If loading fails, returns nullptr.
     *
     * @param path The file path to the OBJ file.
     * @return std::unique_ptr<Mesh> A unique pointer to the loaded Mesh,
     *         or nullptr if loading fails.
     */
    [[nodiscard]] static std::unique_ptr<Mesh> createFromOBJ(const std::string &path)
    {
        auto mesh = std::make_unique<Mesh>();
        if (!mesh->loadOBJ(path))
        {
            return nullptr;
        }
        return mesh;
    }
};
