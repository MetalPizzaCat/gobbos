#pragma once

#include <stddef.h>
#include <stdint.h>
#include <graphics/texture.hpp>
#include <geometry/rect.hpp>
namespace os::graphics
{
	/// @brief Bitfont representation. Consists of a text arranged as sequence of images in a bitmap
	class Font
	{
	public:
		explicit Font(Texture const *bitmapFontTexture, size_t columnCount, uint64_t charWidth, uint64_t charHeight);

		Texture const *getFontTexture() const { return m_sourceTexture; }

		uint64_t getCharacterWidth() const { return m_charWidth; }
		uint64_t getCharacterHeight() const { return m_charHeight; }

		/// @brief Get glyph position and size on the bitfont texture
		/// @param ch Character to get glyph of
		/// @return Rectangle for the segment of the texture
		std::geometry::Rect getGlyphRect(char ch) const;

	private:
		Texture const *m_sourceTexture;
		size_t m_columns;
		uint64_t m_charWidth;
		uint64_t m_charHeight;
	};

	class FontManager
	{
	public:
		explicit FontManager();
		static FontManager &getInstance()
		{
			static FontManager f;
			return f;
		}

		Font const &getDefaultFont() const { return m_default; }

	private:
		Font m_default;
	};
}