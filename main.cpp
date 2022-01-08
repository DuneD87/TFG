
#define STB_IMAGE_IMPLEMENTATION
#include <reactphysics3d/reactphysics3d.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "GameClasses/Base/Renderer.h"
#include "GameClasses/Base/World.h"
#include "GameClasses/Util/libs/imgui.h"
#include "GameClasses/Util/libs/imgui_impl_glfw.h"
#include "GameClasses/Util/libs/imgui_impl_opengl3.h"

// settings
 unsigned int SCR_WIDTH = 1920;
 unsigned int SCR_HEIGHT = 1080;
bool spotLightEnabled = false, enableCursor = true,fullScreen = true,wireframe=false;
auto *cam = new Camera(glm::vec3(6000.0f, 1000.0f, 6000.0f));
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
    else if (key == GLFW_KEY_N) wireframe = !wireframe;
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
        cam->MovementSpeed += 50;
    if (glfwGetKey(window,GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        cam->MovementSpeed -= 50;
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

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4.5);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4.5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }

    return window;
}

// ---------------------------------------------------
int main()
{

    GLFWwindow *window = createWindow();

    stbi_set_flip_vertically_on_load(true);
    glPatchParameteri(GL_PATCH_VERTICES,3);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    GLint MaxPatchVertices = 0;
    glGetIntegerv(GL_MAX_PATCH_VERTICES, &MaxPatchVertices);
    printf("Max supported patch vertices %d\n", MaxPatchVertices);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    World *world = new World("../Scenes/Scene4.xml","../Textures/SkyBox/SpaceHres/",SCR_WIDTH,SCR_HEIGHT,cam);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");

    while (!glfwWindowShouldClose(window))
    {
        processInput(window, world);
        if (wireframe)
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        else
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        world->renderWorld(wireframe);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        cam->deltaTime = deltaTime;
        lastFrame = currentFrame;
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    delete world;
    glfwTerminate();
    return 0;
}
