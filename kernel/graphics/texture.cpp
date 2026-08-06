#include "texture.hpp"
#include <os/os.hpp>
#include <math.hpp>
#include <graphics/imagetype.hpp>
os::graphics::Texture::Texture(uint8_t *dataStart,
							   uint64_t width,
							   uint64_t height,
							   bool upsideDown,
							   bool uniqueData) : m_data(dataStart),
												  m_width(width),
												  m_height(height),
												  m_upsideDown(upsideDown),
												  m_holdingUniqueData(uniqueData)
{
}

os::graphics::Texture::Texture(uint64_t width, uint64_t height) : m_upsideDown(false), m_holdingUniqueData(true), m_width(width), m_height(height)
{
	m_data = new uint8_t[width * height * 4];
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

void os::graphics::Texture::update(uint8_t *data)
{
	if (!m_holdingUniqueData)
	{
		uint8_t *tmp = m_data;
		m_data = new uint8_t[getDataSize()];
		memcpy(m_data, data, getDataSize());
		return;
	}
	memcpy(m_data, data, getDataSize());
}

void os::graphics::Texture::resize(std::geometry::Vec2i newSize)
{
	uint8_t *newData = new uint8_t[newSize.x * newSize.y * 4];

	for (uint64_t x = 0; x < newSize.x; x++)
	{
		for (uint64_t y = 0; y < newSize.y; y++)
		{
			uint64_t srcX = min<uint64_t>((double)x / (double)newSize.x * (double)m_width, m_width - 1);
			uint64_t srcY = min<uint64_t>((double)y / (double)newSize.y * (double)m_height, m_height - 1);

			for (int i = 0; i < 4; i++)
			{
				newData[i + x * 4 + y * 4 * newSize.x] = m_data[i + srcX * 4 + srcY * 4 * m_width];
			}
		}
	}

	if (m_holdingUniqueData)
	{
		delete m_data;
	}

	m_data = newData;
	m_width = newSize.x;
	m_height = newSize.y;
}

os::graphics::Texture::~Texture()
{
	if (m_holdingUniqueData)
	{
		delete[] m_data;
	}
}