#include "Texture.hpp"

#include "stb_image/stb_image.h"

Texture* Texture::LoadFromFile(TEXTURE_DESC& Descriptor, int FlipImage)
{
	int Channels;

	stbi_set_flip_vertically_on_load(FlipImage);
	unsigned char* Buffer = stbi_load(Descriptor.Path.c_str(), &Descriptor.Width, &Descriptor.Height, &Channels, 0);

	Texture* This = new Texture(Descriptor, Buffer);
	This->m_Channels = Channels;


	if (Buffer) { stbi_image_free(Buffer); }
	return This;
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_ID);
}

void Texture::Bind() const
{
	glActiveTexture(GL_TEXTURE0 + m_Descriptor.Slot);
	glBindTexture(m_Descriptor.Target, m_ID);
}

void Texture::Unbind() const
{
	glActiveTexture(GL_TEXTURE0 + m_Descriptor.Slot);
	glBindTexture(m_Descriptor.Target, 0);
}

Texture::Texture(TEXTURE_DESC& Descriptor, unsigned char* Buffer)
	:m_Descriptor(Descriptor), m_Channels(0)
{
	glActiveTexture(GL_TEXTURE0 + m_Descriptor.Slot);
	glGenTextures(1, &m_ID);
	glBindTexture(m_Descriptor.Target, m_ID);
	glTexImage2D(m_Descriptor.Target,
				 0,
				 m_Descriptor.InternalFormat,
				 m_Descriptor.Width,
				 m_Descriptor.Height,
				 0,
				 m_Descriptor.Format,
				 m_Descriptor.BufferType,
				 Buffer);

	glGenerateMipmap(m_Descriptor.Target);

	glTexParameteri(m_Descriptor.Target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(m_Descriptor.Target, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(m_Descriptor.Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(m_Descriptor.Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(m_Descriptor.Target, 0);
}
