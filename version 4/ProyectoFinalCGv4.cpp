﻿/*---------------------------------------------------------*/
/* ----------------  Proyecto Final             -----------*/
/*-----------------    2023-2   ---------------------------*/
/*------ Cruz Cedillo Daniel Alejandro		  -------------*/
/*------------- No. Cuenta: 316083298---------------*/
/*------ Chávez Flores Giovanni		  -------------*/
/*------------- No. Cuenta: 317053319---------------*/
#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#include <irrKlang.h> //Sonido


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>
#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor *monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(0.0f, 15.0f, 350.0f));//Posicion incial de la camara 
float MovementSpeed = 50.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float range = 0.0f,
rot = 0.0f,
movCamera = 0.0f;
bool First = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f); //Direccion de la fuente de luz
glm::vec3 direccionluz(0.0f, -1.0f, 0.0f);
glm::vec3 myposition02(80.0f, 4.0f, 0.0f);
glm::vec3 myColor01(0.0f, 0.0f, 0.0f);

//Luz escenario
float luzdif = 0.55f,
luzspec = 0.2f;

//luz arcade
float luzarcspec = 0.5f,
luzarcdif = 1.0f;
bool encendidoarcade = true;

//-------------------------------------------------------------------------------------
// Daclaracion de variables que se usaran para las animaciones
// ------------------------------------------------------------------------------------
//Luz ciclo de dia y noche
float luzx = 0.5f, luzy = 0.8f, luzz = 0.8f, noche = 0.0f;
int dia = 0;

//Sonic
float posxs = 0.0f,
poszs = 0.0f,
posys = 0.0f,
rotsonic =0.0f,
incsonic = 0.0f;
int animsonic = 0;

//Freddy
float rotBrazoF = 0.0f;
int animFreddy = 0;
bool Freddyanim = true;

//Chica
float rotBrazoC = 0.0f, 
	rotpanque = 0.0f,
	poszpanque = 18.5f;
int animChica = 0;
bool Chicaanim = false;

//eggman
float eggx = 0.0f,
	eggy = 0.0f,
	eggz = 0.0f,
	rotegg = 0.0f,
	egginc =0.0f;
int animegg=0;

//Cheff
float rotcheff=0.0f,
	poszsar = 13.5f,
	rotsarten=0.0f,
	carnex =0.0f,
	carney = 0.0f,
	carnez =0.0f,
	tempcarne = 0.0f,
	carneinc =1.5f;
int animcheff = 0;

//BALLON BOY
glm::vec3 prueba(0.0f, 0.0f, 0.0f);
float	posX = 0.0f,
posY = 0.0f,
posZ = 0.0f,
rotRodIzq = 0.0f,
rotRodDer = 0.0f,
movPierDer = 0.0f,
movPierIzq = 0.0f,
giroMonito = 0.0f,
movBrazoDer = 0.0f, //necesito agregar variable de dibujo, este es el principio del proceso
movBrazIzq = 0.0f,
movCabeza = 0.0f,
movHombroIzq = 0.0f,
movHombroDer = 0.0f;
bool baja = false;
bool sube = true;
//Camara BB
float BBCameraX = 0.0f, BBCameraZ = 0.0f;

void movCuerpo() {
	if (movPierDer <= 40.0f && sube) {
		movPierDer += 3.0f;
		movHombroDer += 0.1f;
		movBrazoDer += 0.12f;
		movPierIzq -= 3.0f;
		rotRodDer += 1.0f;
		rotRodIzq -= 1.0f;
		movHombroDer -= 0.1f;
		movBrazIzq -= 0.1f;
		if (movPierDer >= 40.0f) {
			sube = false;
			baja = true;
		}
	}
	if (movPierDer >= -50 && baja) {
		movPierDer -= 3.0f;
		movHombroDer -= 0.1f;
		movBrazoDer -= 0.12f;
		movPierIzq += 3.0f;
		rotRodDer -= 1.0f;
		rotRodIzq += 1.0f;
		movHombroDer += 0.1f;
		movBrazIzq += 0.1f;
		if (movPierDer <= -50.0f) {
			baja = false;
			sube = true;
		}
	}
}



//Bunny

float giroBunny = 0.0f;
float	rot_bIzqB = 0.0f,
rot_bDerB = 0.0f,
rot_pIzqB = 0.0f,
rot_pDerB = 0.0f;
int animBunny = 0;
float reproduciranimacion, habilitaranimacion,
guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;


//PARA INPUT CON KEYFRAMES 
void inputKeyframes(bool* keys);


//--------------------------------------------------------------------------------------------
//Rings Keyframes
float rotring = 0.0f;
float poszring = 0.0f;
//Calculo incremento
float incrotring = 0.0f, 
incposz = 0.0f;

int i = 0, pos =0;
#define MAX_FRAMES 50
int i_max_steps = 60;
int i_curr_steps = 0;

typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float poszring;
	float rotring;

}FRAME; //Estructura Frame

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 4;			//Cantidad de Frames
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	std::cout << "Frame Index = " << FrameIndex << std::endl;
	
	KeyFrame[FrameIndex].poszring = poszring;
	KeyFrame[FrameIndex].rotring = rotring;
	
	FrameIndex++;
}

//Punto de incio para reproducir
void resetElements(void)
{
	rotring = KeyFrame[0].rotring;
	poszring = KeyFrame[0].poszring;
}

void interpolation(void)
{
	incrotring= (KeyFrame[playIndex + 1].rotring - KeyFrame[playIndex].rotring) / i_max_steps;
	incposz = (KeyFrame[playIndex + 1].poszring - KeyFrame[playIndex].poszring) / i_max_steps;
}

//-----------------------------------------------------------------------
//Animaciones
//-------------------------------------------------------------------

void animate(void)
{
	//animacion para el ciclo de dia y de noche
	/* El ciclo de dia y de noche sera aproximadamente de 1 min, en le caso 0 es para 
	hacer de dia, el caso 1 es para oscurecer 	y el caso 2 no cambia la iluminacion 
	pero si mantiene los valores ´para simularla */
	switch (dia) {
		case 0: 
			luzx += 0.0005f;
			luzy += 0.0005;
			luzz += 0.0005;
			if (luzx >= 0.8f)
				dia = 1;
			break;
		case 1:
			luzx -= 0.0005f;
			luzy -= 0.0005;
			luzz -= 0.0005;
			if (luzx <= 0.05f) {
				luzdif = 0.0;
				luzspec = 0.0;
				dia = 2;
				printf("noche ");
			}
				
			break;
		case 2:
			noche += 0.001;
			if (noche >= 1.0f) {
				dia = 0;
				noche = 0.0f;
				luzdif = 0.55;
				luzspec = 0.2;
				printf("dia ");
			}
		
	}
	//Luz arcade
	if (encendidoarcade) {
		luzarcspec = 0.5f;
		luzarcdif = 1.0f;
	}
	else {
		luzarcspec = 0.0f;
		luzarcdif = 0.0f;
	}
	
	//--------------------------------------------------------------------------------
	//Animacion por keyframes
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			printf("Frame [%d] reproducido \n", playIndex - 1);
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("Ultimo frame es [%d] \n", FrameIndex - 1);
				//fprintf(guardarFrames,"FrameIndex [%d]" ,FrameIndex);
				printf("Termina animaci�n\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				//printf("entro aqu�\n");
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			poszring += incposz;
			rotring += incrotring;
			i_curr_steps++;
		}
	}
	//--------------------------------------------------------------------------------
	//Animacion Sonic
	/*En esta animacion se usa a sonic hecho bolita y tiene que recorrer una parte del mapa, en le 
	caso 0 sonic avanza al inicio de la rampa, en le caso 1 sonic recorre la rampa circular, en el
	caso 2 sonic continua el camino restante, despues en los casoso 3,4 y 5realiza de forma 
	inversa el recorrido, tambien se le dio una rotacion, para simular que esta girando mientras avanaza*/
	rotsonic += 1.5f;
	switch (animsonic) {
	case 0:
		posys += 0.6f;
			if(posys >= 150)
				animsonic = 1;
		break;
	case 1:
		//Escala * 3 
		poszs = 95 + ( -95 *cos(incsonic));//altura circ 
		posys =  150 + ( 95 *sin(incsonic));//largo circ 
		incsonic += 0.01;

		posxs -= 0.1;
		if (incsonic >= 6.5) { //3
			animsonic = 2;
		}
		if (posxs <= -66) {
			posxs = -66.0f;
		}
		break;
	case 2:
		posys += 0.6f;
		if (posys >= 350)
			animsonic = 3;
		break;

	case 3:
		posys -= 0.6f;
		if (posys <= 150)
			animsonic = 4;
		break;
	case 4:
		poszs = 95 + (-95 * cos(incsonic));//altura circ 
		posys = 150 + (95 * sin(incsonic));//largo circ 
		incsonic -= 0.01;

		posxs += 0.1;
		if (incsonic <= 0.0) { 
			incsonic = 0.0f;	//1.5
			animsonic = 5;
		}
		break;
	case 5:
		posys -= 0.6f;
		if (posys <= 0) {
			posys = 0;
			animsonic = 0;
		}
			
		break;
	}
	//--------------------------------------------------------------------------------
	//Animacion Ring
	/*Esta animacion se le da giro al ring*/
	//rotring += 2.5;
	//if (rotring >= 180)
		//rotring = 0.0f;
	//--------------------------------------------------------------------------------
	//Animacion Saludo Freddy
	/*Se busca darle una animacion de saludo, para ello se le da animacion al brazo realiando 
	rotaciones en el brazo, en el caso 0 el brazo sube por medioD de una rotacion y en el 
	caso 1 el brazo deciende por emdio de otra rotacion*/
	if (Freddyanim) {
		switch (animFreddy) {
		case 0:
			rotBrazoF += 1.0f;
			if (rotBrazoF >= 45)
				animFreddy = 1;
			break;
		case 1:

			rotBrazoF -= 1.0f;
			if (rotBrazoF <= -45)
				animFreddy = 0;
			break;
		}
	}
	//--------------------------------------------------------------------------------
	//Animacion Eggman
	/*En este caso se busco darle la animacon de eggman elevandose al cielo, y despues
	de hacerlo que de vueltas a lo largo del edificio, el caso 1, 2, 3  y 4 se encargan
	de posiconarlo en el lugar de despegue, en el caso 5 se le da la animacion de 
	despegue y en el caso 6 se le da la animacion de dar vueltas al edificio*/
	switch (animegg) {
		case 0:
			eggy += 0.7f;
			if (eggy >= 70)
				animegg = 1;
			break;
		case 1:
			rotegg += 0.7f;

			if (rotegg >= 90)
				animegg = 2;
			break;
		case 2:
			eggx += 0.7f;
			if (eggx >= 200)
				animegg = 3;
		break;
		case 3:
			rotegg -= 0.7f;

			if (rotegg <= 0)
				animegg = 4;
			break;
		case 4:
			eggy -= 0.7f;
			if (eggy <= 40)
				animegg = 5;
			break;
		case 5:
			eggz += 0.3f;
			eggx = 200 * cos(egginc);
			eggz += 0.3f;
			eggy = 200 * sin(egginc);
			eggz += 0.3f;
			egginc += 0.01f;
			rotegg -= 0.4f;
			if (eggz >= 100)
				animegg = 6;
			break;
		case 6:
			rotegg -= 0.46f;
			eggx = 200 * cos(egginc);
			eggy = 200 * sin(egginc);
			egginc += 0.008f;
			break;
	}
	//--------------------------------------------------------------------------------
	//Animacio Chica
	/*En esta animacion se busca que chica lance un panque al aire y que de vueltas
	a lo largo del recorrido, en el caso 0 el panque y el brazo se mueven al mismo 
	tiempo, y empieza la rotacion del panque, en le caso 1 el panque se eleva y rota
	y en el caso 2 	el panque deciende con el brazo de chica mientras termina su 
	rotacion*/
	if (Chicaanim) {
		switch (animChica) {
		case 0:
			rotBrazoC -= 0.3f;
			poszpanque += 0.1f;
			rotpanque += 2.7;
			if (rotBrazoC <= -20)
				animChica = 1;
			break;
		case 1:
			poszpanque += 0.3f;
			if (poszpanque >= 25)
				animChica = 2;
			break;
		case 2:
			rotBrazoC += 0.3f;
			rotpanque += 2.7;
			poszpanque -= 0.105f;
			if (rotpanque >= 360)
				rotpanque = 0;
			if (rotBrazoC >= 0)
				animChica = 0;
			break;
		}
	}
	//--------------------------------------------------------------------------------
	//Animacion Cheff
	/*La animacion consiste en que el cheff lanza el sarten al aire con la carne,
	y la misma vuela hasta un plato que hay sobre la mesa, en el caso 0 el cheff 
	lanza el sarten por medio de una rotacion en sus brazos, el sarten aumenta su
	altura mientras tiene una pequeña rotacion, la carne tambien aumenta su altura
	mientras realiza un trayectoria curva, en el caso 1 se hace una pequeña pausa
	en el caso 2 y 3 cambia a otra rotacion y continua la animacion para llegar 
	al plato, en el caso 4 la carne se da una pequeña pausa a la carn en el 
	plato antes de repetir la animacion*/
	switch(animcheff){
	case 0:
		rotcheff += 0.3f;
		poszsar += 0.1;
		rotsarten += 0.25;
		carnez = -15.0 * cos(carneinc);
		carney = 12.0 * sin(carneinc);
		carneinc += 0.015;
		if (carneinc >= 3) { //3
			carneinc = 3.0f;	//1.5
		}
		if (rotcheff >= 30)
			animcheff = 1;
		break;
	case 1:
		carneinc += 0.04;
		if (carneinc >= 3.1) {
			animcheff = 2;
		}
		break;
	case 2:
		carnez = -15.0 * cos(carneinc);
		carney = 70 * sin(carneinc);
		carneinc += 0.005;
		rotcheff -= 0.3f;
		rotsarten -= 0.25;
		poszsar -= 0.1;

		if (rotcheff <= 0) {
			animcheff = 3;
		}		
		break;
	case 3:
		carnez = -15.0 * cos(carneinc);
		carney = 70 * sin(carneinc);
		carneinc += 0.008;
		if (carneinc >= 4.85) {
			animcheff = 4;
		}
		break;
	case 4:
		carneinc += 0.01;
		if (carneinc >= 7) {
			carneinc = 1.5f;
			animcheff = 0;
		}
		break;
	}
	/*ANIMACIÓN BUNNY*/
	switch (animBunny) {
	case 0:
		//rot_bIzqB = 0.0f,
		//rot_bDerB = 0.0f,
		//rot_pIzqB = 0.0f,
		//rot_pDerB = 0.0f;
		rot_bIzqB += 0.15f;
		if (rot_bIzqB >= 70.0f) {
			animBunny = 1;
		}
		break;
	case 1:
		rot_bDerB += 0.15f;
		if (rot_bDerB >= 70.0f) {
			animBunny = 2;
		}
		break;
	case 2:
		rot_bIzqB -= 0.15f;
		if (rot_bIzqB <= 0.0f) {
			animBunny = 3;
		}
		break;
	case 3:
		rot_bDerB -= 0.15f;
		if (rot_bDerB <= 0.0f) {
			animBunny = 0;
		}
		break;
	}
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// --------------------
	// glfw window creation
	// --------------------
	
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proyecto Final", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//-------------------------------------------------------------------------
	//Sonido de fondo
	//---------------------------------------------------------------------
	irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();
	
	if (!engine) {
		printf("Could not startup engine ");
		return 0; //eror inciando engine
	}

	engine->play2D("fnaf.mp3", true);
	engine->setSoundVolume(0.4f);
	//--------------------------------
	// configure global opengl state
	// ------------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights_mod.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/rightcity.jpg", 
		"resources/skybox/leftcity.jpg",
		"resources/skybox/topcity.jpg",
		"resources/skybox/bottomcity.jpg",
		"resources/skybox/frontcity.jpg", 
		"resources/skybox/backcity.jpg" 
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
	Model piso("resources/objects/piso/piso.obj");
	
	//--------------------------------------------------------------------------------
	//Modelos Proyecto
	//-------------------------------------------------------------------------------
	//Elementos sin animacion
	//--------------------------------------------------------------------------------
	Model restaurante("resources/objects/restaurante/rest.obj");
	Model mesa("resources/objects/mesa/mesa.obj");
	Model pastel("resources/objects/pastel/pastel.obj");
	Model micro("resources/objects/microfono/micro.obj");
	Model globodec("resources/objects/globos/globodec.obj");
	Model cocina("resources/objects/cocina/cocina.obj");
	Model bar("resources/objects/bar/bar.obj");
	Model cortina("resources/objects/cortina/cortina.obj");
	Model Arcade1("resources/objects/arcade/a1.obj");
	Model Arcade2("resources/objects/arcade/a2.obj");
	Model Arcade3("resources/objects/arcade/a3.obj");
	Model luzTecho("resources/objects/luz/luztecho.obj");
	//--------------------------------------------------------------------------------
	//Modelos con animacion
	//--------------------------------------------------------------------------------
	//Sonic
	Model mapa("resources/objects/sonic/mapa.obj");
	Model sonic("resources/objects/sonic/sonic.obj");
	//ring
	Model ring("resources/objects/ring/ring.obj");
	//EggMAn
	Model Eggman("resources/objects/Eggman/Eggman.obj");
	//Freddy
	Model Freddy("resources/objects/Freddy/Freddy.obj");
	Model FreddyBrazo("resources/objects/Freddy/FreddyBrazo.obj");
	//Chica
	Model Chica("resources/objects/Chica/chica.obj");
	Model ChicaBrazo("resources/objects/Chica/chicabrazo.obj");
	Model panque("resources/objects/Chica/panque.obj");
	//Cheff
	Model cheff("resources/objects/cocinero/cheff.obj");
	Model cheffbd("resources/objects/cocinero/cheffbd.obj");
	Model cheffbi("resources/objects/cocinero/cheffbi.obj");
	Model sarten("resources/objects/cocinero/sarten.obj");
	Model carne("resources/objects/cocinero/carne.obj");
	Model plato("resources/objects/cocinero/plato.obj");

	//Bunny
	Model Bunny("resources/objects/Bunny/cuerpoBunny.obj");
	Model BunnyBrazoIzq("resources/objects/Bunny/bIzqBunny.obj");
	Model BunnyBrazoDer("resources/objects/Bunny/bDerBunny.obj");
	Model BunnyPieIzq("resources/objects/Bunny/pIzqBunny.obj");
	Model BunnyPieDer("resources/objects/Bunny/pDerBunny.obj");
	//Globo

	//Ballon boy
	Model torsoBB("resources/objects/BallonBoy/torso.obj");
	Model cabezaBB("resources/objects/BallonBoy/cabeza.obj");
	Model molinoBB("resources/objects/BallonBoy/molino.obj");
	Model hombroDerBB("resources/objects/BallonBoy/hombroDer.obj");
	Model hombroIzqBB("resources/objects/BallonBoy/hombroIzq.obj");
	Model brazoDerBB("resources/objects/BallonBoy/brazoDer.obj");
	Model brazoIzqBB("resources/objects/BallonBoy/brazoIzq.obj");
	Model piernaDerArrBB("resources/objects/BallonBoy/piernaDerArr.obj");
	Model piernaDerAbBB("resources/objects/BallonBoy/piernaDerAb.obj");
	Model piernaIzqArrBB("resources/objects/BallonBoy/piernaIzqArr.obj");
	Model piernaIzqAbBB("resources/objects/BallonBoy/piernaDerAb.obj");
	Model globoBB("resources/objects/BallonBoy/globo.obj");
	Model letreroBB("resources/objects/BallonBoy/letrero.obj");

	//para keyframes
	animate();
	//Inicialización de KeyFrames


	////Inicialización de KeyFrames
	//for (int i = 0; i < MAX_FRAMES; i++)
	//{

	//}
	string arch = "ring.txt";
	ifstream archivo(arch.c_str());
	string lineas;


	while (getline(archivo, lineas)) {

		printf("%i ", i);
		switch (i) {
		case 0:
			KeyFrame[pos].poszring = std::stof(lineas);
			break;
		case 1:
			KeyFrame[pos].rotring = std::stof(lineas);
			break;

		}
		if (i < 2) {
			i++;
		}
		if (i == 2) {
			pos++;
			i = 0;
		}

	}

	glm::mat4 projection = glm::perspective(camera.getZoom(), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);
		
		if (!camera.getIsometric()) {
			projection = glm::perspective(camera.getZoom(), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
		}
		else {
			projection = glm::ortho(-camera.getIsoZoom(), camera.getIsoZoom(), -camera.getIsoZoom(), camera.getIsoZoom(), -30.0f, 40.0f);

		}

		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		//------------------------------------------------------------------------------------------------------------------------
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(luzx, luzy, luzz));//Da luz a todo
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.0f, 0.0f, 0.0f));	//Da luz desde un únto
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));	//Brillo sobre una superficie

		//Luz lampara salon
		staticShader.setVec3("pointLight[0].position", glm::vec3(0.0, 70.0f, -150.0f));
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(luzdif, luzdif, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(luzspec, luzspec, luzspec));
		staticShader.setFloat("pointLight[0].constant", 0.008f); //Potencia de la luz
		staticShader.setFloat("pointLight[0].linear", 0.00009f); //distancia de luz, control mas fino
		staticShader.setFloat("pointLight[0].quadratic", 0.00032f);//distancia de luz, es mas brusco, a mas pequeño menor atenuacion mas viaja la luz

		staticShader.setVec3("pointLight[1].position", glm::vec3(0.0, 70.0f, -50.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(luzdif, luzdif, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(luzspec, luzspec, luzspec));
		staticShader.setFloat("pointLight[1].constant", 0.008f);
		staticShader.setFloat("pointLight[1].linear", 0.00009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.00032f);

		//Luz cocina
		staticShader.setVec3("pointLight[2].position", glm::vec3(-200.0, 70.0f, -50.0f));
		staticShader.setVec3("pointLight[2].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[2].diffuse", glm::vec3(luzdif, luzdif, 0.0f));
		staticShader.setVec3("pointLight[2].specular", glm::vec3(luzspec, luzspec, luzspec));
		staticShader.setFloat("pointLight[2].constant", 0.008f);
		staticShader.setFloat("pointLight[2].linear", 0.00009f);
		staticShader.setFloat("pointLight[2].quadratic", 0.00032f);

		//Luz arcade
		staticShader.setVec3("pointLight[3].position", glm::vec3(170, 20.0f, 10.0f));
		staticShader.setVec3("pointLight[3].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[3].diffuse", glm::vec3(luzarcdif, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[3].specular", glm::vec3(luzarcspec, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[3].constant", 0.008f);
		staticShader.setFloat("pointLight[3].linear", 0.1f);
		staticShader.setFloat("pointLight[3].quadratic", 0.0032f);

		//fuente de luz reflector
		staticShader.setVec3("spotLight[0].position", glm::vec3(camera.Position.x, camera.Position.y, camera.Position.z));//Posicion	
		staticShader.setVec3("spotLight[0].direction", glm::vec3(camera.Front.x, camera.Front.y, camera.Front.z));//Direccion a donde apunta la luz
		staticShader.setVec3("spotLight[0].ambient", glm::vec3(0.3f, 0.3f, 0.3f));//
		staticShader.setVec3("spotLight[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(10.0f)));//Maxima iluminacion
		staticShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(20.0f)));//Disminucion de la intensidad
		staticShader.setFloat("spotLight[0].constant", 0.5f);
		staticShader.setFloat("spotLight[0].linear", 0.0009f);//Distancia que viajara la luz
		staticShader.setFloat("spotLight[0].quadratic", 0.005);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);


		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);


		// Restaurante
		// -------------------------------------------------------------------------------------------------------------------------
		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(0.0f, -0.7f, -100.0f));
		model = glm::scale(model, glm::vec3(4.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		restaurante.Draw(staticShader);

		//Mesas
		// -------------------------------------------------------------------------------------------------------------------------
		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(-30.0f, 0.0f, -170.0f));
		model = glm::scale(model, glm::vec3(6.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		mesa.Draw(staticShader);

		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(30.0f, 0.0f, -170.0f));
		model = glm::scale(model, glm::vec3(6.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		mesa.Draw(staticShader);

		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(30.0f, 0.0f, -100.0f));
		model = glm::scale(model, glm::vec3(6.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		mesa.Draw(staticShader);

		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(-30.0f, 0.0f, -100.0f));
		model = glm::scale(model, glm::vec3(6.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		mesa.Draw(staticShader);

		//Pastel
		// -------------------------------------------------------------------------------------------------------------------------
		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(-30.0f, 11.0f, -170.0f));
		model = glm::scale(model, glm::vec3(2.0));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		pastel.Draw(staticShader);

		//Sonic
		// -------------------------------------------------------------------------------------------------------------------------
		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(300.0f, 5.0f, 150.0f));
		model = glm::scale(model, glm::vec3(8.0));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		mapa.Draw(staticShader);

		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(posxs + 340.0f, poszs + 11.0f, posys)); //(340,10,0)
		model = glm::scale(model, glm::vec3(3.0));
		model = glm::rotate(model, glm::radians(rotsonic), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		sonic.Draw(staticShader);

		//Rings
		// -------------------------------------------------------------------------------------------------------------------------
		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(340.0f, 10.0f, 150.0f));
		model = glm::scale(model, glm::vec3(4.0));
		model = glm::rotate(model, glm::radians(rotring), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		ring.Draw(staticShader);

		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(340.0f, 10.0f, 100.0f));
		model = glm::scale(model, glm::vec3(4.0));
		model = glm::rotate(model, glm::radians(rotring), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		ring.Draw(staticShader);

		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(340.0f, 10.0f, 50.0f));
		model = glm::scale(model, glm::vec3(4.0));
		model = glm::rotate(model, glm::radians(rotring), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		ring.Draw(staticShader);

		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(250.0f, 10.0f, 150.0f));
		model = glm::scale(model, glm::vec3(4.0));
		model = glm::rotate(model, glm::radians(rotring), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		ring.Draw(staticShader);

		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(250.0f, 10.0f, 200.0f));
		model = glm::scale(model, glm::vec3(4.0));
		model = glm::rotate(model, glm::radians(rotring), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		ring.Draw(staticShader);

		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(250.0f, 10.0f, 250.0f));
		model = glm::scale(model, glm::vec3(4.0));
		model = glm::rotate(model, glm::radians(rotring), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		ring.Draw(staticShader);

		//Microfono
		// -------------------------------------------------------------------------------------------------------------------------
		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(100.0f, 7.5f, -110.0f));
		model = glm::scale(model, glm::vec3(150.0));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		micro.Draw(staticShader);

		//Cocina
		// -------------------------------------------------------------------------------------------------------------------------
		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(-165.0f, 0.0f, 10.0f));
		model = glm::scale(model, glm::vec3(13.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		cocina.Draw(staticShader);

		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(-220.0f, 0.0f, 10.0f));
		model = glm::scale(model, glm::vec3(13.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		cocina.Draw(staticShader);

		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(-180.0f, 0.0f, -70.0f));
		model = glm::scale(model, glm::vec3(6.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		mesa.Draw(staticShader);

		//Mesa bar
		// -------------------------------------------------------------------------------------------------------------------------
		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(-55.0f, 0.0f, -10.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		bar.Draw(staticShader);

		//Cortinas Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(123.0f, 7.0f, -115.0f));
		model = glm::scale(model, glm::vec3(11.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		cortina.Draw(staticShader);

		//Arcade
		// -------------------------------------------------------------------------------------------------------------------------
		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(180.0f, 0.0f, -10.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Arcade1.Draw(staticShader);

		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(180.0f, 0.0f, 10.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Arcade1.Draw(staticShader);

		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(140.0f, 0.0f, -35.0f));
		model = glm::scale(model, glm::vec3(0.4f));
		staticShader.setMat4("model", model);
		Arcade2.Draw(staticShader);

		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(160.0f, 0.0f, -35.0f));
		model = glm::scale(model, glm::vec3(0.4f));
		staticShader.setMat4("model", model);
		Arcade2.Draw(staticShader);

		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(140.0f, 0.0f, 33.0f));
		model = glm::scale(model, glm::vec3(1.15f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Arcade3.Draw(staticShader);

		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(160.0f, 0.0f, 33.0f));
		model = glm::scale(model, glm::vec3(1.15f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Arcade3.Draw(staticShader);

		//Freddy
		// -------------------------------------------------------------------------------------------------------------------------
		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(40.0f, 0.0f, 50.0f));
		model = glm::scale(model, glm::vec3(10.0));
		staticShader.setMat4("model", model);
		Freddy.Draw(staticShader);

		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(47.0f, 34.5f, 48.0f));
		model = glm::scale(model, glm::vec3(10.0));
		model = glm::rotate(model, glm::radians(rotBrazoF), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		FreddyBrazo.Draw(staticShader);

		//Eggman
		// -------------------------------------------------------------------------------------------------------------------------
		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(eggx, eggz, eggy));
		model = glm::scale(model, glm::vec3(3.0));
		model = glm::rotate(model, glm::radians(rotegg), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Eggman.Draw(staticShader);

		//Chica
		// -------------------------------------------------------------------------------------------------------------------------
		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -220.0f));
		model = glm::scale(model, glm::vec3(0.3));
		staticShader.setMat4("model", model);
		Chica.Draw(staticShader);

		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(-4.5f, 17.0f, -218.5f));
		model = glm::scale(model, glm::vec3(0.3));
		model = glm::rotate(model, glm::radians(rotBrazoC), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		ChicaBrazo.Draw(staticShader);

		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(-4.5f, poszpanque, -212.0f));
		model = glm::scale(model, glm::vec3(0.025));
		model = glm::rotate(model, glm::radians(rotpanque), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		panque.Draw(staticShader);

		//cheff
		// -------------------------------------------------------------------------------------------------------------------------
		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(-180.0f, 0.0f, 0.0f));//(-180,0,0)
		model = glm::scale(model, glm::vec3(14.0f));
		staticShader.setMat4("model", model);
		cheff.Draw(staticShader);

		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(-182.0f, 13.5f, 0.0f));//(-180,0,0)
		model = glm::scale(model, glm::vec3(14.0f));
		model = glm::rotate(model, glm::radians(rotcheff), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(105.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		cheffbd.Draw(staticShader);

		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(-178.0f, 13.5f, 0.0f));//(-180,0,0)
		model = glm::scale(model, glm::vec3(14.0f));
		model = glm::rotate(model, glm::radians(-1 * rotcheff), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(75.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		cheffbd.Draw(staticShader);

		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(-180.0f, poszsar, 7.0f));//(-180,13.5,0)
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotsarten), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		sarten.Draw(staticShader);

		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(-180.0f, 11.2f, -70.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		plato.Draw(staticShader);

		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(-180.0f, carnez + 13.5, carney));//(-180,13.5,12.0)
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		carne.Draw(staticShader);

		//Bunny
		// -------------------------------------------------------------------------------------------------------------------------
		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(-85.0f, -0.5f, -10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0));
		staticShader.setMat4("model", model);
		Bunny.Draw(staticShader);

		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(-85.0f, -0.5f, -10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0));
		model = glm::rotate(model, glm::radians(rot_bIzqB), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		BunnyBrazoIzq.Draw(staticShader);

		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(-85.0f, -0.5f, -10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0));
		model = glm::rotate(model, glm::radians(rot_bDerB), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		BunnyBrazoDer.Draw(staticShader);

		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(-85.0f, -0.5f, -10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0));
		model = glm::rotate(model, glm::radians(rot_pIzqB), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		BunnyPieIzq.Draw(staticShader);

		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(-85.0f, -0.5f, -10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0));
		model = glm::rotate(model, glm::radians(rot_pDerB), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		BunnyPieDer.Draw(staticShader);

		//Pasto Diorama
		// -------------------------------------------------------------------------------------------------------------------------
		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		else {
			model = glm::mat4(1.0f);
		}
		model = glm::translate(model, glm::vec3(0.0f, -13.25f, 0.0f));
		model = glm::scale(model, glm::vec3(50.0f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// BallonBoy
		// -------------------------------------------------------------------------------------------------------------------------
		glm::mat4 pgbb = glm::mat4(1.0f); // BALLONBOY PIVOTE-GLOBO
		glm::mat4 plbb = glm::mat4(1.0f); // BALLONBOY PIVOTE-LETRERO
		glm::mat4 torsoTemp = glm::mat4(1.0f); //BALLONBOY-PIVOTE-TORSO
		glm::mat4 phdbb = glm::mat4(1.0f); //BALLONBOY PIVOTE-HOMBRO-DERECHO
		glm::mat4 phibb = glm::mat4(1.0f); // BALLONBOY PIVOTE-HOMBRO-IZQUIERDO
		glm::mat4 pribb = glm::mat4(1.0f); // BALLONBOY PIVOTE-RODILLA-IZQUIERDA
		glm::mat4 prdbb = glm::mat4(1.0f); // BALLONBOY PIVOTE-RODILLA-DERECHA

		BBCameraX = 1.75f * glm::cos(glm::radians(camera.getYaw()));
		BBCameraZ = 1.5f * glm::sin(glm::radians(camera.getYaw()));

		//TORZO

		model = glm::translate(model, glm::vec3(100.0f, 15.0f, 100.0f));
		model = glm::scale(model, glm::vec3(0.65f));
		if (camera.getIsometric()) {
			model = camera.ConfIsometric(model);
		}
		model = glm::translate(model, glm::vec3(camera.getPosition().x + BBCameraX, camera.getPosition().y, camera.getPosition().z) + BBCameraZ); //Intentar rolar con este
		torsoTemp = model = glm::rotate(model, glm::radians(-camera.getYaw() + 90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		torsoBB.Draw(staticShader);
		//CABEZA
		model = glm::translate(torsoTemp, glm::vec3(0.0f, 10.5f, 1.5f)); //-1.8f
		//model = glm::scale(model, glm::vec3(0.75f));
		model = glm::rotate(model, glm::radians(movCabeza), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 10.5f, 1.5f));
		staticShader.setMat4("model", model);
		cabezaBB.Draw(staticShader);

		//HOMBRO_DERECHO
		model = glm::translate(torsoTemp, glm::vec3(3.0f, 4.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.75f));
		model = glm::rotate(model, glm::radians(movHombroDer), glm::vec3(0.0f, 0.0f, 1.0f));
		phdbb = model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		hombroDerBB.Draw(staticShader);

		//BRAZO_DERECHO
		model = glm::translate(phdbb, glm::vec3(6.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.75f));
		plbb = model = glm::rotate(model, glm::radians(movBrazoDer), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		brazoDerBB.Draw(staticShader);

		//HOMBRO_IZQUIERDO
		model = glm::translate(torsoTemp, glm::vec3(-3.0f, 4.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.75f));
		model = glm::rotate(model, glm::radians(-movHombroIzq), glm::vec3(0.0f, 0.0f, 1.0f));
		phibb = model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		hombroIzqBB.Draw(staticShader);

		//BRAZO_IZQUIERDO
		model = glm::translate(phibb, glm::vec3(-5.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.75f));
		pgbb = model = glm::rotate(model, glm::radians(-movBrazIzq), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		brazoIzqBB.Draw(staticShader);

		//PIERNA DERECHA ARRIBA
		model = glm::translate(torsoTemp, glm::vec3(5.0f, -7.0f, 0.0f)); //-1.8f
		//model = glm::scale(model, glm::vec3(0.75f));
		model = glm::rotate(model, glm::radians(movPierDer), glm::vec3(1.0f, 0.0f, 0.0f));
		prdbb = model = glm::translate(model, glm::vec3(1.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		piernaDerArrBB.Draw(staticShader);

		//PIERNA DERECHA ABAJO 
		model = glm::translate(prdbb, glm::vec3(0.0f, -10.0f, -0.5f)); //-1.8f
		//model = glm::scale(model, glm::vec3(0.75f));
		model = glm::rotate(model, glm::radians(rotRodDer), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, -0.5f));
		staticShader.setMat4("model", model);
		piernaDerAbBB.Draw(staticShader);

		//PIERNA IZQUIERDA ARRIBA
		model = glm::translate(torsoTemp, glm::vec3(-5.0f, -7.0f, 0.0f)); //-1.8f
		//model = glm::scale(model, glm::vec3(0.75f));
		model = glm::rotate(model, glm::radians(movPierIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		pribb = model = glm::translate(model, glm::vec3(-1.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		piernaIzqArrBB.Draw(staticShader);

		//PIERNA IZQUIERDA ABAJO
		model = glm::translate(pribb, glm::vec3(0.0f, -10.0f, -0.5f)); //-1.8f
		//model = glm::scale(model, glm::vec3(0.75f));
		model = glm::rotate(model, glm::radians(rotRodIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, -0.5f));
		staticShader.setMat4("model", model);
		piernaDerAbBB.Draw(staticShader);

		//GLOBO
		model = glm::translate(pgbb, glm::vec3(-9.55f, 0.0f, -0.5f)); //-1.8f
		model = glm::rotate(model, glm::radians(-movBrazIzq), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(0.75f));
		//model = glm::translate(model, glm::vec3(0.0f, 2.0f, -0.5f));
		staticShader.setMat4("model", model);
		globoBB.Draw(staticShader);
		//LETRERO
		model = glm::translate(plbb, glm::vec3(9.55f, 0.0f, -0.5f)); //-1.8f
		model = glm::rotate(model, glm::radians(movBrazoDer), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::translate(model, glm::vec3(0.0f, 2.0f, -0.5f));
		//model = glm::scale(model, glm::vec3(0.75f));
		staticShader.setMat4("model", model);
		letreroBB.Draw(staticShader);



		//Luz Techo
		//--------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 70.0f, -150.0f));
		model = glm::scale(model, glm::vec3(0.7f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		luzTecho.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 70.0f, -50.0f));
		model = glm::scale(model, glm::vec3(0.7f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		luzTecho.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-200.0f, 70.0f, -50.0f));
		model = glm::scale(model, glm::vec3(0.7f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		luzTecho.Draw(staticShader);

		//Pasto Diorama
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -13.25f, 0.0f));
		model = glm::scale(model, glm::vec3(50.0f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);

		
		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
		
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
		
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
		
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
		
	}

	//Activcar animaciones 
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) //Activar o desactivar anmacion Freddy
		Freddyanim ^= true;
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) //Activar o desactivar animacion Chica
		Chicaanim ^= true;
	/*if (key == GLFW_KEY_3 && action == GLFW_PRESS)
		animacion ^= true;*/
		//Activar o desactivar luces
	if (key == GLFW_KEY_9 && action == GLFW_PRESS)
		encendidoarcade ^= true;


	

	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}
	//BALLONBOY

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		posZ--;
		movCuerpo();
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		posX++;
		movCuerpo();
	}


	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		posZ++;
		movCuerpo();
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		posX--;
		movCuerpo();
	}

	//CABEZA
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS && movCabeza < 75.0f)
		movCabeza += 3.5f;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS && movCabeza > -75.0f)
		movCabeza -= 3.5f;

	//ROTACION
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		giroMonito--;
		movCuerpo();
	}
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		giroMonito++;
		movCuerpo();
	}
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		camera.setIsometric(true);
		
	}
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
		camera.setIsometric(false);
		
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		camera.setZoom(camera.getIsoZoom() + 0.5f);
		if (camera.getIsoZoom() >= 30.0f) camera.setZoom(30.0f);
		
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		camera.setZoom(camera.getIsoZoom() - 0.5f);
		if (camera.getIsoZoom() <= 2.0f) camera.setZoom(2.0f);
		
	}
	
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
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