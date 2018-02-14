#include "pgm.h"

int main()
{
	image_t *myimage, *adaptimage,*bwimage,*erosionimage,*expansionimage;

	myimage=read_image("D:\\cells-plant.pgm");//载入pgm文件
	if (!write_image(myimage, "D:\\copy.pgm"))//写出pgm文件
	{
		printf("error!");
		return 0;
	}
	if(!write_histogram(histogram_image(myimage),"D:\\histogram.pgm"))//取直方图数据并写出
	{
		printf("error!");
	    return 0;
    }
	adaptimage = adapt_contrast_image(myimage, 2);//对比度改变
	if (!write_image(adaptimage, "D:\\adapt.pgm"))//
	{
		printf("error!");
		return 0;
	}
	bwimage = grew_to_bw(myimage, 128);//黑白化
	if (!write_image(bwimage, "D:\\bw.pgm"))
	{
		printf("error!");
		return 0;
	}
	erosionimage = erosion_image(myimage);//侵蚀灰度图
	if (!write_image(erosionimage, "D:\\erosion.pgm"))
	{
		printf("error!");
		return 0;
	}
	erosionimage = erosion_image(bwimage);//膨胀灰度图
	if (!write_image(erosionimage, "D:\\bw-erosion.pgm"))
	{
		printf("error!");
		return 0;
	}
	expansionimage = expansion_image(myimage);//侵蚀黑白图
	if (!write_image(expansionimage, "D:\\expansion.pgm"))
	{
		printf("error!");
		return 0;
	}
	expansionimage = expansion_image(bwimage);//膨胀黑白图
	if (!write_image(expansionimage, "D:\\bw-expansion.pgm"))
	{
		printf("error!");
		return 0;
	}
	printf("finish!\n");
	system("PAUSE");
	return 0;

}