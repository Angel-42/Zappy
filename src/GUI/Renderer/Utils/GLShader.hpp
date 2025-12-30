/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** GLShader.hpp
*/

#pragma once

#define GLEW_STATIC

#include <GL/glew.h>
#include <string>
#include <stdexcept>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class GLShader
{
public:
    GLShader(const char *vertexSrc, const char *fragSrc);
    ~GLShader();

    void use() const { glUseProgram(m_id); }
    GLuint id() const { return m_id; }

    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(id(), name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }

private:
    GLuint m_id;
    static bool checkCompile(GLuint obj, bool isProgram, const std::string &stage);
};
