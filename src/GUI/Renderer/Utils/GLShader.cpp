/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** GLShader.cpp
*/

#include "GLShader.hpp"

GLShader::GLShader(const char *vertexSrc, const char *fragSrc)
{
    GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vertexSrc, nullptr);
    glCompileShader(vert);
    if (!checkCompile(vert, false, "VERTEX"))
        throw std::runtime_error("Vertex shader compile failed");

    GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fragSrc, nullptr);
    glCompileShader(frag);
    if (!checkCompile(frag, false, "FRAGMENT"))
        throw std::runtime_error("Fragment shader compile failed");

    m_id = glCreateProgram();
    glAttachShader(m_id, vert);
    glAttachShader(m_id, frag);
    glLinkProgram(m_id);
    if (!checkCompile(m_id, true, "PROGRAM"))
        throw std::runtime_error("Shader link failed");

    glDeleteShader(vert);
    glDeleteShader(frag);
}

GLShader::~GLShader()
{
    glDeleteProgram(m_id);
}

bool GLShader::checkCompile(GLuint obj, bool isProgram, const std::string &stage)
{
    GLint ok = 0;
    if (isProgram)
        glGetProgramiv(obj, GL_LINK_STATUS, &ok);
    else
        glGetShaderiv(obj, GL_COMPILE_STATUS, &ok);

    if (!ok)
    {
        char info[512];
        if (isProgram)
            glGetProgramInfoLog(obj, 512, nullptr, info);
        else
            glGetShaderInfoLog(obj, 512, nullptr, info);
        std::cerr << stage << " ERROR\n"
                  << info << std::endl;
        return false;
    }
    return true;
}
