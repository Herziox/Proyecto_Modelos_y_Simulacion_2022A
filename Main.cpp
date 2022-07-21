//------- Ignore this ----------
#include<filesystem>
namespace fs = std::filesystem;
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



// Coordinación de Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      /   
	-0.5f, -0.5f,  0.5f,     0.9f, 0.0f, 0.0f, //0
	-0.5f, -0.5f, -0.5f,      0.9f, 0.0f, 0.0f, //1
	 0.5f, -0.5f, -0.5f,      0.9f, 0.0f, 0.0f, //2
	 0.5f, -0.5f,  0.5f,     0.9f, 0.0f, 0.0f, //3
	-0.5f, 0.5f,  0.5f,      0.9f, 0.0f, 0.0f,  //4 
	-0.5f, 0.5f, -0.5f,      0.9f, 0.0f, 0.0f,  //5
	 0.5f, 0.5f, -0.5f,      0.9f, 0.0f, 0.0f,  //6
	 0.5f, 0.5f,  0.5f,      0.9f, 0.0f, 0.0f	  //7
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3,

	0, 1, 4,
	0, 1, 5,

	0, 3, 7,
	0, 7, 4,

	6, 4, 5,
	6, 7, 4,

	6, 3, 7,
	6, 2, 3,

	6, 5, 1,
	6, 1, 2
};


int main()
{

	plane p;
	p.NewPoints(4);

	p.p[0].x = -1.0;
	p.p[0].y = 1.0;
	p.p[0].z = 1.0;

	p.p[1].x = 1.0;
	p.p[1].y = 1.0;
	p.p[1].z = 1.0;

	p.p[2].x = 1.0;
	p.p[2].y = 1.0;
	p.p[2].z = -1.0;

	p.p[3].x = -1.0;
	p.p[3].y = 1.0;
	p.p[3].z = -1.0;
	cout << "N�mero de puntos totales" << endl;
	cout << p.NP << endl;
	cout << "Puntos de p[0]" << endl;
	p.p[0].print();
	cout << "Puntos de p[1]" << endl;
	p.p[1].print();
	cout << "Puntos de p[2]" << endl;
	p.p[2].print();
	cout << "Puntos de p[3]" << endl;
	p.p[3].print();
	cout << "N�mero de puntos totales despues de borrae" << endl;
	//p.DeletePoint(0);
	cout << p.NP << endl;
	p.p[0].print();
	cout << "Puntos de p[1]" << endl;
	p.p[1].print();
	cout << "Puntos de p[2]" << endl;
	p.p[2].print();
	cout << "Puntos de p[3]" << endl;
	p.p[3].print();


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
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();



	/*
	* I'm doing this relative path thing in order to centralize all the resources into one folder and not
	* duplicate them between tutorial folders. You can just copy paste the resources from the 'Resources'
	* folder and then give a relative path from this folder to whatever resource you want to get to.
	* Also note that this requires C++17, so go to Project Properties, C/C++, Language, and select C++17
	*/
	std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
	std::string texPath = "/Resources/YoutubeOpenGL 7 - Going 3D/";

	/*// Texture
	Texture brickTex((parentDir + texPath + "brick.png").c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);*/
	//brickTex.texUnit(shaderProgram, "tex0", 0);
	
	// Original code from the tutorial
	/*Texture brickTex("brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	brickTex.texUnit(shaderProgram, "tex0", 0);*/



	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
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