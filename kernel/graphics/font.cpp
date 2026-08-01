#include "font.hpp"
extern "C" uint8_t _binary_assets_vga_font_dos_gob_start;
extern "C" uint8_t _binary_assets_vga_font_dos_gob_end;

os::graphics::FontManager::FontManager() : m_default(
											   Font(Texture::fromGob(&_binary_assets_vga_font_dos_gob_start, &_binary_assets_vga_font_dos_gob_end), 32, 12, 21))
{
}

os::graphics::Font::Font(Texture const *bitmapFontTexture,
						 size_t columnCount,
						 uint64_t charWidth,
						 uint64_t charHeight) : m_sourceTexture(bitmapFontTexture),
												m_columns(columnCount),
												m_charWidth(charWidth),
												m_charHeight(charHeight)

{
}

std::geometry::Rect os::graphics::Font::getGlyphRect(char ch) const
{
	// we do this because the fonts aren't going to have info for these special characters
	// the characters beyond the 127 of the ascii we will use magic ig
	ch -= ' ';
	uint64_t y = (uint64_t)ch / m_columns;
	uint64_t x = (uint64_t)ch % m_columns;
	return std::geometry::Rect(x * m_charWidth, y * m_charHeight, m_charWidth, m_charHeight);
}
