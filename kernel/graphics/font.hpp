#pragma once

#include <stddef.h>
#include <stdint.h>
namespace os::font
{
	/// @brief Bitfont representation. Consists of a text
	class Font
	{
	public:
		explicit Font(uint8_t *data, size_t dataSize, size_t columnCount, uint32_t charWidth, uint32_t charHeight);

	private:
		size_t m_columns;
		uint32_t m_cellWidth;
		uint32_t m_cellHeight;
		uint8_t *m_dataStart;
		size_t m_size;
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

		Font const &getDefaultFont() const;

	private:
		Font m_default;
	};
}