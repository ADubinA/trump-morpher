#version 330

uniform vec2 center;
uniform int time;
uniform int iter;

in vec3 position1;
void main() {
	vec2 z, c;
	float scale = 2.2	;
	
	vec4 color1 = vec4(0.0,0.0,0.0,0.0);
	vec4 color2 = vec4(1.0,1.0,0.0,0.0);
	
	c.x = 1.3333 * ((position1.x + 1.0)/2.0 - 0.25) * scale - center.x;
	c.y = ((position1.y+1.0)/2.0 - 0.5) * scale - center.y;

	int i;
	z = c;
	for(i=0; i<iter; i++) {
		float x = (z.x * z.x - z.y * z.y) + c.x;
		float y = (z.y * z.x + z.x * z.y) + c.y;

		if((x * x + y * y) >4) break;
		z.x = x;
		z.y = y;
	}
   // gl_FragColor = vec4((position1.xy+vec2(1.0,1.0))/2.0,0.0,1.0);
//	gl_FragColor = vec4( color2*(float(i)) / 100.0);
	if(i==iter)
		discard;
	else
		//gl_FragColor = color2*vec4(z.x*z.x/(z.x*z.x+z.y*z.y),z.y*z.y/(z.x*z.x+z.y*z.y),0,0);
		gl_FragColor = mix(color1,color2,(z.x*z.x+z.y*z.y)/4.0 );
}
