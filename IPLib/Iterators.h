#ifndef ITER_H
#define ITER_H

#pragma once

namespace Image
{
	template <typename T> class PixelBuffer
	{
	public:
		typedef T PixelType;

		PixelBuffer(){}

	private:
		T*			_pixels;
		size_t		_width;
		size_t		_height;
		size_t		_stride;
	};
}

#endif

