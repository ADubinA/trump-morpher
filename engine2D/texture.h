#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture
{
public:
	Texture(int width,int hieght,int num);
	Texture(const std::string& fileName);

	void Bind(unsigned int slot = 0);
	void AfterSwap();
	inline int GetWidth(){return width;}
	inline int GetHeight(){return height;}
	virtual ~Texture();
protected:
private:
	Texture(const Texture& texture) {}
	void operator=(const Texture& texture) {}
	int width,height;
	unsigned int m_texture;
	int texNum;
};

#endif
