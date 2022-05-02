#pragma once

#include <glad/glad.h>
#include <string>

struct TEXTURE_DESC
{
	int				Slot;
	GLuint			Target;
	GLuint			InternalFormat;
	GLuint			Format;
	GLuint			BufferType;
	std::string		Path;
	
	int				Width				= 0;
	int				Height				= 0;
};

class Texture
{
public:
	static Texture* LoadFromFile(TEXTURE_DESC& Descriptor, int FlipImage);

	const TEXTURE_DESC& Properties() { return m_Descriptor; }
	const GLuint GetID() { return m_ID; }
	~Texture();

	void Bind() const;
	void Unbind() const;

private:
	Texture(TEXTURE_DESC& Descriptor, unsigned char* Buffer);

	GLuint			m_ID;
	TEXTURE_DESC	m_Descriptor;
	int				m_Channels;
};