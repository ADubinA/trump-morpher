#version 330 

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float time;
uniform vec4 coeffs;
uniform vec4 src_lines[20];
uniform vec4 dst_lines[20];
uniform ivec4 sizes; 

in vec2 x;	// vertex texture
in vec3 position1;	// vertex position

vec2 perp(vec2 v);
float calcDist(vec2 P, vec2 Q, float v, float u, vec2 x);
vec2 calcPix(vec2 x, vec4 normalize[16], vec4 inter_line[16]);

void main()
{
	vec2 P1, Q1, P2, Q2;
	vec2 x1, x2;
	float t = time;
	// t = 1.0;
	// t = 0.2;
	// normalize vectors, this should be done in the vertex shader, not here
	vec4 normalize_src[16], normalize_dst[16], inter_line[16];
	
	for (int i = 0; i<16; i++) 
	{
		// normalize source
		P1.x = (src_lines[i].x)/sizes.z; 
		P1.y = 1-(src_lines[i].y)/sizes.w; 
		Q1.x = (src_lines[i].z)/sizes.z; 
		Q1.y = 1-(src_lines[i].w)/sizes.w; 
		normalize_src[i] = vec4(P1,Q1);
		
		// normalize destination
		P2.x = (dst_lines[i].x)/sizes.z;
		P2.y = 1-(dst_lines[i].y)/sizes.w;
		Q2.x = (dst_lines[i].z)/sizes.z;
		Q2.y = 1-(dst_lines[i].w)/sizes.w;
		normalize_dst[i] = vec4(P2,Q2);
		
		inter_line[i] = t * normalize_dst[i] + (1 - t) * normalize_src[i];
	}
	
	x1 = calcPix(x, normalize_src, inter_line);
	x2 = calcPix(x, normalize_dst, inter_line);
	
	gl_FragColor =  t*texture(texture1,x2)+ (1 - t) *  texture(texture2,x1) ;

}

// finds the perpendicular vector to v
vec2 perp(vec2 v) 
{
   return vec2(-v.y,v.x);
}

// calculates the dist (length of pixel from line)
float calcDist(vec2 P, vec2 Q, float v, float u, vec2 x)
{
	if (u>0 && u<1)
		return abs(v);
	else if(u<0)
		return length(P-x);
	return length(Q-x);
		
}

vec2 calcPix(vec2 x, vec4 normalize[16], vec4 inter_line[16])
	{
		vec2 P1, Q1, P2, Q2;
		vec2 x2, Di;
		vec2 DSUM = vec2(0.0,0.0);
		float weightsum = 0.0;
		float u, v, dist, weight;
		
		for (int i = 0; i<16 ; i++)
		{				
			P1 = normalize[i].xy;
			Q1 = normalize[i].zw;
			P2 = inter_line[i].xy;
			Q2 = inter_line[i].zw;
						
			// calculate u
			u = dot(x-P1,Q1-P1)/pow(length(Q1-P1),2);
			
			// calculate v
			v = dot(x-P1,perp(Q1-P1))/length(Q1-P1);

			// calculate x2 ( for next image)
			x2 = P2 + u*(Q2-P2) + v*perp(Q2-P2)/length(Q2-P2);
			
			// calculate displacment D_i = x2-x1
			Di = x2-x;
			
			// calculate dist
			dist = calcDist(P1, Q1, v, u, x);
			
			// calculate weight 
			weight = pow(((pow(length(P1-Q1),coeffs.w))/(coeffs.y + dist)),coeffs.z); 
			
			// DSUM += Di*weight 
			DSUM += Di * weight;
			
			// weightsum += weight
			weightsum += weight;
		}
		return x + (DSUM / weightsum);

	}



