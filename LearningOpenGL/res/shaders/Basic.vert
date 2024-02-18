#version 330 core

// input attribute at index 0
// input attribute is a vec4 (will be cast) called position
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;	// v - varying

uniform mat4 u_MVP;	// Model View Projection matrix

void main()
{
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
}