//
// Created by drive on 8/7/21.
//

#include "../Util/libs/imgui_impl_opengl3.h"
#include "../Util/libs/imgui_impl_glfw.h"
#include "Renderer.h"
#include "../Entities/Planet/Planet.h"

Renderer::Renderer() {

}

Renderer::Renderer(unsigned int scrWidth, unsigned int scrHeight, Camera *camera, const char* skyboxPath) {
    std::cout<<"Resolution: "<<scrWidth<<"x"<<scrHeight<<std::endl;
    this->camera = camera;
    sunDir = glm::vec3(-1);
    spotLight =  Light("spotLight",glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(1.0f, 1.0f, 1.0f),
                                 glm::vec3(1.0f, 1.0f, 1.0f),1.0,0.09,0.032,-1,-1);
    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightShader("../Shaders/lightingShaderVertex.shader",
                       "../Shaders/lightingShaderFragment.shader",
                       "../Shaders/lightingShaderTCS.shader",
                       "../Shaders/lightingShaderTES.shader");
    /*Shader lightShader("../Shaders/lightingShaderVertex.shader",
                       "../Shaders/lightingShaderFragment.shader");*/
    Shader spriteShader("../Shaders/lightingShaderVertex.shader","../Shaders/alphaTextureTest.fs");
    Shader outlineShader = Shader("../Shaders/lightingShaderVertex.shader", "../Shaders/singleColorShader.fs");
    Shader screenShader = Shader("../Shaders/PostProcess/screenShader.vs","../Shaders/PostProcess/screenShader.fs");
    Shader skyboxShader = Shader("../Shaders/skyboxShader.vs","../Shaders/skyboxShader.fs");
    Shader depthSMapShader = Shader("../Shaders/depthShadowMappingVertex.shader","../Shaders/depthShadowMappingFragment.shader");
    Shader lightInstancedShader("../Shaders/lightingShaderInstancedVertex.shader", "../Shaders/lightingShaderFragment.shader");
    Shader depthSMapInstancedShader("../Shaders/depthShadowMappingInstancedVertex.shader", "../Shaders/depthShadowMappingFragment.shader");
    Shader waterShader("../Shaders/waterShaderVertex.shader", "../Shaders/waterShaderFragment.shader");
    shaders.push_back(lightShader);//0
    shaders.push_back(spriteShader);//1
    shaders.push_back(outlineShader);//2
    shaders.push_back(screenShader);//3
    shaders.push_back(skyboxShader);//4
    shaders.push_back(depthSMapShader);//5
    shaders.push_back(lightInstancedShader);//6
    shaders.push_back(depthSMapInstancedShader);//7
    shaders.push_back(waterShader);//8
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    setupFrameBuffer();
    setupSkyBox(skyboxPath);
}

void Renderer::renderSkybox(glm::mat4 view, glm::mat4 projection) {
    glDepthFunc(GL_LEQUAL);
    shaders[4].use();
    renderLoopCamera(shaders[4],view,projection,true);
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default*/
}

void Renderer::drawEntities(std::vector<Entity*> worldEnts, glm::mat4 view, glm::mat4 projection,Shader &shader, bool drawEffects) {

    for (int i = 0; i < worldEnts.size();i++)
    {
        if (worldEnts[i]->getType() != 4)
        {
            worldEnts[i]->entityShader.use();
            renderLoopCamera(worldEnts[i]->entityShader,view,projection);
            worldEnts[i]->entityShader.setFloat("material.shininess", 64.0f);
            worldEnts[i]->entityShader.setVec3("viewPos",camera->Position);
            worldEnts[i]->entityShader.setVec3("lightPos", sunPos);
            worldEnts[i]->entityShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
            worldEnts[i]->entityShader.setInt("nPointLights",nPointLights);
            worldEnts[i]->draw();
            if (worldEnts[i]->getType() == 3)
            {

                Planet * planet = dynamic_cast<Planet *>(worldEnts[i]);
                planet->renderGui();
                planet->setWaterTexture(reflexion);
                planet->setNoDrawEffects(drawEffects);
            }
        }
        else
        {
            shaders[8].use();
            renderLoopCamera(shaders[8],view,projection);
            worldEnts[i]->draw(shaders[8],false,-1);
            shader.use();
            renderLoopCamera(shader,view,projection);

        }

    }
}

void Renderer::render(vector<Entity *> worldEnts, std::vector<std::pair<std::vector<glm::mat4>, PhysicsObject *>> ents,
                      bool wireframe) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    //Reflexion framebuffer pass and render to window
    glm::mat4 view;
    camera->invertCameraPitch();
    glm::vec3 oldCamPos = this->camera->Position;
    this->camera->Position = this->camera->Position + (this->camera->Up * -100.f);
    view = this->camera->GetViewMatrix();
    //projection
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(this->camera->Zoom), (float)800/600, setting_near, setting_far);
    glBindFramebuffer(GL_FRAMEBUFFER,reflexionFBO);
    renderScene(worldEnts,ents,wireframe,view,projection,800,600,false);
    ImGui::Begin("Game Window");
    ImGui::SetWindowFontScale(setting_fontSize);
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddImage((void*)reflexion,
                       pos,
                       ImVec2(pos.x + 800, pos.y + 600),
                       ImVec2(0, 1),
                       ImVec2(1, 0));
    ImGui::End();
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    camera->invertCameraPitch();
    this->camera->Position = oldCamPos;
    view = this->camera->GetViewMatrix();
    //Shadowmap framebuffer pass and render to window
    glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
    renderShadowMap(worldEnts,ents);
    ImGui::Begin("Game Window 2");
    ImGui::SetWindowFontScale(setting_fontSize);
    pos = ImGui::GetCursorScreenPos();
    ImDrawList* drawList2 = ImGui::GetWindowDrawList();
    drawList2->AddImage((void*)depthMap,
                       pos,
                       ImVec2(pos.x + 1024, pos.y + 720),
                       ImVec2(0, 1),
                       ImVec2(1, 0));
    ImGui::End();
    glBindFramebuffer(GL_FRAMEBUFFER,0);

    //render to main screen
    if (!wireframe)
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    projection = glm::perspective(glm::radians(this->camera->Zoom), (float)setting_scrWidth/setting_scrHeight, setting_near, setting_far);
    renderScene(worldEnts,ents,wireframe,view,projection,setting_scrWidth,setting_scrHeight,true);
    if (!wireframe)
    {
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
}


void Renderer::renderScene(vector<Entity*> worldEnts,std::vector<std::pair<std::vector<glm::mat4>,PhysicsObject*>> ents,
                           bool wireframe,glm::mat4 &view, glm::mat4 &projection, float width, float height, bool drawEffects) {


    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    renderSkybox(view,projection);
    drawEntities(worldEnts,view,projection,shaders[0],!drawEffects);
    // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture

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

void Renderer::renderLoopCamera(Shader shader,glm::mat4 view, glm::mat4 projection,bool skybox) {
    //view
    if (skybox)
        view = glm::mat4(glm::mat3(camera->GetViewMatrix())); // remove translation from the view matrix
    shader.setMat4("view",view);
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, setting_scrWidth, setting_scrHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textColorBuffer, 0);

    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, setting_scrWidth, setting_scrHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
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

    glGenFramebuffers(1, &reflexionFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, reflexionFBO);

    glGenTextures(1, &reflexion);
    glBindTexture(GL_TEXTURE_2D, reflexion);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, reflexion, 0);

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
        std::string(path)+"bottom"+format,
        std::string(path)+"top"+format,
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
    glViewport(0, 0, setting_scrWidth, setting_scrHeight);
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



void Renderer::preRender(vector<Entity *> worldEnts) {
    for (auto ent: worldEnts)
    {
        if (ent->getType() == 3)
        {
            dynamic_cast<Planet*>(ent)->setSun(sun);
        }
    }
}

void Renderer::addSun(Light *sunLight) {
    this->sun = sunLight;
}






