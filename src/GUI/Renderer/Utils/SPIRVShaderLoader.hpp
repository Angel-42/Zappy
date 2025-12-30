/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** SPIRVShaderLoader.hpp
*/

#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <filesystem>
#include <format>
#include <GL/glew.h>

class SPIRVShaderLoader
{
public:
    [[nodiscard]]
    static std::vector<uint32_t> loadBinary(const std::filesystem::path &path)
    {
        if (!std::filesystem::exists(path))
            throw std::runtime_error(std::format("SPIR-V file does not exist: {}", path.string()));

        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if (!file)
            throw std::runtime_error(std::format("Failed to open SPIR-V file: {}", path.string()));

        auto fileSize = file.tellg();
        if (fileSize % 4 != 0)
            throw std::runtime_error(std::format("SPIR-V file size is not a multiple of 4: {}", path.string()));

        std::vector<uint32_t> buffer(static_cast<size_t>(fileSize) / 4);
        file.seekg(0);
        file.read(reinterpret_cast<char *>(buffer.data()), fileSize);
        return buffer;
    }

    [[nodiscard]]
    static GLuint createShader(GLenum stage, const std::filesystem::path &path, const char *entryPoint = "main")
    {
        auto spirv = loadBinary(path);
        GLuint shader = glCreateShader(stage);
        glShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), static_cast<GLsizei>(spirv.size() * sizeof(uint32_t)));
        glSpecializeShader(shader, entryPoint, 0, nullptr, nullptr);

        GLint success = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            throw std::runtime_error(std::format("SPIR-V shader compilation failed: {}", infoLog));
        }
        return shader;
    }
};
