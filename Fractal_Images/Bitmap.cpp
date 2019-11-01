#include <fstream>
using std::ofstream;
using std::ios;
#include "Bitmap.h"
#include "BitmapInfoHeader.h"
#include "BitmapFileHeader.h"

Bitmap::Bitmap() : Bitmap(0, 0) {}

Bitmap::Bitmap(int width, int height) :	_width(width), _height(height), _pPixels(new uint8_t[width * height * 3]{ }) {}

Bitmap::~Bitmap()
{
	delete[] _pPixels;
}

bool Bitmap::write(string filename)
{
	if (_pPixels == nullptr)
		return false;

	BitmapFileHeader fileHeader;
	BitmapInfoHeader infoHeader;

	fileHeader.dataOffset = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);
	fileHeader.fileSize = fileHeader.dataOffset + _width * _height * 3;

	infoHeader.width = _width;
	infoHeader.height = _height;

	ofstream file;
	file.open(filename, ios::out | ios::binary);

	if (!file)
		return false;

	file.write((char*)&fileHeader, sizeof(fileHeader));
	file.write((char*)&infoHeader, sizeof(infoHeader));
	file.write((char*)_pPixels, _width * _height * 3);

	file.close();

	if (!file)
		return false;

	return true;
}

void Bitmap::setPixel(int x, int y, Color c) {
	uint8_t* pPixel = _pPixels;

	pPixel += y * 3 * _width + x * 3;

	pPixel[0] = c.b;
	pPixel[1] = c.g;
	pPixel[2] = c.r;
}