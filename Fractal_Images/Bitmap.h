#pragma once
#include <string>
using std::string;
#include <cstdint>
#include <memory>

struct Color {
	uint8_t r{ 0 };
	uint8_t g{ 0 };
	uint8_t b{ 0 };
};

class Bitmap {
private:
	int _width{ 0 };
	int _height{ 0 };
	uint8_t* _pPixels{ nullptr };

public:
	Bitmap();
	Bitmap(int width, int height);
	~Bitmap();

	void setPixel(int x, int y, Color c);
	bool write(string filename);
};