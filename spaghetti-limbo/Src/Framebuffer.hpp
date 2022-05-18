#pragma once

#include <glad/glad.h>
#include <memory>
#include "Texture.hpp"

class Framebuffer
{
public:
	// creates a framebuffer with 1 color attachment
	static Framebuffer* FramebufferC1(int ColorSlot, int DepthStencilSlot, int Width, int Height);

	~Framebuffer();
	void Bind() const;
	void Unbind() const;

	inline const auto& GetColor() const { return m_Color.get(); }
	inline const auto& GetID() const { return m_ID; }

private:
	Framebuffer(TEXTURE_DESC& Color, TEXTURE_DESC& Depth);

	GLuint						m_ID;
	std::unique_ptr<Texture>	m_Color;
	std::unique_ptr<Texture>	m_DepthStencil;
};