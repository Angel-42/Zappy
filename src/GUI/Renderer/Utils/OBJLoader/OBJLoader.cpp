/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** OBJLoader.cpp
*/

#include "OBJLoader.hpp"

bool OBJLoader::loadOBJ(const std::string &path)
{
    std::ifstream in(path);
    if (!in)
    {
        std::cerr << "Failed to open OBJ file: " << path << "\n";
        return false;
    }

    std::string line, mtlFile;
    while (std::getline(in, line))
    {
        std::istringstream iss(line);
        std::string tag;
        iss >> tag;

        if (tag == "v")
        {
            if (!parseVertex(iss))
                return false;
        }
        else if (tag == "vt")
        {
            if (!parseTextureCoordinate(iss))
                return false;
        }
        else if (tag == "usemtl")
        {
            iss >> matName; // Skip material name
        }
        else if (tag == "mtllib")
        {
            iss >> mtlFile;
        }
        else if (tag == "f")
        {
            if (!parseFace(iss, line))
                return false;
        }
    }

    if (!mtlFile.empty())
        loadMTL(mtlFile);

    return true;
}

bool OBJLoader::parseVertex(std::istringstream &iss)
{
    Vec3 v;
    if (!(iss >> v.x >> v.y >> v.z))
    {
        std::cerr << "Error parsing vertex.\n";
        return false;
    }
    vertices.push_back(v);
    return true;
}

bool OBJLoader::parseTextureCoordinate(std::istringstream &iss)
{
    Vec3 uv;
    if (!(iss >> uv.x >> uv.y))
    {
        std::cerr << "Error parsing texture coordinate.\n";
        return false;
    }
    uvs.push_back(uv);
    return true;
}

bool OBJLoader::parseFace(std::istringstream &iss, const std::string &line)
{
    std::vector<int> vi, ti, ni; // Vertex, texture, and normal indices
    char slash;

    while (iss)
    {
        int v = -1, t = -1, n = -1; // Default indices to -1 if not provided
        if (!(iss >> v))
            break;
        if (iss.peek() == '/')
        {
            iss >> slash;
            if (iss.peek() != '/')
            {
                iss >> t;
            }
            if (iss.peek() == '/')
            {
                iss >> slash >> n;
            }
        }
        vi.push_back(v - 1); // Convert 1-based index to 0-based
        ti.push_back(t - 1); // Convert 1-based index to 0-based
        ni.push_back(n - 1); // Convert 1-based index to 0-based
    }

    if (vi.size() < 3)
    {
        std::cerr << "Error: Face has fewer than 3 vertices: " << line << "\n";
        return false;
    }

    return triangulateFace(vi, ti, line);
}

bool OBJLoader::triangulateFace(const std::vector<int> &vi, const std::vector<int> &ti, const std::string &line)
{
    for (size_t i = 1; i + 1 < vi.size(); ++i)
    {
        if (vi[0] < 0 || vi[0] >= (int)vertices.size() ||
            vi[i] < 0 || vi[i] >= (int)vertices.size() ||
            vi[i + 1] < 0 || vi[i + 1] >= (int)vertices.size())
        {
            std::cerr << "Face vertex index out of bounds: " << line << "\n";
            return false;
        }

        Tri t;
        t.v0 = vertices[vi[0]];
        t.v1 = vertices[vi[i]];
        t.v2 = vertices[vi[i + 1]];

        if (!ti.empty() && ti[0] >= 0 && ti[i] >= 0 && ti[i + 1] >= 0)
        {
            if (ti[0] >= (int)uvs.size() || ti[i] >= (int)uvs.size() || ti[i + 1] >= (int)uvs.size())
            {
                std::cerr << "Face texture index out of bounds: " << line << "\n";
                return false;
            }
            t.uv0 = uvs[ti[0]];
            t.uv1 = uvs[ti[i]];
            t.uv2 = uvs[ti[i + 1]];
        }

        triangles.push_back(t);
    }
    return true;
}

bool OBJLoader::loadMTL(const std::string &mtlPath)
{
    std::ifstream in(mtlPath);
    if (!in)
    {
        std::cerr << "Failed to open MTL file: " << mtlPath << "\n";
        return false;
    }

    std::string texFile = parseMTLFile(in);
    if (!texFile.empty())
    {
        return loadTexture(texFile);
    }
    else
    {
        useDefaultTexture();
    }
    return true;
}

std::string OBJLoader::parseMTLFile(std::ifstream &in)
{
    std::string line, texFile;
    while (std::getline(in, line))
    {
        std::istringstream iss(line);
        std::string tag;
        iss >> tag;
        if (tag == "map_Kd")
        {
            iss >> texFile;
        }
        else if (tag == "map_Ks")
        {
            std::string specularFile;
            iss >> specularFile;
            if (!specularFile.empty())
            {
                (void)loadSpecularMap(specularFile);
            }
        }
        else if (tag == "map_Bump")
        {
            std::string bumpFile;
            iss >> bumpFile;
            if (!bumpFile.empty())
            {
                (void)loadBumpMap(bumpFile);
            }
        }
    }
    return texFile;
}

bool OBJLoader::loadSpecularMap(const std::string &specularFile)
{
    if (!specularMap.loadFromFile(specularFile))
    {
        std::cerr << "Failed to load specular map: " << specularFile << "\n";
        return false;
    }
    // Convert specular map to image
    specularImage = specularMap.copyToImage();
    std::cout << "Loaded specular map: " << specularFile << " (" << specularMap.getSize().x << "x" << specularMap.getSize().y << ")\n";
    return true;
}

bool OBJLoader::loadBumpMap(const std::string &bumpFile)
{
    if (!bumpMap.loadFromFile(bumpFile))
    {
        std::cerr << "Failed to load bump map: " << bumpFile << "\n";
        return false;
    }
    // Convert bump map to image
    bumpImage = bumpMap.copyToImage();
    std::cout << "Loaded bump map: " << bumpFile << " (" << bumpMap.getSize().x << "x" << bumpMap.getSize().y << ")\n";
    return true;
}

bool OBJLoader::loadTexture(const std::string &texFile)
{
    if (!texture.loadFromFile(texFile))
    {
        std::cerr << "Failed to load texture: " << texFile << "\n";
        return false;
    }
    std::cout << "Loaded texture: " << texFile << " (" << texture.getSize().x << "x" << texture.getSize().y << ")\n";
    return true;
}

void OBJLoader::useDefaultTexture()
{
    std::cerr << "No texture found in MTL file. Using default color.\n";
    texture.create(1, 1);
    texture.setPixel(0, 0, sf::Color::White);
}
