#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
//==================================
#include "Header.h"

//VariablesGlobales
bool loadedRoom;
room r;
int NumTri = 0;
source s;
int N_RAYOS = 20;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void cargarSala();


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    //Version de Open gl
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _APPLE_
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proyecto B1 - Modelos y Simulacion", NULL, NULL);
    // Check si falla al crear
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    //Introduce th wwindow into the current context
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


    Shader sala("lightcube.vs", "lightcube.fs");
    Shader fuente("lightcube.vs", "lightcube.fs");
    Shader particula("lightcube.vs", "lightcube.fs");

    //CARGAR SALA
    cargarSala();

    //ARREGLO DE VERTICES DE SALA
    int numeroTriangulos = NumTri;
    float verticesSala[108];
    int contSala = 0;

    for (int i = 0; i < r.NP; i++) {
        for (int j = 0; j < r.p[i].NT; j++) {
            verticesSala[contSala] = r.p[i].t[j].p0.x ;
            contSala++;
            verticesSala[contSala] = r.p[i].t[j].p0.y ;
            contSala++;
            verticesSala[contSala] = r.p[i].t[j].p0.z ;
            contSala++;


            verticesSala[contSala] = r.p[i].t[j].p1.x ;
            contSala++;
            verticesSala[contSala] = r.p[i].t[j].p1.y ;
            contSala++;
            verticesSala[contSala] = r.p[i].t[j].p1.z ;
            contSala++;


            verticesSala[contSala] = r.p[i].t[j].p2.x;
            contSala++;
            verticesSala[contSala] = r.p[i].t[j].p2.y;
            contSala++;
            verticesSala[contSala] = r.p[i].t[j].p2.z;
            contSala++;

        }
    }


    //ARREGLO DE VERTICES DE FUENTE (ICOSAEDRO)
    float verticesFuente[180];
    int contFuente = 0;
    for (int i = 0; i < 20; i++) {

        verticesFuente[contFuente] = s.IcoFace[i].p0.x;
        contFuente++;


        verticesFuente[contFuente] = s.IcoFace[i].p0.y;
        contFuente++;


        verticesFuente[contFuente] = s.IcoFace[i].p0.z;
        contFuente++;


        verticesFuente[contFuente] = s.IcoFace[i].p1.x;
        contFuente++;

        verticesFuente[contFuente] = s.IcoFace[i].p1.y;
        contFuente++;


        verticesFuente[contFuente] = s.IcoFace[i].p1.z;
        contFuente++;


        verticesFuente[contFuente] = s.IcoFace[i].p2.x;
        contFuente++;


        verticesFuente[contFuente] = s.IcoFace[i].p2.y;
        contFuente++;

        verticesFuente[contFuente] = s.IcoFace[i].p2.z;
        contFuente++;


    }

    //NUMERO DE RAYOS
    s.createRays(N_RAYOS);

    //REFLEXIONES
    reflection* reflexiones = r.RayTracing(s.p, s.Rays, s.NRAYS);

    int idRayo = 6;
    int nPunto = 0;

    // PUNTO DE PARTIDA
    point inicio;
    inicio.x = reflexiones[idRayo].r[nPunto].x;
    inicio.y = reflexiones[idRayo].r[nPunto].y;
    inicio.z = reflexiones[idRayo].r[nPunto].z;

    nPunto++;
    // PUNTO DE LLEGADA
    point llegada;
    llegada.x = reflexiones[idRayo].r[nPunto].x;
    llegada.y = reflexiones[idRayo].r[nPunto].y;
    llegada.z = reflexiones[idRayo].r[nPunto].z;




    // Configuración de VBO y VAO para SALA
    unsigned int salaVBO, salaVAO;
    glGenVertexArrays(1, &salaVAO);
    glGenBuffers(1, &salaVBO);

    glBindBuffer(GL_ARRAY_BUFFER, salaVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesSala), verticesSala, GL_STATIC_DRAW);


    glBindVertexArray(salaVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // Configuración de VBO y VAO para FUENTE
    unsigned int fuenteVBO, fuenteVAO;
    glGenVertexArrays(1, &fuenteVAO);
    glGenBuffers(1, &fuenteVBO);

    glBindBuffer(GL_ARRAY_BUFFER, fuenteVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesFuente), verticesFuente, GL_STATIC_DRAW);


    glBindVertexArray(fuenteVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // Configuración de VBO y VAO para PARTICULA
    unsigned int particulaVBO, particulaVAO;
    glGenVertexArrays(1, &particulaVAO);
    glGenBuffers(1, &particulaVBO);

    glBindBuffer(GL_ARRAY_BUFFER, particulaVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesSala), verticesSala, GL_STATIC_DRAW);


    glBindVertexArray(particulaVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    double tiempo = 0.0;
    double tiempoAux = 0.0;
    double distancia = 0.0;
    double distanciaAux = 0.0;
    double velocidad = 3.0;
    double velocidadAux = velocidad;


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {


        // per-frame time logic
       // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
// ------
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //DIBUJAR LA SALA
        sala.use();
        sala.setVec3("objectColor", 1.0f, 1.0f, 1.0f);
        sala.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        sala.setVec3("lightPos", lightPos);
        sala.setVec3("viewPos", camera.Position);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        sala.setMat4("projection", projection);
        sala.setMat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);
        sala.setMat4("model", model);

        glBindVertexArray(salaVAO);
        glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        //DIBUJAR LA FUENTE
        fuente.use();
        fuente.setVec3("objectColor", 1.0f, 1.0f, 0.0f);
        fuente.setVec3("lightColor", 1.0f, 1.0f, 0.0f);
        fuente.setVec3("lightPos", lightPos);
        fuente.setVec3("viewPos", camera.Position);
        glm::mat4 projection2 = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view2 = camera.GetViewMatrix();
        fuente.setMat4("projection", projection2);
        fuente.setMat4("view", view2);
        glm::mat4 model2 = glm::mat4(1.0f);
        fuente.setMat4("model", model2);

        glBindVertexArray(fuenteVAO);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_TRIANGLES, 0, 60);

        
        //CALCULO DE TRAYECTORIAS Y ANIMACION
        tiempo = glfwGetTime() - tiempoAux;
        distancia = inicio.distancia(llegada);
        distanciaAux = tiempo * velocidad;

        if (distanciaAux >= distancia) {
            inicio = llegada;
            nPunto++;
            llegada.x = reflexiones[idRayo].r[nPunto].x;
            llegada.y = reflexiones[idRayo].r[nPunto].y;
            llegada.z = reflexiones[idRayo].r[nPunto].z;
            tiempoAux = glfwGetTime();
        }

        velocidadAux = velocidad / distancia;
        Vector vecDistancia = llegada - inicio;
        Vector vecTraslacion = vecDistancia * (tiempo * velocidadAux);
        
        //DIBUJAR Y REFLECTAR PARTICULA
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(inicio.x + vecTraslacion.x, inicio.y + vecTraslacion.y, inicio.z + vecTraslacion.z));
        model = glm::scale(model, glm::vec3(0.05f)); // a smaller cube
        particula.use();
        particula.setMat4("model", model);
        particula.setMat4("projection", projection);
        particula.setVec3("objectColor", 1.0f, 0.0f, 0.0f);
        particula.setMat4("view", view);
        

        glBindVertexArray(particulaVAO);
        glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
        glDrawArrays(GL_TRIANGLES, 0, 60);



        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    // optional: de-allocate all resources once they've outlived their purpose:
// ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &salaVAO);
    glDeleteBuffers(1, &salaVBO);
    glDeleteVertexArrays(1, &fuenteVAO);
    glDeleteBuffers(1, &fuenteVBO);
    glDeleteVertexArrays(1, &particulaVAO);
    glDeleteBuffers(1, &particulaVBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
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

    //If I want to stay in ground level (xz plane)
    //camera.Position.y = 0.0f;

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
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
//================================================================================================================================================

void cargarSala() {
    if (!loadedRoom) {

        r.NewPlanes(6);
        //-------------square 1 back
        r.p[0].NewPoints(4);
        r.p[0].p[0].x = -2.0f;
        r.p[0].p[0].y = 2.0f;
        r.p[0].p[0].z = 2.0f;
        r.p[0].p[1].x = -2.0f;
        r.p[0].p[1].y = -2.0f;
        r.p[0].p[1].z = 2.0f;
        r.p[0].p[2].x = -2.0f;
        r.p[0].p[2].y = -2.0f;
        r.p[0].p[2].z = -2.0f;
        r.p[0].p[3].x = -2.0f;
        r.p[0].p[3].y = 2.0f;
        r.p[0].p[3].z = -2.0f;
        r.p[0].PointGenTriangle();

        //-------------square 2 front
        r.p[1].NewPoints(4);
        r.p[1].p[0].x = 2.0f;
        r.p[1].p[0].y = 2.0f;
        r.p[1].p[0].z = 2.0f;
        r.p[1].p[1].x = 2.0f;
        r.p[1].p[1].y = -2.0f;
        r.p[1].p[1].z = 2.0f;
        r.p[1].p[2].x = 2.0f;
        r.p[1].p[2].y = -2.0f;
        r.p[1].p[2].z = -2.0f;
        r.p[1].p[3].x = 2.0f;
        r.p[1].p[3].y = 2.0f;
        r.p[1].p[3].z = -2.0f;
        r.p[1].PointGenTriangle();

        //-------------square 3 left
        r.p[2].NewPoints(4);
        r.p[2].p[0].x = -2.0f;
        r.p[2].p[0].y = -2.0f;
        r.p[2].p[0].z = 2.0f;
        r.p[2].p[1].x = 2.0f;
        r.p[2].p[1].y = -2.0f;
        r.p[2].p[1].z = 2.0f;
        r.p[2].p[2].x = 2.0f;
        r.p[2].p[2].y = -2.0f;
        r.p[2].p[2].z = -2.0f;
        r.p[2].p[3].x = -2.0f;
        r.p[2].p[3].y = -2.0f;
        r.p[2].p[3].z = -2.0f;
        r.p[2].PointGenTriangle();

        //-------------square 4 right
        r.p[3].NewPoints(4);
        r.p[3].p[0].x = 2.0f;
        r.p[3].p[0].y = 2.0f;
        r.p[3].p[0].z = 2.0f;
        r.p[3].p[1].x = -2.0f;
        r.p[3].p[1].y = 2.0f;
        r.p[3].p[1].z = 2.0f;
        r.p[3].p[2].x = -2.0f;
        r.p[3].p[2].y = 2.0f;
        r.p[3].p[2].z = -2.0f;
        r.p[3].p[3].x = 2.0f;
        r.p[3].p[3].y = 2.0f;
        r.p[3].p[3].z = -2.0f;
        r.p[3].PointGenTriangle();

        //-------------square 5 top
        r.p[4].NewPoints(4);
        r.p[4].p[0].x = -2.0f;
        r.p[4].p[0].y = -2.0f;
        r.p[4].p[0].z = 2.0f;
        r.p[4].p[1].x = -2.0f;
        r.p[4].p[1].y = 2.0f;
        r.p[4].p[1].z = 2.0f;
        r.p[4].p[2].x = 2.0f;
        r.p[4].p[2].y = 2.0f;
        r.p[4].p[2].z = 2.0f;
        r.p[4].p[3].x = 2.0f;
        r.p[4].p[3].y = -2.0f;
        r.p[4].p[3].z = 2.0f;
        r.p[4].PointGenTriangle();

        //-------------square 1 bottom
        r.p[5].NewPoints(4);
        r.p[5].p[0].x = -2.0f;
        r.p[5].p[0].y = 2.0f;
        r.p[5].p[0].z = -2.0f;
        r.p[5].p[1].x = -2.0f;
        r.p[5].p[1].y = -2.0f;
        r.p[5].p[1].z = -2.0f;
        r.p[5].p[2].x = 2.0f;
        r.p[5].p[2].y = -2.0f;
        r.p[5].p[2].z = -2.0f;
        r.p[5].p[3].x = 2.0f;
        r.p[5].p[3].y = 2.0f;
        r.p[5].p[3].z = -2.0f;
        r.p[5].PointGenTriangle();


        //Calcular los normales del plano
        int cont_t = 0;
        for (int i = 0; i < r.NP; i++) {
            r.p[i].n = NormalPlano(r.p[i]);
            for (int j = 0; j < r.p[i].NT; j++) {
                r.p[i].t[j].CalculateProjection();
                r.p[i].t[j].Centroid();
                r.p[i].t[j].ID = cont_t;
                cont_t++;
            }

        }

        NumTri = cont_t;
        //numRec = 27;




    }
}


