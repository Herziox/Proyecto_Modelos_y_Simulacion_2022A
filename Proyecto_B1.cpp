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
bool salaCargada;
room r;
int NumTri = 0;
source s;
int N_RAYOS = 20;
receptor r1;
int durSim = 1000;
matInt matRoomTime; //Matriz de tiempo de Triangulos de la sala
matDouble matRoomDist; //Matriz de distancia de Triangulos de la sala
matDouble matRoomAngles; // Matriz de angulos de Triangulos de la sala

matInt matRecTime; //Matriz de tiempo de Triangulos del receptor
matDouble matRecDist; //Matriz de distancia de Triangulos del receptor
matDouble matRecAngles; // Matriz de angulos de Triangulos del receptor



MatEnergia matEnergia; // Matriz de energia
reflection* reflexiones;
int N_DIV = 4;
Vector** vis_vector;
double** vis_modvec;
int** vis_timacu;



using namespace std;




void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void cargarSala();
void calcular();


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
    /*int dimVertSala = 6 * r.p[0].NT * 3 * 3;
    float* verticesSala;
    verticesSala = NULL;
    verticesSala = new float[dimVertSala];
    cout << dimVertSala<<" Numero triangulos: "<< NumTri << endl;*/
    float verticesSala[1728];
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
    calcular();

    //NUMERO DE RAYOS
    //s.createRays(N_RAYOS);

    //REFLEXIONES
    //reflection* reflexiones = r.RayTracing(s.p, s.Rays, s.NRAYS);

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
    double velocidad = SPEED;
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
        glDrawArrays(GL_TRIANGLES, 0, 576);


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
        glDrawArrays(GL_TRIANGLES, 0, 576);



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


/*============================================================================================//
    1.Calcule el centroide de todos los triángulos de la sala.
//============================================================================================*/

/*============================================================================================//
    2. Genere una matriz de tipo double que almacene la distancia que existe entre los centros de
    los diferentes triángulos de la sala.Está matriz tendrá una dimensión de n×n donde n será el
    número de triángulos de la sala.
//============================================================================================*/

/*============================================================================================//
     3. Calcule todos los “tiempos de vuelo” en milisegundos que demorarían las reflexiones difusas
     para llegar de un centroide a otro (entre todos los triángulos que apliquen, es decir que sean
     visibles) considerando una velocidad de transmisión de 340 m/s (constante V_SON). Dichos
     tiempos también deberán ser almacenados en una matriz “tiempo” de tipo int de n×n elementos.
//============================================================================================*/

/*============================================================================================//
     4. Calcule el porcentaje de la energía que será enviada a cada triángulo, considerando el
     ángulo sólido (área de triángulo proyectado a una distancia de 0.2) entre todos los triángulos
     visibles desde el punto de partida (centroide). Estos porcentajes también deberán almacenarse
     en una matriz “porcentaje” de tipo double de n×n elementos
//============================================================================================*/

/*============================================================================================//
    5. Distribuya la energía difusa según la matriz “porcentaje” entre todos los triángulos que
    sean visibles entre sí, respetando el tiempo de vuelo/desplazamiento dado por la matriz
    “tiempo”.  Para registrar este cálculo, actualice la matriz energía, mE, (espacio/tiempo)
    según corresponda.
//============================================================================================*/

/*============================================================================================//
    6. La visibilidad de un triángulo con respecto a otro, para el caso en cuestión, se dará en
    función que ambos triángulos no pertenezcan al mismo plano.
//============================================================================================*/

/*============================================================================================//
    7. La energía que se transmita de manera difusa, perderá energía en función del coeficiente
    de absorción.La energía residual se volverá a transmitir difusamente, sin pérdidas adicionales.
//============================================================================================*/

 /*============================================================================================//
    8.	En cada transición, se debe transmitir energía al(a los) receptor(es).El porcentaje
        será dado en función del área de un disco proyectado a 0.2 del centroide del triángulo.
        Para esto también es necesario tener el tiempo de vuelo entre los triángulos
        y los receptores.Utilice vectores o matrices para pre calcular estos datos.
//============================================================================================*/

/*============================================================================================//
    9.	La duración de la transición de energía no debe sobrepasar 1 segundo.
//============================================================================================*/


void cargarSala() {
    // CREACIÓN DE LA SALA
    if (!salaCargada) {

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
        //r.p[0].PointGenTriangle();//Actualizar
        r.p[0].MoreTriangle(N_DIV);


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
        //r.p[1].PointGenTriangle();//Actualizar
        r.p[1].MoreTriangle(N_DIV);


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
        //r.p[2].PointGenTriangle();//Actualizar
        r.p[2].MoreTriangle(N_DIV);


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
        //r.p[3].PointGenTriangle();//Actualizar
        r.p[3].MoreTriangle(N_DIV);


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
        //r.p[4].PointGenTriangle();//Actualizar
        r.p[4].MoreTriangle(N_DIV);

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
        //r.p[5].PointGenTriangle(); //Actualizar
        r.p[5].MoreTriangle(N_DIV);

        // Creación de baricentros de triángulos de los planos de la sala  [PARTE 1]
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


        // CREACIÓN DE RECEPTORES 
        r.NewReceptor(N_REC);
        int cont_rec = 0;
        for (int i = -1; i < 2; i++) {
            for (int j = -1; j < 2; j++) {
                for (int k = -1; k < 2; k++) {
                    cout << i << " " << j << " " << k << endl;
                    r.r[cont_rec].p.x = i;
                    r.r[cont_rec].p.y = j;
                    r.r[cont_rec].p.z = k;
                    cont_rec++;
                }
            }
        }

        // Creación de matrices de tiempo, distancia y ángulos sólidos para la sala y receptores

        NumTri = cont_t;

        matRoomDist.init(NumTri, NumTri); //[PARTE 2]
        matRoomTime.init(NumTri, NumTri); //[PARTE 3]
        matRoomAngles.init(NumTri, NumTri); //[PARTE 4]       

        matRecTime.init(N_REC, NumTri);
        matRecDist.init(N_REC, NumTri);
        matRecAngles.init(N_REC, NumTri);

        //Arreglo
        double* areaT;
        areaT = NULL;
        areaT = new double[NumTri];
        int idTri1 = 0;
        int idTri2 = 0;

        for (int i = 0; i < NumTri; i++) {
            areaT[i] = 0.0;
        }

        //CALCULO DE DISTANCIAS, TIEMPO DE VUELO Y PORCENTAJES
        for (int i = 0; i < r.NP; i++) {

            for (int j = 0; j < r.p[i].NT; j++) {
               // TRIANGULO 1
                idTri1 = r.p[i].t[j].ID;
                for (int k = 0; k < r.NP; k++) {

                    for (int l = 0; l < r.p[k].NT; l++) {
                        // TRIANGULO 2
                        idTri2 = r.p[k].t[l].ID;

                        

                        if (i != k) { // PARTE 6
                            matRoomDist.d[idTri1][idTri2] = r.p[i].t[j].bc.distancia(r.p[k].t[l].bc); // PARTE 2
                            matRoomTime.i[idTri1][idTri2] = int(1000 * matRoomDist.d[idTri1][idTri2] / V_SON); // PARTE 3
                            matRoomAngles.d[idTri1][idTri2] = r.p[k].t[l].solidAngle(r.p[i].t[j].bc); // PARTE 4
                            areaT[idTri1] += matRoomAngles.d[idTri1][idTri2];
                        }
                    }
                }
                // Cálculo de las distancias, tiempo de vuelo y porcentajes de receptores
                for (int m = 0; m < r.NR; m++) {
                    matRecDist.d[m][idTri1] = r.r[m].p.distancia(r.p[i].t[j].bc);
                    matRecTime.i[m][idTri1] = int(1000 * matRecDist.d[m][idTri1] / V_SON);
                    matRecAngles.d[m][idTri1] = r.r[m].solidAngle(r.p[i].t[j].bc);
                }

            }
        }

        // MATRIZ DE PORCENTAJES
        for (int i = 0; i < NumTri; i++) {
            for (int j = 0; j< NumTri; j++) {
                matRoomAngles.d[i][j] = matRoomAngles.d[i][j]/areaT[i];
            };
        }

        for (int i = 0; i < N_REC; i++) {
            for (int j = 0; j < NumTri; j++) {
                matRecAngles.d[i][j] = matRecAngles.d[i][j] / areaT[j];
            };
        }

      
        cout << "Grabar archivo de Distancia Triangulos Room" << endl;
        matRoomDist.grabarArchivo('d', NumTri);
        cout << "Grabar archivo de Tiempo Triangulos Room" << endl;
        matRoomTime.grabarArchivo('t', NumTri);
        cout << "Grabar archivo de Porcentajes Triangulos Room" << endl;
        matRoomAngles.grabarArchivo('p', NumTri);


        cout << "Grabar archivo de Distancia Triangulos Receptor" << endl;
        matRecDist.grabarArchivo('D', N_REC);
        cout << "Grabar archivo de Tiempo Triangulos Receptor" << endl;
        matRecTime.grabarArchivo('T', N_REC);
        cout << "Grabar archivo de Porcentajes Triangulos Receptor" << endl;
        matRecAngles.grabarArchivo('P', N_REC);

        
    }
}

void calcular() {
    double eneRay, eneRes; //Energía del rayo y energía residual;
    int t_vuelo = 0; // Tiempo de vuelo del rayo
    for (int i = 0; i < r.NR; i++) {
        r.r[i].createTimeSamples(DUR_SIM);
    }
    s.eF = 100;
    s.createRays(N_RAYOS);

    eneRay = s.eF / s.NRAYS;
    double alfa, delta; //Coeficientes de absorción y difusión
    alfa = 0.2;
    delta = 0.15;

    reflexiones = NULL; // reflexion
    reflexiones = r.RayTracing(s.p, s.Rays, s.NRAYS); // trazado de rayos

    matEnergia.init(NumTri, durSim); // matriz de energia /Definir en la creaciòn de la sala en nùmero de triangulos

    //Definimos la primera dimension (# de rayos)
    vis_vector = new Vector * [s.NRAYS]; // se crea un vector con todos los rayos creados.
    vis_modvec = new double* [s.NRAYS]; // se crea un vector para cada modulo de cada rayo
    vis_timacu = new int* [s.NRAYS]; // se crea un vector para los instantes de tiempo de cada rayo

    for (int R = 0; R < s.NRAYS; R++) {

        eneRes = eneRay; //Inicializo energía residual

        //Definimos la segunda dimension (# de reflexiones)
        vis_vector[R] = new Vector[reflexiones[R].N - 1]; // vectores de reflexion por cada rayo
        vis_modvec[R] = new double[reflexiones[R].N]; // modulo de cada vector para cada reflexion
        vis_timacu[R] = new int[reflexiones[R].N]; // tiempo acumulado de las reflexiones
        vis_modvec[R][0] = 0.0;
        vis_timacu[R][0] = 0;

        for (int i = 0; i < reflexiones[R].N - 1; i++) { 
            //Guardo el vector con la ruta de la reflexión
            vis_vector[R][i] = reflexiones[R].r[i + 1] - reflexiones[R].r[i];
            //Registro la distancia acumulada de las reflexiones anteriores
            vis_modvec[R][i + 1] = vis_modvec[R][i] + reflexiones[R].d[i + 1];
            vis_timacu[R][i + 1] = int((1000 * vis_modvec[R][i + 1] / V_SON));

        }

        //Captación en receptor de reflexiones especulares y carga de la matriz de energia difusa
        for (int i = 1; i < reflexiones[R].N; i++) {
            int indTri, indTim; //Indices para la matriz de energìa difusa (transición de energía)
            indTim = vis_timacu[R][i]; // recupero el tiempo para las reflexiones del rayo
            indTri = reflexiones[R].idTriangle[i]; // recupero el indice del triangulo en el que rebota.


            matEnergia.energia[indTri][indTim] += (eneRes * (1 - alfa) * delta); //Carga de matriz energia difusa // PARTE 5
            for (int j = 0; j < r.NR; j++) {
                r.r[j].receptionRayTracing(reflexiones[R].r[i - 1], vis_vector[R][i - 1], vis_timacu[R][i], r.maxd, eneRes); //Captación del receptor energia especular //PARTE 8

            }
            eneRes = eneRes * (1 - alfa) * (1 - delta); // nueva energia que se va a transmitir en la nueva reflexion
        }
    }

    for (int t_sim = 0; t_sim < DUR_SIM; t_sim++) {
        for (int i = 0; i < NumTri; i++) {
            for (int j = 0; j < NumTri; j++) {
                t_vuelo = t_sim + matRoomTime.i[i][j];

                if (t_vuelo < DUR_SIM) {
                    matEnergia.energia[j][t_vuelo] += (matEnergia.energia[i][t_sim] * matRoomAngles.d[i][j]) * (1 - alfa) * delta; //PARTE 7
                }
                
            };

            for (int k = 0; k < r.NR; k++) {
                t_vuelo = t_sim + matRecTime.i[k][i];
                
                if (t_vuelo < DUR_SIM) {
                    r.r[k].eR[t_vuelo] +=  (matEnergia.energia[i][t_sim] * matRoomAngles.d[k][i]); //PARTE 7
                }

            };
        }
    }

    for (int i = 0; i < r.NR; i++) {
        r.r[i].grabarArchivo();
    }
    cout << "Grabar archivo de Energia" << endl;
    matEnergia.grabarArchivo('e', s.NRAYS, NumTri);
}


