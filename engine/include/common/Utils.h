#ifndef COMMON_UTILS
#define COMMON_UTILS

#include "Trace.h"

#include "glm/glm.hpp"

#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>

#ifdef _MSC_VER

typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef __int16 int16_t;
typedef unsigned __int16 uint16_t;

#else
#include <stdint.h>
#endif

namespace Utils
{
	namespace
	{
		struct bmp_magic {
			unsigned char magic[2];
		};

		struct bmp_header {
			uint32_t filesz;
			uint16_t creator1;
			uint16_t creator2;
			uint32_t offset;
		};

		struct bmp_info_header {
			uint32_t header_sz;
			int32_t width;
			int32_t height;
			uint16_t nplanes;
			uint16_t bitspp;
			uint32_t compress_type;
			uint32_t bmp_bytesz;
			int32_t hres;
			int32_t vres;
			uint32_t ncolors;
			uint32_t nimpcolors;
			uint32_t redmask;
			uint32_t greenmask;
			uint32_t bluemask;
			uint32_t alphamask;
			unsigned char filler[68];
		};

		enum bmp_compress_type {
			BI_RGB,
			BI_RLE8,
			BI_RLE4,
			BI_BITFIELDS,
			BI_JPEG,
			BI_PNG
		};
	}

	inline std::string readfile(const std::string filename)
	{
		std::ifstream ifs(filename.c_str());
		if (!ifs.is_open()) {
			Trace::warning("Could not open: %s\n", filename.c_str());
			return "";
		}
		std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

		return str;
	}

	inline void print_vector(const char * msg, const glm::vec3 & vec)
	{
		Trace::info("%s: (%f, %f, %f)\n", msg, vec.x, vec.y, vec.z);
	}

	// TODO: do not inline these
	class Random
	{
	public:
		Random()
		{
			srand((unsigned int)time(NULL));
		}

		Random(unsigned int seed)
		{
			srand(seed);
		}

		inline double rand01()
		{
			return double(rand()) / double(RAND_MAX);
		}

		inline double rand11()
		{
			return (double(rand()) / double(RAND_MAX)) * 2.0 - 1.0;
		}
	};

	inline bool read_texture(const char * filename, int* width, int* height, unsigned char** data)
	{
		// create binary input stream
		std::basic_ifstream<unsigned char> ifs(filename, std::ios::in | std::ios::binary);

		// header structures
		bmp_magic magic;
		bmp_header header;
		bmp_info_header info_header;

		// read signature
		ifs.read((unsigned char*)&magic, sizeof(bmp_magic));

		// check that the signature is 'BM'
		if (magic.magic[0] != 0x42 || magic.magic[1] != 0x4D) {
			Trace::error("Could not load texture (wrong format!)\n");
			return false;
		}

		// read bitmap file header, nothing really interesting here..
		ifs.read((unsigned char*)&header, sizeof(bmp_header));

		// read header size
		ifs.read((unsigned char*)&info_header, sizeof(uint32_t));
		ifs.seekg((int)ifs.tellg() - sizeof(uint32_t));

		// read DIB header (assume BITMAPINFOHEADER header for simplicity).
		ifs.read((unsigned char*)&info_header, info_header.header_sz);

		// only support BITMAPINFOHEADER
		if (info_header.header_sz < 40) {
			Trace::error("Could not load texture (unsupported header format!)");
			return false;
		}

		// only support uncompressed images
		if (info_header.compress_type != 0 && info_header.compress_type != 3) {
			Trace::error("Could not load texture (compression is unsupported!)");
			return false;
		}

		// only support default palette size
		if (info_header.ncolors != 0) {
			Trace::error("Could not load texture (found color table, not supported!)");
			return false;
		}

		// only support 24 or 32 bits per pixel
		if (info_header.bitspp != 24 && info_header.bitspp != 32) {
			Trace::error("Could not load texture (only support 24 or 32 bpp!)");
			return false;
		}

		const unsigned bytespp = info_header.bitspp / 8;

		// move to the start of the pixel array
		ifs.seekg(header.offset);

		*width = info_header.width;
		*height = info_header.height;

		// allocate memory for the pixels
		*data = new unsigned char[info_header.width * info_header.height * bytespp];

		// flip BMP format y-coordinate
		for (int y = info_header.height - 1; y >= 0; y--) {
		//for (int y = 0; y < info_header.width; y++) {
			for (int x = 0; x < info_header.width; x++) {
				// read 3 bytes (24bpp) from the bitmap
				unsigned char * bgr = new unsigned char[bytespp];
				ifs.read(bgr, bytespp);

				// flip BGR to RGB
				//for (unsigned b = 0; b < bytespp; b++)
				(*data)[y*info_header.width*bytespp + x*bytespp + 0] = bgr[0];
				(*data)[y*info_header.width*bytespp + x*bytespp + 1] = bgr[1];
				(*data)[y*info_header.width*bytespp + x*bytespp + 2] = bgr[2];
				(*data)[y*info_header.width*bytespp + x*bytespp + 3] = bgr[3];
				//(*data)[y*info_header.width*3 + x*3 + 1] = bgr[1];
				//(*data)[y*info_header.width*3 + x*3 + 2] = bgr[0];
				delete [] bgr;
			}
			// after each pixel row, check if we should expect a pad to fill rows to be dividable by 4
			int pad = info_header.width % 4;
			if (pad > 0) {
				// skip the pad
				ifs.seekg((int)ifs.tellg() + pad);
			}
		}

		return true;
	}
}

#endif /* COMMON_UTILS */
