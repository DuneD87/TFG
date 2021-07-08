//
// Created by drive on 8/7/21.
//

#include "Scene.h"


Scene::Scene(const char * path,unsigned int scrWidth, unsigned int scrHeight, Camera &camera) {
    this->scrWidth = scrWidth;
    this->scrHeight = scrHeight;
    this->camera = camera;
    spotLight =  Light("spotLight",glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(1.0f, 1.0f, 1.0f),
                                 glm::vec3(1.0f, 1.0f, 1.0f),1.0,0.09,0.032);
    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightShader("../Shaders/lightingShader.vs", "../Shaders/lightingShader.fs");
    Shader spriteShader("../Shaders/lightingShader.vs","../Shaders/alphaTextureTest.fs");
    Shader outlineShader = Shader("../Shaders/lightingShader.vs", "../Shaders/singleColorShader.fs");
    Shader screenShader = Shader("../Shaders/PostProcess/screenShader.vs","../Shaders/PostProcess/screenShader.fs");
    shaders.push_back(lightShader);
    shaders.push_back(spriteShader);
    shaders.push_back(outlineShader);
    shaders.push_back(screenShader);
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    XmlParser parser(path);
    models = parser._models;
    lights = parser._lights;
    effects = parser._effects;
    setupFrameBuffer();
}

void Scene::renderScene() {
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    // set uniforms
    shaders[2].use();
    renderLoopCamera(shaders[2]);
    shaders[1].use();
    renderLoopCamera(shaders[1]);
    shaders[0].use();
    renderLoopCamera(shaders[0]);
    shaders[0].setFloat("material.shininess", 64.0f);
    shaders[0].setVec3("viewPos",camera.Position);
    shaders[0].addLights(lights);

    //Draw
    std::vector<int> selectedeItems;
    for (int i = 0; i < models.size();i++)
    {
        models[i].Draw(shaders[0],false);

    }
    shaders[1].use();
    for (int i = 0; i < effects.size();i++)
    {
        //Render should be a function, handle semi-transparent objects sorting them to draw in order ( pre-render function? )
        effects[i].Draw(shaders[1],false);
    }
    //drawables[1].outlineObject(outlineShader,glm::vec3(1.1));
    for (int i = 0; i < selectedeItems.size();i++)
        models[selectedeItems[i]].outlineObject(shaders[2],glm::vec3(1.1));

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

void Scene::renderLoopCamera(Shader shader) {
    //view
    glm::mat4 view;
    view = this->camera.GetViewMatrix();
    shader.setMat4("view",view);

    //projection
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(this->camera.Zoom), (float)scrWidth/scrHeight, 0.1f, 100.0f);
    shader.setMat4("projection",projection);
    glm::mat4 cameraModel(1.0f);
    shader.setMat4("model", cameraModel);
}

void Scene::setupFrameBuffer() {
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
}

void Scene::setPostProcess(unsigned int index) {
    shaders[3] = Shader("../Shaders/PostProcess/screenShader.vs",postProcessPath[index].c_str());
}
