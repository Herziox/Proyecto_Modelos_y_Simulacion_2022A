
//------------------------------

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Texture.h" //Librería de texturas
#include"shaderClass.h" //Librería de Shaderes
#include"VAO.h" //Librería de VAO
#include"VBO.h" //Librería de VBO
#include"EBO.h" //Librería de EBO
#include"Camera.h" //Librería de Cámara
#include"Header.h" //Librería de Cámara
#include"Definitions.h" //Librería de Cámara

using namespace std;


//TAMAÑO DE CÁMARA
const unsigned int width = 800;
const unsigned int height = 800;

const int N_RAYOS = 10;



// Coordinación de Vertices coordinates
GLfloat vertices[108];

// Coordinación de Vertices coordinates
GLfloat verticesFuente[180];

// Indices for vertices order
GLuint indices[108];


GLuint indicesFuente[180];


int main()
{

	//Declaracion de la sala
	room r;
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


	int cont_vertices = 0;
	for (int i = 0; i < r.NP; i++) {
		plane planeAux = r.p[i];
		for (int j = 0; j < planeAux.NT; j++) {
			triangle triangleAux = planeAux.t[j];
			point pointAux;

			pointAux = triangleAux.p0;

			indices[cont_vertices] = cont_vertices;
			vertices[cont_vertices] = pointAux.x;
			cont_vertices++;

			indices[cont_vertices] = cont_vertices;
			vertices[cont_vertices] = pointAux.y;
			cont_vertices++;

			indices[cont_vertices] = cont_vertices;
			vertices[cont_vertices] = pointAux.z;
			cont_vertices++;

			pointAux = triangleAux.p1;

			indices[cont_vertices] = cont_vertices;
			vertices[cont_vertices] = pointAux.x;
			cont_vertices++;

			indices[cont_vertices] = cont_vertices;
			vertices[cont_vertices] = pointAux.y;
			cont_vertices++;

			indices[cont_vertices] = cont_vertices;
			vertices[cont_vertices] = pointAux.z;
			cont_vertices++;

			pointAux = triangleAux.p2;

			indices[cont_vertices] = cont_vertices;
			vertices[cont_vertices] = pointAux.x;
			cont_vertices++;

			indices[cont_vertices] = cont_vertices;
			vertices[cont_vertices] = pointAux.y;
			cont_vertices++;

			indices[cont_vertices] = cont_vertices;
			vertices[cont_vertices] = pointAux.z;
			cont_vertices++;

		}
	}

	//Declaracion de la fuente
	source s;
	
	

	int contFuente = 0;
	for (int i = 0; i < 20; i++) {
		indicesFuente[contFuente] = contFuente;
		verticesFuente[contFuente] = s.IcoFace[i].p0.x;
		contFuente++;

		indicesFuente[contFuente] = contFuente;
		verticesFuente[contFuente] = s.IcoFace[i].p0.y;
		contFuente++;

		indicesFuente[contFuente] = contFuente;
		verticesFuente[contFuente] = s.IcoFace[i].p0.z;
		contFuente++;


		indicesFuente[contFuente] = contFuente;
		verticesFuente[contFuente] = s.IcoFace[i].p1.x;
		contFuente++;

		indicesFuente[contFuente] = contFuente;
		verticesFuente[contFuente] = s.IcoFace[i].p1.y;
		contFuente++;

		indicesFuente[contFuente] = contFuente;
		verticesFuente[contFuente] = s.IcoFace[i].p1.z;
		contFuente++;


		indicesFuente[contFuente] = contFuente;
		verticesFuente[contFuente] = s.IcoFace[i].p2.x;
		contFuente++;

		indicesFuente[contFuente] = contFuente;
		verticesFuente[contFuente] = s.IcoFace[i].p2.y;
		contFuente++;

		indicesFuente[contFuente] = contFuente;
		verticesFuente[contFuente] = s.IcoFace[i].p2.z;
		contFuente++;

	}

	//NUMERO DE RAYOS
	s.createRays(N_RAYOS);

	//REFLEXIONES
	reflection* reflexiones = r.RayTracing(s.p, s.Rays, s.NRAYS);

	int idRayo = 0;
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





	// Initializar GLFW
	glfwInit();

	// Decirle a GLFW que versión de OpenGL se esta usando 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Decirle a GLFW que se esta usando el perfil CORE
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Crear un objeto GLFWwindow  de  800 x 800 pixeles, llamado "Proyecto B1"
	GLFWwindow* window = glfwCreateWindow(width, height, "Proyecto B1", NULL, NULL);
	// Comprobación de error si la ventana no se crea
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introducir la ventana en el contexto actual
	glfwMakeContextCurrent(window);

	//Cargar GLAD para que configure OpenGL
	gladLoadGL();
	// Especificar la ventana gráfica de OpenGL en la ventana
	// En este caso, la ventana gráfica va de x = 0, y = 0, a x = 800, y = 800
	glViewport(0, 0, width, height);


	// Genera el objeto Shader usando shaders default.vert y default.frag
	Shader shaderProgram("default.vert", "default.frag");


	// Generar Vertex Array Object y vincularlos
	VAO VAO1;
	VAO1.Bind();

	// Generar un objeto de búfer de vértices y vincularlo a los vértices
	VBO VBO1(vertices, sizeof(vertices));
	// Generar un  objeto de búfer de elemento y vincularlo a los índices
	EBO EBO1(indices, sizeof(indices));

	// Vincula atributos de VBO como coordenadas y colores a VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	//VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Generar Vertex Array Object y vincularlos
	VAO VAO2;
	VAO2.Bind();

	// Generar un objeto de búfer de vértices y vincularlo a los vértices
	VBO VBO2(verticesFuente, sizeof(verticesFuente));
	// Generar un  objeto de búfer de elemento y vincularlo a los índices
	EBO EBO2(indicesFuente, sizeof(indicesFuente));

	// Vincula atributos de VBO como coordenadas y colores a VAO
	VAO2.LinkAttrib(VBO2, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	//VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO2.Unbind();
	VBO2.Unbind();
	EBO2.Unbind();




	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	double tiempo = 0.0;
	double tiempoAux = 0.0;
	double distancia = 0.0;
	double distanciaAux = 0.0;
	double velocidad = 0.5;
	double velocidadAux = 0.5;

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		//CALCULO DE TRAYECTORIAS Y ANIMACION
		tiempo = glfwGetTime() - tiempoAux;
		distancia = inicio.distancia(llegada);
		distanciaAux = tiempo * velocidad;

		if (distanciaAux >= distancia) {
			inicio = llegada;
			nPunto++;
			inicio.x = reflexiones[idRayo].r[nPunto].x;
			inicio.y = reflexiones[idRayo].r[nPunto].y;
			inicio.z = reflexiones[idRayo].r[nPunto].z;
			tiempoAux = glfwGetTime();
		}

		velocidadAux = velocidad / distancia;
		Vector vecDistancia = llegada.restaPuntos(inicio);
		Vector vecTraslacion = vecDistancia * tiempo * velocidadAux;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(inicio.x+vecTraslacion.x, inicio.z + vecTraslacion.z, inicio.z + vecTraslacion.z));


		// Specify the color of the background
		//glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();

		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

		// Binds texture so that is appears in rendering
		//brickTex.Bind();
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);


		VAO2.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, sizeof(indicesFuente) / sizeof(int), GL_UNSIGNED_INT, 0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}


	

	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	//brickTex.Delete();
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}