#pragma once

#include <stdint.h>
#include <stddef.h>
namespace os::graphics
{
	class Texture
	{
	public:
		/// @brief Load image data from two pointers
		/// @param dataStart
		/// @param dataEnd
		explicit Texture(uint8_t *dataStart, uint64_t width, uint64_t height, bool upsideDown = false);

		/// @brief Create new instance of an image object stored in the Bitmap format. Currently bitmap is assumed to be in 32bit ARGB format
		/// @param dataStart Pointer to the start of the memory block that contains the bitmap
		/// @param dataEnd Pointer to the end of the memory block that contains the bitmap
		/// @return
		static Texture *fromBmp(uint8_t *dataStart, uint8_t *dataEnd);
		uint8_t *getData() const { return m_data; }

		uint64_t getWidth() const { return m_width; }
		uint64_t getHeight() const { return m_height; }

		bool isUpsideDown() const { return m_upsideDown; }

	private:
		uint8_t *m_data;
		uint64_t m_width;
		uint64_t m_height;
		bool m_upsideDown;
	};
}