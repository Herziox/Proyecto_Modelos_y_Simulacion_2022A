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


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void laodRoom();


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

//Exercise 13
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proyecto Modelos y Simulacion", NULL, NULL);
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

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    //Exercise 13 Task 1
    // build and compile our shader zprogram
    // ------------------------------------
    //Shader lightingShader("D:/Users/mlcon/2022-A/Modelos y Simulacion/proyectos/shaders/shader_exercise13t3_colors.vs", "D:/Users/mlcon/2022-A/Modelos y Simulacion/proyectos/shaders/shader_exercise13t3_colors.fs");
    //Shader lightCubeShader("D:/Users/mlcon/2022-A/Modelos y Simulacion/proyectos/shaders/shader_exercise13_lightcube.vs", "D:/Users/mlcon/2022-A/Modelos y Simulacion/proyectos/shaders/shader_exercise13_lightcube.fs");
   // Shader lightingShader("shader_1.vs", "shader_1.fs");
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    Shader lightingShader("lightcube.vs", "lightcube.fs");
    Shader ico("lightcube.vs", "lightcube.fs");
    Shader rayo("lightcube.vs", "lightcube.fs");

    laodRoom();


    int numeroTriangulos = NumTri;
    float vertices1[108];
    int contradork = 0;
    float vertices2[180];
    int contadorIco = 0;




    for (int i = 0; i < r.NP; i++) {
        for (int j = 0; j < r.p[i].NT; j++) {
            vertices1[contradork] = r.p[i].t[j].p0.x * 0.1;
            contradork++;
            vertices1[contradork] = r.p[i].t[j].p0.y * 0.1;
            contradork++;
            vertices1[contradork] = r.p[i].t[j].p0.z * 0.1;
            contradork++;


            vertices1[contradork] = r.p[i].t[j].p1.x * 0.1;
            contradork++;
            vertices1[contradork] = r.p[i].t[j].p1.y * 0.1;
            contradork++;
            vertices1[contradork] = r.p[i].t[j].p1.z * 0.1;
            contradork++;


            vertices1[contradork] = r.p[i].t[j].p2.x * 0.1;
            contradork++;
            vertices1[contradork] = r.p[i].t[j].p2.y * 0.1;
            contradork++;
            vertices1[contradork] = r.p[i].t[j].p2.z * 0.1;
            contradork++;

        }
    }


    contadorIco = 0;
    for (int i = 0; i < 20; i++) {
        vertices2[contadorIco] = s.IcoFace[i].p0.x * 0.1;
        contadorIco++;
        vertices2[contadorIco] = s.IcoFace[i].p0.y * 0.1;
        contadorIco++;
        vertices2[contadorIco] = s.IcoFace[i].p0.z * 0.1;
        contadorIco++;


        vertices2[contadorIco] = s.IcoFace[i].p1.x * 0.1;
        contadorIco++;
        vertices2[contadorIco] = s.IcoFace[i].p1.y * 0.1;
        contadorIco++;
        vertices2[contadorIco] = s.IcoFace[i].p1.z * 0.1;
        contadorIco++;


        vertices2[contadorIco] = s.IcoFace[i].p2.x * 0.1;
        contadorIco++;
        vertices2[contadorIco] = s.IcoFace[i].p2.y * 0.1;
        contadorIco++;
        vertices2[contadorIco] = s.IcoFace[i].p2.z * 0.1;
        contadorIco++;


    }
    point p;


    s.createRays(20);
    r.RayTracing(p, s.Rays, s.NRAYS);

    /*for (int i = 0; i < 20; i++) {
        printf("Rayos: x: %f, y: %f, z: %f\n", s.Rays[i].x, s.Rays[i].y, s.Rays[i].z);
    }*/

    point o;

    reflection* arrayreflecciones;

    arrayreflecciones = r.RayTracing(o, s.Rays, s.NRAYS);

    for (int i = 0; i < 200; i++) {
        printf("punto de golpe: x: %f, y: %f, z: %f\n", arrayreflecciones[1].r[i].x, arrayreflecciones[1].r[i].y, arrayreflecciones[1].r[i].z);
    }

    reflection arrayDePuntosDeChoque = arrayreflecciones[1];


    point puntoDePrueba;

    point puntoDeOrigen;

    puntoDeOrigen.x = arrayreflecciones[1].r[0].x * 0.1;
    puntoDeOrigen.y = arrayreflecciones[1].r[0].y * 0.1;
    puntoDeOrigen.z = arrayreflecciones[1].r[0].z * 0.1;

    puntoDePrueba.x = arrayreflecciones[1].r[1].x * 0.1;
    puntoDePrueba.y = arrayreflecciones[1].r[1].y * 0.1;
    puntoDePrueba.z = arrayreflecciones[1].r[1].z * 0.1;







    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);


    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    unsigned int VBO2, cubeVAO2;
    glGenVertexArrays(1, &cubeVAO2);
    glGenBuffers(1, &VBO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);


    glBindVertexArray(cubeVAO2);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    unsigned int VBO3, cubeVAO3;
    glGenVertexArrays(1, &cubeVAO3);
    glGenBuffers(1, &VBO3);

    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);


    glBindVertexArray(cubeVAO3);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);




    //Exercise 13 Task 2
    //glVertexAttribPointer(1, 0, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    double tiempo1 = 0;
    int contadorTemporal = 0;


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

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("objectColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

        //Exercise 13 Task 2
        lightingShader.setVec3("lightPos", lightPos);

        //Exercise 13 Task 3
        lightingShader.setVec3("viewPos", camera.Position);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();


        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        // render the cube
        glBindVertexArray(cubeVAO);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        ico.use();
        ico.setVec3("objectColor", 1.0f, 0.0f, 0.0f);
        ico.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

        //Exercise 13 Task 2
        ico.setVec3("lightPos", lightPos);

        //Exercise 13 Task 3
        ico.setVec3("viewPos", camera.Position);

        // view/projection transformations
        glm::mat4 projection2 = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view2 = camera.GetViewMatrix();


        ico.setMat4("projection", projection2);
        ico.setMat4("view", view2);

        // world transformation
        glm::mat4 model2 = glm::mat4(1.0f);
        ico.setMat4("model", model2);

        // render the cube
        glBindVertexArray(cubeVAO2);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_TRIANGLES, 0, 60);


        rayo.use();
        rayo.setMat4("projection", projection);
        rayo.setMat4("view", view);
        rayo.setVec3("objectColor", 1.0f, 1.0f, 1.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(puntoDeOrigen.x + ((puntoDePrueba.x) * (glfwGetTime() - tiempo1) * 0.05), puntoDeOrigen.y + ((puntoDePrueba.y) * (glfwGetTime() - tiempo1) * 0.05), puntoDeOrigen.z + ((puntoDePrueba.z) * (glfwGetTime() - tiempo1) * 0.05)));

        if ((puntoDeOrigen.distancia(puntoDePrueba) * (glfwGetTime() - tiempo1) * SPEED) >= puntoDeOrigen.distancia(puntoDePrueba)) {

            tiempo1 = glfwGetTime();
            puntoDeOrigen = puntoDePrueba;

            contadorTemporal++;
            puntoDePrueba.x = arrayreflecciones[1].r[contadorTemporal].x * 0.1;
            puntoDePrueba.y = arrayreflecciones[1].r[contadorTemporal].y * 0.1;
            puntoDePrueba.z = arrayreflecciones[1].r[contadorTemporal].z * 0.1;
        };

        model = glm::translate(model, glm::vec3(puntoDeOrigen.x + ((puntoDePrueba.x - puntoDeOrigen.x)) * (glfwGetTime() - tiempo1) * SPEED, puntoDeOrigen.y + ((puntoDePrueba.y - puntoDeOrigen.y)) * (glfwGetTime() - tiempo1) * SPEED, puntoDeOrigen.z + ((puntoDePrueba.z - puntoDeOrigen.z) * (glfwGetTime() - tiempo1)) * SPEED));


        model = glm::scale(model, glm::vec3(0.01f)); // a smaller cube
        rayo.setMat4("model", model);

        glBindVertexArray(cubeVAO3);
        glDrawArrays(GL_TRIANGLES, 0, 60);






        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    // optional: de-allocate all resources once they've outlived their purpose:
// ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

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

void laodRoom() {
    if (!loadedRoom) {
        int nDivTri;
        nDivTri = 2;
        r.NewPlanes(6);// Genearra 6 planos
        //square back
        r.p[0].NewPoints(4);// Gnererar los 4 puntos

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



        //square front
        r.p[1].NewPoints(4);// Gnererar los 4 puntos

        r.p[1].p[0].x = 2.0f;
        r.p[1].p[0].y = 2.0f;
        r.p[1].p[0].z = 2.0f;

        r.p[1].p[1].x = 2.0f;
        r.p[1].p[1].y = 2.0f;
        r.p[1].p[1].z = -2.0f;

        r.p[1].p[2].x = 2.0f;
        r.p[1].p[2].y = -2.0f;
        r.p[1].p[2].z = -2.0f;

        r.p[1].p[3].x = 2.0f;
        r.p[1].p[3].y = -2.0f;
        r.p[1].p[3].z = 2.0f;

        r.p[1].PointGenTriangle();

        //square left
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

        //square right
        r.p[3].NewPoints(4);// Gnererar los 4 puntos

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


        //square top
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

        //square bottom
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

        /*
        for (int i = 0; i < 6;i++) {
            printf("+++++++++++++++++++++++++++++\n");
            printf("Plano%d\n", i + 1);
            for (int j = 0; j < 2;j++) {
                printf("Punto 1\n");
                printf("x: %f, y: %f, z: %f \n", r.p[i].t[j].p0.x, r.p[i].t[j].p0.y, r.p[i].t[j].p0.z);
                printf("Punto 2\n");
                printf("x: %f, y: %f, z: %f \n", r.p[i].t[j].p1.x, r.p[i].t[j].p1.y, r.p[i].t[j].p1.z);
                printf("Punto 3\n");
                printf("x: %f, y: %f, z: %f \n", r.p[i].t[j].p2.x, r.p[i].t[j].p2.y, r.p[i].t[j].p2.z);
            }
            printf("+++++++++++++++++++++++++++++\n");
        }
        */

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


