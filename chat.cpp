#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cmath>

#pragma pack(push, 1)
struct BitmapFileHeader {
    char fileType[2];
    int fileSize;
    short reserved1;
    short reserved2;
    int dataOffset;
};

struct BitmapInfoHeader {
    int headerSize;
    int width;
    int height;
    short planes;
    short bitsPerPixel;
    int compression;
    int imageSize;
    int xPixelsPerMeter;
    int yPixelsPerMeter;
    int totalColors;
    int importantColors;
};
#pragma pack(pop)

void printHexDump(const std::vector<unsigned char>& data) {
    for (size_t i = 0; i < data.size(); i++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data[i]) << " ";
        if ((i + 1) % 16 == 0) {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}

std::vector<unsigned char> readBitmapData(std::ifstream& fileStream, int dataSize) {
    std::vector<unsigned char> data(dataSize);
    fileStream.read(reinterpret_cast<char*>(data.data()), dataSize);
    return data;
}

std::vector<std::vector<int>> loadPixels(const std::vector<unsigned char>& imageData, int width, int height, int bytesPerPixel) {
    std::vector<std::vector<int>> pixels(height, std::vector<int>(width));

    int padding = (4 - ((width * bytesPerPixel) % 4)) % 4;

    int dataIndex = 0;
    for (int i = height - 1; i >= 0; i--) {
        for (int j = 0; j < width; j++) {
            int offset = j * bytesPerPixel;
            int blue = imageData[dataIndex + offset];
            int green = imageData[dataIndex + offset + 1];
            int red = imageData[dataIndex + offset + 2];

            pixels[i][j] = (red << 16) | (green << 8) | blue;
        }

        dataIndex += (width * bytesPerPixel) + padding;
    }

    return pixels;
}

int main() {
    std::string filename = "test.bmp"; // Replace with the path to your bitmap file

    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
        return 1;
    }

    // Read and interpret the bitmap file header (14 bytes)
    BitmapFileHeader fileHeader;
    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(BitmapFileHeader));
    std::cout << "Bitmap File Header:" << std::endl;
    std::cout << "File Type: " << fileHeader.fileType[0] << fileHeader.fileType[1] << std::endl;
    std::cout << "File Size: " << fileHeader.fileSize << " bytes" << std::endl;
    std::cout << "Reserved: " << fileHeader.reserved1 << ", " << fileHeader.reserved2 << std::endl;
    std::cout << "Data Offset: " << fileHeader.dataOffset << " bytes" << std::endl;

    // Read and interpret the bitmap information header (40 bytes)
    BitmapInfoHeader infoHeader;
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(BitmapInfoHeader));
    std::cout << "Bitmap Information Header:" << std::endl;
    std::cout << "Header Size: " << infoHeader.headerSize << " bytes" << std::endl;
    std::cout << "Image Width: " << infoHeader.width << " pixels" << std::endl;
    std::cout << "Image Height: " << infoHeader.height << " pixels" << std::endl;
    std::cout << "Bits per Pixel: " << infoHeader.bitsPerPixel << std::endl;
    std::cout << "Image Size: " << infoHeader.imageSize << " bytes" << std::endl;

    // Skip to the image data
    file.seekg(fileHeader.dataOffset, std::ios::beg);

    // Read the image data
    int bytesPerPixel = infoHeader.bitsPerPixel / 8;
    std::vector<unsigned char> imageData = readBitmapData(file, infoHeader.imageSize);

    // Load the pixels
    std::vector<std::vector<int>> pixels = loadPixels(imageData, infoHeader.width, infoHeader.height, bytesPerPixel);

    // Print the image data hexdump
    std::cout << "Image Data Hexdump:" << std::endl;
    for (const auto& row : pixels) {
        for (const auto& pixel : row) {
            if(pixel) std::cout << "@";
            else std::cout << " ";
        }
        std::cout << std::endl;
    }

    file.close();
    return 0;
}
