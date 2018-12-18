#version 330 

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float time;
uniform vec4 coeffs;
uniform vec4 src_lines[20];
uniform vec4 dst_lines[20];
uniform ivec4 sizes; 

in vec2 uv;
in vec3 position1;



void main()
{
	//vec3 s = vec3((sin(6.28*float(time%iter)/float(iter))+vec3(1.0,1.0,1.0))/2.0);
	//vec3 s = vec3((sin(12.52*float(time%iter)/float(iter)*position1)+vec3(1.0,1.0,1.0))/2.0);
	//gl_FragColor = texture(texture1,uv);//vec4(0.1,0.9,1.0,1.0);
	gl_FragColor = time *texture(texture1,uv ) + (1 - time)*texture(texture2,uv);
	
}

