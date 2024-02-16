#version 330 core

// input attribute at index 0
// input attribute is a vec4 (will be cast) called position
layout(location = 0) in vec4 position;

void main()
{
	gl_Position = position;
}