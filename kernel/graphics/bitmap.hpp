#pragma once
#include <stdint.h>

namespace os::graphics
{
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
} // namespace os::graphics
