#pragma once
#include <stdint.h>

namespace os::graphics
{
	/**
	 * @brief Header info for most common(used ny GIMP and aseprite) header i found. Order taken from win32
	 *
	 */
	struct BitmapHeaderV3Info
	{
		uint32_t size;
		int32_t width;
		int32_t height;
		uint16_t planes;
		uint16_t bitCount;
		uint32_t compression;
		uint32_t sizeImage;
		int32_t xPelsPerMeter;
		int32_t yPelsPerMeter;
		uint32_t clrUsed;
		uint32_t clrImportant;
	};

	/**
	 * @brief Header for the custom image format i made that is just a cut down bitmap. Stores info in a way that works better for limine framebuffer. And doesn't store the image upside down
	 *
	 */
	struct GobImageHeader
	{
		char signature[4];
		uint64_t size;
		uint32_t width;
		uint32_t height;
	};

} // namespace os::graphics
