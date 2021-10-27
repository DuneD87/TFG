//
// Created by drive on 21/7/21.
//

#include "BasicTerrain.h"

BasicTerrain::BasicTerrain(float width, float height, int wSeg, int hSeg, const glm::vec3 &position,
                           const char **path, uint nTextures,  glm::vec3 up):width(width),
                           height(height),wSeg(wSeg),hSeg(hSeg),
                           position(position), nText(nTextures){
    this->type = 3;
    lowestPoint = 0;
    highestPoint = 0;
    this->upVector = up;
    xAxis = glm::vec3(up.y,up.x,up.z);
    yAxis = glm::cross(upVector,xAxis);
    setupMesh(path);
    translateTerrain(position);
}



void BasicTerrain::setupMesh(const char ** path)
{

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
    std::pair<std::vector<Vertex>,std::vector<unsigned int>> *plane = makePlane(width,height,hSeg,wSeg,1500,noise);
    std::vector<Vertex> vertices = plane->first;
    for (int i = 0; i < vertices.size();i++)
    {
        float y = vertices[i].Position.y;
        if (y < lowestPoint)
            lowestPoint = y;
        else if (y > highestPoint)
            highestPoint = y;
    }

    terrainMesh = new Mesh(vertices,plane->second,path,nText,1);
}

void BasicTerrain::draw(Shader &shader, bool outLined, int depthMap) {
    shader.setInt("isTerrain",1);
    shader.setFloat("hPoint",highestPoint);
    shader.setFloat("lPoint",lowestPoint);
    shader.setVec3("upVector",upVector);
    terrainMesh->Draw(shader,outLined,depthMap,false,false);
    shader.setInt("isTerrain",0);
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

const glm::vec3 &BasicTerrain::getXAxis() const {
    return xAxis;
}

const glm::vec3 &BasicTerrain::getYAxis() const {
    return yAxis;
}

std::string BasicTerrain::toString() {
    return std::string();
}

