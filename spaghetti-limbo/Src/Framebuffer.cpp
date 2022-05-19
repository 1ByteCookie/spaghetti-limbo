#include "Framebuffer.hpp"

#include <iostream>

Framebuffer* Framebuffer::FramebufferC1(uint32_t ColorSlot,
										uint32_t DepthStencilSlot,
										uint32_t Width,
										uint32_t Height)
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


	Texture* Color			= Texture::FramebufferAttachment(cDescriptor);
	Texture* DepthStencil	= Texture::FramebufferAttachment(dsDescriptor);
	
	Framebuffer* This = new Framebuffer(Color, DepthStencil);

	return This;
}


Framebuffer* Framebuffer::FBOMultisample(uint32_t ColorSlot,
										 uint32_t DepthStencilSlot,
										 uint32_t Width,
										 uint32_t Height,
										 uint32_t Samples)
{
	TEXTURE_DESC cDescriptor = { };
	cDescriptor.Slot			= ColorSlot;
	cDescriptor.Target			= GL_TEXTURE_2D_MULTISAMPLE;
	cDescriptor.InternalFormat	= GL_RGB;
	cDescriptor.Width			= Width;
	cDescriptor.Height			= Height;

	TEXTURE_DESC dsDescriptor = { };
	dsDescriptor.Slot			= DepthStencilSlot;
	dsDescriptor.Target			= GL_TEXTURE_2D_MULTISAMPLE;
	dsDescriptor.InternalFormat	= GL_DEPTH24_STENCIL8;
	dsDescriptor.Format			= GL_DEPTH_STENCIL;
	dsDescriptor.BufferType		= GL_UNSIGNED_INT_24_8;
	dsDescriptor.Width			= Width;
	dsDescriptor.Height			= Height;


	Texture* Color			= Texture::MultisampleAttachment(cDescriptor, Samples);
	Texture* DepthStencil	= Texture::MultisampleAttachment(dsDescriptor, Samples);

	Framebuffer* This = new Framebuffer(Color, DepthStencil);

	return This;
}


Framebuffer* Framebuffer::FBOIntermediate(	uint32_t ColorSlot,
											uint32_t Width,
											uint32_t Height)
{
	TEXTURE_DESC cDescriptor = { };
	cDescriptor.Slot			= ColorSlot;
	cDescriptor.Target			= GL_TEXTURE_2D;
	cDescriptor.InternalFormat	= GL_RGB;
	cDescriptor.Format			= GL_RGB;
	cDescriptor.BufferType		= GL_UNSIGNED_BYTE;
	cDescriptor.Width			= Width;
	cDescriptor.Height			= Height;
	Texture* Color = Texture::FramebufferAttachment(cDescriptor);

	Framebuffer* This = new Framebuffer(Color);
	return This;
}


void Framebuffer::Blit(Framebuffer* Source, Framebuffer* Destination)
{
	Source->Bind(READ);
	if (Destination) { Destination->Bind(WRITE); }
	else { glBindFramebuffer(WRITE, 0); }


	glBlitFramebuffer(	0,
						0,
						Source->m_Color->Properties().Width,
						Source->m_Color->Properties().Height,
						0,
						0,
						Source->m_Color->Properties().Width,
						Source->m_Color->Properties().Height,
						GL_COLOR_BUFFER_BIT,
						GL_LINEAR);

	Source->Unbind();
	
	if (Destination) { Destination->Unbind(); }
}


void Framebuffer::ClearState()
{
	glBindFramebuffer(READ_WRITE, 0);
	glBindFramebuffer(READ, 0);
	glBindFramebuffer(WRITE, 0);
}


Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &m_ID);
}


void Framebuffer::Bind(Bindflag Flag)
{
	glBindFramebuffer(Flag, m_ID);
	m_State = Flag;
}


void Framebuffer::Unbind()
{
	glBindFramebuffer(m_State, 0);
	m_State = UNBOUND;
}


Framebuffer::Framebuffer(Texture* Color, Texture* DepthStencil)
	:	m_Color( Color )
	,	m_DepthStencil( DepthStencil )
	,	m_State(READ_WRITE)
{
	glGenFramebuffers(1, &m_ID);
	
	
	Bind(READ_WRITE);

	glFramebufferTexture2D( GL_FRAMEBUFFER,
							GL_COLOR_ATTACHMENT0,
							m_Color->Properties().Target,
							m_Color->GetID(),
							0);
	
	glFramebufferTexture2D( GL_FRAMEBUFFER,
							GL_DEPTH_STENCIL_ATTACHMENT,
							m_DepthStencil->Properties().Target,
							m_DepthStencil->GetID(),
							0);


	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << this << ": Framebuffer creation failed!" << std::endl;

	m_Color->Bind();
	m_DepthStencil->Bind();

	Unbind();
}

Framebuffer::Framebuffer(Texture* Color)
	: m_Color(Color)
	, m_DepthStencil(nullptr)
	, m_State(READ_WRITE)
{
	glGenFramebuffers(1, &m_ID);


	Bind(READ_WRITE);

	glFramebufferTexture2D(	GL_FRAMEBUFFER,
							GL_COLOR_ATTACHMENT0,
							m_Color->Properties().Target,
							m_Color->GetID(),
							0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << this << ": Framebuffer creation failed!" << std::endl;

	m_Color->Bind();

	Unbind();
}