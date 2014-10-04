#ifndef PIXEL_H
#define PIXEL_H
#pragma once

#include <QImage>

namespace Pixel
{
	template<typename T> void clamp(T& value, const T& minValue, const T& maxValue)
	{
		if(value < minValue) value = minValue;
		if(maxValue < value) value = maxValue;
	}

	template<typename T> T Y(QRgb value)
	{
		T red = value& 0xff;
		T green = (value >> 8) & 0xff;
		T blue = (value >> 16) & 0xff;
		return (T)(0.299*red + 0.587*green + 0.114*blue);
	}
	template<typename T> T U(QRgb value)
	{
		T red = value & 0xff;
		T green = (value >> 8) & 0xff;
		T blue = (value >> 16) & 0xff;
		return (T)(-0.14713*red - 0.28886*green + 0.436*blue);
	}
	template<typename T> T V(QRgb value)
	{
		T red = value & 0xff;
		T green = (value >> 8) & 0xff;
		T blue = (value >> 16) & 0xff;
		return (T)(0.615*red - 0.51499*green - 0.10001*blue);
	}

}

#endif // PIXEL_H
