#ifndef SHADER_INCLUDED_H
#define SHADER_INCLUDED_H

#include <string>
//#include "debugTimer.h"
#include "glm\glm.hpp"

class Shader
{
public:
	Shader(const std::string& fileName);

	void Bind();
	void Update(float t);
	void Shader::set_uniform1f(int indx, float val);
	void Shader::set_uniform1i(int indx, int val);
	void Shader::set_uniform2f(int indx,float x,float y);
	void set_uniform4v(int indx,int size, glm::vec4 *val);
	void set_uniform4vi(int indx, glm::ivec4 val);

	virtual ~Shader();
protected:
private:
	static const unsigned int NUM_SHADERS = 2;
	static const unsigned int NUM_UNIFORMS = 7;
	void operator=(const Shader& shader) {}
	Shader(const Shader& shader) {}

	std::string LoadShader(const std::string& fileName);
	void CheckShaderError(unsigned int shader, unsigned int flag, bool isProgram, const std::string& errorMessage);
	unsigned int CreateShader(const std::string& text, unsigned int type);
	
	unsigned int m_program;
	unsigned int m_shaders[NUM_SHADERS];
	unsigned int m_uniforms[NUM_UNIFORMS];
	//DebugTimer t;
};

#endif
