#include "text.hpp"
#include <math.hpp>
#include <graphics/graphics.hpp>
#include <memory.hpp>

os::graphics::Text::Text(Font const *font) : m_font(font) {}

void os::graphics::Text::setText(std::string const &text)
{
	uint64_t width = 0;
	uint64_t height = 0;
	uint64_t currWidth = 0;

	for (size_t i = 0; i < text.len(); i++)
	{
		// we need to calculate the full size of the final texture in a way where copying all symbols can work
		currWidth++;
		if (text[i] == '\n')
		{
			height++;
			width = max(width, currWidth);
			currWidth = 0;
		}
	}
	width = max(width, currWidth);
	height++;

	uint8_t *data = new uint8_t[width * height * m_font->getCharacterWidth() * m_font->getCharacterHeight() * 4];
	memset(data, 0, width * height * m_font->getCharacterWidth() * m_font->getCharacterHeight() * 4);
	uint64_t x = 0;
	uint64_t y = 0;
	const uint64_t pitch = m_font->getCharacterWidth() * 4;
	// now we can do bunch of texture copying
	// we have to do two loops because we need to calculate the texture size to copy into first

	// memset(data, 0xff, width * height * m_font->getCharacterWidth() * m_font->getCharacterHeight() * 4);
	// Rect glyphRect = m_font->getGlyphRect('!');
	// for (uint64_t j = 0; j < m_font->getCharacterHeight(); j++)
	// {
	// 	// mempattern(0xffff0000, data + j * width * m_font->getCharacterWidth() * 4, glyphRect.getWidth());
	// 	//		memcpy(data + j * width * m_font->getCharacterWidth() * 4, m_font->getFontTexture() + j * m_font->getFontTexture()->getWidth() * 4 + glyphRect.x * 4, glyphRect.getWidth() * 4);
	// 	memcpy(data + j * width * m_font->getCharacterWidth() * 4, m_font->getFontTexture()->getData() + j * m_font->getFontTexture()->getWidth() * 4 + glyphRect.x * 4, glyphRect.getWidth() * 4);
	// }
	for (size_t i = 0; i < text.len(); i++)
	{
		for (uint64_t j = 0; j < m_font->getCharacterHeight(); j++)
		{
			Rect glyphRect = m_font->getGlyphRect(text[i]);
			memcpy(
				data + x * m_font->getCharacterWidth() * 4 + ((y + j) * width * m_font->getCharacterWidth() * 4),
				m_font->getFontTexture()->getData() + (j + glyphRect.y) * m_font->getFontTexture()->getWidth() * 4 + glyphRect.x * 4,
				glyphRect.getWidth() * 4);
		}
		x++;
		if (text[i] == '\n')
		{
			x = 0;
			y++;
		}
	}

	if (m_resultTexture != nullptr)
	{
		// alternatively we would just resize the texture itself but this brings up an interesting question
		// how to differentiate between just resizing memory and resizing to scale?
		// would just resizing alter only memory?
		delete m_resultTexture;
	}
	// for now i choose just remaking it since Text is not meant to return texture either way
	// we mark data as unique since we just hand it over to the texture
	m_resultTexture = new Texture(data, width * m_font->getCharacterWidth(), height * m_font->getCharacterHeight(), false, true);
}

void os::graphics::Text::draw()
{
	if (m_resultTexture == nullptr)
	{
		return;
	}
	Graphics::getInstance().drawTexture(m_resultTexture, m_position, nullptr);
}

os::graphics::Text::~Text()
{
	if (m_resultTexture != nullptr)
	{
		delete m_resultTexture;
	}
}
