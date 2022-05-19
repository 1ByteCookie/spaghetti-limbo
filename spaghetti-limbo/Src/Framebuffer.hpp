#pragma once

#include <glad/glad.h>
#include <memory>
#include "Texture.hpp"

class Framebuffer
{
public:
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

	~Framebuffer();
	void Bind() const;
	void Unbind() const;

	inline const auto& GetColor() const { return m_Color.get(); }
	inline const auto& GetID() const { return m_ID; }

private:
	Framebuffer(Texture* Color, Texture* DepthStencil);

	GLuint						m_ID;
	std::unique_ptr<Texture>	m_Color;
	std::unique_ptr<Texture>	m_DepthStencil;
};