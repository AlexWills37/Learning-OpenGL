#include "VertexArrayObject.h"
#include "GLErrorManager.h"

VertexArrayObject::VertexArrayObject()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
	
}

VertexArrayObject::~VertexArrayObject()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArrayObject::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();		// Bind this array so that the buffer is addded to THIS
	vb.Bind();	// Bind the vertex buffer to load it into memory

	// Create a VertexAttribPointer for each attribute in the layout
	const auto& attributes = layout.GetAttributes();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < attributes.size(); i++) {
		const auto& attribute = attributes[i];

		// Now use the layout to make attribute pointers to store in the current VAO
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, attribute.count, attribute.type, attribute.normalized, 
			layout.GetStride(), (const void*)offset));

		offset += attribute.count * VertexBufferAttribute::GetSizeOfType(attribute.type);
	}

	// A note on the glVertexAttribPointer method
	// index: This attribute will be index 0
	// size: The attribute has 2 components (x, y)
	// type: It will be defined as floats
	// normalized: We do not want to normalize the values
	// stride: to get from one vertex to the next, this is the size to jump (2 floats)
	// pointer: now that we are in the vertex, we offset 0 bytes to get to the beginning of this attribute
}

void VertexArrayObject::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArrayObject::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
