#include "texture.hpp"
#include <os/os.hpp>
#include <graphics/imagetype.hpp>
os::graphics::Texture::Texture(uint8_t *dataStart,
							   uint64_t width,
							   uint64_t height,
							   bool upsideDown) : m_data(dataStart),
												  m_width(width),
												  m_height(height),
												  m_upsideDown(upsideDown)
{
}

os::graphics::Texture *os::graphics::Texture::fromGob(uint8_t *dataStart, uint8_t *dataEnd)
{
	if (dataStart[0] != 'G' || dataStart[1] != 'O' || dataStart[2] != 'B' || dataStart[3] != 'I')
	{
		return nullptr;
	}

	GobImageHeader *header = reinterpret_cast<GobImageHeader *>(dataStart);
	return new Texture(dataStart + sizeof(GobImageHeader) + 1, header->width, header->height, false);
}

os::graphics::Texture *os::graphics::Texture::fromBmp(uint8_t *dataStart, uint8_t *dataEnd)
{
	if (dataStart[0] != 'B' || dataStart[1] != 'M')
	{
		return nullptr;
	}

	uint32_t *size = reinterpret_cast<uint32_t *>(dataStart + 2);

	uint32_t *offset = reinterpret_cast<uint32_t *>(dataStart + 10);

	uint32_t *dibHeaderSize = reinterpret_cast<uint32_t *>(dataStart + 14);

	if (*dibHeaderSize != 56)
	{
		panic_with_message("Unsupported bitmap format");
	}

	BitmapHeaderV3Info *header = reinterpret_cast<BitmapHeaderV3Info *>(dataStart + 14);

	// bitmap images are stored upsidedown because of os/2 nonsense
	// so we have to invert them in memory
	// alternatively we could just add a flag that tells if texture is inverted and draws it as such if so

	return new Texture(dataStart + *offset, header->width, header->height, true);
}
