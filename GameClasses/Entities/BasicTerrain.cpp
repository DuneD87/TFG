//
// Created by drive on 21/7/21.
//

#include "BasicTerrain.h"

BasicTerrain::BasicTerrain(float width, float height, int wSeg, int hSeg, const glm::vec3 &position,
                           const std::vector<Texture> &textures, const char **path, uint nTextures):width(width),
                           height(height),wSeg(wSeg),hSeg(hSeg),
                           position(position),textures(textures), nText(nTextures){
    this->type = 3;
    lowestPoint = 0;
    highestPoint = 0;
    setupMesh(path);
}



void BasicTerrain::setupMesh(const char ** path) {
    float halfWidth = width/2;
    float halfHeight = height/2;

    int gridX = std::floor(wSeg);
    int gridY = std::floor(hSeg);

    int gridX1 = gridX + 1;
    int gridY1 = gridY + 1;

    float segWidth = width / gridX;
    float segHeight = height / gridY;

    std::vector<Vertex> vertex;
    std::vector<Texture> text;
    std::vector<unsigned int> indices;
    srand(time(NULL));
    float hMap = 100;
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
    noise.SetFractalType(FastNoiseLite::FractalType_PingPong);
    noise.SetFractalOctaves(8);
    noise.SetFractalLacunarity(1.0f);
    noise.SetFractalGain(0.9);
    noise.SetFractalWeightedStrength(0.9);
    noise.SetFractalPingPongStrength(0.85);
    noise.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_Euclidean);
    noise.SetCellularReturnType(FastNoiseLite::CellularReturnType_Distance2Add);
    noise.SetCellularJitter(1.0f);

    //Vertices and texcoords
    for (int iy = 0; iy < gridY1; iy++)
    {
        float z = iy * segHeight - halfHeight;
        for (int ix = 0; ix < gridX1; ix++)
        {
            Vertex auxVert;
            float x = ix * segWidth - halfWidth;
            float y = noise.GetNoise((float)ix,(float)iy)*1500;
            if (y < lowestPoint)
                lowestPoint = y;
            else if (y > highestPoint)
                highestPoint = y;
            auxVert.Position = glm::vec3(x,y,z);
            auxVert.TexCoords = glm::vec2(((float)ix/(gridX)),(float)iy/(gridY))*glm::vec2(wSeg,hSeg) ;
            vertex.push_back(auxVert);
        }
    }
    std::cout<<"Highest point: "<<highestPoint<<" Lowest point: "<<lowestPoint<<std::endl;
    //Indices
    for (int iy = 0; iy < gridY; iy++)
    {
        for (int ix=0; ix < gridX; ix++)
        {
            uint a = ix+gridX1*iy;
            uint b = ix + gridX1 * (iy + 1);
            uint c = (ix + 1) + gridX1 * (iy + 1);
            uint d = (ix + 1) + gridX1 * iy;
            indices.push_back(a);
            indices.push_back(b);
            indices.push_back(d);
            indices.push_back(b);
            indices.push_back(c);
            indices.push_back(d);
        }
    }
    //Normals
    for (int i = 0; i < indices.size();i+=3)
    {
        unsigned int iA = indices[i];
        unsigned int iB = indices[i+1];
        unsigned int iC = indices[i+2];
        auto edgeAB = vertex[iB].Position - vertex[iA].Position;
        auto edgeAC = vertex[iC].Position - vertex[iA].Position;
        auto norm = glm::normalize(glm::cross(edgeAB,edgeAC));
        vertex[iA].Normal = norm;
        vertex[iB].Normal = norm;
        vertex[iC].Normal = norm;
    }

    terrainMesh = new Mesh(vertex,indices,path,nText);
}

void BasicTerrain::draw(Shader &shader, bool outLined, int depthMap) {
    glUniform1i(glGetUniformLocation(shader.ID,"isTerrain"),1);
    glUniform1f(glGetUniformLocation(shader.ID,"hPoint"),highestPoint);
    glUniform1f(glGetUniformLocation(shader.ID,"lPoint"),lowestPoint);
    terrainMesh->Draw(shader,outLined,depthMap);
    glUniform1i(glGetUniformLocation(shader.ID,"isTerrain"),0);
}

BasicTerrain::~BasicTerrain() {
 delete terrainMesh;
}

float BasicTerrain::getPixelHeight(unsigned char *data, float x, float y,float w,float hScale) {
    float pixelHeight = 0;
    if (x < 0 || x >= w || y < 0 || y >= w)
    {
        //std::cout << "x: " << x << " y: " << y << " w: " << w << std::endl;
        pixelHeight = 0.0f;
    }else
    {
        float value = static_cast<float>(*(data + int(x*w + y)));
        pixelHeight = hScale * (value / 255.0f);
        //std::cout<<pixelHeight<<std::endl;
    }
    return pixelHeight;
}

float BasicTerrain::getLowestPoint() const {
    return lowestPoint;
}

float BasicTerrain::getHighestPoint() const {
    return highestPoint;
}

float BasicTerrain::getWidth() const {
    return width;
}

float BasicTerrain::getHeight() const {
    return height;
}

int BasicTerrain::getWSeg() const {
    return wSeg;
}

int BasicTerrain::getHSeg() const {
    return hSeg;
}

