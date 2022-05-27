#pragma once

#include <glad/glad.h>
#include <memory>
#include "Texture.hpp"

class Framebuffer
{
public:

	enum Bindflag
	{
		READ_WRITE	= GL_FRAMEBUFFER,
		READ		= GL_READ_FRAMEBUFFER,
		WRITE		= GL_DRAW_FRAMEBUFFER,

		UNBOUND = 0
	};
	
	// creates a framebuffer with 1 color attachment
	static Framebuffer* FramebufferC1(	uint32_t ColorSlot,
										uint32_t DepthStencilSlot,
										uint32_t Width,
										uint32_t Height);
	
	// creates a multiesample framebuffer
	static Framebuffer* FBOMultisample( uint32_t ColorSlot,
										uint32_t DepthStencilSlot,
										uint32_t Width,
										uint32_t Height,
										uint32_t Samples);

	// creates a framebuffer with only a color attachment
	static Framebuffer* FBOIntermediate(uint32_t ColorSlot,
										uint32_t Width,
										uint32_t Height);

	// creates a framebuffer with only a depth attachment
	static Framebuffer* DepthMap(uint32_t DepthSlot,
								 uint32_t Width,
								 uint32_t Height);

	static void Blit(Framebuffer* Source, Framebuffer* Destination);
	static void ClearState();

	void Bind(Bindflag Flag);
	void Unbind();

	inline const auto& GetColor() const { return m_Color.get(); }
	inline const auto& GetDepth() const { return m_Depth.get(); }
	inline const auto& GetID() const { return m_ID; }


	~Framebuffer();

private:

	Framebuffer(Texture* Color, Texture* DepthStencil);
	Framebuffer(Texture* Color, Texture* Depth, Texture* Stencil);
	
	void Attach(Texture* texture, GLuint AttachmentType);

	GLuint						m_ID;

	std::unique_ptr<Texture>	m_Color;
	std::unique_ptr<Texture>	m_DepthStencil;
	std::unique_ptr<Texture>	m_Depth;
	std::unique_ptr<Texture>	m_Stencil;

	Bindflag					m_State;
};