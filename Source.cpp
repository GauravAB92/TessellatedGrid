#include "OpenGLBase.h"
#include "OpenGLUtils.h"
#include "glm/glm.hpp"
#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class App : public OpenGLBase
{
public:
	App()
	{
		Initialize();
	}

	~App()
	{

	}

	bool Initialize();


	void RenderFrame() override;
	void UpdateFrame() override;

private:
	Pipeline tessgridProgram;
};


bool App :: Initialize()
{
	assert(OpenGLBase::Initialize());

	tessgridProgram.setShader(GL_VERTEX_SHADER, "./grid_vs.glsl");
	tessgridProgram.setShader(GL_FRAGMENT_SHADER, "./grid_fs.glsl");
	tessgridProgram.setShader(GL_TESS_CONTROL_SHADER, "./grid_tc.glsl");
	tessgridProgram.setShader(GL_TESS_EVALUATION_SHADER, "./grid_te.glsl");

	tessgridProgram.bind();


	glPatchParameteri(GL_PATCH_VERTICES, 4);
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	return true;
}


void App::UpdateFrame()
{
	tessgridProgram.bind();



	glm::mat4 p = glm::perspectiveFovRH(45.0f, (float)mClientWindowWidth, (float)mClientWindowHeight,1.0f, 1000.0f);
	glm::mat4 v = glm::lookAtRH(glm::vec3(0.0f, 10.0f, 5.0f), glm::vec3(0.0f, 10.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(tessgridProgram.getLocation("mvp"), 1, GL_FALSE, reinterpret_cast<const float*>(glm::value_ptr(p*v)));


}

void App::RenderFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, mClientWindowWidth, mClientWindowHeight);

	
	tessgridProgram.bind();

	glDrawArraysInstanced(GL_PATCHES, 0, 4, 4096);
}



int main()
{

	App application;

	application.RenderLoop();

	return 0;
}