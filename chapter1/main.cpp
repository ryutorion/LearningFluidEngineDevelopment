#include <array>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <string>
#include <thread>

const size_t kBufferSize = 80;
const char * kGrayScaleTable = " .:~=+*#%@";
const size_t kGrayScaleTableSize = sizeof(kGrayScaleTable) / sizeof(char);

using namespace std;
using namespace std::chrono;

void updateWave(const double timeInterval, double & x, double & speed)
{
	x += timeInterval * speed;
	if(x > 1.0)
	{
		speed *= -1.0;
		x = 1.0 + timeInterval * speed;
	}
	else if(x < 0.0)
	{
		speed *= -1.0;
		x = timeInterval * speed;
	}
}

void accumulateWaveToHeightField(
	const double x,
	const double waveLength,
	const double maxHeight,
	array<double, kBufferSize> & heightField
)
{
	const double quaterWaveLength = 0.25 * waveLength;
	const int start = static_cast<int>((x - quaterWaveLength) * kBufferSize);
	const int end = static_cast<int>((x + quaterWaveLength) * kBufferSize);

	for(int i = start; i < end; ++i)
	{
		int iNew = i;
		if(i < 0)
		{
			iNew = -i - 1;
		}
		else if(i >= static_cast<int>(kBufferSize))
		{
			iNew = 2 * kBufferSize - i - 1;
		}

		double distance = abs((i + 0.5) / kBufferSize - x);
		double height = maxHeight * 0.5 * (cos(min(distance * M_PI / quaterWaveLength, M_PI)) + 1.0);
		heightField[iNew] += height;
	}
}

void draw(const array<double, kBufferSize> & heightField)
{
	const string clear(kBufferSize, '\b');
	string buffer(kBufferSize, ' ');

	for(size_t i = 0; i < kBufferSize; ++i)
	{
		double height = heightField[i];
		size_t tableIndex = min(
			static_cast<size_t>(floor(kGrayScaleTableSize * height)),
			kGrayScaleTableSize - 1
		);
		buffer[i] = kGrayScaleTable[tableIndex];
	}

	printf("%s%s", clear.c_str(), buffer.c_str());
	fflush(stdout);
}

int main()
{
	const double waveLengthX = 0.8;
	const double waveLengthY = 1.2;

	const double maxHeightX = 0.5;
	const double maxHeightY = 0.4;

	double x = 0.0;
	double y = 1.0;
	double speedX = 1.0;
	double speedY = -0.5;

	const int fps = 100;
	const double timeInterval = 1.0 / fps;

	array<double, kBufferSize> heightField;

	for(int i = 0; i < 1000; ++i)
	{
		updateWave(timeInterval, x, speedX);
		updateWave(timeInterval, y, speedY);

		heightField.fill(0.0);
		accumulateWaveToHeightField(x, waveLengthX, maxHeightX, heightField);
		accumulateWaveToHeightField(y, waveLengthY, maxHeightY, heightField);

		draw(heightField);

		this_thread::sleep_for(milliseconds(1000 / fps));
	}

	return 0;
}
