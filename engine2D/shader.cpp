#define GLEW_STATIC
#include "GL\glew.h"
#include "shader.h"
#include <iostream>
#include <fstream>

Shader::Shader(const std::string& fileName)
{
	// create shader and compilation
	m_program = glCreateProgram(); 
	m_shaders[0] = CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER);
	
	m_shaders[1] = CreateShader(LoadShader(fileName + ".glsl"), GL_FRAGMENT_SHADER);

	for(unsigned int i = 0; i < NUM_SHADERS; i++)
		glAttachShader(m_program, m_shaders[i]);
	//declaration of attributes (in arguments in vertex shader)
	glBindAttribLocation(m_program, 0, "position");
	glBindAttribLocation(m_program, 1, "texCoord");
	glBindAttribLocation(m_program, 2, "normal");
	//linking shader
	glLinkProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Error linking shader program");

	glValidateProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Invalid shader program");
	//decleration of unifore values 
	m_uniforms[0] = glGetUniformLocation(m_program, "time");
	m_uniforms[1] = glGetUniformLocation(m_program, "texture1");
	m_uniforms[2] = glGetUniformLocation(m_program, "texture2");
	m_uniforms[3] = glGetUniformLocation(m_program, "coeffs");
	m_uniforms[4] = glGetUniformLocation(m_program, "src_lines");
	m_uniforms[5] = glGetUniformLocation(m_program, "dst_lines");
	m_uniforms[6] = glGetUniformLocation(m_program, "sizes");
	//t.Start();
}

Shader::~Shader()
{
	for(unsigned int i = 0; i < NUM_SHADERS; i++)
    {
        glDetachShader(m_program, m_shaders[i]);
        glDeleteShader(m_shaders[i]);
    }

	glDeleteProgram(m_program);
}

void Shader::Bind()
{
	glUseProgram(m_program);
}

//update uniforms value in shader
void Shader::Update(float t)
{
	set_uniform1f(0,t);
	set_uniform1i(1,0);
	set_uniform1i(2,1);
}

//load shader from file
std::string Shader::LoadShader(const std::string& fileName)
{
    std::ifstream file;
    file.open((fileName).c_str());

    std::string output;
    std::string line;

    if(file.is_open())
    {
        while(file.good())
        {
            getline(file, line);
			output.append(line + "\n");
        }
    }
    else
    {
		std::cerr << "Unable to load shader: " << fileName << std::endl;
    }

		
	Update(0);
    return output;
}

void Shader::CheckShaderError(unsigned int shader, unsigned int flag, bool isProgram, const std::string& errorMessage)
{
    GLint success = 0;
    GLchar error[1024] = { 0 };

    if(isProgram)
        glGetProgramiv(shader, flag, &success);
    else
        glGetShaderiv(shader, flag, &success);

    if(success == GL_FALSE)
    {
        if(isProgram)
            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        else
            glGetShaderInfoLog(shader, sizeof(error), NULL, error);

        std::cerr << errorMessage << ": '" << error << "'" << std::endl;
    }
}
//shader compilation 
unsigned int Shader::CreateShader(const std::string& text, unsigned int type)
{
    unsigned int shader = glCreateShader(type);

    if(shader == 0)
		std::cerr << "Error compiling shader type " << type << std::endl;

    const GLchar* p[1];
    p[0] = text.c_str();
    GLint lengths[1];
    lengths[0] = text.length();

    glShaderSource(shader, 1, p, lengths);
    glCompileShader(shader);

    CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!");

    return shader;
}

void Shader::set_uniform1f(int indx, float val) {
	if(indx < 	NUM_UNIFORMS)
    {
		glUniform1f(m_uniforms[indx], val);
	}
}

void Shader::set_uniform1i(int indx, int val) {
	if(indx < 	NUM_UNIFORMS)
    {
		glUniform1i(m_uniforms[indx], val);
	}
}

void Shader::set_uniform2f(int indx, float x,float y)
{
	if(indx < 	NUM_UNIFORMS)
    {
		glUniform2f(m_uniforms[indx],x,y);
	}
}

void Shader::set_uniform4v(int indx,int size, glm::vec4 *val)
{
	if(indx < 	NUM_UNIFORMS)
    {
		glUniform4fv(m_uniforms[indx],size, &val[0][0]);
	}
}

void Shader::set_uniform4vi(int indx, glm::ivec4 val)
{
	if(indx < 	NUM_UNIFORMS)
    {
		glUniform4i(m_uniforms[indx],val.x,val.y,val.z,val.w);
	}
}



