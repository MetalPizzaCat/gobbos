#include "font.hpp"
extern "C" uint8_t _binary_assets_vga_font_dos_bmp_start;
extern "C" uint8_t _binary_assets_vga_font_dos_bmp_end;

os::font::FontManager::FontManager() : m_default(Font(
										   &_binary_assets_vga_font_dos_bmp_start,
										   &_binary_assets_vga_font_dos_bmp_end - &_binary_assets_vga_font_dos_bmp_start, 32, 12, 21))
{
}

os::font::Font::Font(uint8_t *data,
					 size_t dataSize,
					 size_t columnCount,
					 uint32_t charWidth,
					 uint32_t charHeight) : m_dataStart(data),
											m_size(dataSize),
											m_columns(columnCount),
											m_cellWidth(charWidth),
											m_cellHeight(charHeight)
{
}