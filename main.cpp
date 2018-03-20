#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "/Users/gadflyloki/Desktop/桌面潜逃/学习总览/正在学习/计算机图形学/linux/assignment/inc/shader_m.h"
#include "/Users/gadflyloki/chengxu/test/test/camera.h"
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(0.0f, 0.0f, 3.0f);

void drawQuad() {}
void drawTri()
{
    glBegin(GL_TRIANGLES);
    glColor3f(1, 1, 1);
    glVertex3f(0, 0, 1);
    glVertex3f(1, 0, 0);
    glVertex3f(0, 1, 0);
    
    glEnd();
    
    glBegin(GL_TRIANGLES);
    glColor3f(1, 1, 0);
    glVertex3f(0, 0, 1);
    glVertex3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1, 0);
    glVertex3f(1, 0, 0);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(1, 0, 1);
    glVertex3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);
    glEnd();
}
void ShowVec(const std::vector<GLfloat>& valList)
{
    for (std::vector<GLfloat>::const_iterator iter = valList.cbegin();
         iter != valList.cend(); iter++)
    {
        std::cout << (*iter) << std::endl;
    }
}

int main(int argc, char* argv[])
{
    glfwInit();
    glewExperimental = GL_TRUE;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window =
    glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "myopengl", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, (char*)"Couldn't initialize GLEW library.\n");
        exit(EXIT_FAILURE);
    }
    
    // configure global opengl state
    // -----------------------------  only depicted one layer
    glEnable(GL_DEPTH_TEST);
    
    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader(
                          "/Users/gadflyloki/chengxu/test/test/2.2.basic_lighting.vs",
                          "/Users/gadflyloki/chengxu/test/test/2.2.basic_lighting.fs");
    Shader lampShader("/Users/gadflyloki/chengxu/test/test/2.2.lamp.vs",
                      "/Users/gadflyloki/chengxu/test/test2.2.lamp.fs");
    Shader sphereShader("/Users/gadflyloki/chengxu/test/test/sphereShader.vs",
                        "/Users/gadflyloki/chengxu/test/test/sphereShader.fs");
    
    float televertices[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f};
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  -0.5f, -0.5f,
        0.0f,  0.0f,  -1.0f, 0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
        0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, -0.5f, 0.5f,  -0.5f,
        0.0f,  0.0f,  -1.0f, -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f,
        
        -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,
        0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  -0.5f, 0.5f,  0.5f,
        0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,
        
        -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  -0.5f,
        -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, 0.5f,
        -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  -0.5f,
        1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
        0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, 0.5f,
        1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        
        -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, -0.5f,
        0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
        0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, 0.5f,
        0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,
        
        -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  -0.5f,
        0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,
        0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f};
    
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindVertexArray(cubeVAO);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the
    // updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);
    
    // third
    
    int i, j;
    int lats = 40;
    int longs = 40;
    std::vector<GLfloat> vertices1;
    std::vector<GLuint> indices;
    int indicator = 0;
    int radius = 1.0;
    GLfloat hstep = 180.0 / (lats - 1);
    GLfloat wstep = 360.0 / (longs);
    GLfloat a = 0.0, b = 0.0;
    for (i = 0; i <= lats; i++, a += hstep)
    {
        for (j = 0; j <= longs; j++, b += wstep)
        {
            GLfloat x = sin(glm::radians(a)) * cos(glm::radians(b));
            GLfloat y = sin(glm::radians(a)) * sin(glm::radians(b));
            GLfloat z = cos(glm::radians(a));
            std::cout << x << " " << y << " " << z << std::endl;
            vertices1.push_back(x);
            vertices1.push_back(y);
            vertices1.push_back(z);
        }
    }
    i = 0, j = 0;
    for (i = 0; i < lats; ++i)
    {
        for (j = longs * i; j < longs * (i + 1) - 1; j++)
        {
            indices.push_back(j);
            indices.push_back(j + 1);
            indices.push_back(j + longs + 1);
            indices.push_back(j);
            indices.push_back(j + longs + 1);
            indices.push_back(j + longs);
        }
        indices.push_back(j);
        indices.push_back(j - (longs - 1));
        indices.push_back(j + 1);
        indices.push_back(j);
        indices.push_back(j + 1);
        indices.push_back(j + lats);
    }
    // create and bind buffer
    unsigned int svbo, svao, ebo, numsToDraw;
    glGenVertexArrays(1, &svao);
    glBindVertexArray(svao);
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLint),
                 &indices[0], GL_STATIC_DRAW);
    glGenBuffers(1, &svbo);
    glBindBuffer(GL_ARRAY_BUFFER, svbo);
    glBufferData(GL_ARRAY_BUFFER, vertices1.size() * sizeof(GLfloat),
                 &vertices1[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);
    //    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //    ShowVec(vertices);
    numsToDraw = indices.size();
    // teleshaderstart
    unsigned int tvao, tvbo;
    glGenVertexArrays(1, &tvao);
    glBindVertexArray(tvao);
    glGenBuffers(1, &tvbo);
    glBindBuffer(GL_ARRAY_BUFFER, tvbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(televertices), televertices,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);
    // teleshaderend
    glewExperimental = GL_TRUE;
    
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();
    glEnable(GL_MULTISAMPLE);
    
    //    initPMV();
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("objectColor", 1.0f, 0.8f, 0.31f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("lightPos", lightPos);
        lightingShader.setVec3("viewPos", camera.Position);
        
        // view/projection transformations
        glm::mat4 projection = glm::perspective(
                                                glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT,
                                                0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        
        // world transformation
        glm::mat4 model;
        
        lightingShader.setMat4("model", model);
        
        // render the cube
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        // also draw the lamp object
        lampShader.use();
        lampShader.setMat4("projection", projection);
        view = camera.GetViewMatrix();
        lampShader.setMat4("view", view);
        
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(0.0f));
        model = glm::scale(model, glm::vec3(1.0f));  // a smaller cube
        lampShader.setMat4("model", model);
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        // drawTri();
        //        drawSphere();
        
        glm::vec3 spherePos(0.0f, 1.0f, 0.0f);
        sphereShader.use();
        sphereShader.setMat4("projection", projection);
        sphereShader.setMat4("view", view);
        model = glm::mat4();
        model = glm::translate(model, spherePos);
        model = glm::scale(model, glm::vec3(1.0f));  // a smaller cube
        sphereShader.setMat4("model", model);
        glBindVertexArray(svao);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLE_STRIP, 6 * 40 * 40, GL_UNSIGNED_INT, 0);
        
        // simianti---begin
        //        spherePos=glm::vec3(0.0f, -1.0f, 0.0f);
        //        sphereShader.use();
        //        model = glm::scale(model, glm::vec3(1.0f));
        //        glBindVertexArray(tvao);
        //        glDrawArrays(GL_TRIANGLES, 0, 36);
        // simianti---end
        //
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &svao);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width
    // and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset =
    lastY - ypos;  // reversed since y-coordinates go from bottom to top
    
    lastX = xpos;
    lastY = ypos;
    
    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}


