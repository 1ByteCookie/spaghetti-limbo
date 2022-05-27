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

	Framebuffer* This = new Framebuffer(Color, nullptr, nullptr);
	return This;
}


Framebuffer* Framebuffer::DepthMap(uint32_t DepthSlot,
							 uint32_t Width,
							 uint32_t Height)
{
	TEXTURE_DESC dDescriptor = { };
	dDescriptor.Slot			= DepthSlot;
	dDescriptor.Target			= GL_TEXTURE_2D;
	dDescriptor.InternalFormat	= GL_DEPTH_COMPONENT;
	dDescriptor.Format			= GL_DEPTH_COMPONENT;
	dDescriptor.BufferType		= GL_FLOAT;
	dDescriptor.Width			= Width;
	dDescriptor.Height			= Height;
	Texture* Depth = Texture::FramebufferAttachment(dDescriptor);

	Framebuffer* This = new Framebuffer(nullptr, Depth, nullptr);
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
	if (m_Color.get())
		m_Color->Bind();

	if (m_Depth.get())
		m_Depth->Bind();

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
	,	m_Depth(nullptr)
	,	m_Stencil(nullptr)
	,	m_State(READ_WRITE)
{
	glGenFramebuffers(1, &m_ID);
	
	
	Bind(READ_WRITE);
	{
		if (Color)
			Attach(Color, GL_COLOR_ATTACHMENT0);

		if (DepthStencil)
			Attach(DepthStencil, GL_DEPTH_STENCIL_ATTACHMENT);
	}


	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << this << ": Framebuffer creation failed!" << std::endl;

	m_Color->Bind();
	m_DepthStencil->Bind();

	Unbind();
}

Framebuffer::Framebuffer(Texture* Color, Texture* Depth, Texture* Stencil)
	:	m_Color( Color )
	,	m_DepthStencil(nullptr)
	,	m_Depth( Depth )
	,	m_Stencil( Stencil )
	,	m_State( READ_WRITE )
{
	glGenFramebuffers(1, &m_ID);


	Bind(READ_WRITE);
	{
		if (Color)
			Attach(Color, GL_COLOR_ATTACHMENT0);

		if (Depth)
			Attach(Depth, GL_DEPTH_ATTACHMENT);

		if (Stencil)
			Attach(Stencil, GL_STENCIL_ATTACHMENT);
	}

	if (!Color)
	{
		glReadBuffer(GL_NONE);
		glDrawBuffer(GL_NONE);
	}


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << this << ": Framebuffer creation failed!" << std::endl;


	Unbind();
}

void Framebuffer::Attach(Texture* texture, GLuint AttachmentType)
{
	glFramebufferTexture2D( READ_WRITE,
							AttachmentType,
							texture->Properties().Target,
							texture->GetID(),
							0);
}