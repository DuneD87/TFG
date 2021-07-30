//
// Created by drive on 8/7/21.
//

#include "Renderer.h"

Renderer::Renderer() {

}

Renderer::Renderer(unsigned int scrWidth, unsigned int scrHeight, Camera *camera, const char* skyboxPath) {
    this->scrWidth = scrWidth;
    this->scrHeight = scrHeight;
    this->camera = camera;
    spotLight =  Light("spotLight",glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(1.0f, 1.0f, 1.0f),
                                 glm::vec3(1.0f, 1.0f, 1.0f),1.0,0.09,0.032,-1,-1);
    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightShader("../Shaders/lightingShaderVertex.shader", "../Shaders/lightingShaderFragment.shader");
    Shader spriteShader("../Shaders/lightingShaderVertex.shader","../Shaders/alphaTextureTest.fs");
    Shader outlineShader = Shader("../Shaders/lightingShaderVertex.shader", "../Shaders/singleColorShader.fs");
    Shader screenShader = Shader("../Shaders/PostProcess/screenShader.vs","../Shaders/PostProcess/screenShader.fs");
    Shader skyboxShader = Shader("../Shaders/skyboxShader.vs","../Shaders/skyboxShader.fs");
    Shader depthSMapShader = Shader("../Shaders/depthShadowMappingVertex.shader","../Shaders/depthShadowMappingFragment.shader");
    Shader lightInstancedShader("../Shaders/lightingShaderInstancedVertex.shader", "../Shaders/lightingShaderFragment.shader");
    Shader depthSMapInstancedShader("../Shaders/depthShadowMappingInstancedVertex.shader", "../Shaders/depthShadowMappingFragment.shader");
    shaders.push_back(lightShader);//0
    shaders.push_back(spriteShader);//1
    shaders.push_back(outlineShader);//2
    shaders.push_back(screenShader);//3
    shaders.push_back(skyboxShader);//4
    shaders.push_back(depthSMapShader);//5
    shaders.push_back(lightInstancedShader);//6
    shaders.push_back(depthSMapInstancedShader);//7
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    setupFrameBuffer();
    setupSkyBox(skyboxPath);
}

void Renderer::renderScene(vector<Entity*> worldEnts,std::vector<std::pair<std::vector<glm::mat4>,PhysicsObject*>> ents) {
    glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
    renderShadowMap(worldEnts,ents);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    shaders[2].use();
    renderLoopCamera(shaders[2]);
    shaders[1].use();
    renderLoopCamera(shaders[1]);
    shaders[0].use();

    renderLoopCamera(shaders[0]);

    glViewport(0, 0, scrWidth, scrHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


    shaders[0].setFloat("material.shininess", 64.0f);
    shaders[0].setVec3("viewPos",camera->Position);
    shaders[0].setVec3("lightPos", sunPos);
    shaders[0].setMat4("lightSpaceMatrix", lightSpaceMatrix);
    shaders[0].setInt("nPointLights",nPointLights);
    //Draw
    std::vector<int> selectedeItems;
    for (int i = 0; i < worldEnts.size();i++)
    {

        worldEnts[i]->draw(shaders[0],false,depthMap);
        if (worldEnts[i]->getType() == 2 && dynamic_cast<Light*>(worldEnts[i])->getSubType() == "dirLight")
        {

            sunDir = dynamic_cast<Light*>(worldEnts[i])->getDirection();
        }
        else if (worldEnts[i]->getType() == 3)
        {
            sunPos = camera->Position
                    + glm::vec3(0.0f,dynamic_cast<BasicTerrain*>(worldEnts[i])->getHighestPoint(),0.0f)
                    + camera->Front * 100.0f;
        }

    }
    shaders[6].use();
    renderLoopCamera(shaders[6]);


    shaders[6].setFloat("material.shininess", 64.0f);
    shaders[6].setVec3("viewPos",camera->Position);
    shaders[6].setVec3("lightPos", sunPos);
    shaders[6].setMat4("lightSpaceMatrix", lightSpaceMatrix);
    shaders[6].setInt("nPointLights",nPointLights);

    for (int i = 0; i < worldEnts.size();i++)
    {
        if (worldEnts[i]->getType() == 2)
        {
            worldEnts[i]->draw(shaders[6]);
        }
    }
    for (int i = 0; i < ents.size();i++)
        renderInstanced(ents[i],shaders[6]);

    for (int i = 0; i < selectedeItems.size();i++)
        worldEnts[selectedeItems[i]]->getModel()->outlineObject(shaders[2],glm::vec3(1.1));

    glDepthFunc(GL_LEQUAL);
    shaders[4].use();
    renderLoopCamera(shaders[4],true);
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default
   // shaders[1].use();
    /*for (int i = 0; i < effects.size();i++)
    {
        //Render should be a function, handle semi-transparent objects sorting them to draw in order ( pre-render function? )
        effects[i].Draw(shaders[1],false,-1);
    }
     */
    // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
    // clear all relevant buffers
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    shaders[3].use();
    glBindVertexArray(quadVAO);
    glBindTexture(GL_TEXTURE_2D, textColorBuffer);	// use the color attachment texture as the texture of the quad plane
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::renderInstanced(std::pair<std::vector<glm::mat4>,PhysicsObject*> ent,Shader &shader) {
    for (int i = 0; i < ent.second->getModel()->meshes.size();i++)
    {
        ent.second->getModel()->meshes[i]->bindTextures(shader,depthMap);
        glBindVertexArray(ent.second->getModel()->meshes[i]->VAO);
        glDrawElementsInstanced(GL_TRIANGLES, ent.second->getModel()->meshes[i]->indices.size(), GL_UNSIGNED_INT, 0, ent.first.size());
        glBindVertexArray(0);
    }
}

void Renderer::renderLoopCamera(Shader shader,bool skybox) {
    //view
    glm::mat4 view;
    if (skybox)
        view = glm::mat4(glm::mat3(camera->GetViewMatrix())); // remove translation from the view matrix
    else
        view = this->camera->GetViewMatrix();
    shader.setMat4("view",view);

    //projection
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(this->camera->Zoom), (float)scrWidth/scrHeight, 0.1f, 10000.0f);
    shader.setMat4("projection",projection);

    glm::mat4 cameraModel(1.0f);
    shader.setMat4("model", cameraModel);
}

void Renderer::setupFrameBuffer() {
    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
            1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f,  1.0f,  0.0f, 1.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f
    };
    // screen quad VAO
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    shaders[3].use();
    shaders[3].setInt("screenTexture",0);

    // frameBuffer configuration
    // -------------------------
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    // create a color attachment texture
    glGenTextures(1, &textColorBuffer);
    glBindTexture(GL_TEXTURE_2D, textColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, scrWidth, scrHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textColorBuffer, 0);

    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, scrWidth, scrHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
    // now that we actually created the frameBuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // configure depth map FBO
    // -----------------------
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Renderer::setPostProcess(unsigned int index) {
    shaders[3] = Shader("../Shaders/PostProcess/screenShader.vs",postProcessPath[index].c_str());
}

unsigned int Renderer::loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void Renderer::setupSkyBox(const char * path) {
    float skyboxVertices[] = {
            // positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };
    // skybox VAO
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    std::string format = ".png";
    vector<std::string> faces
    {
        std::string(path)+"right"+format,
        std::string(path)+"left"+format,
        std::string(path)+"top"+format,
        std::string(path)+"bottom"+format,
        std::string(path)+"front"+format,
        std::string(path)+"back"+format
    };
    cubemapTexture = loadCubemap(faces);
    shaders[4].use();
    shaders[4].setInt("skybox",0);
}

void Renderer::renderShadowMap(vector<Entity*> worldEnts,std::vector<std::pair<std::vector<glm::mat4>,PhysicsObject*>>ents) {
    // render
    // ------
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_CULL_FACE);
    // 1. render depth of scene to texture (from light's perspective)
    // --------------------------------------------------------------
    glm::mat4 lightProjection, lightView;
    float near_plane = -100.0f, zFar = 500.0f,boxSize = 256.0f;
    lightProjection = glm::ortho(-boxSize, boxSize, -boxSize, boxSize, near_plane, zFar);
    //std::cout<<camera->Front.x<<" "<<camera->Front.y<<" " << camera->Front.z<<std::endl;
    lightView = glm::lookAt(sunPos , sunPos + (sunDir * zFar/2.0f), glm::vec3(0,1,0));
    lightSpaceMatrix = lightProjection * lightView;
    // render scene from light's point of view
    shaders[5].use();
    shaders[5].setMat4("lightSpaceMatrix", lightSpaceMatrix);

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    for (int i = 0; i < worldEnts.size();i++)
    {
        if (worldEnts[i]->getType() == 1)
            worldEnts[i]->draw(shaders[5],false);
    }
    shaders[7].use();
    shaders[7].setMat4("lightSpaceMatrix", lightSpaceMatrix);
    for (int i = 0; i < ents.size();i++)
        renderInstanced(ents[i],shaders[7]);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, scrWidth, scrHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
}

void Renderer::addShader(Shader &shader) {
    shaders.push_back(shader);
}

void Renderer::removeShader(int shaderId) {
    std::vector<Shader>::iterator  it;
    for (it = shaders.begin(); it < shaders.end(); it++)
        if (it->ID == shaderId)
            shaders.erase(it);
}

Renderer::~Renderer() {
    std::cout<<"Destroying renderer"<<std::endl;
    glDeleteVertexArrays(1,&quadVAO);
    glDeleteVertexArrays(1,&skyboxVAO);
    glDeleteBuffers(1,&quadVBO);
    glDeleteBuffers(1,&skyboxVBO);
    glDeleteTextures(1,&cubemapTexture);
    glDeleteFramebuffers(1,&depthMap);
    glDeleteFramebuffers(1,&depthMapFBO);
    for (auto shader: shaders)
        glDeleteShader(shader.ID);
}



