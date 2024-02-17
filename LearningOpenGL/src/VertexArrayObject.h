#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

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