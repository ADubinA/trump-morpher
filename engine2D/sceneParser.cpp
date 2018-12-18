#include "SceneParser.h"
#include <iostream>

//parse line to vec4
glm::vec4 Scene::parseVec4(const std::string& line) 
{
    unsigned int tokenLength = line.length();
    const char* tokenString = line.c_str();
    
    unsigned int vertIndexStart = 2;
    
    while(vertIndexStart < tokenLength)
    {
        if(tokenString[vertIndexStart] != ' ')
            break;
        vertIndexStart++;
    }
    
    unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float x = ParseFloatValue(line, vertIndexStart, vertIndexEnd);
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float y = ParseFloatValue(line, vertIndexStart, vertIndexEnd);
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float z = ParseFloatValue(line, vertIndexStart, vertIndexEnd);
    
	vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float w = ParseFloatValue(line, vertIndexStart, vertIndexEnd);

    return glm::vec4(x,y,z,w);

    //glm::vec3(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()))
}

void Scene::loadtoShader(Shader &shader)
{
			shader.set_uniform4v(3,1,&coeffs);			
			shader.set_uniform4v(4,src_lines.size(),&src_lines[0]);
			shader.set_uniform4v(5,dst_lines.size(),&dst_lines[0]);
			shader.set_uniform4vi(6,sizes);
				//objects[1].x +=0.001;
}

void Scene::PrintScene()
{
	std::cout<<"coeffs: a = "<<coeffs[1]<<" b= "<<coeffs[2]<<" p = "<<coeffs[3]<<std::endl; 
	std::cout<<"picture size "<<sizes[2]<<"X"<<sizes[3]<<std::endl;
	std::cout<<"lines: "<<std::endl; 
	for (int i = 0; i < sizes[0]; i++)
	{
		std::cout<<"source: ("<<src_lines[i].x<<" , "<<src_lines[i].y<<") , ("<<src_lines[i].z<<" , "<<src_lines[i].w<<")"<<std::endl;
		std::cout<<"distination: ("<<dst_lines[i].x<<" , "<<dst_lines[i].y<<") , ("<<dst_lines[i].z<<" , "<<dst_lines[i].w<<")"<<std::endl;
	}		
}

//parsing file and save width and hight of the image
Scene::Scene(const std::string& fileName,int width,int height)
{
	std::ifstream file;
    file.open((fileName).c_str());
	
    std::string line;
    if(file.is_open())
    {
        while(file.good())
        {
            getline(file, line);
        
            unsigned int lineLength = line.length();
            
            if(lineLength < 2)
                continue;
            
            const char* lineCStr = line.c_str();
            
            switch(lineCStr[0])
            {
                case 'c':
					coeffs = parseVec4(line);
				break;
				case 's':
					src_lines.push_back( parseVec4(line));
				break;
				case 'd':
					dst_lines.push_back( parseVec4(line));
				break;
			}
		}
		sizes =  glm::ivec4(src_lines.size(),dst_lines.size(),width,height);
	}
	else
	{
		char buf[100];
		//std::cout<<"can not open file!"<<std::endl;
		strerror_s(buf,errno);
		std::cerr << "Error: " << buf; 
		sizes = glm::ivec4(0,0,0,0);
	}
	
}


//shader functions
glm::vec2 Scene::perp(glm::vec2 v)
{
	return glm::vec2(-v.y, v.x);
}

float Scene::calcDist(glm::vec2 P, glm::vec2 Q, float v, float u, glm::vec2 x)
{
	if (u>0 && u<1)
		return abs(v);
	else if (u<0)
		return glm::length(P - x);
	return glm::length(Q - x);
}

glm::vec2 Scene::calcPix(glm::vec2 x, glm::vec4 normalize[16], glm::vec4 inter_line[16])
{
	glm::vec2 P1, Q1, P2, Q2;
	glm::vec2 x2, Di;
	glm::vec2 DSUM = glm::vec2(0.0, 0.0);
	float weightsum = 0.0;
	float u, v, dist, weight;

	for (int i = 0; i<2; i++)
	{
		P1.x = normalize[i].x;
		P1.y = normalize[i].y;
		Q1.x = normalize[i].z;
		Q1.y = normalize[i].w;
		P2.x = inter_line[i].x;
		P2.y = inter_line[i].y;
		Q2.x = inter_line[i].z;
		Q2.y = inter_line[i].w;

		// calculate u
		u = (glm::dot((x - P1), (Q1 - P1))) / (pow((glm::length(Q1 - P1)), 2));

		// calculate v
		v = (glm::dot((x - P1), perp(Q1 - P1))) / (glm::length(Q1 - P1));

		// calculate x2 ( for next image)
		x2 = P2 + u * (Q2 - P2) + ((v*perp(Q2 - P2)) / (glm::length(Q2 - P2)));

		// calculate displacment D_i = x2-x1
		Di = x2 - x;

		// calculate dist
		dist = calcDist(P1, Q1, v, u, x);

		// calculate weight                  p          a                 b
		weight = pow(((pow(glm::length(P1 - Q1), coeffs.w)) / (coeffs.y + dist)), coeffs.z);

		// DSUM += Di*weight 
		DSUM += Di * weight;

		// weightsum += weight
		weightsum += weight;
	}
	return x + (DSUM / weightsum);
}

void Scene::start_shader(float time, glm::vec2 x)
{
	glm::vec2 P1, Q1, P2, Q2;
	glm::vec2 x1, x2;
	float t = time;
	// t = 1.0;
	//t = 0.5;
	// normalize vectors, this should be done in the vertex shader, not here
	glm::vec4 normalize_src[16], normalize_dst[16], inter_line[16];

	for (int i = 0; i<2; i++)
	{
		// normalize source
		P1.x = (src_lines[i].x) / sizes.z;
		P1.y = 1 - (src_lines[i].y) / sizes.w;
		Q1.x = (src_lines[i].z) / sizes.z;
		Q1.y = 1 - (src_lines[i].w) / sizes.w;
		normalize_src[i] = glm::vec4(P1, Q1);

		// normalize destination
		P2.x = (dst_lines[i].x) / sizes.z;
		P2.y = 1 - (dst_lines[i].y) / sizes.w;
		Q2.x = (dst_lines[i].z) / sizes.z;
		Q2.y = 1 - (dst_lines[i].w) / sizes.w;
		normalize_dst[i] = glm::vec4(P2, Q2);

		inter_line[i] = t * normalize_src[i] + (1 - t) * normalize_dst[i];
	}

	x1 = calcPix(x, normalize_src, inter_line);
	x2 = calcPix(x, normalize_dst, inter_line);

	std::cout << "x1: " << x1.x << ", " << x1.y <<"T! "<< t<< std::endl;
	getchar();
	//system("pause");
}

glm::ivec4 Scene::getsizes()
{
	return sizes;
}
