/*
Práctica 5: Optimización y Carga de Modelos
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
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;
//float angulocola = 0.0f;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;
Model Goddard_M;
Model Cola;
Model PataDD;
Model PataDIz;
Model PataTIz;
Model PataTD;
Model Mandi;
Model CCoche;
Model LlantaDD;
Model LlantaDIz;
Model LlantaTD;
Model LlantaTIz;
Model Cofre;

Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_m.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_m.frag";





void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.5f, 7.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 1.0f);
	/*
	Goddard_M = Model();
	Goddard_M.LoadModel("Models/Cuerpo1.obj");

	Cola = Model();
	Cola.LoadModel("Models/Cola.obj");

	PataDD = Model();
	PataDD.LoadModel("Models/PataDD.obj");


	PataDIz = Model();
	PataDIz.LoadModel("Models/PATADIz.obj");

	PataTIz = Model();
	PataTIz.LoadModel("Models/PataTIz.obj");


	PataTD = Model();
	PataTD.LoadModel("Models/PataTD.obj");


	Mandi = Model();
	Mandi.LoadModel("Models/Mandi.obj");
	*/
	CCoche  = Model();
	CCoche.LoadModel("Models/CCoche.obj");

	Cofre = Model();
	Cofre.LoadModel("Models/Cofre.obj");

	LlantaDD = Model();
	LlantaDD.LoadModel("Models/LlantaDD.obj");

	LlantaDIz = Model();
	LlantaDIz.LoadModel("Models/LlantaDIz.obj");

	LlantaTD = Model();
	LlantaTD.LoadModel("Models/LlantaDD.obj");

	LlantaTIz = Model();
	LlantaTIz.LoadModel("Models/LlantaTIz.obj");


	
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

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
		//Se dibuja el Skybox
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		// INICIA DIBUJO DEL PISO
		color = glm::vec3(0.5f, 0.5f, 0.5f); //piso de color gris
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMesh();
		/*
		//------------*INICIA DIBUJO DE NUESTROS DEMÁS OBJETOS-------------------*
		//Goddard
		color = glm::vec3(0.0f, 0.0f, 0.0f); //modelo de goddard de color negro
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -1.5f));
		//modelaux = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Goddard_M.RenderModel();//modificar por el modelo sin las 4 patas y sin cola
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		//En sesión se separara una parte del modelo de Goddard y se unirá por jeraquía al cuerpo
		modelaux = model;
		
		//Cola
		color = glm::vec3(0.0f, 0.0f, 0.0f); //modelo de goddard de color negro

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.75f, 0.8f, -1.5f));
		//modelaux = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cola.RenderModel();//modificar por la cola
		color = glm::vec3(0.0f, 0.0f, 1.0f);

		//pata delantera derecha
		modelaux = model;
		color = glm::vec3(0.0f, 0.0f, 0.0f); //modelo de goddard de color negro

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.1f, 1.0f, -0.9f));
		float angulo = glm::clamp(mainWindow.getarticulacion1(), -45.0f, 45.0f);
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 0.0f, 1.0f));
		float angulo9 = glm::clamp(mainWindow.getarticulacion10(), -45.0f, 45.0f);
		model = glm::rotate(model, glm::radians(angulo9), glm::vec3(0.0f, 0.0f, -1.0f));
		//modelaux = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PataDD.RenderModel();//modificar para la pata delantera derecha 
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		
		// pata delantera izquierda
		modelaux = model;
		color = glm::vec3(0.0f, 0.0f, 0.0f); //modelo de goddard de color negro

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.1f, 1.0f, -2.3f));
		float angulo1 = glm::clamp(mainWindow.getarticulacion2(), -45.0f, 45.0f);
		model = glm::rotate(model, glm::radians(angulo1), glm::vec3(0.0f, 0.0f, 1.0f));
		float angulo8 = glm::clamp(mainWindow.getarticulacion9(), -45.0f, 45.0f);
		model = glm::rotate(model, glm::radians(angulo8), glm::vec3(0.0f, 0.0f, -1.0f));
		
		//modelaux = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PataDIz.RenderModel();//modificar para pata delantera izquerda
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		//pata trasera derecha
		modelaux = model;
		color = glm::vec3(0.0f, 0.0f, 0.0f); //modelo de goddard de color negro

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.5f, 0.1f, -0.9f));
		float angulo2 = glm::clamp(mainWindow.getarticulacion3(), -45.0f, 45.0f);
		model = glm::rotate(model, glm::radians(angulo2), glm::vec3(0.0f, 0.0f, 1.0f));
		float angulo7 = glm::clamp(mainWindow.getarticulacion8(), -45.0f, 45.0f);
		model = glm::rotate(model, glm::radians(angulo7), glm::vec3(0.0f, 0.0f, -1.0f));
		//modelaux = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PataTD.RenderModel();//modificar por Pata trasera Izquierda
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		//pata trasera izquierda
		modelaux = model;
		color = glm::vec3(0.0f, 0.0f, 0.0f); //modelo de goddard de color negro

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.5f, -0.1f, -2.3f));
		float angulo3 = glm::clamp(mainWindow.getarticulacion4(), -45.0f, 45.0f);
		model = glm::rotate(model, glm::radians(angulo3), glm::vec3(0.0f, 0.0f, 1.0f));
		float angulo6 = glm::clamp(mainWindow.getarticulacion7(), -45.0f, 45.0f);
		model = glm::rotate(model, glm::radians(angulo6), glm::vec3(0.0f, 0.0f, -1.0f));
		//modelaux = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PataTIz.RenderModel();//modificar para Pata trasera Izquierda
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		//Mandibula 
		modelaux = model;
		color = glm::vec3(0.0f, 0.0f, 0.0f); //modelo de goddard de color negro

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.0f, 1.8f, -1.5f));
		float angulo4 = glm::clamp(mainWindow.getarticulacion5(), -45.0f, 45.0f);
		model = glm::rotate(model, glm::radians(angulo4), glm::vec3(0.0f, 0.0f, 1.0f));
		float angulo5 = glm::clamp(mainWindow.getarticulacion6(), -45.0f, 45.0f);
		model = glm::rotate(model, glm::radians(angulo5), glm::vec3(0.0f, 0.0f, -1.0f));
		//modelaux = model;                                          
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mandi.RenderModel();//modificar la mandibula 
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		//En sesión se separara una parte del modelo de Goddard y se unirá por jeraquía al cuerpo
		*/
		// Llanta trasera izquierda
        float ATI = mainWindow.getarticulacion8() * 0.01f;
        float RTI = mainWindow.getarticulacion9() * 0.01f;
        float desplazamientoTI = ATI - RTI;

         // Llanta trasera derecha
         float ATD = mainWindow.getarticulacion6() * 0.01f;
         float RTD = mainWindow.getarticulacion7() * 0.01f;
         float desplazamientoTD = ATD - RTD;

         // Llanta delantera izquierda
         float ADI = mainWindow.getarticulacion4() * 0.01f;
         float RDI = mainWindow.getarticulacion5() * 0.01f;
         float desplazamientoDI = ADI - RDI;

          // Llanta delantera derecha
          float ADD = mainWindow.getarticulacion2() * 0.01f;
          float RDD = mainWindow.getarticulacion3() * 0.01f;
          float desplazamientoDD = ADD - RDD;

		  float desplazamientoTotal = (desplazamientoTI + desplazamientoTD + desplazamientoDI + desplazamientoDD) / 4.0f;
		  glm::mat4 modelCarro = glm::mat4(1.0);
		  modelCarro = glm::translate(modelCarro, glm::vec3(desplazamientoTotal, 0.0f, 0.0f));

          // Cuerpo del cohce color azul 
		  
		glm::mat4 model = modelCarro;
		color = glm::vec3(0.0f, 0.0f, 1.0f); //modelo del cuerpo del coche color azul 
		model = glm::mat4(1.0);
		model = modelCarro;
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -1.5f));
		modelaux = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CCoche.RenderModel();//modificar el coche 
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		
		model = modelaux;

		//Cofre 
		model = modelaux;
		model = modelCarro;
		model = glm::translate(model, glm::vec3(4.181f, 2.42f, -1.75f));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		model = glm :: rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cofre.RenderModel();//modificar el cofre 

		//Lanta delantera derecha
		model = modelaux;
		model = modelCarro;
		model = glm::translate(model, glm::vec3(5.0f, 0.5f, 1.31f));
		color = glm::vec3(0.0f, 0.0f, 0.0f); //color negro de las llantas 
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaDD.RenderModel();//modificar el cofre 
		 
		// Llanta delantera izquierda 
		model = modelaux;
		model = modelCarro;
		model = glm::translate(model, glm::vec3(5.0f, 0.35f, -4.9f));
		color = glm::vec3(0.0f, 0.0f, 0.0f); //color negro de las llantas 
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaDIz.RenderModel();//modificar la llanta delantera izquierda 
		 
		// Llanta trasera derecha 
		model = modelaux;
		model = modelCarro;
		model = glm::translate(model, glm::vec3(-5.0f, 0.5f, 1.31f));
		color = glm::vec3(0.0f, 0.0f, 0.0f); //color negro de las llantas 
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaTD.RenderModel();//modificar la llanta trasera derecha 
				
		// Llanta trasera  izquierda 
		model = modelaux;
		model = modelCarro;
		model = glm::translate(model, glm::vec3(-5.0f, 0.35f, -4.9f));
		color = glm::vec3(0.0f, 0.0f, 0.0f); //color negro de las llantas 
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()), glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion9()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaTIz.RenderModel();//modificar la llanta trasera izqueirda  
		
		
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
//Siguientes modelos
		/* Ejercicio:
		1.- Separar las 4 patas de Goddard del modelo del cuerpo, unir por medio de jerarquía cada pata al cuerpo de Goddard
		2.- Hacer que al presionar una tecla cada pata pueda rotar un máximo de 45° "hacia adelante y hacia atrás"
		*/