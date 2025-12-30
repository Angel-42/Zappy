/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** StructHeader.hpp
**
** This header defines core mathematical and data structures used for 3D model
** representation and manipulation, particularly for OBJ file loading and rendering.
**
** Structures:
** - Vec2: 2D vector with arithmetic, normalization, and dot product operations.
** - Vec3: 3D vector with arithmetic, normalization, dot/cross product operations.
** - Vec4: 4D vector, often used for homogeneous coordinates, with arithmetic and conversion to Vec3.
** - Ray: Represents a 3D ray with origin and direction.
** - Tri: Represents a triangle with 3 vertices and optional UV coordinates.
** - Face: Stores indices for vertices, UVs, and normals for a polygonal face.
** - Material: Stores material properties (colors, shininess, texture paths, etc.) for rendering.
** - OBJModel: Aggregates all geometry and material data for a loaded OBJ model.
**
** Additional Features:
** - Equality and three-way comparison operators for Vec2, Vec3, Vec4.
** - std::hash specializations for Vec2, Vec3, Vec4 to allow their use in hash-based containers.
**
** Dependencies:
** - Standard C++ headers: <iostream>, <fstream>, <sstream>, <string>, <vector>, <cmath>, <optional>, <compare>, <functional>
**
** Usage:
** Include this header in any module that needs to manipulate 2D/3D/4D vectors,
** handle OBJ model data, or work with materials in a rendering context.
*/

#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <optional>
#include <compare>
#include <functional>

struct Vec2
{
    float x, y;
    constexpr Vec2() noexcept : x(0), y(0) {}
    constexpr Vec2(float X, float Y) noexcept : x(X), y(Y) {}

    [[nodiscard]] constexpr Vec2 operator-(const Vec2 &o) const noexcept { return Vec2(x - o.x, y - o.y); }
    [[nodiscard]] constexpr Vec2 operator+(const Vec2 &o) const noexcept { return Vec2(x + o.x, y + o.y); }
    [[nodiscard]] constexpr Vec2 operator*(float s) const noexcept { return Vec2(x * s, y * s); }
    [[nodiscard]] constexpr Vec2 operator/(float s) const noexcept { return Vec2(x / s, y / s); }
    constexpr Vec2 &operator+=(const Vec2 &o) noexcept
    {
        x += o.x;
        y += o.y;
        return *this;
    }
    constexpr Vec2 &operator-=(const Vec2 &o) noexcept
    {
        x -= o.x;
        y -= o.y;
        return *this;
    }
    constexpr Vec2 &operator*=(float s) noexcept
    {
        x *= s;
        y *= s;
        return *this;
    }
    constexpr Vec2 &operator/=(float s) noexcept
    {
        x /= s;
        y /= s;
        return *this;
    }
    [[nodiscard]] float length() const noexcept { return std::sqrt(x * x + y * y); }
    [[nodiscard]] Vec2 normalized() const noexcept
    {
        float len = length();
        return len > 0 ? *this / len : Vec2();
    }
    [[nodiscard]] constexpr float dot(const Vec2 &o) const noexcept { return x * o.x + y * o.y; }

    constexpr auto operator<=>(const Vec2 &) const noexcept = default;
};

struct Vec3
{
    float x, y, z;
    constexpr Vec3() noexcept : x(0), y(0), z(0) {}
    constexpr Vec3(float X, float Y, float Z) noexcept : x(X), y(Y), z(Z) {}

    [[nodiscard]] constexpr Vec3 operator-(const Vec3 &o) const noexcept { return Vec3(x - o.x, y - o.y, z - o.z); }
    [[nodiscard]] constexpr Vec3 operator+(const Vec3 &o) const noexcept { return Vec3(x + o.x, y + o.y, z + o.z); }
    [[nodiscard]] constexpr Vec3 operator*(float s) const noexcept { return Vec3(x * s, y * s, z * s); }
    [[nodiscard]] constexpr Vec3 operator/(float s) const noexcept { return Vec3(x / s, y / s, z / s); }
    constexpr Vec3 &operator+=(const Vec3 &o) noexcept
    {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }
    constexpr Vec3 &operator-=(const Vec3 &o) noexcept
    {
        x -= o.x;
        y -= o.y;
        z -= o.z;
        return *this;
    }
    constexpr Vec3 &operator*=(float s) noexcept
    {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }
    constexpr Vec3 &operator/=(float s) noexcept
    {
        x /= s;
        y /= s;
        z /= s;
        return *this;
    }

    [[nodiscard]] float length() const noexcept { return std::sqrt(x * x + y * y + z * z); }
    [[nodiscard]] Vec3 normalized() const noexcept
    {
        float len = length();
        return len > 0 ? *this / len : Vec3();
    }
    [[nodiscard]] constexpr float dot(const Vec3 &o) const noexcept { return x * o.x + y * o.y + z * o.z; }
    [[nodiscard]] constexpr Vec3 cross(const Vec3 &o) const noexcept
    {
        return Vec3(
            y * o.z - z * o.y,
            z * o.x - x * o.z,
            x * o.y - y * o.x);
    }

    constexpr auto operator<=>(const Vec3 &) const noexcept = default;
};

struct Vec4
{
    float x, y, z, w;
    constexpr Vec4() noexcept : x(0), y(0), z(0), w(0) {}
    constexpr Vec4(float X, float Y, float Z, float W) noexcept : x(X), y(Y), z(Z), w(W) {}
    constexpr Vec4(const Vec3 &v, float W = 1.0f) noexcept : x(v.x), y(v.y), z(v.z), w(W) {}

    [[nodiscard]] constexpr Vec4 operator-(const Vec4 &o) const noexcept { return Vec4(x - o.x, y - o.y, z - o.z, w - o.w); }
    [[nodiscard]] constexpr Vec4 operator+(const Vec4 &o) const noexcept { return Vec4(x + o.x, y + o.y, z + o.z, w + o.w); }
    [[nodiscard]] constexpr Vec4 operator*(float s) const noexcept { return Vec4(x * s, y * s, z * s, w * s); }
    [[nodiscard]] constexpr Vec4 operator/(float s) const noexcept { return Vec4(x / s, y / s, z / s, w / s); }
    constexpr Vec4 &operator+=(const Vec4 &o) noexcept
    {
        x += o.x;
        y += o.y;
        z += o.z;
        w += o.w;
        return *this;
    }
    constexpr Vec4 &operator-=(const Vec4 &o) noexcept
    {
        x -= o.x;
        y -= o.y;
        z -= o.z;
        w -= o.w;
        return *this;
    }
    constexpr Vec4 &operator*=(float s) noexcept
    {
        x *= s;
        y *= s;
        z *= s;
        w *= s;
        return *this;
    }
    constexpr Vec4 &operator/=(float s) noexcept
    {
        x /= s;
        y /= s;
        z /= s;
        w /= s;
        return *this;
    }

    [[nodiscard]] float length() const noexcept { return std::sqrt(x * x + y * y + z * z + w * w); }
    [[nodiscard]] Vec4 normalized() const noexcept
    {
        float len = length();
        return len > 0 ? *this / len : Vec4();
    }
    [[nodiscard]] constexpr float dot(const Vec4 &o) const noexcept { return x * o.x + y * o.y + z * o.z + w * o.w; }
    [[nodiscard]] constexpr Vec3 toVec3() const noexcept { return Vec3(x, y, z); }

    constexpr auto operator<=>(const Vec4 &) const noexcept = default;
};

struct Ray
{
    Vec3 orig, dir;
    constexpr Ray() noexcept = default;
    constexpr Ray(const Vec3 &o, const Vec3 &d) noexcept : orig(o), dir(d) {}
};

struct Tri
{
    Vec3 v0, v1, v2;
    Vec3 uv0, uv1, uv2;
    constexpr Tri() noexcept = default;
    constexpr Tri(const Vec3 &a, const Vec3 &b, const Vec3 &c,
                  const Vec3 &uva = Vec3(), const Vec3 &uvb = Vec3(), const Vec3 &uvc = Vec3()) noexcept
        : v0(a), v1(b), v2(c), uv0(uva), uv1(uvb), uv2(uvc) {}
};

struct Face
{
    std::vector<int> vertexIndices;
    std::vector<int> uvIndices;
    std::vector<int> normalIndices;
    Face() = default;
    ~Face() = default;
    Face(const Face &) = default;
    Face(Face &&) noexcept = default;
    Face &operator=(const Face &) = default;
    Face &operator=(Face &&) noexcept = default;
};

struct Material
{
    std::string name;
    Vec3 diffuseColor;
    Vec3 specularColor;
    Vec3 ambientColor;
    Vec3 emissiveColor;
    std::optional<float> shininess;
    std::optional<float> opacity;
    std::optional<std::string> texturePath;
    std::optional<std::string> specularMapPath;
    std::optional<std::string> normalMapPath;
    std::optional<std::string> alphaMapPath;
    std::optional<std::string> bumpMapPath;
    std::optional<std::string> displacementMapPath;
    std::optional<std::string> reflectionMapPath;
    std::optional<std::string> refractionMapPath;
    std::optional<std::string> roughnessMapPath;
    std::optional<std::string> metallicMapPath;
    std::optional<std::string> occlusionMapPath;
    Material() = default;
    ~Material() = default;
    Material(const Material &) = default;
    Material(Material &&) noexcept = default;
    Material &operator=(const Material &) = default;
    Material &operator=(Material &&) noexcept = default;
};

struct OBJModel
{
    std::vector<Vec3> vertices;
    std::vector<Vec3> normals;
    std::vector<Vec2> uvs;
    std::vector<Face> faces;
    std::vector<Material> materials;
    OBJModel() = default;
    ~OBJModel() = default;
    OBJModel(const OBJModel &) = default;
    OBJModel(OBJModel &&) noexcept = default;
    OBJModel &operator=(const OBJModel &) = default;
    OBJModel &operator=(OBJModel &&) noexcept = default;
};

// Equality and three-way comparison operators
constexpr bool operator==(const Vec2 &a, const Vec2 &b) noexcept { return a.x == b.x && a.y == b.y; }

constexpr bool operator==(const Vec3 &a, const Vec3 &b) noexcept { return a.x == b.x && a.y == b.y && a.z == b.z; }

constexpr bool operator==(const Vec4 &a, const Vec4 &b) noexcept { return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w; }

// std::hash specializations
namespace std
{
    template <>
    struct hash<Vec2>
    {
        std::size_t operator()(const Vec2 &v) const noexcept
        {
            std::size_t h1 = std::hash<float>{}(v.x);
            std::size_t h2 = std::hash<float>{}(v.y);
            return h1 ^ (h2 << 1);
        }
    };
    template <>
    struct hash<Vec3>
    {
        std::size_t operator()(const Vec3 &v) const noexcept
        {
            std::size_t h1 = std::hash<float>{}(v.x);
            std::size_t h2 = std::hash<float>{}(v.y);
            std::size_t h3 = std::hash<float>{}(v.z);
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
    template <>
    struct hash<Vec4>
    {
        std::size_t operator()(const Vec4 &v) const noexcept
        {
            std::size_t h1 = std::hash<float>{}(v.x);
            std::size_t h2 = std::hash<float>{}(v.y);
            std::size_t h3 = std::hash<float>{}(v.z);
            std::size_t h4 = std::hash<float>{}(v.w);
            return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
        }
    };
}
