
#define STB_IMAGE_IMPLEMENTATION
#include <reactphysics3d/reactphysics3d.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "GameClasses/Renderer.h"
#include "GameClasses/World.h"
#include "libs/imgui.h"
#include "libs/imgui_impl_glfw.h"
#include "libs/imgui_impl_opengl3.h"

// settings
 unsigned int SCR_WIDTH = 1920;
 unsigned int SCR_HEIGHT = 1080;
bool spotLightEnabled = false, enableCursor = true,fullScreen = true;
auto *cam = new Camera(glm::vec3(3000.0f, 1000.0f, 3000.0f));
float lastX = SCR_WIDTH/2, lastY = SCR_HEIGHT/2,pitch = 0, yaw = -90, fov = 45;



float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_RELEASE) return; //only handle press events
    if(key == GLFW_KEY_L) spotLightEnabled = !spotLightEnabled;
}

void processInput(GLFWwindow *window,World *world)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam->ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window,GLFW_KEY_SPACE) == GLFW_PRESS)
        cam->ProcessKeyboard(UP,deltaTime);
    if (glfwGetKey(window,GLFW_KEY_C) == GLFW_PRESS)
        cam->ProcessKeyboard(DOWN,deltaTime);
    if (glfwGetKey(window,GLFW_KEY_Q) == GLFW_PRESS)
        cam->ProcessKeyboard(RLEFT,deltaTime);
    if (glfwGetKey(window,GLFW_KEY_E) == GLFW_PRESS)
        cam->ProcessKeyboard(RRIGHT,deltaTime);
    if (glfwGetKey(window,GLFW_KEY_C) == GLFW_PRESS)
        cam->ProcessKeyboard(DOWN,deltaTime);

    if (glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cam->MovementSpeed += 10;

    if (glfwGetKey(window,GLFW_KEY_0) == GLFW_PRESS)
        world->getRenderer()->setPostProcess(5);
    if (glfwGetKey(window,GLFW_KEY_1) == GLFW_PRESS)
        world->getRenderer()->setPostProcess(0);
    if (glfwGetKey(window,GLFW_KEY_2) == GLFW_PRESS)
        world->getRenderer()->setPostProcess(1);
    if (glfwGetKey(window,GLFW_KEY_3) == GLFW_PRESS)
        world->getRenderer()->setPostProcess(2);
    if (glfwGetKey(window,GLFW_KEY_4) == GLFW_PRESS)
        world->getRenderer()->setPostProcess(3);
    if (glfwGetKey(window,GLFW_KEY_5) == GLFW_PRESS)
        world->getRenderer()->setPostProcess(4);

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    if (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_2))
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        enableCursor = false;
    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        enableCursor = true;

    }
    lastX = xpos;
    lastY = ypos;
    if (!enableCursor)
        cam->ProcessMouseMovement(xoffset, yoffset,true);

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    cam->ProcessMouseScroll(yoffset);
}

GLFWwindow * createWindow()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow* window;
    if (fullScreen)
    {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        window = glfwCreateWindow(mode->width, mode->height, "My Title", monitor, NULL);
        SCR_HEIGHT = mode->height;
        SCR_WIDTH = mode->width;
    } else
    {
        window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    }
    // glfw window creation
    // --------------------

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }

    glfwSetKeyCallback(window,key_callback);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    if (!enableCursor)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }
    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);
    //glFrontFace(GL_CW);

    return window;
}

// ---------------------------------------------------
int main()
{

    GLFWwindow *window = createWindow();
    World *world = new World("../Scenes/Scene1.xml","../Textures/SkyBox/space1/",SCR_WIDTH,SCR_HEIGHT,cam);
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----


        processInput(window, world);
        world->renderWorld();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    delete world;
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}
