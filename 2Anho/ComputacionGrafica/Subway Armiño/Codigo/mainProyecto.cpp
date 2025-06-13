#include <glad.h>
#include <glfw3.h>
#include <stdio.h>
#include <iostream>
#include "lecturaShader_0_9.h"
#include "esfera.h"
#include <cmath>
#include <fstream>
#include <vector>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

//#include <SFML\Audio.hpp>

using namespace glm; //Para no tener que declarar glm cada vez que lo usemos

#define GradosToRadianes 0.0175
#define RADIO .5
#define GL_PI 3.14f

float distanciaCamara = 70;
float alpha = 0, beta = 0.4;
bool nieblaActiva = false;

//--------------------
float distX = -10.0, distY = -10.0;
float anguloActual = (asin(distX / sqrt(pow(distX, 2) + pow(distY, 2))));
int canal = 2;
bool pivoteDer = false;
bool haciaDerecha = true;
bool debeCambiarAngulo = false;

bool muerto = false;
float globalTimer = 0.0f;
float velGlobal = 1.0f;

void processInput(GLFWwindow* window);

// settings
unsigned int SCR_WIDTH = 800, SCR_HEIGHT = 800;
int winPosX = 0, winPosY = 0, winSizeW, winSizeH;

extern GLuint setShaders(const char* nVertx, const char* nFrag);
GLuint shaderProgram, skyboxShader;

typedef struct {

	float px, py, pz;	//Posicion inicial
	float angulo_trans;	//Angulo de giro (x)
	float velocidad;	//Velocidad de movimiento
	float sx, sy, sz;	//Escalado en los ejes
	unsigned int VAO;	//Lista de render
	float angulo_y;	//Angulo de giro (y)
	int tex;			//Textura

}objeto;

objeto base = { 0, 1.5 ,0.0 ,0 ,0 ,10 ,2.5 , 4,0 ,0,0 };
objeto cabina = { 5.0, 1.0 ,0 ,0 ,0 ,2.0 , 2.0 ,4.0 ,0,0,0 };
objeto articulacion = { -0.4, 0.4 ,0 ,0 ,0 ,1.0 ,1.0 ,1.0 ,0,35,0 };
objeto brazo = { 0.0, 3 ,0 ,0 ,0 ,0.5 ,6.0 ,0.5 ,0,0,0 };

float zCar[] = { -400,-220,-300,-500,-620,-700 };
float speed[] = {0.2,0.2,0.2,0.2,0.2,0.2};

unsigned int VAO;
unsigned int VAOCuadradoXZ;
unsigned int VAOCubo;
unsigned int VAOEsfera;
unsigned int VAOSprite;

unsigned int VAOArminho;
int numVertexArminho;
unsigned int VAOCarretera;
int numVertexCarretera;
unsigned int skyboxVAO, skyboxVBO, skyboxEBO;

unsigned int cubemapTexture;

int hierbaText;
int arbolText;
int conejoText;
int arminhoText;
int carreteraText;
int muerteText;

short modoCamara = 0;
bool modoNoche = false;
bool pantallaCompleta = false;

float velocidad = 0;
float ang_giro = 0;

double lastTime = glfwGetTime();
double currentTime;
double lapsoTime;
int nbFrames = 0;

GLfloat angleWheel = 0.0f; // ngulo de rotaci n de las ruedas


float trans1 = 0.0f;
float rot1 = 0.0f;
float trans2 = 0.0f;
float rot2 = 0.0f;


float skyboxVertices[] =
{
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f
};

unsigned int skyboxIndices[] =
{
	1, 2, 6,
	6, 5, 1,
	0, 4, 7,
	7, 3, 0,
	4, 5, 6,
	6, 7, 4,
	0, 3, 2,
	2, 1, 0,
	0, 1, 5,
	5, 4, 0,
	3, 7, 6,
	6, 2, 3
};


struct Vertex {
	float x, y, z;       // posición
	float r, g, b;       // color fijo
	float nx, ny, nz;    // normal
	float u, v;          // texcoord
};

std::vector<float> loadOBJ(const std::string& filename) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;
	bool success = false;
	success = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str());

	if (!warn.empty()) std::cout << "WARN: " << warn << std::endl;
	if (!err.empty()) std::cerr << "ERR: " << err << std::endl;

	if (!success) {
		std::cerr << "Failed to load OBJ file!" << std::endl;
		return {};
	}

	std::vector<float> vertices;

	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			
			float x = 0, y = 0, z = 0;
			if (index.vertex_index >= 0 && attrib.vertices.size() >= 3 * size_t(index.vertex_index + 1)) {
				x = attrib.vertices[3 * index.vertex_index + 0];
				y = attrib.vertices[3 * index.vertex_index + 1];
				z = attrib.vertices[3 * index.vertex_index + 2];
			}

			float nx = 0.0f, ny = 1.0f, nz = 0.0f;
			if (index.normal_index >= 0 && attrib.normals.size() >= 3 * size_t(index.normal_index + 1)) {
				nx = attrib.normals[3 * index.normal_index + 0];
				ny = attrib.normals[3 * index.normal_index + 1];
				nz = attrib.normals[3 * index.normal_index + 2];
			}

			float u = 0.0f, v = 0.0f;
			if (index.texcoord_index >= 0 && attrib.texcoords.size() >= 2 * size_t(index.texcoord_index + 1)) {
				u = attrib.texcoords[2 * index.texcoord_index + 0];
				v = attrib.texcoords[2 * index.texcoord_index + 1];
			}

			// Push into vertex array: pos, color, normal, texcoord
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
			vertices.push_back(0.5f);  // color R
			vertices.push_back(0.5f);  // color G
			vertices.push_back(0.5f);  // color B
			vertices.push_back(nx);
			vertices.push_back(ny);
			vertices.push_back(nz);
			vertices.push_back(u);
			vertices.push_back(v);
		}
	}

	return vertices;
}

//Funciones de camara
void myCamara() {
	//Matriz de proyecci n
	mat4 projection;
	projection = mat4(1.0f);
	unsigned int proyectionLoc = glGetUniformLocation(shaderProgram, "projection");
	projection = perspective(radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 2000.0f);
	glUniformMatrix4fv(proyectionLoc, 1, GL_FALSE, value_ptr(projection));

	//Matriz de vista
	mat4 view;
	view = mat4(1.0f);
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
	view = lookAt(
		vec3((float)distanciaCamara * (float)sin(alpha) * cos(beta), (float)distanciaCamara * (float)sin(beta), (float)distanciaCamara * cos(alpha) * cos(beta)),
		vec3(0, 0, 0),
		vec3(0, 1, 0));

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
}
void camaraCenital() {

	mat4 projection;
	projection = mat4(1.0f);
	unsigned int proyectionLoc = glGetUniformLocation(shaderProgram, "projection");
	projection = perspective(radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 2000.0f);
	glUniformMatrix4fv(proyectionLoc, 1, GL_FALSE, value_ptr(projection));


	mat4 view;
	view = mat4(1.0f);
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
	view = lookAt(vec3(0, 200, 0),
		vec3(0, 0, -10),
		vec3(0, 1, 0));

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
}

//Funciones generales
void tiempo() {
	static float unSegundo = 0;
	currentTime = glfwGetTime();
	nbFrames++;
	lapsoTime = currentTime - lastTime;
	unSegundo = unSegundo + lapsoTime;
	if (unSegundo >= 1.0) {
		printf("%f frame/s\n", double(nbFrames));

		if (nbFrames > 5) {
			velGlobal = 300.0f / float(nbFrames);
		}
		
		nbFrames = 0;
		unSegundo = 0;
		globalTimer += 1.0;
	}
	lastTime = currentTime;
}
void movimiento() {

	if (base.px > 40) {
		base.px = -39.5f;
	}
	else if (base.px < -40) {
		base.px = 39.5f;
	}

	if (base.pz > 40) {
		base.pz = -39.5f;
	}
	else if (base.pz < -40) {
		base.pz = 39.5f;
	}

	base.px = base.px + base.velocidad * sin(base.angulo_trans * GradosToRadianes) * lapsoTime;
	base.pz = base.pz + base.velocidad * cos(base.angulo_trans * GradosToRadianes) * lapsoTime;

	angleWheel += base.velocidad / 12;
	if (angleWheel > 360) angleWheel -= 360;
}
void calcAngulo(float angulo) {
	
	if (debeCambiarAngulo) {
		anguloActual = -anguloActual;
		debeCambiarAngulo = false;
	}
	
	if ((angulo - anguloActual) > 0.05) {
		anguloActual += 0.01 * velGlobal;
	}
	else if ((anguloActual - angulo) > 0.05) {
		anguloActual -= 0.01 * velGlobal;
	}
}

int myCargaTexturas(const char* nome) {
	GLuint textura;

	glGenTextures(1, &textura);
	glBindTexture(GL_TEXTURE_2D, textura);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // set texture filtering to GL_LINEAR (near)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // set texture filtering to GL_LINEAR (far)

	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	unsigned char* data = stbi_load(nome, &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else if (nrChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("Failed to load texture\n");
	}

	stbi_image_free(data);
	stbi_set_flip_vertically_on_load(1);

	return textura;
}

void iluminacion() {
	//Luces primer coche
	
	glUniform3f(glGetUniformLocation(shaderProgram, "lightColor"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(shaderProgram, "anguloLuz"), 15.0f);
	glUniform3f(glGetUniformLocation(shaderProgram, "lightPos"), -22, base.py + 0.5f, zCar[0]);
	glUniform3f(glGetUniformLocation(shaderProgram, "luzDir"), sin(base.angulo_trans * GradosToRadianes), 0, cos(base.angulo_trans * GradosToRadianes));
	glUniform3f(glGetUniformLocation(shaderProgram, "viewPos"), base.px, base.py, base.pz);
	
	//Luces segundo coche
	glUniform3f(glGetUniformLocation(shaderProgram, "lightColor2"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(shaderProgram, "anguloLuz2"), 15.0f);
	glUniform3f(glGetUniformLocation(shaderProgram, "lightPos2"), 0, base.py + 0.5f, zCar[1]);
	glUniform3f(glGetUniformLocation(shaderProgram, "luzDir2"), sin(base.angulo_trans * GradosToRadianes), 0, cos(base.angulo_trans * GradosToRadianes));
	glUniform3f(glGetUniformLocation(shaderProgram, "viewPos2"), base.px, base.py, base.pz);

	//Luces tercer coche
	glUniform3f(glGetUniformLocation(shaderProgram, "lightColor3"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(shaderProgram, "anguloLuz3"), 15.0f);
	glUniform3f(glGetUniformLocation(shaderProgram, "lightPos3"), 22, base.py + 0.5f, zCar[2]);
	glUniform3f(glGetUniformLocation(shaderProgram, "luzDir3"), sin(base.angulo_trans * GradosToRadianes), 0, cos(base.angulo_trans * GradosToRadianes));
	glUniform3f(glGetUniformLocation(shaderProgram, "viewPos3"), base.px, base.py, base.pz);

	if (globalTimer > 30.0f) {
		nieblaActiva = false;
		modoNoche = false;
		globalTimer = 0.0f;		
	}

}
void iluminacionDia() {

	//Por el día, luz solar de uno de los focos
	if (globalTimer > 30.0f) {
		float luzActual = 0.7 / ((globalTimer) / 20);
		glUniform3f(glGetUniformLocation(shaderProgram, "lightColor"), luzActual, luzActual, luzActual);
		if (globalTimer > 60.0f) {
			nieblaActiva = true;
			modoNoche = true;
			globalTimer = 0.0f;
		}
	}
	else {
		glUniform3f(glGetUniformLocation(shaderProgram, "lightColor"), 0.7f, 0.7f, 0.7f);
	}
	glUniform1f(glGetUniformLocation(shaderProgram, "anguloLuz"), 360.0f);
	glUniform3f(glGetUniformLocation(shaderProgram, "lightPos"), 5, 50, 5);
	glUniform3f(glGetUniformLocation(shaderProgram, "luzDir"), 0, 0, 0);
	glUniform3f(glGetUniformLocation(shaderProgram, "viewPos"), 0.3f, 0.3f, 0.3f);

	//Apagamos las otras dos luces
	glUniform3f(glGetUniformLocation(shaderProgram, "lightColor2"), 0.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(shaderProgram, "lightColor3"), 0.0f, 0.0f, 0.0f);

}
//Funciones de dibujo
void drawCubo() {
	glBindVertexArray(VAOCubo);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
void drawSprite() {

	glDisable(GL_CULL_FACE); //Deshabilitamos la ocultacion de caras para que se vea el sprite por ambas caras
	glBindVertexArray(VAOSprite);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glEnable(GL_CULL_FACE); //ocultacion caras back
	glCullFace(GL_BACK);
}

void dibujaMuerte() {

	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "model");
	unsigned int colorLoc = glGetUniformLocation(shaderProgram, "Color");
	
	mat4 transform;
	transform = mat4(1.0f); //Crea la matriz identidad
	
	
	transform = translate(transform, vec3(0, 15, 25)); //Hace la operacion de translacion
	
	//transform = rotate(transform, (float)(180 * GradosToRadianes), vec3(0.0f, 0.0f, 1.0f));
	//transform = rotate(transform, (float)(40 * GradosToRadianes), vec3(1.0f, 0.0f, 0.0f));
	transform = scale(transform, vec3(40, 40, 40)); //Hace la operacion de escalado
	transform = rotate(transform, (float)(240 * GradosToRadianes), vec3(1.0f, 0.0f, 0.0f));

	
	
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(transform));
	glUniform3fv(colorLoc, 1, value_ptr(vec3(1, 1, 1))); //Le da color
	//Lo dibuja
	glDisable(GL_CULL_FACE); //Deshabilitamos la ocultacion de caras para que se vea el sprite por ambas caras
	glBindTexture(GL_TEXTURE_2D, muerteText);
	glBindVertexArray(VAOCuadradoXZ);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindTexture(GL_TEXTURE_2D, 0);
}
void dibujaSuelo(mat4 transform, unsigned int transformLoc, unsigned int colorLoc) {
	//SUELO
	transform = mat4(1.0f); //Crea la matriz identidad
	transform = translate(transform, vec3(0, -5.5, 0)); //Hace la operacion de translacion
	transform = scale(transform, vec3(10000, 1, 10000)); //Hace la operacion de escalado
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(transform));
	glUniform3fv(colorLoc, 1, value_ptr(vec3(0, 1, 0))); //Le da color
	//Lo dibuja
	glBindTexture(GL_TEXTURE_2D, hierbaText);
	glBindVertexArray(VAOCuadradoXZ);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindTexture(GL_TEXTURE_2D, 0);
}
void dibujaRuedas(mat4 reference, unsigned int transformLoc, unsigned int colorLoc) {

	//Creo las variables y las inicializo
	mat4 transform = reference;
	mat4 stack;

	glUniform3fv(colorLoc, 1, value_ptr(vec3(0, 0, 0))); //Le da color a todas las ruedas ya
	
	stack = transform;

	transform = translate(transform, vec3(3.5, -0.5, 2.0));
	transform = rotate(transform, (float)(-angleWheel * GradosToRadianes), vec3(0.0f, 0.0f, 1.0f));
	transform = scale(transform, vec3(1.5, 1.5, 1.5));
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(transform));
	drawCubo();

	transform = stack;
	transform = translate(transform, vec3(3.5, -0.5, -2.0));
	transform = rotate(transform, (float)(-angleWheel * GradosToRadianes), vec3(0.0f, 0.0f, 1.0f));
	transform = scale(transform, vec3(1.5, 1.5, 1.5));
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(transform));
	drawCubo();

	transform = stack;
	transform = translate(transform, vec3(-3.5, -0.5, 2.0));
	transform = rotate(transform, (float)(-angleWheel * GradosToRadianes), vec3(0.0f, 0.0f, 1.0f));
	transform = scale(transform, vec3(1.5, 1.5, 1.5));
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(transform));
	drawCubo();

	transform = stack;
	transform = translate(transform, vec3(-3.5, -0.5, -2.0));
	transform = rotate(transform, (float)(-angleWheel * GradosToRadianes), vec3(0.0f, 0.0f, 1.0f));
	transform = scale(transform, vec3(1.5, 1.5, 1.5));
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(transform));
	drawCubo();
}
mat4 dibujaCamionBase(unsigned int transformLoc, unsigned int colorLoc, float posIn = 0, float vel = 0) {

	mat4 transform = mat4(1.0f); //Crea la matriz identidad
	transform = translate(transform, vec3(posIn, 1, vel)); //Hace la operacion de translacion
	transform = rotate(transform, (float)((-90) * GradosToRadianes), vec3(0.0f, 1.0f, 0.0f));
	transform = scale(transform, vec3(3, 3, 3)); //Hace la operacion de escalado
	mat4 stack = transform; //Guarda la traslacion y la rotacion de la base
	transform = translate(transform, vec3(base.px, base.py, base.pz)); //Hace la operacion de translacion
	transform = scale(transform, vec3(7, 3, 4)); //Hace la operacion de escalado

	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(transform));
	glUniform3fv(colorLoc, 1, value_ptr(vec3(0.8, 0, 0.8))); //Le da color
	glBindVertexArray(base.VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	return stack;
}
void dibujaCamionCabina(mat4 reference, unsigned int transformLoc, unsigned int colorLoc) {

	mat4 transform = reference; //Aplica la traslacion y la rotacion de la base pra que la cabina y demas cosas se muevan junto a ella
	transform = translate(transform, vec3(cabina.px, cabina.py, cabina.pz)); //Hace la operacion de translacion
	transform = rotate(transform, (float)(cabina.angulo_trans), vec3(1.0f, 0.0f, 0.0f));
	transform = scale(transform, vec3(cabina.sx, cabina.sy, cabina.sz)); //Hace la operacion de escalado

	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(transform));
	glUniform3fv(colorLoc, 1, value_ptr(vec3(0.8, 0, 1))); //Le da color
	glBindVertexArray(cabina.VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
void dibujaCamion(int posIn = 0, float vel = 0) {

	mat4 stack;
	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "model");
	unsigned int colorLoc = glGetUniformLocation(shaderProgram, "Color");

	stack = dibujaCamionBase(transformLoc, colorLoc, posIn,vel);
	dibujaCamionCabina(stack, transformLoc, colorLoc);
	//dibujaCamionGrua(stack, transformLoc, colorLoc);
	dibujaRuedas(stack, transformLoc, colorLoc);
}

void dibujaCarretera(unsigned int transformLoc, unsigned int colorLoc) {

	mat4 transform = mat4(1.0f); //Crea la matriz identidad

	transform = scale(transform, vec3(5, 5, 2000));                  // PRIMERO escala
	transform = rotate(transform, radians(90.0f), vec3(0, 1, 0));  // Luego rota
	transform = translate(transform, vec3(0, -1, 0));

	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(transform));
	glUniform3fv(colorLoc, 1, value_ptr(vec3(1, 1, 1))); //Le da color
	glBindVertexArray(VAOCarretera);
	glBindTexture(GL_TEXTURE_2D, carreteraText);
	glDrawArrays(GL_TRIANGLES, 0, numVertexCarretera);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void spawnCamion() {
	
	for (int i = 0; i < 6; i++) {
	
		if (zCar[i] > 70) {
			zCar[i] = -500;
			speed[i] = (((rand() % 5) + 5) / 20.0f);
		}

		zCar[i] += speed[i] * velGlobal;

		if (zCar[i] > -2 && zCar[i] < 2 && i % 3 == canal-1) {
			muerto = true;
		}
	}

	dibujaCamion(-22,zCar[0]);
	dibujaCamion(0, zCar[1]);
	dibujaCamion(22, zCar[2]);
	dibujaCamion(-22, zCar[3]);
	dibujaCamion(0, zCar[4]);
	dibujaCamion(22, zCar[5]);
}

void dibujaArminho(unsigned int transformLoc, unsigned int colorLoc) {

	if (pivoteDer) {
		distX = 10.0;
	}
	else {
		distX = -10.0;
	}

	mat4 transform = mat4(1.0f); //Crea la matriz identidad

	transform = translate(transform, vec3(distX, distY, 0)); //Hace la operacion de translacion

	float hipotenusa = sqrt(pow(distX, 2) + pow(distY, 2));
	float angulo = (asin(distX / hipotenusa));
	if (canal != 2) {
		angulo = -angulo;
	}

	calcAngulo(angulo);

	transform = rotate(transform, (float)(anguloActual), vec3(0.0f, 0.0f, 1.0f));
	transform = translate(transform, vec3(0, hipotenusa, 0)); //Hace la operacion de translacion

	if (!haciaDerecha) //Mira si tiene que rotarlo hacia el otro lado
		transform = rotate(transform, (float)(180.0f * GradosToRadianes), vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(transform));
	glUniform3fv(colorLoc, 1, value_ptr(vec3(1, 1, 1))); //Le da color
	glBindVertexArray(VAOArminho);
	glBindTexture(GL_TEXTURE_2D, arminhoText);
	glDrawArrays(GL_TRIANGLES, 0, numVertexArminho);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void dibujaArbol() {

	//Creo las variables y las inicializo
	mat4 transform;
	mat4 stack;
	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "model");
	unsigned int colorLoc = glGetUniformLocation(shaderProgram, "Color");

	glUniform3fv(colorLoc, 1, value_ptr(vec3(0, 0, 0)));

	transform = mat4(1.0f);
	transform = scale(transform, vec3(20, 20, 20));
	transform = translate(transform, vec3(0, 0.5, 0));
	transform = rotate(transform, (float)(180 * GradosToRadianes), vec3(1.0f, 0.0f, 0.0f));
	stack = transform;

	transform = translate(transform, vec3(3.2, 0, 1.7));
	transform = rotate(transform, (float)(40 * GradosToRadianes), vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(transform));
	glBindTexture(GL_TEXTURE_2D, arbolText);
	drawSprite();

	transform = translate(transform, vec3(-0.2, 0, -0.13));
	transform = rotate(transform, (float)(20 * GradosToRadianes), vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(transform));
	drawSprite();

	transform = translate(transform, vec3(0.4, 0, 0));
	transform = rotate(transform, (float)(-40 * GradosToRadianes), vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(transform));
	drawSprite();

	transform = translate(transform, vec3(0.4, 0, 0));
	transform = rotate(transform, (float)(20 * GradosToRadianes), vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(transform));
	drawSprite();

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void display() {

	//Activamos el programa de shaders y hacemos las matrices
	glUseProgram(shaderProgram);

	mat4 transform; //Matriz de transformaci n
	mat4 stack; //Matriz que guarda los valores de las transformaciones

	//Encontramos los valores en el programa de shaders
	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "model");
	unsigned int colorLoc = glGetUniformLocation(shaderProgram, "Color");

	if (modoCamara!=0) camaraCenital();
	else myCamara();

	if (modoNoche)  iluminacion();
	else iluminacionDia();

	//Se dibuja el cuadrado
	int cuboScale = 1;
	transform = mat4(1.0f); //Inicializamos la matriz como una identidad
	transform = scale(transform, vec3(5, 5, 5)); //Hace la operacion de escalado
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(transform));
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, 6); //Lo dibuja

	//Dibujamos el suelo
	//dibujaSuelo(transform, transformLoc, colorLoc);

	//Dibujamos el persoaje
	dibujaCarretera(transformLoc, colorLoc);
	dibujaArminho(transformLoc, colorLoc);
	dibujaSuelo(transform, transformLoc, colorLoc);
	dibujaArbol();

	spawnCamion();

	glBindVertexArray(0); //Desvincula
	glEnable(GL_CULL_FACE); //ocultacion caras back

}

//Funciones de inicializacion de objetos

void setupArbol() {

	unsigned int VBO;
	float vertices[] = {
		 0.5f,  0.5f,  0.0f,   0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.0f,   0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.0f,   0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,

		 0.5f,  0.5f,  0.0f,   0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
		-0.5f, -0.5f,  0.0f,   0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.0f,   0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAOSprite);
	glGenBuffers(1, &VBO);

	// bind the Vertex Array Object first.
	glBindVertexArray(VAOSprite);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//Normales
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//Textura
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
}

void setupCuadradoXZ() {

	unsigned int VBO;
	float vertices[] = {
		-0.5f,  0.0f,  0.5f,   0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
		 0.5f,  0.0f,  0.5f,   0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
		 0.5f,  0.0f, -0.5f,   0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,

		-0.5f,  0.0f,  0.5f,   0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
		 0.5f,  0.0f, -0.5f,   0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
		-0.5f,  0.0f, -0.5f,   0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f
	};

	glGenVertexArrays(1, &VAOCuadradoXZ);
	glGenBuffers(1, &VBO);

	// bind the Vertex Array Object first.
	glBindVertexArray(VAOCuadradoXZ);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//Normales
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//Textura
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
}
void setupArminho() {

	unsigned int VBO;
	
	std::vector<float> objVertices = loadOBJ("modelos/Arminho.obj");

	glGenVertexArrays(1, &VAOArminho);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAOArminho);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, objVertices.size() * sizeof(float), objVertices.data(), GL_STATIC_DRAW);

	numVertexArminho = objVertices.size() / 11;
	//Vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//Normales
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//Textura
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
}
void setupCarretera() {

	unsigned int VBO;

	std::vector<float> objVertices = loadOBJ("modelos/Carretera.obj");

	glGenVertexArrays(1, &VAOCarretera);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAOCarretera);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, objVertices.size() * sizeof(float), objVertices.data(), GL_STATIC_DRAW);

	numVertexCarretera = objVertices.size() / 11;
	//Vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//Normales
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//Textura
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
}

void setupCubo() {

	unsigned int VBO;
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------

	float vertices[] = {
		// Frente (0, 0, 1)
		-.5f, -.5f, .5f,  1.0, 0.0, 0.0,   0.0f, 0.0f, 1.0f,
		 .5f, -.5f, .5f,  1.0, 0.0, 0.0,   0.0f, 0.0f, 1.0f,
		 .5f,  .5f, .5f,  1.0, 0.0, 0.0,   0.0f, 0.0f, 1.0f,
		 .5f,  .5f, .5f,  1.0, 0.0, 0.0,   0.0f, 0.0f, 1.0f,
		-.5f,  .5f, .5f,  1.0, 0.0, 0.0,   0.0f, 0.0f, 1.0f,
		-.5f, -.5f, .5f,  1.0, 0.0, 0.0,   0.0f, 0.0f, 1.0f,

		// Atr�s (0, 0, -1)
		-.5f, -.5f, -.5f,  1.0, 0.0, 0.0,   0.0f, 0.0f, -1.0f,
		 .5f,  .5f, -.5f,  1.0, 0.0, 0.0,   0.0f, 0.0f, -1.0f,
		 .5f, -.5f, -.5f,  1.0, 0.0, 0.0,   0.0f, 0.0f, -1.0f,
		 .5f,  .5f, -.5f,  1.0, 0.0, 0.0,   0.0f, 0.0f, -1.0f,
		-.5f, -.5f, -.5f,  1.0, 0.0, 0.0,   0.0f, 0.0f, -1.0f,
		-.5f,  .5f, -.5f,  1.0, 0.0, 0.0,   0.0f, 0.0f, -1.0f,

		// Izquierda (-1, 0, 0)
		-.5f,  .5f, -.5f,  1.0, 0.0, 0.0,  -1.0f, 0.0f, 0.0f,
		-.5f, -.5f,  .5f,  1.0, 0.0, 0.0,  -1.0f, 0.0f, 0.0f,
		-.5f,  .5f,  .5f,  1.0, 0.0, 0.0,  -1.0f, 0.0f, 0.0f,
		-.5f, -.5f,  .5f,  1.0, 0.0, 0.0,  -1.0f, 0.0f, 0.0f,
		-.5f,  .5f, -.5f,  1.0, 0.0, 0.0,  -1.0f, 0.0f, 0.0f,
		-.5f, -.5f, -.5f,  1.0, 0.0, 0.0,  -1.0f, 0.0f, 0.0f,

		// Derecha (1, 0, 0)
		 .5f,  .5f, -.5f,  1.0, 0.0, 0.0,   1.0f, 0.0f, 0.0f,
		 .5f,  .5f,  .5f,  1.0, 0.0, 0.0,   1.0f, 0.0f, 0.0f,
		 .5f, -.5f,  .5f,  1.0, 0.0, 0.0,   1.0f, 0.0f, 0.0f,
		 .5f, -.5f,  .5f,  1.0, 0.0, 0.0,   1.0f, 0.0f, 0.0f,
		 .5f, -.5f, -.5f,  1.0, 0.0, 0.0,   1.0f, 0.0f, 0.0f,
		 .5f,  .5f, -.5f,  1.0, 0.0, 0.0,   1.0f, 0.0f, 0.0f,

		 // Abajo (0, -1, 0)
		 -.5f, -.5f,  .5f,  1.0, 0.0, 0.0,   0.0f, -1.0f, 0.0f,
		  .5f, -.5f, -.5f,  1.0, 0.0, 0.0,   0.0f, -1.0f, 0.0f,
		  .5f, -.5f,  .5f,  1.0, 0.0, 0.0,   0.0f, -1.0f, 0.0f,
		  .5f, -.5f, -.5f,  1.0, 0.0, 0.0,   0.0f, -1.0f, 0.0f,
		 -.5f, -.5f,  .5f,  1.0, 0.0, 0.0,   0.0f, -1.0f, 0.0f,
		 -.5f, -.5f, -.5f,  1.0, 0.0, 0.0,   0.0f, -1.0f, 0.0f,

		 // Arriba (0, 1, 0)
		 -.5f,  .5f,  .5f,  1.0, 0.0, 0.0,   0.0f, 1.0f, 0.0f,
		  .5f,  .5f,  .5f,  1.0, 0.0, 0.0,   0.0f, 1.0f, 0.0f,
		  .5f,  .5f, -.5f,  1.0, 0.0, 0.0,   0.0f, 1.0f, 0.0f,
		  .5f,  .5f, -.5f,  1.0, 0.0, 0.0,   0.0f, 1.0f, 0.0f,
		 -.5f,  .5f, -.5f,  1.0, 0.0, 0.0,   0.0f, 1.0f, 0.0f,
		 -.5f,  .5f,  .5f,  1.0, 0.0, 0.0,   0.0f, 1.0f, 0.0f
	};


	glGenVertexArrays(1, &VAOCubo);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAOCubo);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	//Atributos de posici n
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Atributo de color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Atributo normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
}
void setupEsfera() {

	unsigned int VBO;

	glGenVertexArrays(1, &VAOEsfera);
	glGenBuffers(1, &VBO);

	// bind the Vertex Array Object first.
	glBindVertexArray(VAOEsfera);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_esfera), vertices_esfera, GL_STATIC_DRAW);

	//Normales
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	//Textura
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
}

//Funciones de inicializacion de OpenGL y de los objetos
void initObjetos() {
	base.VAO = VAOCubo;
	cabina.VAO = VAOCubo;
	articulacion.VAO = VAOEsfera;
	brazo.VAO = VAOCubo;
}
void openGlInit() {
	//Habilito aqui el depth en vez de arriba para los usuarios de linux y mac mejor arriba
	//Incializaciones varias
	glClearDepth(1.0f); //Valor z-buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST); // z-buffer
	glEnable(GL_CULL_FACE); //ocultacion caras back
	glCullFace(GL_BACK);
	glEnable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void teclaCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_C && action == GLFW_PRESS) {
		if (modoCamara == 0)
			modoCamara = 1;
		else
			modoCamara = 0;
	}

	if (key == GLFW_KEY_N && action == GLFW_PRESS) {
		nieblaActiva = !nieblaActiva;
		modoNoche = !modoNoche;
	}

	//Controles del canal
	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		if (canal < 3) {
			canal++;
			haciaDerecha = true;
		}
		if (canal == 3) {
			if (!pivoteDer) {
				debeCambiarAngulo = true;
			}
			pivoteDer = true;
		}
	}

	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		if (canal > 1) {
			canal--;
			haciaDerecha = false;
		}
		if (canal == 1) {
			if (pivoteDer) {
				debeCambiarAngulo = true;
			}
			pivoteDer = false;
		}
	}

	if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
		if (pantallaCompleta) {
			pantallaCompleta = false;
			glfwSetWindowMonitor(window, nullptr, winPosX, winPosY, winSizeW, winSizeH, 0);
		}
		else {
			glfwGetWindowPos(window, &winPosX, &winPosY);
			glfwGetWindowSize(window, &winSizeW, &winSizeH);
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
			pantallaCompleta = true;
		}

	}
}
void tamanoVentanaCallback(GLFWwindow* window, int width, int height)
{
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
	glViewport(0, 0, width, height);
}

void drawMiCubo() {
	
	mat4 transform;

	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "model");
	unsigned int colorLoc = glGetUniformLocation(shaderProgram, "Color");

	glUniform3fv(colorLoc, 1, value_ptr(vec3(1, 0, 1)));

	transform = mat4(1.0f);

	transform = rotate(transform, (float)(rot1), vec3(0.0f, 1.0f, 0.0f));
	transform = translate(transform, vec3(trans1, trans1, 0));
	
	transform = scale(transform, vec3(2, 1, 1));
	transform = scale(transform, vec3(3, 3, 3));

	/*
		glTranslatef(5, 5, 0); //3: define o centro do prisma en 5,5
		glRotatef(45.0f, 0, 1, 0); // 2: inclina 45º respecto a eixo Y
		glScalef(2, 1, 1); /
	*/

	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(transform));
	
	glBindVertexArray(VAOCubo);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void cargarSkybox() {
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &skyboxEBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	std::string facesCubemap[6] =
	{
		"texturas/right.jpg",
		"texturas/left.jpg",
		"texturas/top.jpg",
		"texturas/bottom.jpg",
		"texturas/front.jpg",
		"texturas/back.jpg"
	};

	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (unsigned int i = 0; i < 6; i++) {
		int ancho, alto, nrChannels;
		unsigned char* data = stbi_load(facesCubemap[i].c_str(), &ancho, &alto, &nrChannels, 0);
		if (data) {
			GLenum formato = (nrChannels == 4) ? GL_RGBA : GL_RGB;
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				formato,
				ancho,
				alto,
				0,
				formato,
				GL_UNSIGNED_BYTE,
				data
			);

			stbi_image_free(data);
		}
		else {
			std::cout << "Error al cargar textura: " << facesCubemap[i];
			stbi_image_free(data);
		}
	}
}

void renderSkybox() {
	glDisable(GL_CULL_FACE); //Deshabilitamos la ocultacion de caras para que se vea el sprite por ambas caras

	//Activamos el programa de shaders de la skybox
	glUseProgram(skyboxShader);
	mat4 view = mat4(1.0f);
	mat4 projection = mat4(1.0f);
	view = mat4(glm::mat3(lookAt(
		vec3(
			distanciaCamara * sin(alpha) * cos(beta),
			distanciaCamara * sin(beta),
			distanciaCamara * cos(alpha) * cos(beta)
		),
		vec3(0.0f, 0.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f)
	)));
	projection = perspective(radians(45.0f), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "view"), 1, GL_FALSE, value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "projection"), 1, GL_FALSE, value_ptr(projection));

	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}

void morir(GLFWwindow* window) {
	
	dibujaMuerte();
	glfwSwapBuffers(window);
	Sleep(2000);
	glfwTerminate();
	exit(0);
}

int main(){

	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Creo la ventana									
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Subway Arminho", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, tamanoVentanaCallback);

	srand(time(NULL));

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	openGlInit();

	::ShowWindow(::GetConsoleWindow(), SW_HIDE);  // Oculta la consola

	//Cargo los shaders 
	shaderProgram = setShaders("shaders/shader_luz.vert", "shaders/shader_luz.frag");
	//Shader espec�fico para la skybox
	skyboxShader = setShaders("shaders/shader_skybox.vert", "shaders/shader_skybox.frag");

	// Usa el shader que vas a configurar
	glUseProgram(shaderProgram);

	// Colores y parametros de la niebla
	glUniform1i(glGetUniformLocation(skyboxShader, "nieblaActiva"), nieblaActiva);
	glUniform1f(glGetUniformLocation(shaderProgram, "fogStart"), 0.0f); // Empieza la niebla
	glUniform1f(glGetUniformLocation(shaderProgram, "fogEnd"), 200.0f);   // Totalmente cubierta
	glUniform3f(glGetUniformLocation(shaderProgram, "fogColor"), 0.5f, 0.5f, 0.5f);

	//Ahora usamos el otro shader
	glUseProgram(skyboxShader);
	glUniform1i(glGetUniformLocation(skyboxShader, "skybox"), 0.0f);

	//inicializamos los VAOs 
	setupCuadradoXZ();
	setupEsfera();
	setupCubo();
	setupArminho();
	setupArbol();
	setupCarretera();

	arbolText = myCargaTexturas("texturas/tree.png");
	hierbaText = myCargaTexturas("texturas/grass.png");
	arminhoText = myCargaTexturas("texturas/arminhoTex.jpg");
	carreteraText = myCargaTexturas("texturas/carreteraTex.jpg");
	muerteText = myCargaTexturas("texturas/MORISTE.png");

	initObjetos();

	glfwSetKeyCallback(window, teclaCallback);

	cargarSkybox();

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	// Lazo de la ventana mientras no la cierre
	while (!glfwWindowShouldClose(window))
	{
		//Render de clears y de camara
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glUniform1i(glGetUniformLocation(shaderProgram, "nieblaActiva"), nieblaActiva);

		// input
		processInput(window);

		glDepthFunc(GL_LEQUAL);
		renderSkybox();
		glDepthFunc(GL_LESS);

		tiempo();
		movimiento();
		display();
		//drawMiCubo();

		if (muerto) {
			morir(window);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		trans1 += 0.01;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		trans1 -= 0.01;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		rot1 -= 0.01;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		rot1 += 0.01;
	}
}