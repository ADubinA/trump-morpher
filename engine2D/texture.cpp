#define GLEW_STATIC
#include <GL\glew.h>
#include "texture.h"
#include "stb_image.h"
#include <iostream>

//allocating empty texture
Texture::Texture(int width, int height,int num)
{
	glGenTextures(1, &m_texture);
  //  	glActiveTexture(GL_TEXTURE0 + num);
   glBindTexture(GL_TEXTURE_2D, m_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0); //note GL_RED internal format, to save memory.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);      //GL_REPEAT specified so that CGOL is simulated on a torus.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   // glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+num , m_texture, 0);
   
	GLenum DrawBuffers[2] = {GL_COLOR_ATTACHMENT0+num,GL_BACK};
	glDrawBuffers(2, DrawBuffers);
	
	texNum = num;
	this->width = width;
	this->height = height;
}

//read texturee from file
Texture::Texture(const std::string& fileName)
{
	int numComponents;
	stbi_set_flip_vertically_on_load(1);
    unsigned char* data = stbi_load((fileName).c_str(), &width, &height, &numComponents, 4);

    if(data == NULL)
		std::cerr << "Unable to load texture: " << fileName << std::endl;
        
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    //texture properties definitions 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);       
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
	if(data)
		stbi_image_free(data);
	texNum = 0;
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_texture);
}

//bind shader in slot on GPU's texture table  
void Texture::Bind(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_texture);	
}

void Texture::AfterSwap()
{
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 , m_texture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}