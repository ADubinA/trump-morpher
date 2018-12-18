#include <iostream>
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "debugTimer.h"
#include "inputManager.h"
#include "texture.h"
#include "sceneParser.h"

using namespace glm;

int main(int argc,char** argv)
{
	Display display(800,600,"hello!");
	//Shader shader("res/shaders/mbrot");
	Shader shader("res/shaders/texShader");
	Vertex vertices[] = {Vertex(vec3(-1,-1,0),vec2(0,0),vec3(0,0,1)),Vertex(vec3(1,-1,0),vec2(1,0),vec3(0,0,1)),Vertex(vec3(1,1,0),vec2(1,1),vec3(0,0,1)),Vertex(vec3(-1,1,0),vec2(0,1),vec3(0,0,1))};
	unsigned int indices[] = {0,1,2,0,2,3};
	Mesh mesh(vertices,4,indices,6);
	float t =0.0;
	
	Texture tex1("res/textures/src1.bmp");
	Texture tex2("res/textures/dst1.bmp");
	Scene scn("res/cfg.txt",tex1.GetWidth(),tex1.GetHeight());
	scn.PrintScene();
	tex1.Bind(0);
	tex2.Bind(1);
	glfwSetKeyCallback(display.m_window,key_callback);
	
	//main loop
	while(!glfwWindowShouldClose(display.m_window))
	{
		if(t<1)
			//t+=0.00048828125;
			t += 0.00048828125*100;
		else
			t=0;
		//clear back buffer 
		display.Clear(0.0f, 0.0f, 0.0f, 1.0f);
	
		//bind shader
		shader.Bind();
		shader.Update(t);
		scn.loadtoShader(shader);

		//draw mesh
		mesh.Draw();
		//swap front and back buffer
		display.SwapBuffers();

		glfwPollEvents();
		

	}
	
	//for (int i = 0; i < scn.getsizes()[2]; i++)
	//{
	//	for (int j = 0; j < scn.getsizes()[3]; j++)
	//	{
	//		scn.start_shader(0.5, glm::vec2(i,j));
	//	}
	//}


	
	return 0;
}