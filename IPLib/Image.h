#ifndef IPLIB_H
#define IPLIB_H

#pragma once

#include <QString>
#include <QVector>
#include <QMap>
#include <QImage>

#include "Core.h"
#include "Color.h"
#include "Pixel.h"

#define _MANUAL_CROP

namespace Image
{
	typedef		unsigned int SizeType;

	namespace ChannelType
	{
		enum Enum
		{
			None,
			Red,
			Green,
			Blue,
			Y,
			U,
			V,
			Hue,
			Saturation,
			Value,
			Structural,
		};

		typedef QList<Enum> Collection;

		inline Collection Values()
		{
			Collection result;
			result	<< None
					<< Red
					<< Green
					<< Blue
					<< Y
					<< U
					<< V
					<< Hue
					<< Saturation
					<< Value
					<< Structural
					;
			return result;
		}

		inline QString Name(Enum value)
		{
			switch(value)
			{
			case None:			return "None";
			case Red:			return "Red";
			case Green:			return "Green";
			case Blue:			return "Blue";
			case Y:				return "Y";
			case U:				return "U";
			case V:				return "V";
			case Hue:			return "Hue";
			case Saturation:	return "Saturation";
			case Value:			return "Value";
			case Structural:	return "Structural";
			}
			return "***Bad value***";
		}
		inline bool Parse(const QString& name, Enum& value)
		{
			switch(value)
			{
			if(name == "None")			{ value = None;			return true; }
			if(name == "Red")			{ value = Red;			return true; }
			if(name == "Green")			{ value = Green;		return true; }
			if(name == "Blue")			{ value = Blue;			return true; }
			if(name == "Y")				{ value = Y;			return true; }
			if(name == "U")				{ value = U;			return true; }
			if(name == "V")				{ value = V;			return true; }
			if(name == "Hue")			{ value = Hue;			return true; }
			if(name == "Saturation")	{ value = Saturation;	return true; }
			if(name == "Value")			{ value = Value;		return true; }
			if(name == "Structural")	{ value = Structural;	return true; }
			}
			return false;
		}
	}

	class Location
	{
	public:
		Location() : mY(0), mX(0){}
		Location(int x, int y) : mX(x), mY(y){}

	public:
		__declspec(property(get=getX,put=setX)) int x;
		__declspec(property(get=getY,put=setY)) int y;

		int getX() const { return mX; }
		int getY() const { return mY; }

		void setX(int x) { mX = x; }
		void setY(int y) { mY = y; }

		void adjustX(int value)
		{
			mX += value;
		}
		void adjustY(int value)
		{
			mY += value;
		}

		void adjust(const Location& value)
		{
			mX += value.mX;
			mY += value.mY;
		}
		void adjust(int x, int y)
		{
			mX += x;
			mY += y;
		}

	private:
		int	mX;
		int mY;
	};

	class Size
	{
	public:
		Size() : mWidth(0), mHeight(0){}
		Size(SizeType width, SizeType height) : mWidth(width), mHeight(height){}
		
	public:
		__declspec(property(get=getWidth/*,put=setWidth*/)) int width;
		__declspec(property(get=getHeight/*,put=setHeight*/)) int height;

		int getWidth() const { return mWidth; }
		int getHeight() const { return mHeight; }

		void adjustWidth(int value)
		{
			mWidth += value;
			if(mWidth < 0) mWidth = 0;
		}
		void adjustHeight(int value)
		{
			mHeight += value;
			if(mHeight < 0) mHeight = 0;
		}

		void adjust(const Size& value)
		{
			mWidth += value.mWidth;
			if(mWidth < 0) mWidth = 0;
			mHeight += value.mHeight;
			if(mHeight < 0) mHeight = 0;
		}
		void adjust(int x, int y)
		{
			mWidth += x;
			if(mWidth < 0) mWidth = 0;
			mHeight += y;
			if(mHeight < 0) mHeight = 0;
		}

	private:
		SizeType		mWidth;
		SizeType		mHeight;
	};

	class Rect
	{
	public: 
		Rect(){}
		Rect(int x, int y, SizeType width, SizeType height) : mPosition(x,y), mSize(width, height){}
		Rect(const Location& position, const Size& size) : mPosition(position), mSize(size){}
	public: 
		__declspec(property(get=getX/*,put=setWidth*/)) int x;
		__declspec(property(get=getY/*,put=setWidth*/)) int y;
		__declspec(property(get=getWidth/*,put=setWidth*/)) SizeType width;
		__declspec(property(get=getHeight/*,put=setWidth*/)) SizeType height;
		__declspec(property(get=getLeft/*,put=setWidth*/)) int left;
		__declspec(property(get=getTop/*,put=setWidth*/)) int top;
		__declspec(property(get=getRight/*,put=setWidth*/)) int right;
		__declspec(property(get=getBottom/*,put=setWidth*/)) int bottom;
		__declspec(property(get=getSize/*,put=setWidth*/)) Size size;

		int				getX() const { return mPosition.x; }
		int				getY() const { return mPosition.y; }
		SizeType		getWidth() const {return mSize.width; }
		SizeType		getHeight() const { return mSize.height; }
		int				getLeft() const { return mPosition.x; }
		int				getTop() const { return mPosition.y; }
		int				getRight() const { return mPosition.x + mSize.width - 1; }
		int				getBottom() const { return mPosition.y + mSize.height - 1; }
		Size			getSize() const { return mSize; }

		void			adjust(int left, int top, int right, int bottom)
		{
			mPosition.adjust(left,top);
			mSize.adjust(right,bottom);
		}

		Rect			adjusted(int left, int top, int right, int bottom) const
		{
			Rect rc(*this); rc.adjust(left, top, right, bottom);
			return rc;
		}

	private:
		Location		mPosition;
		Size			mSize;
	};

	template<typename T> class Channel
	{
	public:
		typedef T PixelType;
		typedef QVector<Channel> Collection;
		template<typename T> struct _ScanRef
		{
			_ScanRef(){}
			_ScanRef(T* scan, int* pixel) : Scan(scan), Pixel(pixel){}
			T*		Scan;
			int*	Pixel;
			T& operator[] (int x) { return Scan[Pixel[x]]; }
		};
		typedef _ScanRef<PixelType> ScanRef;


		Channel()
		: mWidth(0)
		, mHeight(0)
		, mBitDepth(0)
		, mStride(0)
		, mChannelType(ChannelType::None)
		, mDepthMask(0)
		, mPixels(0)
		, mScanRefs(0)
		, mScan0(0)
		, mScans(0)
		, mPixIndexes(0)
		, mPixel0(0)
		{
		}

		Channel(SizeType width, SizeType height, SizeType bitDepth, ChannelType::Enum channelType = ChannelType::None)
		: mWidth(0)
		, mHeight(0)
		, mWidthReflection(0)
		, mHeightReflection(0)
		, mBitDepth(0)
		, mStride(0)
		, mChannelType(ChannelType::None)
		, mDepthMask(0)
		, mPixels(0)
		, mScanRefs(0)
		, mScan0(0)
		, mScans(0)
		, mPixIndexes(0)
		, mPixel0(0)
		{
			Initialize(width, height, bitDepth, channelType);
			AddRef();
		
#ifdef _DEBUG
			if(mWidth < 10 && mHeight < 10)
			{
				for(int y = 0; y < mHeight; y++)
				{
					for(int x = 0; x < mWidth; x++)
					{
						mPixels[y*mWidth + x] = (y+1)*10 + x + 1;
					}
				}
				printf("Pixel indexes 0 = %d:\n", mPixel0);
				for(int n= 0; n < mWidth*3 - 2; n++) printf("%d ",mPixIndexes[n] );
				printf("\n\n");
				printf("Scan refs 0 = 0x%08lX:\n",mScanRef0->Scan);
				for(int n= 0; n < mHeight*3 - 2; n++) printf("0x%08lX\n",mScanRefs[n].Scan);
				printf("\n\n");
			}
#endif		
		}

		//
		//	Constructs a shallow copy of a channel
		//
		Channel(const Channel& other)
		: mWidth(0)
		, mHeight(0)
		, mBitDepth(0)
		, mStride(0)
		, mChannelType(ChannelType::None)
		, mDepthMask(0)
		, mPixels(0)
		, mScanRefs(0)
		, mScan0(0)
		, mScans(0)
		, mPixIndexes(0)
		, mPixel0(0)
		{
			*this = other;
		}

		//
		//	Creates a shallow copy of a channel.
		//	Previously allocated data is released.
		//

		Channel& operator = (const Channel& other)
		{
			Release();

			mWidth		 = other.mWidth;
			mHeight		 = other.mHeight;
			mBitDepth	 = other.mBitDepth;
			mStride		 = other.mStride;
			mChannelType = other.mChannelType;
			mDepthMask	 = other.mDepthMask;
			mPixels		 = other.mPixels;
			mScanRefs	 = other.mScanRefs;
			mScan0		 = other.mScan0;
			mScans		 = other.mScans;
			mPixIndexes	 = other.mPixIndexes;
			mPixel0		 = other.mPixel0;

			AddRef();
			return *this;
		}

		~Channel()
		{
			Release();
		}

	protected:
		void Initialize(SizeType height, SizeType width, SizeType bitDepth, ChannelType::Enum channelType = ChannelType::None)
		{
			mWidth = width;
			mHeight = height;
			mWidthReflection = 2 * mWidth - 2;
			mHeightReflection = 2 * mHeight - 2;
			mStride = mWidth*sizeof(PixelType);
			mBitDepth = bitDepth;
			mChannelType = channelType;
			mBoundaries = Rect(-mWidth + 1, -mHeight + 1, mWidth*3 - 2, mHeight*3 - 2);

			mPixels = new PixelType[mWidth*mHeight];
			memset((void*)mPixels, 0, mWidth*mHeight*sizeof(T));

			mPixIndexes = new int[(mWidth - 1) + mWidth + (mWidth - 1)];
			mPixel0 = mPixIndexes + mWidth - 1;

			mScans = new PixelType*[mHeight];

			for(int pix = 0, *p = mPixel0; pix < mWidth; pix++, p++)
			{
				*p	= pix;
			}

			int *pLeft = mPixel0 -1, *pRight= mPixel0 + mWidth;
			for(int pix = 0; pix < mWidth - 1; pix++, pLeft--, pRight++)
			{
				*pLeft  = pix +1;
				*pRight = mWidth - pix - 2;
			}

			mScanRefs = new ScanRef[(mHeight - 1) + mHeight + (mHeight - 1)];
			mScanRef0 = mScanRefs + mHeight - 1; 
			mScan0	  = mPixels;

			ScanRef* p = mScanRef0;
			for(int scan = 0; scan < mHeight; scan++, p++)
			{
				*p			 = ScanRef(mPixels + scan*mWidth, mPixel0);
				mScans[scan] = mPixels + scan*mWidth;
			}
			ScanRef* pTop = mScanRef0 - 1, *pBottom= mScanRef0 + mHeight;
			for(int scan = 0; scan < mHeight - 1; scan++, pTop--, pBottom++)
			{	
				*pTop		= ScanRef(mPixels + (scan + 1)*mWidth, mPixel0);
				*pBottom	= ScanRef(mPixels + (mHeight - scan - 2)*mWidth, mPixel0);
			}
		}

		void	AddRef() 
		{
			if(mPixels)
			{
				if(mRefCount.find(mPixels) != mRefCount.end())
				{
					mRefCount[mPixels] = 0;
				}
				mRefCount[mPixels] = mRefCount[mPixels] + 1;
#ifdef _DEBUG
				printf("Added reference 0x%08lX = %d\n",mPixels,mRefCount[mPixels]);
#endif
			}
		}

		void	Release()
		{
			if(!mPixels)
			{
				return;
			}
#ifdef _DEBUG
				printf("Removing reference 0x%08lX",mPixels,mRefCount[mPixels]);
#endif
			ASSERT(mRefCount.find(mPixels) != mRefCount.end());
			mRefCount[mPixels] = mRefCount[mPixels] - 1;
			if(mRefCount[mPixels] <= 0)
			{
#ifdef _DEBUG
				printf(" = 0, deleting buffer\n");
#endif
				mRefCount.erase(mPixels);
				if(mPixels)
				{
					delete [] mPixels;
					mPixels = 0;
				}
				if(mScanRefs)
				{
					delete [] mScanRefs;
					mScanRefs = 0;
				}
				if(mPixIndexes)
				{
					delete [] mPixIndexes;
					mPixIndexes = 0;
				}
				if(mScans)
				{
					delete []mScans;
					mScans = 0;
				}
			}
#ifdef _DEBUG
			else
			{
				printf(" = %d\n",mRefCount[mPixels]);
			}
#endif
		}

	public:

		__declspec(property(get=getWidth)) int width;
		__declspec(property(get=getHeight)) int height;
		__declspec(property(get=getBoundaries)) Rect boundaries;
		__declspec(property(get=getRect)) Rect rect;
		__declspec(property(get=getStride)) int Stride;


		int  getWidth() const { return mWidth; }
		int  getHeight() const { return mHeight; }
		int  getStride() const { return mStride; }
		Rect getBoundaries() const { return mBoundaries; }
		Rect getRect() const { return Rect(0,0,mWidth, mHeight); }

#ifdef _MANUAL_CROP
		PixelType* operator[](int y) 
		{ 
			return mScans[y]; 
		}
#else
		ScanRef& operator[](int y) 
		{ 
			return mScanRef0[y]; 
		}
#endif
		int CropX(int value) const
		{
			if(value < 0) value = -value;
			if(value >= mWidth) value = mWidthReflection - value;
			return value;
		}
		int CropY(int value) const
		{
			if(value < 0) value = -value;
			if(value >= mHeight) value = mHeightReflection - value;
			return value;
		}

		PixelType&	pixel(int y, int x)
		{
			return mScanRef0[y][mPixel0[x]];
		}

		PixelType&	pixel1(int y, int x)
		{
			return mScans[y][x];
		}


	public:
		QImage	toRGB()
		{
			QImage result(mWidth, mHeight, QImage::Format_RGB32);
			for(int y = 0; y < mHeight; y++)
			{
				for(int x = 0;  < mWidth; x++)
				{
					T pix = pixel(x,y);
					Pixel::clamp<T>(pix,0,255);
					result.setPixel(x,y,QColor(pix,pix,pix).rgb());
				}
			}
			return result;
		}

		static QImage	toRGB(const Channel& red, const Channel& green, const Channel& blue)
		{
			QImage result(mWidth, mHeight, QImage::Format_RGB32);
			for(int y = 0; y < mHeight; y++)
			{
				for(int x = 0;  < mWidth; x++)
				{
					result.setPixel(x,y,QColor(red[y][x],green[y][x],blue[y][x]).rgb());
				}
			}
			return result;
		}

		static Channel red(const QImage& image, int bitDepth = 12)
		{
			Channel result(image.width(), image.height(), bitDepth, ChannelType::Red);
			for(int y = 0; y < mHeight; y++)
			{
				for(int x = 0; x < mWidth; x++)
				{
					QRgb rgb = image.pixel(x,y);
					pixel(y,x) = QColor(rgb).red();
				}
			}
			return result;
		}
		static Channel green(const QImage& image, int bitDepth = 12)
		{
			Channel result(image.width(), image.height(), bitDepth, ChannelType::Red);
			for(int y = 0; y < mHeight; y++)
			{
				for(int x = 0; x < mWidth; x++)
				{
					QRgb rgb = image.pixel(x,y);
					pixel(y,x) = QColor(rgb).green();
				}
			}
			return result;
		}
		static Channel blue(const QImage& image, int bitDepth = 12)
		{
			Channel result(image.width(), image.height(), bitDepth, ChannelType::Red);
			for(int y = 0; y < mHeight; y++)
			{
				for(int x = 0; x < mWidth; x++)
				{
					QRgb rgb = image.pixel(x,y);
					pixel(y,x) = QColor(rgb).blue();
				}
			}
			return result;
		}
		static Channel Y(const QImage& image, int bitDepth = 12)
		{
			Channel result(image.width(), image.height(), bitDepth, ChannelType::Y);
			for(int y = 0; y < image.height(); y++)
			{
				for(int x = 0; x < image.width(); x++)
				{
					QRgb rgb = image.pixel(x,y);
					result.pixel(y,x) = Pixel::Y<PixelType>(rgb);
				}
			}
			return result;
		}

		Channel resample(int insertScans, int removeScans, int insertPixels, int removePixels)
		{
			//	Currently support for 2x upsamling only
			ASSERT(1 == insertScans && 0 == removeScans && 1 == insertPixels && 0 == removePixels);
			if(0 == mWidth || 0 == mHeight)
			{
				return Channel();
			}
			Channel result(mWidth*2, mHeight*2,mBitDepth,mChannelType);
			for(int y = 0, _y = 0; y < mHeight; y++, _y+=2)
			{
				for(int x = 0, _x = 0; x < mWidth; x++, _x+=2)
				{
					result[_y][_x] = pixel(y,x);
				}
			}
			return result;
		}

	public:
		SizeType				mWidthReflection;
		SizeType				mHeightReflection;

	private:
		T*						mPixels;

		SizeType				mWidth;
		SizeType				mHeight;
		SizeType				mBitDepth;
		SizeType				mDepthMask;
		ChannelType::Enum		mChannelType;
		Rect					mBoundaries;
		SizeType				mStride;
		ScanRef*				mScanRefs;
		ScanRef*				mScanRef0;
		PixelType**				mScans;
		PixelType*				mScan0;
		int*					mPixIndexes;
		int*					mPixel0;

		static std::map<typename T*,int>	mRefCount;
	};
	template<typename T> std::map<T*,int> Channel<T>::mRefCount;

	typedef Channel<int>		IntChannel;
	 
	class Iterator
	{
	public:
		Iterator(){}

	public:
		__declspec(property(get=getX)) int x;
		__declspec(property(get=getY)) int y;

		__declspec(property(get=getPosition)) Location position;

		virtual void Reset() = 0;
		virtual bool Next() = 0;

		virtual int getX() const = 0;
		virtual int getY() const = 0;
		virtual Location getPosition() const = 0;

		virtual ~Iterator(){}
	};

	class RectIterator : public Iterator
	{
	public:
		RectIterator(const Rect& rc) : mRect(rc), mPosition(rc.left-1,rc.top) {}

		virtual void Reset()
		{
			mPosition = Location(mRect.left-1, mRect.top);
		}
		virtual bool Next()
		{
			if(mPosition.y > mRect.bottom)
			{
				return false;
			}
			mPosition.adjustX(1);
			if(mPosition.x > mRect.right)
			{
				mPosition.x = mRect.left;
				mPosition.adjustY(1);
				if(mPosition.y > mRect.bottom)
				{
					return false;
				}
			}
			return true;
		}

		virtual int getX() const { return mPosition.x; }
		virtual int getY() const { return mPosition.y; }
		virtual Location getPosition() { return mPosition; }

	private:
		Rect		mRect;
		Location	mPosition;
		bool		mAtEnd;
	};

	template<typename T> class Filter
	{
		typedef T ElementType;
	public:
		Filter(int width, int height, const ElementType*coeffs, int originX = -1, int originY = -1)
		: mWidth(width)
		, mHeight(height)
		, mOriginX(originX)
		, mOriginY(originY)
		, mCoeffs(new ElementType[mWidth*mHeight])
		{
			ASSERT(mWidth % 2 && mHeight % 2);
			if(originX < 0) originX = mWidth/2;
			if(originY < 0) originY = mHeight/2;
			ASSERT(0 <= originX && originX < mWidth);
			ASSERT(0 <= originY && originY < mHeight);

			for(int n = 0; n < mWidth*mHeight; n++)
			{
				mCoeffs[n] = coeffs[n];
			}
		}
		~Filter()
		{
			delete mCoeffs;
		}
	public:
		__declspec(property(get=getWidth)) int width;
		__declspec(property(get=getHeight)) int height;
		__declspec(property(get=getOriginX)) int originX;
		__declspec(property(get=getOriginY)) int originY;


		int getWidth() const { return mWidth; }
		int getHeight() const { return mHeight; }
		int getOriginX() const { return mOriginX; }
		int getOriginY() const { return mOriginY; }

		template<class ReturnType> ReturnType apply(Channel<T>& channel, int atY, int atX) const
		{
			ReturnType value = 0;
			ElementType* pCoeff = mCoeffs;
			for(int _y = atY - mOriginY, toY = atY - mOriginY + mHeight; _y < toY; _y++)
			{
				int __y = channel.CropY(_y);
				for(int _x = atX - mOriginX, toX = atX - mOriginX + mWidth, __x = channel.CropX(_x); _x < toX; __x = channel.CropX(++_x), pCoeff++)
				{
#ifdef _MANUAL_CROP
					value += channel[__y][__x] * *pCoeff;
#else
					value += channel[_y][_x] * *pCoeff;
#endif
				}
			}
			return value;
		}

	protected:
		T*			mCoeffs;
		int			mWidth;
		int			mHeight;
		int			mOriginX;
		int			mOriginY;
	};
}
#endif // IPLIB_H
