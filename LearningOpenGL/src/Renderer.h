#pragma once

#include "VertexArrayObject.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer
{
private:

public:
	void Clear() const;
	void Draw(const VertexArrayObject& va, const IndexBuffer& ib, const Shader& shader) const;
};