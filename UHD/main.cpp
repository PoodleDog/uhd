#include "stdafx.h"
#include <QtCore/QCoreApplication>
#include "Core.h"
#include "Image.h"

#ifdef WIN32
#ifdef _DEBUG
#include <crtdbg.h>
static class HeapWatcher
{
public:
	HeapWatcher()
	{
		_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	}
	virtual ~HeapWatcher()
	{
	}
} heapWatcher;
#endif
#endif

using namespace Image;


int main(int argc, char *argv[])
{
	if(0)
	{
		char*fileNmae = "";
		QImage image(fileNmae);
		IntChannel a = IntChannel::Y(image);
	}

	if(0)
	{
		IntChannel b(5,5,12);
		IntChannel a(b);
		IntChannel c = a;
		return 0;
	}
	if(0)
	{
		IntChannel a(5,5,12);
		for(int y = a.rect.top; y <= a.rect.bottom; y++)
		{
			for(int x = a.rect.left; x <= a.rect.right; x++)
			{
				printf("%d ",a[y][x]);
			}
			printf("\n");
		}
		printf("\n");
		for(int y = a.boundaries.top; y <= a.boundaries.bottom; y++)
		{
			for(int x = a.boundaries.left; x <= a.boundaries.right; x++)
			{
				printf("%d ",a[y][x]);
			}
			printf("\n");
		}
		printf("\n");
		for(int y = a.boundaries.top; y <= a.boundaries.bottom; y++)
		{
			for(int x = a.boundaries.left; x <= a.boundaries.right; x++)
			{
				printf("%d ",a.pixel1(y,x));
			}
			printf("\n");
		}
		printf("\n");
		return 0;
	}
	clock_t start = clock();

	if(1)
	{
		const int cx = 5, cy = 5;
		IntChannel a(1080*2, 1920*2, 12);
		const int nPasses = 2;
		double t1 = 0, t2 = 0, t3 = 0, _t4 = 0;

		int coeffs[] = 
		{
			1,1,1,
			1,1,1,
			1,1,1,
		};

		Image::Filter<int> filter(3,3,coeffs);

		for(int n = 0; n < nPasses; n++)
		{
			ClockTimer ct;
			for(int _y = -cy, y = a.CropY(_y); _y < a.height + cy; y = a.CropY(++_y))
			{
				for(int _x = -cx, x = a.CropX(_x); _x < a.width + cx; x = a.CropX(++_x))
				{
					a[y][x] += 1;
				}
			}
			t1 += ct.milliSeconds;

			for(int y = -cy; y < a.height + cy; y++)
			{
				for(int x = -cx; x < a.width + cx; x++)
				{
					a.pixel(y,x) += 1;
				}
			}
			t2 += ct.milliSeconds;

			for(int _y = -cy, y = a.CropY(_y); _y < a.height + cy; y = a.CropY(++_y))
			{
				for(int _x = -cx, x= a.CropX(_x) ; _x < a.width + cx; x = a.CropY(++_x))
				{
					a.pixel1(y,x) += 1;
				}
			}
			t3 += ct.milliSeconds;

			
			for(int y = 0; y < a.height; y++)
			{
				for(int x= 0; x < a.width; x++)
				{
					a.pixel1(y,x) = filter.apply<int>(a,x,y);
				}
			}

			_t4 += ct.milliSeconds;
			
			printf("Pass %d\r",n + 1);

		}

		printf("[y][x]:\t%lf\n", t1/nPasses);
		printf("pixel(x,y):\t%lf\n", t2/nPasses);
		printf("pixel1(x,y):\t%lf\n", t3/nPasses);
		printf("filter:\t%lf\n", _t4/nPasses);
	}
	return 0;
}
