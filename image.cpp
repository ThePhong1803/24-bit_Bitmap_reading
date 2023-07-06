#include "image.h"

Image::Image(std::string _path, int _imageID) : path(_path), imageID(_imageID) {
	this -> loadImageData();
	memset(this -> signature, '\0', 3);
	memcpy(this -> signature, this -> fileHeader.signature, 2);
}

Image::~Image(){
	delete [] pixels;
}

void Image::getHeaderInfo() {
	std::cout << "File signature: " << this -> signature 				<< std::endl;
	std::cout << "File size: " 		<< this -> fileHeader.fileSize		<< std::endl;
	std::cout << "File reserve: " 	<< this -> fileHeader.reserved 		<< std::endl;
	std::cout << "Data offset: " 	<< this -> fileHeader.dataOffset 	<< std::endl;
}

void Image::getImageInfo() {
	std::cout << "Image width: " 		<< this -> infoHeader.width 		 << std::endl;
	std::cout << "Image height: " 		<< this -> infoHeader.height		 << std::endl;
	std::cout << "Bytes per pixel: " 	<< this -> infoHeader.bitsPerPixel 	 << std::endl;
	std::cout << "Image size: " 		<< this -> infoHeader.imageSize 	 << std::endl;
}

int Image::calculatePadding(int width, int bytesPerPixel) {
	int bytesPerRow = width * bytesPerPixel;
	int padding = 0;
	if (bytesPerRow % 4 != 0) {
		padding = 4 - (bytesPerRow % 4);
	}
	return padding;
}

void Image::hexdump(){
	for(int i = 0; i < this -> infoHeader.width; i++){
		for(int j = 0; j < this -> infoHeader.height; j++){
			std::cout << std::hex << pixels[i * this -> infoHeader.width + j].group() << " ";
		}
		std::cout << std::endl;
	}
}

void Image::testing(){
	const std::string table = " .:-=+*#%@";
	for(int i = 0; i < this -> infoHeader.height; i++){
		for(int j = 0; j < this -> infoHeader.width; j++){
			int idx = pixels[i * this -> infoHeader.width + j].toGreyScale();
			idx = std::round((float(idx) / 255) * 9);
			std::cout << table[idx] << table[idx];
		}
		std::cout << std::endl;
	}
}

void Image::loadImageData() {
	(this -> file).open(this -> path, std::ios::in | std::ios::binary);
	try{
		if(!file.is_open()) throw Image::FileNotFound();
		else {
				/* Reading file header section */
				this -> file.seekg(0, std::ios::beg);
				this -> file.read(reinterpret_cast<char*>(&this -> fileHeader), 14);
					
				/* Reading file infoheader section */
				this -> file.read(reinterpret_cast<char*>(&this -> infoHeader), 40);
				
				//image type check
				if(this -> infoHeader.bitsPerPixel / 8 != 3) throw Image::UnsupportedFormat();
				if(this -> infoHeader.colorPlanes != 1) 	 throw Image::UnsupportedFormat();
				
				//loading image pixels dat
				this -> file.seekg(this -> fileHeader.dataOffset, std::ios::beg);
				this -> pixels = new Pixel[this -> infoHeader.width * this -> infoHeader.height];

				int padding = calculatePadding(this -> infoHeader.width, this -> infoHeader.bitsPerPixel);
				int bytesPerPixel = this -> infoHeader.bitsPerPixel / 8;

				char * rowBuffer = new char[this ->  infoHeader.width * bytesPerPixel + padding];
				for (int i = this -> infoHeader.height - 1; i >= 0; i--) {
					this -> file.read(rowBuffer, this ->  infoHeader.width * bytesPerPixel + padding);
					for (int j = 0; j < this -> infoHeader.width; j++) {
						int offset = j * bytesPerPixel;
						int blue = rowBuffer[offset] & 0xFF;
						int green = rowBuffer[offset + 1] & 0xFF;
						int red = rowBuffer[offset + 2] & 0xFF;
						pixels[i * this -> infoHeader.width + j].R = (std::uint8_t)red;
						pixels[i * this -> infoHeader.width + j].G = (std::uint8_t)green;
						pixels[i * this -> infoHeader.width + j].B = (std::uint8_t)blue;
						std::cout << red << " " << green << " " << blue << std::endl;
					}
				}
				delete [] rowBuffer;
				// Close the file when finished
				file.close();
			}
	} catch(std::exception &e) {
		std::cout << e.what() << std::endl;
	}
}