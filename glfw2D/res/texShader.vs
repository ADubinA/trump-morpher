// Vertex shader for a point-light (local) source, with computation
// done in the fragment shader.
#version 330 


attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;

out vec2 uv;
out vec3 position1;

void main()
{
	//vec3 tnorm =  normalize(gl_NormalMatrix * gl_Normal);
	position1 = position;
	 uv = texCoord;
	//gl_FrontColor = gl_Color;
	gl_Position = vec4(position,1);
	
}
