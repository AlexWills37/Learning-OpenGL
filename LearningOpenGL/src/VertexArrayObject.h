#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

/*
 * VertexArrayObject.h
 * Handles the VertexArrayObject in OpenGL, which contains
 * an Index Buffer and vertex information (from attribute pointers, connecting the vertex buffer).
 * 
 * Usage:
 *		Use AddBuffer() to pair a vertex buffer with a vertex buffer layout, defining the verticies.
 */

class VertexArrayObject
{
private:
	unsigned int m_RendererID;
public:
	VertexArrayObject();
	~VertexArrayObject();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;
};