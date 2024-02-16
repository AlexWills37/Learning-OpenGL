#version 330 core

// We are outputting color
// Layout location defaults to 0 anyway
layout(location = 0) out vec4 color;

// Uniform variable - way to get data from CPU at draw call to the GPU in shader
uniform vec4 u_Color;

void main()
{
	color = u_Color;
}