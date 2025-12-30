/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Skybox.hpp
*/

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>
#include "../../../Utils/GLShader.hpp"

class Skybox
{
public:
    Skybox(const std::vector<std::string> &faces, std::shared_ptr<GLShader> shader);
    ~Skybox();

    Skybox(const Skybox &) = delete;
    Skybox &operator=(const Skybox &) = delete;

    void draw(const glm::mat4 &view, const glm::mat4 &projection);

private:
    GLuint m_vao = 0, m_vbo = 0, m_cubemap = 0;
    std::shared_ptr<GLShader> m_shader;

    void setupCube();
    GLuint loadCubemap(const std::vector<std::string> &faces);
};
