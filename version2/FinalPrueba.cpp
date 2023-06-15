/*---------------------------------------------------------*/
/* ----------------  Práctica 10                -----------*/
/*-----------------    2023-2   ---------------------------*/
/*------------- Alumno:  Chávez Flores Giovanni -----------*/
/*------------- No. Cuenta      317053319   ---------------*/
#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>


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
Camera camera(glm::vec3(0.0f, 10.0f, 90.0f)); // 0.0f, 10.0f, 90.0f
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);
glm::vec3 colorDifuso(0.0f, 0.1f, 0.0f);
glm::vec3 colorAmbiente(0.3f, 0.3f, 0.0f);

//BALLON BOY
glm::vec3 prueba(0.0f, 0.0f, 0.0f);





// posiciones
float	movAuto_x = 0.0f,
		movAuto_z = 0.0f,
		movAuto_y = 0.0f,
		orienta = 0.0f;
bool	animacion = false,
		recorrido1 = true,
		recorrido2 = false,
		recorrido3 = false,
		recorrido4 = false;

float myVariable = 0.0f;

float movMolino = 0.0f;

bool avanza = false;
bool reversa = false;
bool vuela = false;
bool quieto = false;
bool baja = false;
bool sube = true;

//Keyframes (Manipulación y dibujo)
//declarar variables para la manipulación y dibujo (por cada propiedad que quiero animar)
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
float	incX = 0.0f,	//Calculo del incremento
		incY = 0.0f,
		incZ = 0.0f,
		rotInc = 0.0f,
		rotDer = 0.0f,
		giroMonitoInc = 0.0f,
		incBrazDer = 0.0f,
		incBrazIzq = 0.0f,
		incCabeza = 0.0f;
// define, le digo al sistema cuantos cuadros clave va a guardar.
#define MAX_FRAMES 30
int i_max_steps = 60; // Cantidad de cuadros intermedios que se van a calcular  -> incremento=(ValFin-ValIni)/(# de cuadros intermedios)
int i_curr_steps = 0;
typedef struct _frame 
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float rotRodDer;
	float giroMonito;
	float movBrazoDer; //puede o no tener el mismo nombre
	float movBrazIzq;
	float movCabeza;

}FRAME;

FRAME KeyFrame[MAX_FRAMES]; //cada elemento de aqui va a pertener a la estructura 
int FrameIndex = 0;			//introducir número en caso de tener Key guardados
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl; //cuando presione la tecla L el valor que tiene en ese instante las mete al arreglo del keifreim

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].rotRodDer = rotRodDer;
	KeyFrame[FrameIndex].giroMonito = giroMonito;

	KeyFrame[FrameIndex].movBrazoDer = movBrazoDer; // el valor de la variable de 
	KeyFrame[FrameIndex].movBrazIzq = movBrazIzq;
	KeyFrame[FrameIndex].movCabeza = movCabeza;

	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodDer = KeyFrame[0].rotRodDer;
	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;

	movBrazoDer = KeyFrame[0].giroMonito; //va de dibujo en esa propiedad va a recibir el valor guardado en el elemento 0 de ese misma propiedad
	movBrazIzq = KeyFrame[0].giroMonito;
	movCabeza = KeyFrame[0].giroMonito;
	

}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	rotDer = (KeyFrame[playIndex + 1].rotRodDer - KeyFrame[playIndex].rotRodDer) / i_max_steps;
	rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;
	incBrazDer = (KeyFrame[playIndex+1].movBrazoDer-KeyFrame[playIndex].movBrazoDer) / i_max_steps; // Variable incremento ->
	incBrazIzq = (KeyFrame[playIndex + 1].movBrazIzq - KeyFrame[playIndex].movBrazIzq) / i_max_steps;
	incCabeza = (KeyFrame[playIndex + 1].movCabeza - KeyFrame[playIndex].movCabeza) / i_max_steps;


}

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



void animate(void) //crear animaciones de manera automática
{
	lightPosition.x = 100.0f * cos(myVariable);
	lightPosition.z = 100.0f * sin(myVariable);
	myVariable += 0.05f;

	prueba.x = 0.1f * cos(movMolino);
	prueba.z = 0.1f * sin(movMolino);
	movMolino += 0.03f;

	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;

			rotRodIzq += rotInc;
			rotRodDer += rotDer;
			giroMonito += giroMonitoInc;

			movBrazoDer += incBrazDer;
			movBrazIzq += incBrazIzq;
			movCabeza += incCabeza;
			i_curr_steps++;
		}
	}

	if (animacion) // Si quitamos este if, es para que desde el pricipio de la ejecución se realice el movimiento del carro
	{
		 if(avanza)
		 {
			movAuto_z += 3.5f;
			if (movAuto_z >= 100.0f) {
				avanza = false;
				reversa = true;
			}
		 }
		 if (reversa) {
			 movAuto_z -= 3.5f;
			 if (movAuto_z <= 0.0f) {
				 reversa = false;
				 vuela = true;
			 }
		 }
		 if (vuela) {
			 movAuto_y += 3.5f;
			 if (movAuto_y >= 120.0f) {
				 vuela = false;
				 baja = true;
			 }
		 }
		 if (baja) {
			 movAuto_y -= 3.5f;
			 if (movAuto_y <= 0.0f) {
				 baja = false;
			 }
		 }
	
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
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC", NULL, NULL);
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

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights_mod.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
	Model piso("resources/objects/piso/piso.obj");
	Model botaDer("resources/objects/Personaje/bota.obj");
	Model piernaDer("resources/objects/Personaje/piernader.obj");
	Model piernaIzq("resources/objects/Personaje/piernader.obj");
	Model torso("resources/objects/Personaje/torso.obj");
	Model brazoDer("resources/objects/Personaje/brazoder.obj");
	Model brazoIzq("resources/objects/Personaje/brazoizq.obj");
	Model cabeza("resources/objects/Personaje/cabeza.obj");
	Model carro("resources/objects/lambo/carroceria.obj");
	Model llanta("resources/objects/lambo/Wheel.obj");
	Model casaVieja("resources/objects/casa/OldHouse.obj");
	//Model cubo("resources/objects/cubo/cube02.obj");
	Model casaDoll("resources/objects/casa/DollHouse.obj");	
	Model casaBrujas("resources/objects/CasaBrujas/CasaBrujas.obj");

	ModelAnim ajWalk("resources/objects/AJ/AjWalk.dae");
	ajWalk.initShaders(animShader.ID); //asociar la animación al modelo


	//PRUEBA DE BALLON BOY
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


	/*ModelAnim animacionPersonaje("resources/objects/Personaje1/PersonajeBrazo.dae");

	animacionPersonaje.initShaders(animShader.ID);

	ModelAnim ninja("resources/objects/ZombieWalk/ZombieWalk.dae");
	ninja.initShaders(animShader.ID);*/

	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;
	}

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);
		
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

		//Setup Advanced Lights || componentes de iluminación
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.5f, 0.5f, 0.5f)); // Luz que yo no controlo -> las caras menos iluminadas tienden a negro
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.0f, 0.0f, 0.0f)); // Luz que podemos controlar -> las caras más iluminadas
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f)); // simular el brillo en los objetos            

		staticShader.setVec3("pointLight[0].position", lightPosition); // Los objetos que esten cerca se van a ver más iluminados
		staticShader.setVec3("pointLight[0].ambient", colorAmbiente);
		//staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.3f, 0.3f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", colorDifuso);
		//staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", colorAmbiente);
		staticShader.setVec3("pointLight[1].diffuse", colorDifuso);
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 0.08f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[2].position", glm::vec3(80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[2].ambient", glm::vec3(0.0f, 0.1f, 0.2f));
		staticShader.setVec3("pointLight[2].diffuse", glm::vec3(1.0f, 0.8f, 0.9f));
		staticShader.setVec3("pointLight[2].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[2].constant", 1.0f);
		staticShader.setFloat("pointLight[2].linear", 0.009f);
		staticShader.setFloat("pointLight[2].quadratic", 0.00032f);

		staticShader.setVec3("spotLight[0].position", glm::vec3(0.0f, 20.0f, 10.0f));
		staticShader.setVec3("spotLight[0].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(10.0f)));
		staticShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(60.0f)));
		staticShader.setFloat("spotLight[0].constant", 1.0f);
		staticShader.setFloat("spotLight[0].linear", 0.0009f);
		staticShader.setFloat("spotLight[0].quadratic", 0.0005f);

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
	
		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-40.3f, 1.75f, 0.3f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.2f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		//animacionPersonaje.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Segundo Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(40.3f, 1.75f, 0.3f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.5f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		//ninja.Draw(animShader);

		/*Código para mandar a llamar al compañero AJ*/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(90.0f, 0.0f, 10.0f));
		model = glm::scale(model, glm::vec3(0.25f));
		animShader.setMat4("model", model);
		ajWalk.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//Aplicar una operación traslación para ocupar como referencia el origen del sistema
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-120.0f, 0.0f, 20.0f));
		model = glm::scale(model, glm::vec3(2.5f));
		staticShader.setMat4("model", model); //Los datos mandarlos al shader
		casaBrujas.Draw(staticShader);
		//Dibujar la casa en un punto, cambiarle tamaño, shader y se dibuja

		model = glm::translate(glm::mat4(1.0f), glm::vec3(250.0f, 0.0f, -10.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		casaDoll.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -70.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		casaVieja.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Carro
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(15.0f + movAuto_x, -1.0f + movAuto_y, movAuto_z));// AQUIUUUUUUUUUUUUUUUUUUUUUUUUIIIIIIIII
		tmp = model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		carro.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, 12.9f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Izq delantera

		model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, 12.9f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Der delantera

		model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, -14.5f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Der trasera

		model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, -14.5f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Izq trase

		// -------------------------------------------------------------------------------------------------------------------------
		// BallonBoy
		// -------------------------------------------------------------------------------------------------------------------------
		glm::mat4 pgbb = glm::mat4(1.0f); // BALLONBOY PIVOTE-GLOBO
		glm::mat4 plbb = glm::mat4(1.0f); // BALLONBOY PIVOTE-LETRERO
		glm::mat4 torsoTemp = glm::mat4(1.0f);
		glm::mat4 phdbb = glm::mat4(1.0f); //BALLONBOY PIVOTE-HOMBRO-DERECHO
		glm::mat4 phibb = glm::mat4(1.0f); // BALLONBOY PIVOTE-HOMBRO-IZQUIERDO
		glm::mat4 pribb = glm::mat4(1.0f); // BALLONBOY PIVOTE-RODILLA-IZQUIERDA
		glm::mat4 prdbb = glm::mat4(1.0f); // BALLONBOY PIVOTE-RODILLA-DERECHA
		
		//TORZO
		model = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 23.0f, 100.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		torsoTemp = model = glm::rotate(model, glm::radians(giroMonito), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		torsoBB.Draw(staticShader);
		//CABEZA
		model = glm::translate(torsoTemp, glm::vec3(0.0f, 10.5f, 1.5f)); //-1.8f
		model = glm::rotate(model, glm::radians(movCabeza), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 10.5f, 1.5f));
		staticShader.setMat4("model", model);
		cabezaBB.Draw(staticShader);
		
		//HOMBRO_DERECHO
		model = glm::translate(torsoTemp, glm::vec3(3.0f, 4.0f, 0.0f));
		model = glm::rotate(model, glm::radians(movHombroDer), glm::vec3(0.0f, 0.0f, 1.0f));
		phdbb = model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		hombroDerBB.Draw(staticShader);

		//BRAZO_DERECHO
		model = glm::translate(phdbb, glm::vec3(6.0f, 0.0f, 0.0f));
		plbb = model = glm::rotate(model, glm::radians(movBrazoDer), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		brazoDerBB.Draw(staticShader);

		//HOMBRO_IZQUIERDO
		model = glm::translate(torsoTemp, glm::vec3(-3.0f, 4.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-movHombroIzq), glm::vec3(0.0f, 0.0f, 1.0f));
		phibb = model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		hombroIzqBB.Draw(staticShader);

		//BRAZO_IZQUIERDO
		model = glm::translate(phibb, glm::vec3(-5.0f, 0.0f, 0.0f));
		pgbb = model = glm::rotate(model, glm::radians(-movBrazIzq), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		brazoIzqBB.Draw(staticShader);

		//PIERNA DERECHA ARRIBA
		model = glm::translate(torsoTemp, glm::vec3(5.0f, -7.0f, 0.0f)); //-1.8f
		model = glm::rotate(model, glm::radians(movPierDer), glm::vec3(1.0f, 0.0f, 0.0f));
		prdbb = model = glm::translate(model, glm::vec3(1.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		piernaDerArrBB.Draw(staticShader);

		//PIERNA DERECHA ABAJO 
		model = glm::translate(prdbb, glm::vec3(0.0f, -10.0f, -0.5f)); //-1.8f
		model = glm::rotate(model, glm::radians(rotRodDer), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, -0.5f));
		staticShader.setMat4("model", model);
		piernaDerAbBB.Draw(staticShader);

		//PIERNA IZQUIERDA ARRIBA
		model = glm::translate(torsoTemp, glm::vec3(-5.0f, -7.0f, 0.0f)); //-1.8f
		model = glm::rotate(model, glm::radians(movPierIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		pribb = model = glm::translate(model, glm::vec3(-1.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		piernaIzqArrBB.Draw(staticShader);

		//PIERNA IZQUIERDA ABAJO
		model = glm::translate(pribb, glm::vec3(0.0f, -10.0f, -0.5f)); //-1.8f
		model = glm::rotate(model, glm::radians(rotRodIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, -0.5f));
		staticShader.setMat4("model", model);
		piernaDerAbBB.Draw(staticShader);

		//GLOBO
		model = glm::translate(pgbb, glm::vec3(-9.55f, 0.0f, -0.5f)); //-1.8f
		model = glm::rotate(model, glm::radians(-movBrazIzq), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::translate(model, glm::vec3(0.0f, 2.0f, -0.5f));
		staticShader.setMat4("model", model);
		globoBB.Draw(staticShader);
		//LETRERO
		model = glm::translate(plbb, glm::vec3(9.55f, 0.0f, -0.5f)); //-1.8f
		model = glm::rotate(model, glm::radians(movBrazoDer), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::translate(model, glm::vec3(0.0f, 2.0f, -0.5f));
		staticShader.setMat4("model", model);
		letreroBB.Draw(staticShader);


		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		tmp = model = glm::rotate(model, glm::radians(giroMonito), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		torso.Draw(staticShader);

		//Pierna Der
		model = glm::translate(tmp, glm::vec3(-0.5f, 0.0f, -0.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(rotRodDer), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		piernaDer.Draw(staticShader);

		//Pie Der
		model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		staticShader.setMat4("model", model);
		botaDer.Draw(staticShader);

		//Pierna Izq
		model = glm::translate(tmp, glm::vec3(0.5f, 0.0f, -0.1f));
		model = glm::rotate(model, glm::radians(rotRodIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		piernaIzq.Draw(staticShader);

		//Pie Iz
		model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		staticShader.setMat4("model", model);
		botaDer.Draw(staticShader);	//Izq trase

		//Brazo derecho
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(movBrazoDer), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		brazoDer.Draw(staticShader);

		//Brazo izquierdo
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(movBrazIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		brazoIzq.Draw(staticShader);

		//Cabeza
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(movCabeza), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0));
		staticShader.setMat4("model", model);
		cabeza.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Caja Transparente --- Siguiente Práctica
		// -------------------------------------------------------------------------------------------------------------------------
		/*glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -70.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		cubo.Draw(staticShader);
		glEnable(GL_BLEND);*/
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
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	//To Configure Model
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
	//HOMBRO DERECHO
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS && movHombroDer < 2.0f)
		movHombroDer += 3.5f;
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && movHombroDer > -0.1f)
		movHombroDer -= 3.5f;
	//HOMBRO IZQUIERDO
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && movHombroIzq < 2.0f)
		movHombroIzq += 3.5f;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && movHombroIzq > -0.1f)
		movHombroIzq -= 3.5f;
	//BRAZO DERECHO
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS && movBrazoDer < 0.5f)
		movBrazoDer += 3.5f;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS && movBrazoDer > 0.0f)
		movBrazoDer -= 3.5f;
	//BRAZO IZQUIERDO
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && movBrazIzq < 0.5f)
		movBrazIzq += 3.5f;
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && movBrazIzq > 0.0f)
		movBrazIzq -= 3.5f;

	//PIERNA ARRIBA DERECHA
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && movPierDer < 70.0f)
		movPierDer += 3.5f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && movPierDer > -90.0f)
		movPierDer -= 3.5f;
	//PIERNA ARRIBA IZQUIERDA
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && movPierIzq < 70.0f)
		movPierIzq += 3.5f;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && movPierIzq > -90.0f)
		movPierIzq -= 3.5f;

	//RODILLA DERECHA
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS && rotRodDer < 70.0f)
		rotRodDer++;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && rotRodDer > 0.0f)
		rotRodDer--;

	//RODILLA IZQUIERDA
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && rotRodIzq < 70.0f)
		rotRodIzq++;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && rotRodIzq > 0.0f)
		rotRodIzq--;

	//ROTACION
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		giroMonito--;
		movCuerpo();
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		giroMonito++;
		movCuerpo();
	}
	/*
	//Y
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		posY--;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		posY++;
	*/
	//Car animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		avanza = true;
		animacion ^= true; //originalmente es un xor || animacion = !animacion;
	}
	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		movAuto_z = 0.0f;
	}
	//To play KeyFrame animation 

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}

	//Cambiar el color de la luz
	if (key == GLFW_KEY_0 && action == GLFW_PRESS) {
		colorAmbiente.x = 1.0f;
		colorDifuso.x = 1.0f;
	}
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		colorAmbiente.x = 0.0f;
		colorAmbiente.y = 1.0f;
		colorDifuso.x = 0.0f;
		colorDifuso.y = 1.0f;
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		colorAmbiente.x = 0.0f;
		colorAmbiente.y = 0.0f;
		colorAmbiente.z = 1.0f;
		colorDifuso.x = 0.0f;
		colorDifuso.y = 0.0f;
		colorDifuso.z = 1.0f;
	}
	if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
		colorAmbiente.x = 0.3f;
		colorAmbiente.y = 0.3f;
		colorAmbiente.z = 0.0f;
		colorDifuso.x = 0.0f;
		colorDifuso.y = 0.1f;
		colorDifuso.z = 0.0f;
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