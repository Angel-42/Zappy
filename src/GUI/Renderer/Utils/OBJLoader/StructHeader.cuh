/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** StructHeader.cuh
*/

#pragma once

struct Vec3
{
    float x, y, z;
    __host__ __device__ Vec3() : x(0), y(0), z(0) {}
    __host__ __device__ Vec3(float X, float Y, float Z) : x(X), y(Y), z(Z) {}
    __host__ __device__ Vec3 operator-(const Vec3 &o) const { return Vec3(x - o.x, y - o.y, z - o.z); }
    __host__ __device__ Vec3 operator+(const Vec3 &o) const { return Vec3(x + o.x, y + o.y, z + o.z); }
    __host__ __device__ Vec3 operator*(float s) const { return Vec3(x * s, y * s, z * s); }
};

struct Ray
{
    Vec3 orig, dir;
    __host__ __device__ Ray() : orig(), dir() {}
    __host__ __device__ Ray(const Vec3 &o, const Vec3 &d) : orig(o), dir(d) {}
};

struct Tri
{
    Vec3 v0, v1, v2;
    Vec3 uv0, uv1, uv2;
    __host__ __device__ Tri() : v0(), v1(), v2(), uv0(), uv1(), uv2() {}
    __host__ __device__ Tri(const Vec3 &a, const Vec3 &b, const Vec3 &c,
                            const Vec3 &uva = Vec3(), const Vec3 &uvb = Vec3(), const Vec3 &uvc = Vec3())
        : v0(a), v1(b), v2(c), uv0(uva), uv1(uvb), uv2(uvc) {}
};
