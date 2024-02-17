#pragma once
/*
 * IndexBuffer.h
 * Manages an OpenGL Index Buffer.
 * The index buffer specifies how the vertices (from the currentlty bound VAO)
 * are grouped into primitives.
 */
class IndexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count;	// Number of indices
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;
	inline unsigned int GetCount() const { return m_Count; }
};