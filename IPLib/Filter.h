#ifndef IPLIB_H
#define IPLIB_H

#pragma once

namespace Filters
{
	template <type T> class LinearFilter
	{
		typedef T ElementType;
	public:
		BaseFilter(){}

	protected:
		int mWidth;
		int mHeight;
		T*  mCoeffs;
	};
}

#endif

