#pragma once

#include <stdint.h>
#include <stddef.h>

namespace os::graphics
{
	class Texture
	{
	public:
		
		explicit Texture(uint8_t *dataStart, uint64_t width, uint64_t height, bool upsideDown = false, bool uniqueData = false);

		/// @brief Create empty texture(filled with 0x00000000) of a given size. Will always have unique newly allocated data
		/// @param size Size of the texture
		explicit Texture(uint64_t width, uint64_t height);

		/// @brief Create new instance of an image object stored in the Bitmap format. Currently bitmap is assumed to be in 32bit ARGB format
		/// @param dataStart Pointer to the start of the memory block that contains the bitmap
		/// @param dataEnd Pointer to the end of the memory block that contains the bitmap
		/// @return Pointer to the newly created texture
		static Texture *fromBmp(uint8_t *dataStart, uint8_t *dataEnd);

		/// @brief Create new instance of an image object stored in the Gob format. Even more simplified alternative to bitmap
		/// @param dataStart Pointer to the start of the memory block that contains the image data
		/// @param dataEnd Pointer to the end of the memory block that contains the image data
		/// @return
		static Texture *fromGob(uint8_t *dataStart, uint8_t *dataEnd);

		/// @brief Get pointer to the raw data stored in the image. Note: This value can change if texture is modified as a copy of data will be created so it shouldn't be hold onto
		/// @return Pointer to the inner data
		uint8_t *getData() const { return m_data; }


		uint64_t getWidth() const { return m_width; }
		uint64_t getHeight() const { return m_height; }

		bool isUpsideDown() const { return m_upsideDown; }

		/// @brief Update current pixel data with new values. Data will be copied into the memory so sizes must match.
		/// @param data
		void update(uint8_t *data);

		size_t getDataSize() const { return m_width * m_height * 4; }

		~Texture();

	private:
		uint8_t *m_data;
		uint64_t m_width;
		uint64_t m_height;
		bool m_upsideDown;
		/// @brief  Whether a unique copy of the data was created for purposes of modification
		bool m_holdingUniqueData;
	};
}