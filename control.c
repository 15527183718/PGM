#include "pgm.h"

int main()
{
	image_t *myimage, *adaptimage,*bwimage,*erosionimage,*expansionimage;

	myimage=read_image("D:\\cells-plant.pgm");//����pgm�ļ�
	if (!write_image(myimage, "D:\\copy.pgm"))//д��pgm�ļ�
	{
		printf("error!");
		return 0;
	}
	if(!write_histogram(histogram_image(myimage),"D:\\histogram.pgm"))//ȡֱ��ͼ���ݲ�д��
	{
		printf("error!");
	    return 0;
    }
	adaptimage = adapt_contrast_image(myimage, 2);//�Աȶȸı�
	if (!write_image(adaptimage, "D:\\adapt.pgm"))//
	{
		printf("error!");
		return 0;
	}
	bwimage = grew_to_bw(myimage, 128);//�ڰ׻�
	if (!write_image(bwimage, "D:\\bw.pgm"))
	{
		printf("error!");
		return 0;
	}
	erosionimage = erosion_image(myimage);//��ʴ�Ҷ�ͼ
	if (!write_image(erosionimage, "D:\\erosion.pgm"))
	{
		printf("error!");
		return 0;
	}
	erosionimage = erosion_image(bwimage);//���ͻҶ�ͼ
	if (!write_image(erosionimage, "D:\\bw-erosion.pgm"))
	{
		printf("error!");
		return 0;
	}
	expansionimage = expansion_image(myimage);//��ʴ�ڰ�ͼ
	if (!write_image(expansionimage, "D:\\expansion.pgm"))
	{
		printf("error!");
		return 0;
	}
	expansionimage = expansion_image(bwimage);//���ͺڰ�ͼ
	if (!write_image(expansionimage, "D:\\bw-expansion.pgm"))
	{
		printf("error!");
		return 0;
	}
	printf("finish!\n");
	system("PAUSE");
	return 0;

}