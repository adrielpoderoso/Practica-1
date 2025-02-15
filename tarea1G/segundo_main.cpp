#include <stdio.h>
#include <string.h>
#include <glew.h>
#include "GLFW/glfw3.h"
#include "variables.h" // Variables globales (VAO, VBO, shader)

// Vertex Shader
static const char* vShader = "#version 330\n"
"layout (location = 0) in vec3 pos;\n"
"void main() {\n"
"   gl_Position = vec4(pos, 1.0f);\n"
"}";

// Fragment Shader (blanco)
static const char* fShader = "#version 330\n"
"out vec4 color;\n"
"void main() {\n"
"   color = vec4(1.0, 1.0, 1.0, 1.0); // Blanco\n"
"}";

void CrearTriangulo() {
    printf("Iniciando creación del triángulo...\n");

    static const GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    printf("Datos de vértices asignados.\n");

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    printf("Triángulo creado correctamente.\n");
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {
    GLuint theShader = glCreateShader(shaderType);
    glShaderSource(theShader, 1, &shaderCode, NULL);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = {0};
    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        printf("Error al compilar shader %d: %s\n", shaderType, eLog);
        return;
    }

    glAttachShader(theProgram, theShader);
    glDeleteShader(theShader);
}

void CompileShaders() {
    shader = glCreateProgram();
    if (!shader) {
        printf("Error creando el shader\n");
        return;
    }

    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    glLinkProgram(shader);
    GLint result = 0;
    GLchar eLog[1024] = {0};
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error al linkear: %s\n", eLog);
        return;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error al validar: %s\n", eLog);
        return;
    }
}
