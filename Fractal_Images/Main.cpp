#include <iostream>
#include "Bitmap.h"

using std::cout;

int main()
{
	const int WIDTH = 800;
	const int HEIGHT = 600;

	Bitmap bmap(WIDTH, HEIGHT);
	Color temp{ 255,255,255 };

	bmap.setPixel(WIDTH / 2, HEIGHT / 2, temp);

	double min = 9999999;
	double max = -999999;

	for (auto x = 0; x < WIDTH; ++x)
	{
		for (auto y = 0; y < HEIGHT; ++y)
		{
			double fractalX = 0;
			double fractalY = 0;

			fractalX = (x - (WIDTH - 1.0) / 2.0f) * 2.0 / (WIDTH - 1.0);
			fractalX = (y - (HEIGHT - 1.0) / 2.0f) * 2.0 / (HEIGHT - 1.0);

			if (fractalX < min) min = fractalX;
			if (fractalX > max) max = fractalX;
		}
	}

	cout << "min: " << min << "  max: " << max << "\n";

	if (bmap.write("test.bmp"))
		cout << "Bitmap generated \n";
	else
		cout << "Unsuccessful \n";
	return 0;
}