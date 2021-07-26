//
// Created by drive on 21/7/21.
//

#include "BasicTerrain.h"

BasicTerrain::BasicTerrain(float width, float height, int wSeg, int hSeg, const glm::vec3 &position,
                           const std::vector<Texture> &textures, const char *path):width(width),height(height),wSeg(wSeg),hSeg(hSeg),
                           position(position),textures(textures){
    this->type = 3;
    setupMesh(path);
}



void BasicTerrain::setupMesh(const char * path) {
    float halfWidth = width/2;
    float halfHeight = height/2;

    int gridX = std::floor(wSeg);
    int gridY = std::floor(hSeg);

    int gridX1 = gridX + 1;
    int gridY1 = gridY + 1;

    float segWidth = width / gridX;
    float segHeight = height / gridY;
    int w;
    int h;
    int comp;
    stbi_set_flip_vertically_on_load(false);
    unsigned char* image = stbi_load(path, &w, &h, &comp, 1);

    stbi_set_flip_vertically_on_load(true);
    if(image == nullptr)
        throw(std::string("Failed to load heightmap"));
    std::cout<<w<<std::endl;
    std::vector<Vertex> vertex;
    std::vector<Texture> text;
    std::vector<unsigned int> indices;
    srand(time(NULL));
    float hMap = 100;
    //Vertices and texcoords
    for (int iy = 0; iy < gridY1; iy++)
    {
        float z = iy * segHeight - halfHeight;
        for (int ix = 0; ix < gridX1; ix++)
        {
            Vertex auxVert;
            float x = ix * segWidth - halfWidth;
            float y = bilinearSample(ix,iy,image,w);
            auxVert.Position = glm::vec3(x,-15+y,z);
            auxVert.TexCoords = glm::vec2(((float)ix/(gridX)),(float)iy/(gridY))*glm::vec2(wSeg,hSeg) ;
            vertex.push_back(auxVert);
        }
    }
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

    terrainMesh = new Mesh(vertex,indices,"grass2.png","../Textures/");
    stbi_image_free(image);
}

void BasicTerrain::draw(Shader &shader, bool outLined, int depthMap) {
    terrainMesh->Draw(shader,outLined,depthMap);
}

BasicTerrain::~BasicTerrain() {
 delete terrainMesh;
}

float BasicTerrain::bilinearSample(float x, float y, unsigned char* data,float width) {

    float w = width - 1.0f;
    float h = height - 1.0f;

    float x1 = std::floor(x*w);
    float y1 = std::floor(y*h);
    float x2 = std::clamp(float(x1) + 1.0f,0.0f,w);
    float y2 = std::clamp(float(y1) + 1.0f,0.0f,h);

    float xp = x * w - x1;
    float yp = y * h - y1;

    float p11 = getPixelHeight(data,x1,y1,width,1);
    float p21 = getPixelHeight(data,x2,y1,width,1);
    float p12 = getPixelHeight(data,x1,y2,width,1);
    float p22 = getPixelHeight(data,x2,y2,width,1);

    float px1 = lerp(float(yp),p11,p21);
    float px2 = lerp(float(xp),p12,p22);
    float sample = lerp(yp,px1,px2)*10;
    std::cout<<sample<<std::endl;
    return sample;

}

float BasicTerrain::getPixelHeight(unsigned char *data, float x, float y,float w,float hScale) {
    float pixelHeight = 0;
    if (x < 0 || x >= w || y < 0 || y >= w)
        pixelHeight = 0.0f;
    else
    {
        float value = static_cast<float>(*(data + int(x*w + y)));
        pixelHeight = hScale * (value / 255.0f);
    }
    return pixelHeight;
}

