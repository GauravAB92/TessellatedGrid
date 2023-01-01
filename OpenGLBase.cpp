#include "OpenGLBase.h"

//global pointer to class to be used for callbacks
OpenGLBase* base;



//C signature callbacks required for glfw

void resize(GLFWwindow* window, int width, int height)
{
	
	base->OnResize(width, height);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	base->OnMouseMove(xpos, ypos);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	base->OnKeyPressed(key, scancode, action, mods);
}


void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	base->OnMouseButtonPressed(button,action,mods,xpos,ypos);


}


OpenGLBase::OpenGLBase() 
{
	base = this;
}


OpenGLBase::~OpenGLBase()
{
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}




bool OpenGLBase::InitializeWindowAndOpenGLContext()
{
	assert(glfwInit() == GLFW_TRUE);

	//glfwWindowHint(GLFW_SAMPLES, 64);

	GLFWwindow* window = glfwCreateWindow(
		mClientWindowWidth, mClientWindowHeight, "OpenGL Application", nullptr, nullptr
	);
	
	if (!window)
	{
		glfwTerminate();
		std::cerr << "glfw failed to create window\n";
		return false;
	}

	mWindow = window;

	glfwMakeContextCurrent(mWindow);

	glewExperimental = true;

	GLenum err_no = glewInit();

	if (err_no != GLEW_OK)
	{
		std::cerr << glewGetErrorString(err_no) << "\n";
		glfwDestroyWindow(mWindow);
		glfwTerminate();
		return false;
	}


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

	return true;
}


bool OpenGLBase::Initialize() 
{
	if (!InitializeWindowAndOpenGLContext())
	{

		return false;
	}

	//Set CallBacks

	glfwSetWindowSizeCallback(mWindow, resize);
	glfwSetInputMode(mWindow, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
	glfwSetCursorPosCallback(mWindow, mouseCallback);
	glfwSetMouseButtonCallback(mWindow, mouseButtonCallback);
	glfwSetKeyCallback(mWindow, keyCallback);
	
	return true;
}

void OpenGLBase::RenderLoop()
{
	
	while (!glfwWindowShouldClose(mWindow))
	{
		

		UpdateFrame();
		RenderFrame();

		glfwPollEvents();
		glfwSwapBuffers(mWindow);
	}
}
