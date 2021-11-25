
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "learnopengl/shader.h"
#include "learnopengl/camera.h"
#include "learnopengl/model.h"

#include <iostream>
#include <math.h>

#define PI 3.1415926
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);
unsigned int loadCubemap(vector<std::string> faces);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
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

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader grassShader("resources/shader/grassGround.vs","resources/shader/grassGround.fs");
    Shader ourShader("resources/shader/1.model_loading.vs", "resources/shader/1.model_loading.fs");
    Shader shader("resources/shader/6.1.cubemaps.vs", "resources/shader/6.1.cubemaps.fs");
    Shader skyboxShader("resources/shader/skyboxShader.vs", "resources/shader/skyboxShader.fs");
    Shader horse1Shader("resources/shader/horse1.vs", "resources/shader/horse1.fs");
    Shader man1Shader("resources/shader/man1.vs","resources/shader/man1.fs");
    Shader man2Shader("resources/shader/man2.vs", "resources/shader/man2.fs");
    Shader man3Shader("resources/shader/man3.vs", "resources/shader/man3.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
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

    Model ourModel("resources/Horse/10026_Horse_v01_it2.obj");
    Model horse1Model("resources/Horse/10026_Horse_v01_it2.obj");
    //Model ourModel("resources/stickman/stickman.OBJ");
    Model grassGroundModel("resources/grass/10450_Rectangular_Grass_Patch_v1_iterations-2.obj");
    Model man1Model("resources/stickman/stickman.OBJ");
    Model man2Model("resources/stickman/stickman.OBJ");
    Model man3Model("resources/stickman/stickman.OBJ");

    // cube VAO
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // load textures
    // -------------
    unsigned int cubeTexture = loadTexture("resources/textures/texture.jpeg");

    vector<std::string> faces
    {
        "resources/textures/skybox/right.jpg",
        "resources/textures/skybox/left.jpg",
        "resources/textures/skybox/top.jpg",
        "resources/textures/skybox/bottom.jpg",
        "resources/textures/skybox/front.jpg",
        "resources/textures/skybox/back.jpg"
    };
    unsigned int cubemapTexture = loadCubemap(faces);

    // shader configuration
    // --------------------
    shader.use();
    shader.setInt("texture1", 0);

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);


    glm::vec3 p1 = glm::vec3(-40.0f, 0.0f, -40.0f);
    glm::vec3 p2 = glm::vec3(-40.0f, 0.0f, 40.0f);
    glm::vec3 p3 = glm::vec3(40.0f, 0.0f, 40.0f);
    glm::vec3 p4 = glm::vec3(40.0f, 0.0f, -40.0f);
    glm::vec3 p5 = glm::vec3(-40.0f, 0.0f, -40.0f);
    // render loop
    // -----------
    float finaltime = 0.0f;
    float firstTime = 0.0f;
    float stopTime = 8.0f;
    //glm::vec3 lastPosi = p1;
    float lastx = p1.x;
    float lasty = p1.y;
    float lastz = p1.z;

    glm::vec3 lightPosition = glm::vec3(1.0f,15.0f,0.0f);
    float tFrame = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        tFrame = tFrame + deltaTime;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        //transform = glm::translate(transform, glm::vec3(0.9f, -0.9f, 0.9f));
        //transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));

        // get matrix's uniform location and set matrix
        
        // draw scene as normal





        //horse1Shader.use();
        
        firstTime += deltaTime;
        if (firstTime > stopTime) {
            firstTime = firstTime - stopTime;
        }
        finaltime = firstTime / stopTime;
        

        //glm::vec3 posi = (1 - tFrame) * (1 - tFrame) * p1 + 2 * (tFrame) * (1 - tFrame) * p2 + tFrame * tFrame * p3;
        //glm::vec3 deltaPosi = posi - lastPosi;
        //lastPosi = posi;
        float posix = pow((1 - finaltime),4) * p1.x + 4 * pow((1 - finaltime), 3) * finaltime * p2.x + 6 * pow((finaltime), 2) * pow((1 - finaltime),2) * p3.x + 4 * pow((finaltime), 3) * (1 - finaltime) * p4.x + pow((finaltime), 4) * p5.x;
        float posiy = pow((1 - finaltime),4) * p1.y + 4 * pow((1 - finaltime), 3) * finaltime * p2.y + 6 * pow((finaltime), 2) * pow((1 - finaltime),2) * p3.y + 4 * pow((finaltime), 3) * (1 - finaltime) * p4.y + pow((finaltime), 4) * p5.y;
        float posiz = pow((1 - finaltime),4) * p1.z + 4 * pow((1 - finaltime), 3) * finaltime * p2.z + 6 * pow((finaltime), 2) * pow((1 - finaltime),2) * p3.z + 4 * pow((finaltime), 3) * (1 - finaltime) * p4.z + pow((finaltime), 4) * p5.z;
        float deltax = posix - lastx;
        float deltay = posiy - lasty;
        float deltaz = posiz - lastz;
        shader.use();
        glm::mat4 model = glm::mat4(1.0f);
       // model = glm::translate(model, deltaPosi);
       // model = glm::translate(model, glm::vec3(deltax, deltay, deltaz));
        model = glm::translate(model, glm::vec3(5.0f, -0.5f, 0.5f));
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        // cubes
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        man1Shader.use();
        glm::mat4 man1model = glm::mat4(1.0f);
        glm::mat4 man1view = camera.GetViewMatrix();
        glm::mat4 man1projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        man1model = glm::translate(man1model, glm::vec3(-2.0f, -1.5f, -10.0f)); // translate it down so it's at the center of the scene
        //man1model = glm::rotate(man1model, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
        man1model = glm::scale(man1model, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down
        man1Shader.setMat4("view", man1view);
        man1Shader.setMat4("projection", man1projection);
        man1Shader.setVec3("lightPosition", lightPosition);
        man1Shader.setVec3("viewPos", camera.Position);
        man1Shader.setMat4("model", man1model);
        man1Model.Draw(man1Shader);

        man2Shader.use();
        glm::mat4 man2model = glm::mat4(1.0f);
        glm::mat4 man2view = camera.GetViewMatrix();
        glm::mat4 man2projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        if (tFrame > 16.0f && tFrame < 18.0f) {
            man2model = glm::translate(man2model, glm::vec3(0.0f, -3.5f, -10.0f)); // translate it down so it's at the center of the scene
            man2model = glm::scale(man2model, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down
        }
        else if (tFrame > 24.0f && tFrame < 26.0f) {
            man2model = glm::translate(man2model, glm::vec3(0.0f, 3.5f, -10.0f)); // translate it down so it's at the center of the scene
            man2model = glm::scale(man2model, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down
        }
        else if (tFrame > 32.0f && tFrame < 34.0f) {
            man2model = glm::translate(man2model, glm::vec3(0.0f, 3.5f, -10.0f)); // translate it down so it's at the center of the scene
            man2model = glm::scale(man2model, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down
        }
        else {
            man2model = glm::translate(man2model, glm::vec3(0.0f, -0.5f, -10.0f)); // translate it down so it's at the center of the scene
            man2model = glm::scale(man2model, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down
        }

        man2Shader.setMat4("view", man2view);
        man2Shader.setMat4("projection", man2projection);
        man2Shader.setVec3("lightPosition", lightPosition);
        man2Shader.setVec3("viewPos", camera.Position);
        man2Shader.setMat4("model", man2model);
        man2Model.Draw(man2Shader);

        man3Shader.use();
        glm::mat4 man3model = glm::mat4(1.0f);
        glm::mat4 man3view = camera.GetViewMatrix();
        glm::mat4 man3projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        if (tFrame > 16.0f && tFrame < 18.0f) {
            man3model = glm::translate(man3model, glm::vec3(3.0f, 3.3f, -10.0f)); // translate it down so it's at the center of the scene
            man3model = glm::scale(man3model, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down
        }
        else if (tFrame > 24.0f && tFrame < 26.0f) {
            man3model = glm::translate(man3model, glm::vec3(3.0f, -1.3f, -10.0f)); // translate it down so it's at the center of the scene
            man3model = glm::scale(man3model, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down
        }
        else if (tFrame > 32.0f && tFrame < 34.0f) {
            man3model = glm::translate(man3model, glm::vec3(3.0f, -1.3f, -10.0f)); // translate it down so it's at the center of the scene
            man3model = glm::scale(man3model, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down
        }
        else {
            man3model = glm::translate(man3model, glm::vec3(3.0f, -0.3f, -10.0f)); // translate it down so it's at the center of the scene
            man3model = glm::scale(man3model, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down
        }

        man3Shader.setMat4("view", man3view);
        man3Shader.setMat4("projection", man3projection);
        man3Shader.setVec3("lightPosition", lightPosition);
        man3Shader.setVec3("viewPos", camera.Position);
        man3Shader.setMat4("model", man3model);
        man3Model.Draw(man3Shader);

        ourShader.use();
        glm::mat4 modelk = glm::mat4(1.0f);
        //float ABlength = sqrt(deltax * deltax + deltaz * deltaz);
        float angle = finaltime * 2 * PI;
        glm::mat4 ourview = camera.GetViewMatrix();
        glm::mat4 ourprojection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        modelk = glm::translate(modelk, glm::vec3(deltax, deltay, deltaz));
        modelk = glm::rotate(modelk, (angle), glm::vec3(0.0f, 1.0f, 0.0f));
        if (tFrame < 16.0f) {
            modelk = glm::translate(modelk, glm::vec3(0.0f, -2.4f, 0.0f)); // translate it down so it's at the center of the scene
            modelk = glm::rotate(modelk, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
            modelk = glm::scale(modelk, glm::vec3(0.003f, 0.003f, 0.003f));	// it's a bit too big for our scene, so scale it down
        }
        else if (tFrame < 24.0f) {
            modelk = glm::translate(modelk, glm::vec3(0.0f, -0.4f, 0.0f)); // translate it down so it's at the center of the scene
            modelk = glm::rotate(modelk, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
            modelk = glm::scale(modelk, glm::vec3(0.006f, 0.006f, 0.006f));	// it's a bit too big for our scene, so scale it down
        }
        else if (tFrame < 32.0f) {
            modelk = glm::translate(modelk, glm::vec3(0.0f, -0.4f, 0.0f)); // translate it down so it's at the center of the scene
            modelk = glm::rotate(modelk, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
            modelk = glm::scale(modelk, glm::vec3(0.004f, 0.004f, 0.004f));	// it's a bit too big for our scene, so scale it down
        }
        else {
            modelk = glm::translate(modelk, glm::vec3(0.0f, -0.4f, 0.0f)); // translate it down so it's at the center of the scene
            modelk = glm::rotate(modelk, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
            modelk = glm::scale(modelk, glm::vec3(0.004f, 0.004f, 0.004f));	// it's a bit too big for our scene, so scale it down
        }

        ourShader.setMat4("view", ourview);
        ourShader.setMat4("projection", ourprojection);
        ourShader.setVec3("lightPosition", lightPosition);
        ourShader.setVec3("viewPos", camera.Position);
        ourShader.setMat4("model", modelk);
        ourModel.Draw(ourShader);

        horse1Shader.use();
        glm::mat4 horse1model = glm::mat4(1.0f);
        glm::mat4 horse1view = camera.GetViewMatrix();
        glm::mat4 horse1projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        float horse1angle = finaltime * 2 * PI;
        horse1model = glm::translate(horse1model, glm::vec3(deltax, deltay, deltaz));
        horse1model = glm::rotate(horse1model, (horse1angle), glm::vec3(0.0f, 1.0f, 0.0f));
        if (tFrame < 16.0f) {
            horse1model = glm::translate(horse1model, glm::vec3(0.0f, -0.4f, 10.0f)); // translate it down so it's at the center of the scene
            horse1model = glm::rotate(horse1model, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
            horse1model = glm::scale(horse1model, glm::vec3(0.009f, 0.009f, 0.009f));	// it's a bit too big for our scene, so scale it down
        }
        else if (tFrame < 24.0f) {
            horse1model = glm::translate(horse1model, glm::vec3(0.0f, -2.4f, 10.0f)); // translate it down so it's at the center of the scene
            horse1model = glm::rotate(horse1model, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
            horse1model = glm::scale(horse1model, glm::vec3(0.008f, 0.008f, 0.008f));	// it's a bit too big for our scene, so scale it down
        }
        else if (tFrame < 32.0f) {
            horse1model = glm::translate(horse1model, glm::vec3(0.0f, -2.4f, 10.0f)); // translate it down so it's at the center of the scene
            horse1model = glm::rotate(horse1model, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
            horse1model = glm::scale(horse1model, glm::vec3(0.005f, 0.005f, 0.005f));	// it's a bit too big for our scene, so scale it down
        }
        else {
            horse1model = glm::translate(horse1model, glm::vec3(0.0f, -2.4f, 10.0f)); // translate it down so it's at the center of the scene
            horse1model = glm::rotate(horse1model, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
            horse1model = glm::scale(horse1model, glm::vec3(0.005f, 0.005f, 0.005f));	// it's a bit too big for our scene, so scale it down
        }

        horse1Shader.setMat4("view", horse1view);
        horse1Shader.setMat4("projection", horse1projection);
        horse1Shader.setVec3("lightPosition", lightPosition);
        horse1Shader.setVec3("viewPos", camera.Position);
        horse1Shader.setMat4("model", horse1model);
        horse1Model.Draw(horse1Shader);


        grassShader.use();
        glm::mat4 grassview = camera.GetViewMatrix();
        glm::mat4 grassprojection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 grassmodel = glm::mat4(1.0f);
        grassmodel = glm::translate(grassmodel, glm::vec3(0.0f, -10.0f, 0.0f));
        grassmodel = glm::rotate(grassmodel, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
        //grassmodel = glm::rotate(grassmodel, glm::vec3(0.0f, 0.0f, 1.0f));
        //grassmodel = glm::translate(grassmodel, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        //grassmodel = glm::scale(grassmodel, glm::vec3(4.0f, 4.0f, 4.0f));	// it's a bit too big for our scene, so scale it down
        grassShader.setMat4("model", grassmodel);
        grassShader.setMat4("view", grassview);
        grassShader.setMat4("projection", grassprojection);
        grassGroundModel.Draw(grassShader);
        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &skyboxVBO);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
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

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

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

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
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