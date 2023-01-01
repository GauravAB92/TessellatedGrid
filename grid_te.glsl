#version 410 core

layout(quads, fractional_odd_spacing, ccw) in;


uniform mat4 mvp;



void main(void)
{
	vec4 p1 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);
	vec4 p2 = mix(gl_in[2].gl_Position, gl_in[3].gl_Position, gl_TessCoord.x);

	vec4 pos = mix(p2, p1, gl_TessCoord.y);


	gl_Position = (mvp) * pos;
}