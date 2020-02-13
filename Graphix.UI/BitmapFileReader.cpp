#include "BitmapFileReader.h"

std::vector<DirectX::XMFLOAT4> BitmapFileReader::ReadFile(std::string path)
{
	int OFFSET_BMP_HEADER = 0;
	int OFFSET_PIXEL_DATA_OFFSET = 10;
	int OFFSET_DIB_HEADER = 14;
	int OFFSET_DIB_HEADER_WIDTH = OFFSET_DIB_HEADER + 4;
	int OFFSET_DIB_HEADER_HEIGHT = OFFSET_DIB_HEADER + 8;
	int OFFSET_DIB_HEADER_BPP = OFFSET_DIB_HEADER + 14;
	int OFFSET_DIB_HEADER_COMPRESSION = OFFSET_DIB_HEADER + 16;
	int OFFSET_DIB_HEADER_IMAGESIZE = OFFSET_DIB_HEADER + 36;

	std::vector<DirectX::XMFLOAT4> pixels;

	FILE* handle;
	errno_t err = fopen_s(&handle, path.c_str(), "rb");
	fseek(handle, 0, SEEK_END);
	long filesize = ftell(handle);
	fseek(handle, OFFSET_BMP_HEADER, 0);

	// Check header
	std::vector<unsigned char> headerType;
	headerType.resize(2);
	fread_s(&headerType[0], 2, sizeof(unsigned char), 2, handle);
	if (headerType[0] != 'B' || headerType[1] != 'M')
	{
		// Unsupported bitmap type
		return pixels;
	}

	// Find out where the pixel data is
	std::vector<unsigned char> pixelDataOffsetRaw;
	pixelDataOffsetRaw.resize(4);
	unsigned int pixelDataOffset;
	fseek(handle, OFFSET_PIXEL_DATA_OFFSET, 0);
	fread_s(&pixelDataOffsetRaw[0], 4, sizeof(unsigned char), 4, handle);
	pixelDataOffset = *(unsigned int *)&pixelDataOffsetRaw[0];

	// Get various bitmap details
	std::vector<unsigned char> widthRaw;
	std::vector<unsigned char> heightRaw;
	std::vector<unsigned char> bppRaw;
	std::vector<unsigned char> compressionRaw;
	std::vector<unsigned char> imageSizeRaw;
	widthRaw.resize(4);
	heightRaw.resize(4);
	bppRaw.resize(4);
	compressionRaw.resize(4);
	imageSizeRaw.resize(4);
	fseek(handle, OFFSET_DIB_HEADER_WIDTH, 0);
	fread_s(&widthRaw[0], 4, sizeof(unsigned char), 4, handle);
	fseek(handle, OFFSET_DIB_HEADER_HEIGHT, 0);
	fread_s(&heightRaw[0], 4, sizeof(unsigned char), 4, handle);
	fseek(handle, OFFSET_DIB_HEADER_BPP, 0);
	fread_s(&bppRaw[0], 4, sizeof(unsigned char), 4, handle);
	fseek(handle, OFFSET_DIB_HEADER_COMPRESSION, 0);
	fread_s(&compressionRaw[0], 4, sizeof(unsigned char), 4, handle);
	fseek(handle, OFFSET_DIB_HEADER_IMAGESIZE, 0);
	fread_s(&imageSizeRaw[0], 4, sizeof(unsigned char), 4, handle);

	int width = *(int*)&widthRaw[0];
	int height = *(int*)&heightRaw[0];
	int bpp = *(int*)&bppRaw[0];
	int compression = *(int*)&compressionRaw[0];
	int imageSize = *(int*)&imageSizeRaw[0];

	if (bpp % 3 > 0)
	{
		// Unsupported pixel type
		return pixels;
	}

	if (compression != 0)
	{
		// We don't support compression -- must be BI_RGB
		return pixels;
	}

	// Calculate various dimensions
	int pixelBytes = bpp / 8;
	long rowsizeBytes = width * pixelBytes;
	if (rowsizeBytes % 4 > 0)
	{
		rowsizeBytes += (rowsizeBytes % 4);
	}
	long pixelCount = width * height;
	long byteCount = rowsizeBytes * height;

	// Prepare pixel data arrays
	std::vector<unsigned char> pixelDataRaw;
	pixelDataRaw.resize(byteCount);
	pixels.resize(pixelCount);

	// Read the pixel data
	fseek(handle, pixelDataOffset, 0);
	fread_s(&pixelDataRaw[0], byteCount, sizeof(unsigned char), byteCount, handle);

	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{
			int b = pixelDataRaw[w * pixelBytes + h * rowsizeBytes];
			int g = pixelDataRaw[w * pixelBytes + h * rowsizeBytes + 1];
			int r = pixelDataRaw[w * pixelBytes + h * rowsizeBytes + 2];
			DirectX::XMFLOAT4* pixel = new DirectX::XMFLOAT4(r, g, b, 255);
			pixels[w + width * h ] = *pixel;
		}
	}

	return pixels;
}
