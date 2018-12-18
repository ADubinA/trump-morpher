#version 330 

uniform vec2 center;
uniform int iter;
uniform int time;
in vec3 position1;

void main()
{
	
	//vec3 s = vec3((sin(6.28*float(time%iter)/float(iter))+vec3(1.0,1.0,1.0))/2.0);
	vec3 s = vec3((sin(12.52*float(time%iter)/float(iter)*position1)+vec3(1.0,1.0,1.0))/2.0);
	gl_FragColor = vec4(clamp(((position1+vec3(1.0,1.0,1.0))/2.0)*s,0.0,1.0),1);//vec4(0.1,0.9,1.0,1.0);
}