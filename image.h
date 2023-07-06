#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cmath>
#include <string.h>
#pragma once


#pragma pack(push, 1)
struct BitmapFileHeader {
    char signature[2];
    std::uint32_t fileSize;
    std::uint32_t reserved;
    std::uint32_t dataOffset;
};

struct BitmapInfoHeader {
    std::uint32_t headerSize;
    std::int32_t width;
    std::int32_t height;
    std::uint16_t colorPlanes;
    std::uint16_t bitsPerPixel;
    std::uint32_t compression;
    std::uint32_t imageSize;
    std::int32_t horizontalResolution;
    std::int32_t verticalResolution;
    std::uint32_t colorsUsed;
    std::uint32_t importantColors;
};
#pragma pack(pop)

class Image {
	private:
	char 				signature[3];
	BitmapFileHeader    fileHeader;
	BitmapInfoHeader	infoHeader;
	std::ifstream 		file;						// File contain image data
	std::string 		label;						// Label for image for classification
	std::string 		path;						// Path to the image location
	int 				imageID;					// Image ID
	std::vector<std::vector<int>>	pixels;			// Container for pixel array, include R, G, B value.
	public:
	Image() {}																	// Default constructor
	Image(std::string _path, int _imageID);	  									// Constructor with image path and load image}
	~Image();
	void loadImageData();														// Load pixel data into the array;
	void getHeaderInfo();
	void getImageInfo();
	void hexdump();
	void testing();
	int convertGrayScale(int pixel);

	private:
	class UnsupportedFormat : public std::exception {
		friend class Image;
		std::string msg;
		public:
		UnsupportedFormat() {
			msg = "Unsupported Bitmap Image Format\n";
		}
		const char* what() const throw() {
			return msg.c_str();
		}
	};
	
	class FileNotFound : public std::exception {
		friend class Image;
		std::string msg;
		public:
		FileNotFound() {
			msg = "File not found\n";
		}
		const char* what() const throw() {
			return msg.c_str();
		}
	};
};
