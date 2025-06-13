#include <glad.h>
#include <glfw3.h>
#include <stdio.h>
#include <iostream>
#include "lecturaShader_0_9.h"
#include "esfera.h"
#include <cmath>
#include <fstream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace glm; //Para no tener que declarar glm cada vez que lo usemos

#define GradosToRadianes 0.0175
#define RADIO .5
#define GL_PI 3.14f

float distanciaCamara = 50;
float alpha = 0.35, beta = 0.78;

void processInput(GLFWwindow* window);

// settings
unsigned int SCR_WIDTH = 800, SCR_HEIGHT = 800;
int winPosX = 0, winPosY = 0, winSizeW,winSizeH;

extern GLuint setShaders(const char* nVertx, const char* nFrag);
GLuint shaderProgram;

typedef struct {

	float px, py, pz;	//Posicion inicial
	float angulo_trans;	//Angulo de giro (x)
	float velocidad;	//Velocidad de movimiento
	float sx, sy, sz;	//Escalado en los ejes
	unsigned int VAO;	//Lista de render
	float angulo_y;	//Angulo de giro (y)
	int tex;			//Textura

}objeto;

objeto base = { 0, 1.5 ,0.5 ,0 ,0 ,10 ,2 ,4 ,0 ,0,0 };
objeto cabina = { 5.0, 1.0 ,0 ,0 ,0 ,2.0f ,3.0f ,4.0f ,0,0,0 };
objeto articulacion = { -0.4, 0.4 ,0 ,0 ,0 ,1.0 ,1.0 ,1.0 ,0,35,0 };
objeto brazo = { 0.0, 3 ,0 ,0 ,0 ,0.5 ,6.0 ,0.5 ,0,0,0 };

unsigned int VAO;
unsigned int VAOCuadradoXZ;
unsigned int VAOCubo;
unsigned int VAOEsfera;
unsigned int VAOSprite;

int hierbaText;
int arbolText;
int conejoText;

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

// Variables globales para la c mara
float camX = 0.0f, camY = 0.0f, camZ = 3.0f;  // Posici n inicial de la c mara
float yawMio = -90.0f, pitchMio = 0.0f;  // Rotaci n de la c mara
float speed = 0.05f;  // Velocidad de movimiento

//Funciones de camara
void myCamara() {
	//Matriz de proyecci n
	mat4 projection;
	projection = mat4(1.0f);
	unsigned int proyectionLoc = glGetUniformLocation(shaderProgram, "projection");
	projection = perspective(radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 200.0f);
	glUniformMatrix4fv(proyectionLoc, 1, GL_FALSE, value_ptr(projection));

	//Matriz de vista
	mat4 view;
	view = mat4(1.0f);
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
	view = lookAt(vec3((float)distanciaCamara * (float)sin(alpha) * cos(beta), (float)distanciaCamara * (float)sin(beta), (float)distanciaCamara * cos(alpha) * cos(beta)),
		vec3(0, 0, 0),
		vec3(0, 1, 0));

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
}
void myPrimeraPersona(objeto coche) {

	mat4 projection;
	projection = mat4(1.0f);
	unsigned int proyectionLoc = glGetUniformLocation(shaderProgram, "projection");
	projection = perspective(radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 200.0f);
	glUniformMatrix4fv(proyectionLoc, 1, GL_FALSE, value_ptr(projection));


	mat4 view;
	view = mat4(1.0f);
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
	view = lookAt(vec3(coche.px, coche.py + 9, coche.pz),
		vec3(coche.px + sin((coche.angulo_trans) * GradosToRadianes), 10, coche.pz + cos((coche.angulo_trans) * GradosToRadianes)),
		vec3(0, 1, 0));

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
}
void myTerceraPersona(objeto coche) {

	mat4 projection;
	projection = mat4(1.0f);
	unsigned int proyectionLoc = glGetUniformLocation(shaderProgram, "projection");
	projection = perspective(radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 200.0f);
	glUniformMatrix4fv(proyectionLoc, 1, GL_FALSE, value_ptr(projection));

	vec3 pos = vec3(
		coche.px - 15.0f * sin(coche.angulo_trans * GradosToRadianes),
		coche.py + 10.0f,
		coche.pz - 15.0f * cos(coche.angulo_trans * GradosToRadianes)
	);

	vec3 obj = vec3(
		coche.px + 10.0f * sin(coche.angulo_trans * GradosToRadianes),
		2.0f,
		coche.pz + 10.0f * cos(coche.angulo_trans * GradosToRadianes)
	);

	mat4 view;
	view = mat4(1.0f);
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
	view = lookAt(pos, obj, vec3(0, 1, 0));
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
		nbFrames = 0;
		unSegundo = 0;
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

	//Color de la luz
	unsigned int lightLoc = glGetUniformLocation(shaderProgram, "lightColor");
	glUniform3f(lightLoc, 0.7f, 0.7f, 0.7f);

	unsigned int lightAngle = glGetUniformLocation(shaderProgram, "anguloLuz");
	glUniform1f(lightAngle, 15.0f);

	//Posicion de la luz
	unsigned int ligthPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
	glUniform3f(ligthPosLoc, base.px, base.py, base.pz);

	//Direccion de la luz
	unsigned int ligthDirLoc = glGetUniformLocation(shaderProgram, "luzDir");
	glUniform3f(ligthDirLoc, sin(base.angulo_trans * GradosToRadianes), 0, cos(base.angulo_trans * GradosToRadianes));

	//Luz especular
	unsigned int viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
	glUniform3f(viewPosLoc, base.px, base.py, base.pz);
}
void iluminacionDia() {
	
	//Color de la luz
	unsigned int lightLoc = glGetUniformLocation(shaderProgram, "lightColor");
	glUniform3f(lightLoc, 1, 1, 1);

	unsigned int lightAngle = glGetUniformLocation(shaderProgram, "anguloLuz");
	glUniform1f(lightAngle, 360.0f);

	//Posicion de la luz,
	unsigned int ligthPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
	glUniform3f(ligthPosLoc, 5, 20, 5);

	//Direccion de la luz
	unsigned int ligthDirLoc = glGetUniformLocation(shaderProgram, "luzDir");
	glUniform3f(ligthDirLoc, 0, 0, 0);

	//Luz especular
	unsigned int viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
	glUniform3f(viewPosLoc, 0.3f, 0.3f, 0.3f);
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

void dibujaSuelo(mat4 transform, unsigned int transformLoc, unsigned int colorLoc) {
	//SUELO
	transform = mat4(1.0f); //Crea la matriz identidad
	transform = scale(transform, vec3(10, 1, 10)); //Hace la operacion de escalado
	transform = scale(transform, vec3(10, 1, 10)); //Hace la operacion de escalado
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(transform));
	glUniform3fv(colorLoc, 1, value_ptr(vec3(0, 1, 0))); //Le da color
	//Lo dibuja
	glBindTexture(GL_TEXTURE_2D, hierbaText);
	glBindVertexArray(VAOCuadradoXZ);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindTexture(GL_TEXTURE_2D, 0);
}
void dibujaRuedas() {

	//Creo las variables y las inicializo
	mat4 transform;
	mat4 stack;
	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "model");
	unsigned int colorLoc = glGetUniformLocation(shaderProgram, "Color");

	glUniform3fv(colorLoc, 1, value_ptr(vec3(0, 0, 0))); //Le da color a todas las ruedas ya

	transform = mat4(1.0f);
	transform = translate(transform, vec3(base.px, base.py, base.pz));
	transform = rotate(transform, (float)((base.angulo_trans - 90) * GradosToRadianes), vec3(0.0f, 1.0f, 0.0f));
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
mat4 dibujaCamionBase(unsigned int transformLoc, unsigned int colorLoc) {
	
	mat4 transform = mat4(1.0f); //Crea la matriz identidad
	transform = translate(transform, vec3(base.px, base.py, base.pz)); //Hace la operacion de translacion
	transform = rotate(transform, (float)((base.angulo_trans - 90) * GradosToRadianes), vec3(0.0f, 1.0f, 0.0f));
	mat4 stack = transform; //Guarda la traslacion y la rotacion de la base
	transform = scale(transform, vec3(base.sx, base.sy, base.sz)); //Hace la operacion de escalado

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
void dibujaCamionGrua(mat4 reference, unsigned int transformLoc, unsigned int colorLoc) {
	
	//Articulacion
	
	mat4 transform = reference;
	transform = translate(transform, vec3(articulacion.px, articulacion.py, articulacion.pz));
	transform = rotate(transform, (float)((articulacion.angulo_trans) * GradosToRadianes), vec3(1, 0, 0));
	transform = rotate(transform, (float)((articulacion.angulo_y) * GradosToRadianes), vec3(0, 0, 1));
	reference = transform; //Guardamos las ransformaciones de posicion relativas de la grua
	transform = scale(transform, vec3(articulacion.sx, articulacion.sy, articulacion.sz));

	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(transform));
	glUniform3fv(colorLoc, 1, value_ptr(vec3(0, 1, 1))); //Le da color
	glBindVertexArray(articulacion.VAO);
	glDrawArrays(GL_TRIANGLES, 0, 1080);

	//Brazo

	transform = reference;
	transform = translate(transform, vec3(brazo.px, brazo.py, brazo.pz));
	transform = rotate(transform, (float)((brazo.angulo_trans) * GradosToRadianes), vec3(1.0f, 0.0f, 0.0f));
	transform = scale(transform, vec3(brazo.sx, brazo.sy, brazo.sz));

	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(transform));
	glBindVertexArray(brazo.VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
void dibujaCamion(mat4 transform, unsigned int transformLoc, unsigned int colorLoc) {
	
	mat4 stack;
	dibujaRuedas();
	stack = dibujaCamionBase(transformLoc, colorLoc);
	dibujaCamionCabina(stack, transformLoc, colorLoc);
	dibujaCamionGrua(stack, transformLoc, colorLoc);
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
	stack = transform;

	transform = translate(transform, vec3(-0.7, 0, -0.7));
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

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
void dibujaConejo() {

	//Creo las variables y las inicializo
	mat4 transform;

	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "model");
	unsigned int colorLoc = glGetUniformLocation(shaderProgram, "Color");

	//glUniform3fv(colorLoc, 1, value_ptr(vec3(0.5f, 0.5f, 0.5f)));

	transform = mat4();
	transform = scale(transform, vec3(2.5f, 5, 5));
	transform = translate(transform, vec3(3, 0.5f, -3));
	transform = rotate(transform, (float)(180 * GradosToRadianes), vec3(1.0f, 0.0f, 0.0f));
	transform = rotate(transform, (float)(25 * GradosToRadianes), vec3(0.0f, 1.0f, 0.0f));
	
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(transform));
	glBindTexture(GL_TEXTURE_2D, conejoText);
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

	//Render de clears y de camara
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	myCamara();
	if (modoCamara > 0)
		myPrimeraPersona(base);
	if (modoCamara == 2)
		myTerceraPersona(base);

	if (modoNoche) iluminacion();
	else iluminacionDia();

	//Se dibuja el cuadrado
	int cuboScale = 1;
	transform = mat4(1.0f); //Inicializamos la matriz como una identidad
	transform = scale(transform, vec3(5, 5, 5)); //Hace la operacion de escalado
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(transform));
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, 6); //Lo dibuja


	//Dibujamos el suelo
	dibujaSuelo(transform, transformLoc, colorLoc);

	//Dibujamos el camion
	dibujaCamion(transform, transformLoc, colorLoc);
	
	dibujaArbol();
	dibujaConejo();
	
	glBindVertexArray(0); //Desvincula
}

//Funciones de inicializacion de objetos

void setupArbol() {

	unsigned int VBO;
	float vertices[] = {
		 0.5f,  0.5f,  0.0f,   0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.0f,   0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
		-0.5f, -0.5f,  0.0f,   0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.0f,   0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.0f,   0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.0f,   0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f 
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

		// Atrás (0, 0, -1)
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
		else if (modoCamara == 1)
			modoCamara = 2;
		else
			modoCamara = 0;
	}

	if (key == GLFW_KEY_N && action == GLFW_PRESS) {
		if (modoNoche)
			modoNoche = false;
		else
			modoNoche = true;
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

int main()
{
	
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Creo la ventana									
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Grua", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, tamanoVentanaCallback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	openGlInit();

	//Cargo los shaders 
	shaderProgram = setShaders("shaders/shader_luz.vert", "shaders/shader_luz.frag");

	//inicializamos los VAOs 
	setupCuadradoXZ();
	setupEsfera();
	setupCubo();
	setupArbol();

	conejoText = myCargaTexturas("textures/mips.png");
	hierbaText = myCargaTexturas("textures/grass.png");
	arbolText = myCargaTexturas("textures/tree.png");

	initObjetos();

	glfwSetKeyCallback(window, teclaCallback);

	// Lazo de la ventana mientras no la cierre
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		tiempo();
		movimiento();
		display();

		// glfw: swap 
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


	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {		
			distanciaCamara -= 0.03f;
		}
		else {
			base.velocidad += 0.01f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			distanciaCamara += 0.03f;
		}
		else {
			base.velocidad -= 0.01f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		base.velocidad = 0.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		base.angulo_trans += 0.2f;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		base.angulo_trans -= 0.2f;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		if (articulacion.angulo_trans > -50)
			articulacion.angulo_trans -= 0.2f;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		if (articulacion.angulo_trans < 50)
			articulacion.angulo_trans += 0.2f;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		if (articulacion.angulo_y < 50)
			articulacion.angulo_y += 0.2f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		if (articulacion.angulo_y > -50)
			articulacion.angulo_y -= 0.2f;
	}
}