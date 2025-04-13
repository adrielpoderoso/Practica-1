/*
Práctica 7: Iluminación 1
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;


Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;
//AGREGAR LOS MODELS
Model CarroBase_M;
Model LlantaDelanteraDerecha_M;
Model LlantaDelanteraIzquierda_M;
Model LlantaTraseraIzquierda_M;
Model LlantaTraseraDerecha_M;
Model Cajuela_M;
Model Capotaje_M;
Model Lampara_M;
Model ZimHouse_M;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture llantaTexture;
Texture capotajeTexture;
Texture octaedroTexture;


Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;


Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;

// luces zim
PointLight pointLightsZim;  // Luz específica para la casa Zim
bool luzZimEncendida = false;  // Estado de la luz

//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
    unsigned int vLength, unsigned int normalOffset)
{
    for (size_t i = 0; i < indiceCount; i += 3)
    {
        unsigned int in0 = indices[i] * vLength;
        unsigned int in1 = indices[i + 1] * vLength;
        unsigned int in2 = indices[i + 2] * vLength;
        glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
        glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
        glm::vec3 normal = glm::cross(v1, v2);
        normal = glm::normalize(normal);

        in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
        vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
        vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
        vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
    }

    for (size_t i = 0; i < verticeCount / vLength; i++)
    {
        unsigned int nOffset = i * vLength + normalOffset;
        glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
        vec = glm::normalize(vec);
        vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
    }
}


void CreateObjects()
{
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    GLfloat vertices[] = {
        //  x      y      z         u     v         nx    ny    nz
            -1.0f, -1.0f, -0.6f,    0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
            0.0f, -1.0f, 1.0f,      0.5f, 0.0f,     0.0f, 0.0f, 0.0f,
            1.0f, -1.0f, -0.6f,     1.0f, 0.0f,     0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,       0.5f, 1.0f,     0.0f, 0.0f, 0.0f
    };

    unsigned int floorIndices[] = {
        0, 2, 1,
        1, 2, 3
    };

    GLfloat floorVertices[] = {
        -10.0f, 0.0f, -10.0f,   0.0f, 0.0f,     0.0f, -1.0f, 0.0f,
        10.0f, 0.0f, -10.0f,    10.0f, 0.0f,    0.0f, -1.0f, 0.0f,
        -10.0f, 0.0f, 10.0f,    0.0f, 10.0f,    0.0f, -1.0f, 0.0f,
        10.0f, 0.0f, 10.0f,     10.0f, 10.0f,   0.0f, -1.0f, 0.0f
    };

    unsigned int vegetacionIndices[] = {
       0, 1, 2,
       0, 2, 3,
       4,5,6,
       4,6,7
    };

    GLfloat vegetacionVertices[] = {
        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f,      1.0f, 0.0f,     0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f,       1.0f, 1.0f,     0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f,      0.0f, 1.0f,     0.0f, 0.0f, 0.0f,

        0.0f, -0.5f, -0.5f,     0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
        0.0f, -0.5f, 0.5f,      1.0f, 0.0f,     0.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.5f,       1.0f, 1.0f,     0.0f, 0.0f, 0.0f,
        0.0f, 0.5f, -0.5f,      0.0f, 1.0f,     0.0f, 0.0f, 0.0f,


    };

    Mesh* obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 32, 12);
    meshList.push_back(obj1);

    Mesh* obj2 = new Mesh();
    obj2->CreateMesh(vertices, indices, 32, 12);
    meshList.push_back(obj2);

    Mesh* obj3 = new Mesh();
    obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
    meshList.push_back(obj3);

    Mesh* obj4 = new Mesh();
    obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
    meshList.push_back(obj4);

    calcAverageNormals(indices, 12, vertices, 32, 8, 5);

    calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

void CrearOctaedro()
{

    unsigned int octaedro_indices[] = {
        // Cara 1
        0, 1, 2,
        // Cara 2
        3, 4, 5,
        // Cara 3
        6, 7, 8,
        // Cara 4
        9, 10, 11,
        // Cara 5
        12, 13, 14,
        // Cara 6
        15, 16, 17,
        // Cara 7
        18, 19, 20,
        // Cara 8
        21, 23, 22,
    };



    GLfloat octaedro_vertices[] = {
        //  X      Y      Z      S      T      NX  NY  NZ

         0.0f,  0.0f,  1.0f,   0.00f, 0.75f,  0.0f, -1.0f, -1.0f,
         0.0f,  1.0f,  0.0f,   0.25f, 1.00f,  0.0f, -1.0f, -1.0f,
         1.0f,  0.0f,  0.0f,   0.50f, 0.75f,  0.0f, -1.0f, -1.0f,

         // Cara 1 – Dado 2 
              0.0f,  0.0f,  1.0f,   0.00f, 0.75f,  1.0f, -1.0f, -1.0f,
              0.0f,  1.0f,  0.0f,   0.25f, 0.50f,  1.0f, -1.0f, -1.0f,
             -1.0f,  0.0f,  0.0f,   0.50f, 0.75f,  1.0f, -1.0f, -1.0f,

             // Cara 2 – Dado 3  
                  0.0f,  0.0f, -1.0f,   0.25f, 0.50f,  1.0f, -1.0f, 1.0f,
                  0.0f,  1.0f,  0.0f,   0.50f, 0.75f,  1.0f, -1.0f, 1.0f,
                 -1.0f,  0.0f,  0.0f,   0.75f, 0.50f,  1.0f, -1.0f, 1.0f,

                 // Cara 3 – Dado 4  
                      0.0f,  0.0f, -1.0f,   0.50f, 0.75f,  -1.0f, -1.0f, 1.0f,
                      0.0f,  1.0f,  0.0f,   0.75f, 0.50f,  -1.0f, -1.0f, 1.0f,
                      1.0f,  0.0f,  0.0f,   1.00f, 0.75f,  -1.0f, -1.0f, 1.0f,

                      // Cara 4 – Dado 5 
                               1.0f,  0.0f,  0.0f,   0.75f, 0.5f,   -1.0f, 1.0f, -1.0f,
                               0.0f,  0.0f,  1.0f,   0.25f, 0.5f,   -1.0f, 1.0f, -1.0f,
                               0.0f, -1.0f,  0.0f,   0.5f,  0.25f,  -1.0f, 1.0f, -1.0f,

                               // Cara 5 – Dado 6  
                                    0.0f,  0.0f,  1.0f,   0.00f, 0.25f,  1.0f, 1.0f, -1.0f,
                                    0.0f, -1.0f,  0.0f,   0.25f, 0.50f,  1.0f, 1.0f, -1.0f,
                                   -1.0f,  0.0f,  0.0f,   0.50f, 0.25f,  1.0f, 1.0f, -1.0f,

                                   // Cara 6 – Dado 7  
                                       -1.0f,  0.0f, 0.0f,   0.50f, 0.25f,  -1.0f, 1.0f, -1.0f,
                                        0.0f, -1.0f, 0.0f,   0.75f, 0.50f,  -1.0f, 1.0f, -1.0f,
                                        0.0f,  0.0f,-1.0f,   1.00f, 0.25f,  -1.0f, 1.0f, -1.0f,

                                        // Cara 7 – Dado 8  
                                             0.0f,  0.0f, -1.0f,   0.00f, 0.25f, -1.0f, 1.0f, -1.0f,
                                             0.0f, -1.0f,  0.0f,   0.25f, 0.0f,  -1.0f, 1.0f, -1.0f,
                                             1.0f,  0.0f,  0.0f,   0.50f, 0.25f, -1.0f, 1.0f, -1.0f,
    };

    unsigned int vertexCount = sizeof(octaedro_vertices) / sizeof(GLfloat);
    unsigned int indexCount = sizeof(octaedro_indices) / sizeof(unsigned int);

    Mesh* octaedro = new Mesh();
    octaedro->CreateMesh(
        octaedro_vertices,
        octaedro_indices,
        vertexCount,
        indexCount
    );



    meshList.push_back(octaedro);

}


int main()
{
    mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
    mainWindow.Initialise();

    CreateObjects();
    CreateShaders();
    CrearOctaedro();

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

    brickTexture = Texture("Textures/brick.png");
    brickTexture.LoadTextureA();
    dirtTexture = Texture("Textures/dirt.png");
    dirtTexture.LoadTextureA();
    plainTexture = Texture("Textures/plain.png");
    plainTexture.LoadTextureA();
    pisoTexture = Texture("Textures/piso.tga");
    pisoTexture.LoadTextureA();
    AgaveTexture = Texture("Textures/Agave.tga");
    AgaveTexture.LoadTextureA();
    llantaTexture = Texture("Textures/llanta.png");
    llantaTexture.LoadTextureA();
    capotajeTexture = Texture("Textures/capotajetexture.tga");
    capotajeTexture.LoadTextureA();
    octaedroTexture = Texture("textures/octaedro.tga");
    octaedroTexture.LoadTextureA();


    Kitt_M = Model();
    Kitt_M.LoadModel("Models/kitt_optimizado.obj");
    Llanta_M = Model();
    Llanta_M.LoadModel("Models/llanta_optimizada.obj");
    Blackhawk_M = Model();
    Blackhawk_M.LoadModel("Models/uh60.obj");
    //CARRO BASE
    //AGREGAR DIRECCIONES DE LOS MODELS: Goddard_M=Model(); &&  Goddard_M.LoadModel("Models/goddard_base.obj");
    CarroBase_M = Model();
    CarroBase_M.LoadModel("Models/carrobase.obj");
    LlantaDelanteraDerecha_M = Model();
    LlantaDelanteraDerecha_M.LoadModel("Models/llantadelanteraderecha.obj");
    LlantaDelanteraIzquierda_M = Model();
    LlantaDelanteraIzquierda_M.LoadModel("Models/llantadelanteraizquierda.obj");
    LlantaTraseraIzquierda_M = Model();
    LlantaTraseraIzquierda_M.LoadModel("Models/llantatraseraizquierda.obj");
    LlantaTraseraDerecha_M = Model();
    LlantaTraseraDerecha_M.LoadModel("Models/llantatraseraderecha.obj");
    Cajuela_M = Model();
    Cajuela_M.LoadModel("Models/cajuela.obj");
    Capotaje_M = Model();
    Capotaje_M.LoadModel("Models/capotaje.obj");
    //LAMPARA
    Lampara_M = Model();
    Lampara_M.LoadModel("Models/lampara.obj");
    ZimHouse_M = Model();
    ZimHouse_M.LoadModel("Models/zimhouse.obj");

    Kitt_M = Model();
    Kitt_M.LoadModel("Models/kitt_optimizado.obj");
    Llanta_M = Model();
    Llanta_M.LoadModel("Models/llanta_optimizada.obj");


    std::vector<std::string> skyboxFaces;
    skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
    skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
    skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
    skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
    skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
    skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

    skybox = Skybox(skyboxFaces);

    Material_brillante = Material(4.0f, 256);
    Material_opaco = Material(0.3f, 4);

    //luz direccional, sólo 1 y siempre debe de existir
    mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
        0.3f, 0.3f,
        0.0f, 0.0f, -1.0f);



    //DECLARACIONNES DE PONTLIGHT (FOCO) MAX 2
    unsigned int pointLightCount = 0;//contador de luces PUNTUALES(foco)


    // POINTLIGHT 0 – lámpara callejera (realista)
    pointLights[0] = PointLight(
        1.0f, 0.1f, 0.0f,     // color cálido
        2.3f, 5.0f,            // ambientIntensity, diffuseIntensity
        -3.0f, 2.0f, 2.0f,     // posición
        1.0f, 0.7f, 0.7f     // atenuación: valores típicos para luz de calle
    );

    pointLightCount++;

    // LUZ CASA ZIM
    pointLights[1] = PointLight(
        0.0f, 1.0f, 1.0f,     // Color cian
        4.0f, 10.0f,          // Mucha intensidad ambiental y difusa
        -3.0f, -2.0f, -4.0f,  // Posición 
        0.002f, 0.2f, 0.8f      // Atenuación fuerte para que no se disperse mucho
    );
    pointLightCount++; // Incrementa el contador
    

    //DECLARACIONES DE SPOTLIGHT(LINTERNA) MAX 4
    unsigned int spotLightCount = 0;//contador de luces PUNTUALES(linterna)

    //SPOTLIGHT 0 FARO DEL CARRO
    spotLights[0] = SpotLight(0.4f, 0.4f, 1.0f,//color
        2.6f, 1.5f,//intensidad y atenuacion 
        20.0f, 5.0f, -20.0f,
        0.0f, -1.0f, 0.0f,
        1.0f, 0.09f, 0.02f,//EXPERIMENTACION DE VALORES: CONSTANTE ||LINEAL || EXPONENCIAL
        25.0f
    );
    spotLightCount++;

    //SPOTLIGHT 1 FARO DEL HELICOPTERO
    spotLights[1] = SpotLight(1.0f, 1.0f, 0.0f,  // Color amarillo 
        1.0f, 0.5f,//intensidad y atenuacion 
        20.0f, 5.0f, -20.0f,//posicion
        0.0f, -1.0f, 0.0f,//direcionn
        0.01f, 0.05f, 0.005f,//atenuacion// caida :: brillo, se va apagando, distancia
        25.0f//angulo de apertura
    );spotLightCount++;

    //LUZ DEL CAPOTAJE DEL CARRO 
    spotLights[2] = SpotLight(1.0f, 0.5f, 0.0f,  // Color naranja 
        0.8f, 1.5f,      // Intensidad ambiental y difusa
        0.0f, 0.0f, 0.0f, // Posición
        0.0f, -1.0f, 0.0f, // Dirección inicial (apuntando hacia abajo)
        0.3f, 0.2f, 0.1f,  // Atenuación
        30.0f              // Ángulo de apertura
    );
    spotLightCount++;

    //LUCES TRASERAS DEL CARRO 
    spotLights[3] = SpotLight(
        1.0f, 0.2f, 0.2f,    // Color rojo
        1.8f, 1.5f,          // Intensidad ambiental y difusa
        0.0f, 0.0f, 0.0f,    // Posición 
        1.0f, -0.1f, 0.0f,   // Dirección inicial (X positiva)
        0.3f, 0.2f, 0.1f,    // Atenuación (constante, lineal, exponencial)
        25.0f                // Ángulo de apertura
    );
    spotLightCount++; 


    GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
        uniformSpecularIntensity = 0, uniformShininess = 0;
    GLuint uniformColor = 0;
    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
    ////Loop mientras no se cierra la ventana
    while (!mainWindow.getShouldClose())
    {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        deltaTime += (now - lastTime) / limitFPS;
        lastTime = now;



        //Recibir eventos del usuario
        glfwPollEvents();
        camera.keyControl(mainWindow.getsKeys(), deltaTime);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        // Clear the window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
        shaderList[0].UseShader();
        uniformModel = shaderList[0].GetModelLocation();
        uniformProjection = shaderList[0].GetProjectionLocation();
        uniformView = shaderList[0].GetViewLocation();
        uniformEyePosition = shaderList[0].GetEyePositionLocation();
        uniformColor = shaderList[0].getColorLocation();

        //información en el shader de intensidad especular y brillo
        uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
        uniformShininess = shaderList[0].GetShininessLocation();

        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
        glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);



        //CONFIGURACION  DE POSICION DE LAS LUCES

        // Luz del carro
        glm::vec3 luzCarro = glm::vec3(mainWindow.gettrasladocoche() + 1.0f, -0.3f, 0.0);//Posicion
        glm::vec3 lightDir = glm::vec3(1.0f, 0.0f, 0.0f);//Direccion
        spotLights[0].SetFlash(luzCarro, lightDir);

        //luz del helicoptero
        glm::vec3 luzHelicoptero = glm::vec3((mainWindow.gettrasladohelicoptero()) / 3.32 + -0.8f, 4.9f, 6.0);//Posicion
        glm::vec3 lightDirHelicoptero = glm::vec3(-1.0f, 0.0f, 0.0f);//Direccion
        spotLights[1].SetFlash(luzHelicoptero, lightDirHelicoptero);

      
        //INCI0 DE LUZ CAPOTAJE
        float apertura = mainWindow.getcapotaje(); // Ángulo actual (0° cerrado, 45° abierto)

        // 1. Posición exacta del foco 
        glm::vec3 luzPos = glm::vec3(
            mainWindow.gettrasladocoche() + 1.3f,  // 1.3m frente al centro del coche
            0.6f + (apertura * 0.01f),            // Altura base + ajuste por apertura
            0.0f                                   
        );

        // 2. Dirección dinámica (apunta hacia adelante y abajo)
        glm::vec3 luzDir = glm::vec3(
            1.0f,                                  // Hacia adelante (eje X)
            -0.7f + (apertura * -0.005f),          // Inclinación hacia abajo
            0.0f
        );

        // 3. Activar/desactivar luz
        if (apertura > 5.0f) { // Si está abierto > 5°
            spotLights[2].SetFlash(luzPos, luzDir);
        }
        else { // Si está cerrado
            spotLights[2].SetFlash(
                glm::vec3(0.0f, -1000.0f, 0.0f),   // la luz ira fuera de la vista
                luzDir
            );
        }
        //FINAL DE LUZ CAPOTAJE

        //INICIO DE LUZ TRASERA CARRO 
        if (mainWindow.getretrocediendo()) {
            // POSICIÓN: 1.5m atrás del coche y 0.4m de altura
            glm::vec3 luzTraseraPos = glm::vec3(
                mainWindow.gettrasladocoche() - 1.8f,
                -0.5f,
                0.0f
            );
            // DIRECCIÓN: Hacia atrás (eje X positivo) y ligeramente abajo
            glm::vec3 luzTraseraDir = glm::vec3(-1.0f, -0.1f, 0.0f);

            // ASIGNAR LUZ TRASERA (usando spotLights[3])
            spotLights[3].SetFlash(luzTraseraPos, luzTraseraDir);
        }
        else {
            // "APAGAR" moviendo la luz fuera de la escena
            spotLights[3].SetFlash(glm::vec3(0.0f, -1000.0f, 0.0f), glm::vec3(1.0f, -0.1f, 0.0f));
        }
        //FIN DE LUZ TRASERA CARRO

        //INICIO DE LA LUZ CASA DE ZIM
        unsigned int lucesActivas = pointLightCount;

        // Si la luz de la casa Zim está apagada, no la cuentes
        if (!mainWindow.getluzModeloEncendida()) {
            lucesActivas = 1; // solo la lámpara (pointLights[0])
        }
        //FIN DE LA LUZ CASA DE ZIM
        
        
        //información al shader de fuentes de iluminación
        shaderList[0].SetDirectionalLight(&mainLight);
        shaderList[0].SetSpotLights(spotLights, spotLightCount);
        shaderList[0].SetPointLights(pointLights, lucesActivas);//enviar solo las luces activas

        glm::mat4 model(1.0);
        glm::mat4 modelaux(1.0);
        glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));

        pisoTexture.UseTexture();
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

        meshList[2]->RenderMesh();
        //------------*INICIA DIBUJO DE NUESTROS DEMÁS OBJETOS-------------------*
        //BASE DEL CARRO
        color = glm::vec3(0.0f, 0.0f, 1.0f);
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(mainWindow.gettrasladocoche(), 0.0f, 0.0f));
        modelaux = model;//chekpoint
        glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        CarroBase_M.RenderModel();

        //LLANTA DELANTERA DERECHA
        model = modelaux;//libera chekpoint
        model = glm::translate(model, glm::vec3(1.45f, 0.42f, 0.83f));
        model = glm::rotate(model, glm::radians(mainWindow.getllantadelanteraderecha()), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        LlantaDelanteraDerecha_M.RenderModel();

        //LLANTA DELANTERA IZQUIERDA
        model = modelaux;//libera chekpoint
        model = glm::translate(model, glm::vec3(1.5f, 0.5f, -0.95f));
        model = glm::rotate(model, glm::radians(mainWindow.getllantadelanteraizquierda()), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        LlantaDelanteraIzquierda_M.RenderModel();


        //LLANTA TRASERA IZQUIERDA
        model = modelaux;//libera chekpoint 
        color = glm::vec3(1.0f, 0.0f, 1.0f);
        model = glm::translate(model, glm::vec3(-1.15f, 0.5f, -0.95f));
        model = glm::rotate(model, glm::radians(mainWindow.getllantatraseraizquierda()), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        LlantaTraseraIzquierda_M.RenderModel();



        //LLANTA TRASERA DEREHA
        model = modelaux;//libera chekpoint
        color = glm::vec3(1.0f, 0.0f, 1.0f);
        model = glm::translate(model, glm::vec3(-1.15f, 0.5f, 0.82f));
        model = glm::rotate(model, glm::radians(mainWindow.getllantatraseraderecha()), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        LlantaTraseraDerecha_M.RenderModel();


        //CAJUELA
        model = modelaux;//libera chekpoint
        color = glm::vec3(1.0f, 0.0f, 0.0f);
        model = glm::translate(model, glm::vec3(-1.4f, 1.0f, -0.02f));
        model = glm::rotate(model, glm::radians(mainWindow.getcajuela()), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Cajuela_M.RenderModel();


        //CAPOTAJE
        model = modelaux; // libera checkpoint
        color = glm::vec3(1.0f, 2.0f, 1.0f);
        model = glm::translate(model, glm::vec3(1.0f, 1.08f, -0.08f));
        model = glm::rotate(model, glm::radians(mainWindow.getcapotaje()), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Capotaje_M.RenderModel();


        //HELICOPTERO
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f, 5.0f, 6.0));
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
        model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(0.0f, -(mainWindow.gettrasladohelicoptero()), 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Blackhawk_M.RenderModel();

        //LAMPARA
        color = glm::vec3(0.0f, 0.0f, 0.0f);
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 2.0f));
        glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Lampara_M.RenderModel();


        //  Octaedro 
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 4.5f, -2.0f));
        model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        octaedroTexture.UseTexture();
        meshList[4]->RenderMesh();
        
        //casa zim
        color = glm::vec3(0.0f, 0.0f, 0.0f);
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(-3.0f, -1.0f, -10.0f));
        glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        ZimHouse_M.RenderModel();

        //Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));



        //blending: transparencia o traslucidez
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        AgaveTexture.UseTexture();
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[3]->RenderMesh();
        glDisable(GL_BLEND);



        glUseProgram(0);

        mainWindow.swapBuffers();
    }

    return 0;
}
