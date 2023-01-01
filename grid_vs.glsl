#version 410 core


void main(void)
{
	const vec4 vertices[] = vec4[](
		vec4(-0.5, 0.0, -0.5, 1.0),
		vec4(0.5, 0.0, -0.5, 1.0),
		vec4(-0.5, 0.0, 0.5, 1.0),
		vec4(0.5, 0.0, 0.5, 1.0)
		);

	//grab the indices
	int x = gl_InstanceID & 63; // mod 64
	int y = gl_InstanceID >> 6; // div 64

	gl_Position = vertices[gl_VertexID] + vec4(float(x - 32), 0.0, float(y - 32), 0.0);

}