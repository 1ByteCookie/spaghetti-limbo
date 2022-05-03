#include "Framebuffer.hpp"

#include <iostream>

Framebuffer* Framebuffer::FramebufferC1(int ColorSlot, int DepthStencilSlot, int Width, int Height)
{
	TEXTURE_DESC cDescriptor = { };
	cDescriptor.Slot			= ColorSlot;
	cDescriptor.Target			= GL_TEXTURE_2D;
	cDescriptor.InternalFormat	= GL_RGB;
	cDescriptor.Format			= GL_RGB;
	cDescriptor.BufferType		= GL_UNSIGNED_BYTE;
	cDescriptor.Width			= Width;
	cDescriptor.Height			= Height;

	TEXTURE_DESC dsDescriptor = { };
	dsDescriptor.Slot			= DepthStencilSlot;
	dsDescriptor.Target			= GL_TEXTURE_2D;
	dsDescriptor.InternalFormat	= GL_DEPTH24_STENCIL8;
	dsDescriptor.Format			= GL_DEPTH_STENCIL;
	dsDescriptor.BufferType		= GL_UNSIGNED_INT_24_8;
	dsDescriptor.Width			= Width;
	dsDescriptor.Height			= Height;

	Framebuffer* This = new Framebuffer(cDescriptor, dsDescriptor);

	return This;
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &m_ID);
}

void Framebuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
}

void Framebuffer::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::Framebuffer(TEXTURE_DESC& Color, TEXTURE_DESC& DepthStencil)
	:	m_Color( Texture::FramebufferAttachment(Color) )
	,	m_DepthStencil( Texture::FramebufferAttachment(DepthStencil) )
{
	glGenFramebuffers(1, &m_ID);
	Bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Color->GetID(), 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthStencil->GetID(), 0);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << this << ": Framebuffer creation failed!" << std::endl;

	m_Color->Bind();
	m_DepthStencil->Bind();

	Unbind();
}
