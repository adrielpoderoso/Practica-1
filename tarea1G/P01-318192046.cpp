#include <stdio.h>
#include <cstdlib> // Para rand()
#include <ctime>   // Para inicializar la semilla aleatoria
#include <glew.h>
#include "GLFW/glfw3.h"
#include "variables.h"
#include "segundo_main.h"

GLuint VAO, VBO, shader;


GLuint VAO_H, VBO_H;
GLuint VAO_T, VBO_T;
GLuint VAO_L, VBO_L;
const int WIDTH = 800, HEIGHT = 600;
float bgColor[3] = {0.0f, 0.0f, 0.0f};
double lastTime = 0.0;

// Vertex Shader
static const char* vShader = "#version 330 core\n"
"layout (location = 0) in vec3 pos;\n"
"void main() {\n"
"   gl_Position = vec4(pos, 1.0);\n"
"}";

// Fragment Shader
static const char* fShader = "#version 330 core\n"
"out vec4 color;\n"
"void main() {\n"
"   color = vec4(1.0, 1.0, 1.0, 1.0);\n"
"}";

void CrearFiguras() {
    GLfloat verticesH[] = {
        // Pierna izquierda
        -0.6f,  0.3f, 0.0f, -0.55f, -0.3f, 0.0f, -0.6f, -0.3f, 0.0f,
        -0.6f,  0.3f, 0.0f, -0.55f,  0.3f, 0.0f, -0.55f, -0.3f, 0.0f,
        // Pierna derecha
        -0.45f,  0.3f, 0.0f, -0.4f, -0.3f, 0.0f, -0.45f, -0.3f, 0.0f,
        -0.45f,  0.3f, 0.0f, -0.4f,  0.3f, 0.0f, -0.4f, -0.3f, 0.0f,
        // Barra media bien alineada
        -0.6f,  0.0f, 0.0f, -0.4f,  0.00f, 0.0f, -0.4f, -0.05f, 0.0f,
        -0.6f,  0.0f, 0.0f, -0.6f, -0.05f, 0.0f, -0.4f, -0.05f, 0.0f
        
        
    };
    glGenVertexArrays(1, &VAO_H);
    glGenBuffers(1, &VBO_H);
    glBindVertexArray(VAO_H);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_H);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesH), verticesH, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    
    
    GLfloat verticesT[] = {
           // Barra superior de la T
           -0.2f,  0.3f, 0.0f,  0.2f,  0.3f, 0.0f,  0.2f,  0.25f, 0.0f,
           -0.2f,  0.3f, 0.0f, -0.2f,  0.25f, 0.0f,  0.2f,  0.25f, 0.0f,
           // Cuerpo vertical de la T
           -0.025f, 0.3f, 0.0f,  -0.025f,  -0.3f, 0.0f,  0.025f, -0.3f, 0.0f,
           0.025f,  -0.3f, 0.0f,  -0.025f,  0.3f, 0.0f,  0.025f, 0.3f, 0.0f,

       };
       glGenVertexArrays(1, &VAO_T);
       glGenBuffers(1, &VBO_T);
       glBindVertexArray(VAO_T);
       glBindBuffer(GL_ARRAY_BUFFER, VBO_T);
       glBufferData(GL_ARRAY_BUFFER, sizeof(verticesT), verticesT, GL_STATIC_DRAW);
       glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
       glEnableVertexAttribArray(0);
       glBindVertexArray(0);
    
    GLfloat verticesL[] = {
        //Barra vertical de la L
        0.4f,  0.3f, 0.0f,  0.4f,  -0.3f, 0.0f,  0.45f,  -0.3f, 0.0f,
        0.4f,  0.3f, 0.0f,  0.45f,  0.3f, 0.0f,  0.45f,  -0.3f, 0.0f,
        //Barra horizontal de la L
        0.45f,  -0.25f, 0.0f,  0.55f,  -0.25f, 0.0f,  0.45f,  -0.3f, 0.0f,
        0.45f,  -0.3f, 0.0f,  0.55f,  -0.25f, 0.0f,  0.55f,  -0.3f, 0.0f,
    };
    glGenVertexArrays(1, &VAO_L);
    glGenBuffers(1, &VBO_L);
    glBindVertexArray(VAO_L);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_L);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesL), verticesL, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}


int main() {
    srand(time(0));
    if (!glfwInit()) {
        glfwTerminate();
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Ventana OpenGL", NULL, NULL);
    if (!mainWindow) {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(mainWindow);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }
    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_DEPTH_TEST); // Activar buffer de profundidad
    CrearFiguras();
    CompileShaders();
    glUseProgram(shader);
    lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(mainWindow)) {
        glfwPollEvents();
        double currentTime = glfwGetTime();
        if (currentTime - lastTime >= 2.0) {
            bgColor[0] = static_cast<float>(rand()) / RAND_MAX;
          //bgColor[0] = static_cast<float>(rand(1200)) / 32767; VALOR 0.36
            bgColor[1] = static_cast<float>(rand()) / RAND_MAX;
          //bgColor[0] = static_cast<float>(rand(25000)) / 32767; VALOR 0.76
            bgColor[2] = static_cast<float>(rand()) / RAND_MAX;
          //bgColor[0] = static_cast<float>(rand(10000)) / 32767; VALOR 0.30
            lastTime = currentTime;
        }
        glClearColor(bgColor[0], bgColor[1], bgColor[2], 1.0f);
      //glClearColor(0.36, 0.76,     0.30,         1.0f, 1.0f); VERDE AMARILLENTO
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// LIMPIAR COLOR
        glUseProgram(shader);
        glBindVertexArray(VAO_H);
        glDrawArrays(GL_TRIANGLES, 0, 18);
        glBindVertexArray(VAO_T);
        glDrawArrays(GL_TRIANGLES, 0, 12);
        glBindVertexArray(VAO_L);
        glDrawArrays(GL_TRIANGLES, 0, 12);
        glBindVertexArray(0);
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            printf("Error de OpenGL: %d\n", error);
        }
        glUseProgram(0);
        glfwSwapBuffers(mainWindow);
    }
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return 0;
}
