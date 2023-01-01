#pragma once

#include "glew.h"	// extensions
#include <gl/GL.h>	    // gl lib
#include <GLFW/glfw3.h>	// windowing lib
#include <cassert>
#include <iostream>

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"glfw3.lib")



class OpenGLBase
{
protected:
	OpenGLBase();
	OpenGLBase(const OpenGLBase& rhs) = delete;
	OpenGLBase& operator=(const OpenGLBase& rhs) = delete;
	virtual ~OpenGLBase();

public:

	virtual bool Initialize();
	void RenderLoop();

	//Convinience input overrides
	
	virtual void OnResize(int width, int height) {}
	virtual void OnMouseMove(double xpos, double ypos) {}
	virtual void OnMouseButtonPressed(int button, int action, int mods, double xpos, double ypos) {}
	virtual void OnKeyPressed(int key, int scancode, int action, int mods) {}

protected:

	bool InitializeWindowAndOpenGLContext();

//Exposed functions
protected:

	virtual void RenderFrame() = 0;
	virtual void UpdateFrame() = 0;
	


protected:
	uint32_t mClientWindowWidth = 1024;
	uint32_t mClientWindowHeight = 1024;
	bool mFullScreenState = false;
	GLFWwindow* mWindow = nullptr;
};


