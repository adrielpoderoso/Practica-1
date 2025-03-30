#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);
	}
	bool* getsKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	GLfloat getrotay() { return rotay; }
	GLfloat getrotax() { return rotax; }
	GLfloat getrotaz() { return rotaz; }
	GLfloat gettrasladocoche() { return trasladocoche; }
	GLfloat getllantadelanteraderecha() { return llantadelanteraderecha; }
	GLfloat getllantadelanteraizquierda() { return llantadelanteraizquierda; }
	GLfloat getllantatraseraizquierda() { return llantatraseraizquierda; }
	GLfloat getllantatraseraderecha() { return llantatraseraderecha; }
	GLfloat getcajuela() { return cajuela; }
	GLfloat getcapotaje() { return capotaje; }


	~Window();
private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLfloat rotax, rotay, rotaz;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat trasladocoche;
	GLfloat llantadelanteraderecha;
	GLfloat llantadelanteraizquierda;
	GLfloat llantatraseraizquierda;
	GLfloat llantatraseraderecha;
	GLfloat cajuela;
	GLfloat capotaje;
	bool mouseFirstMoved;
	void createCallbacks();
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
};

