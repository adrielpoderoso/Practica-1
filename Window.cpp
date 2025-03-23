#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	rotax = 0.0f;
	rotay = 0.0f;
	rotaz = 0.0f;
	trasladocoche = 0.0f;
	llantadelanteraderecha = 0.0f;
	llantadelanteraizquierda = 0.0f;
	cajuela = 0.0f;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "PRACTICA 5 TRIGUEROS LOPEZ HECTOR ADRIAN", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_R)
	{
		theWindow->rotay += 10.0; //rotar sobre el eje y 10 grados
	}
	else if (key == GLFW_KEY_E)
	{
		theWindow->rotax += 10.0;
	}
	else if (key == GLFW_KEY_T)
	{
		theWindow->rotaz += 10.0;
	}


	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_D, 0);
		//printf("se presiono la tecla: %s\n",key_name);
	}
	//INICIO DE CARRO BASE
	if (key == GLFW_KEY_L) {
		theWindow->trasladocoche += 2.0f;
	}
	if (key == GLFW_KEY_K) {
		theWindow->trasladocoche -= 2.0f;
	}
	//FIN DE CARRO BASE

	//INICIO LLANTA DELANTERA DERECHA
	if (key == GLFW_KEY_C) {
		theWindow->llantadelanteraderecha += 10;
		if (theWindow->llantadelanteraderecha >= 360) {
			theWindow->llantadelanteraderecha -= 360;  // Normaliza el ángulo
		}
	}
	if (key == GLFW_KEY_V) {
		theWindow->llantadelanteraderecha -= 10;
		if (theWindow->llantadelanteraderecha < 0) {
			theWindow->llantadelanteraderecha += 360;  // Normaliza el ángulo
		}
	}
	//FIN LLANTA DELANTERA DERECHA
	//INICIO LLANTA DELANTERA IZQUIERDA
	if (key == GLFW_KEY_C) {
		theWindow->llantadelanteraizquierda += 10;
		if (theWindow->llantadelanteraizquierda >= 360) {
			theWindow->llantadelanteraizquierda -= 360;  // Normaliza el ángulo
		}
	}
	if (key == GLFW_KEY_V) {
		theWindow->llantadelanteraizquierda -= 10;
		if (theWindow->llantadelanteraizquierda < 0) {
			theWindow->llantadelanteraizquierda += 360;  // Normaliza el ángulo
		}
	}
	//FIN LLANTA DELANTERA IZQUIERDA
	//INICIO LLANTA TRASERA IZQUIERDA
	if (key == GLFW_KEY_C) {
		theWindow->llantatraseraizquierda += 10;
		if (theWindow->llantatraseraizquierda >= 360) {
			theWindow->llantatraseraizquierda -= 360;  // Normaliza el ángulo
		}
	}
	if (key == GLFW_KEY_V) {
		theWindow->llantatraseraizquierda -= 10;
		if (theWindow->llantatraseraizquierda < 0) {
			theWindow->llantatraseraizquierda += 360;  // Normaliza el ángulo
		}
	}
	//FIN LLANTA TRASERA IZQUIERDA
    //INICIO LLANTA TRASERA DERECHA
	if (key == GLFW_KEY_C) {
		theWindow->llantatraseraderecha += 10;
		if (theWindow->llantatraseraderecha >= 360) {
			theWindow->llantatraseraderecha -= 360;  // Normaliza el ángulo
		}
	}
	if (key == GLFW_KEY_V) {
		theWindow->llantatraseraderecha -= 10;
		if (theWindow->llantatraseraderecha < 0) {
			theWindow->llantatraseraderecha += 360;  // Normaliza el ángulo
		}
	}
	//FIN LLANTA TRASERA DERECHA
// INICIO DE CAJUELA
	if (key == GLFW_KEY_N) {
		if (theWindow->cajuela >= 0) {

		}
		else {
			theWindow->cajuela += 10;
		}
	}
	if (key == GLFW_KEY_M) {
		if (theWindow->cajuela < -45) {

		}
		else {
			theWindow->cajuela -= 10;
		}

	}

	// FIN DE CAJUELA


	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}

}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
