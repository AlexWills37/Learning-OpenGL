#version 330 core

// We are outputting color
// Layout location defaults to 0 anyway
layout(location = 0) out vec4 color;

// Input Varying variable (interpolated from vertex outputs in vertex shader)
in vec2 v_TexCoord;


// Uniform variable - way to get data from CPU at draw call to the GPU in shader
uniform vec4 u_Color;
uniform sampler2D u_Texture;	// Sampler2D is an index to the texture slot we are using

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = texColor;
}