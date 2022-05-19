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


Texture* Texture::FramebufferAttachment(TEXTURE_DESC& Descriptor)
{
	Texture* This = new Texture(Descriptor, nullptr);

	return This;
}


Texture* Texture::MultisampleAttachment(TEXTURE_DESC& Descriptor, uint32_t Samples)
{
	Texture* This = new Texture(Descriptor, Samples);
	
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
	:	m_Descriptor(Descriptor)
	,	m_Channels(0)
{
	glGenTextures(1, &m_ID);
	
	glActiveTexture(GL_TEXTURE0 + m_Descriptor.Slot);
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

	glGenerateMipmap(Descriptor.Target);
	glTexParameteri(Descriptor.Target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(Descriptor.Target, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(Descriptor.Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(Descriptor.Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(m_Descriptor.Target, 0);
}


Texture::Texture(TEXTURE_DESC& Descriptor, uint32_t Samples)
	: m_Descriptor(Descriptor)
	, m_Channels(0)
{
	glGenTextures(1, &m_ID);
	

	glActiveTexture(GL_TEXTURE0 + m_Descriptor.Slot);
	glBindTexture(m_Descriptor.Target, m_ID);
	

	glTexImage2DMultisample(m_Descriptor.Target,
							Samples,
							m_Descriptor.InternalFormat,
							m_Descriptor.Width,
							m_Descriptor.Height,
							GL_TRUE);

	glBindTexture(m_Descriptor.Target, 0);
}
