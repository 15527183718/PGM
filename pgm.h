#ifndef __PGM_H__  
#define __PGM_H__ 
#include <stdio.h>
#include <stdlib.h>
#define MAX_BRIGHTNESS  255 /* 最大像素值 */
#define MAX_READLINE    512 /* 读入行长 */
#define MAX_BUFFERSIZE  256 /* 最大缓存 */
#define MAX_WRITELINE   10  /*写出行长*/
typedef struct
{
	int Vmax;//最大像素值
	int rows,cols;//行，列（宽，高）
	unsigned char * data;//图片数据	
}image_t;
typedef struct
{
	int Vmax;//最大像素值
	int Hmax;//直方图最值
	int *data;//直方图数据
}histogram_t;
image_t *allocate_image()//image分配空间
{
	image_t *NewImage;
	NewImage = (image_t*)malloc(sizeof(image_t));
	if (NewImage == NULL)
	{
		printf("IMGALLOC:Couldn't allocate image structure\n");
		return(NULL);
	}
	NewImage->rows = 0;
	NewImage->cols = 0;
	NewImage->Vmax = 0;
	NewImage->data = NULL;
	return(NewImage);
}
histogram_t *allocate_histogram()//histogram分配空间
{
	histogram_t *NewHistogram;
	NewHistogram = (histogram_t*)malloc(sizeof(histogram_t));
	if (NewHistogram == NULL)
	{
		printf("HGMALLOC:Couldn't allocate histogram structure\n");
		return(NULL);
	}
	NewHistogram->Vmax = 0;
	NewHistogram->Hmax = 0;
	NewHistogram->data = NULL;
	return(NewHistogram);
}
void setpixel_image(image_t *img, int r, int c, unsigned char val)//置放像素值
{
	int nc;
	nc = img->cols;
	img->data[(r*nc) + c] = val;
}
unsigned char getpixel_image(image_t *img, int r, int c)//取像素值
{
	int nc;
	nc = img->cols;
	return (img->data[(r * nc) + c]);
}
image_t* read_image(char *filename)//pgm文件载入成image
{
	image_t *NewImage;
	FILE *pgm;
	char line[MAX_READLINE], intbuf[MAX_BUFFERSIZE], ch,check;
	int type, nc, nr, maxval, i, j, k, found;
	NewImage = allocate_image();
	if ((pgm = fopen(filename, "r")) == NULL)
	{
		printf("IMGOPEN:Couldn't open '%s'\n",filename);
		return(NULL);
	}
	for (check = '#'; check == '#';)
	{
		fgets(line, MAX_READLINE-1, pgm);
		sscanf(line, "%c", &check);
	}
	sscanf(line, "P%d", &type);	
		if (type != 2)
		{
			printf("IMGOPEN:Not P2 files\n");
			fclose(pgm);
			return(NULL);
		}
		for (check = '#'; check == '#';)
		{
			fgets(line, MAX_READLINE-1, pgm);
			sscanf(line, "%c", &check);
		}
		sscanf(line, "%d %d", &nc, &nr);
		NewImage->rows = nr;
		NewImage->cols = nc;
		for (check = '#'; check == '#';)
		{
			fgets(line, MAX_READLINE - 1, pgm);
			sscanf(line, "%c", &check);
		}		
		sscanf(line, "%d", &maxval);
		if (maxval > MAX_BRIGHTNESS)
		{
			printf("IMGOPEN: Only handles pgm files of 8 bits or less\n");
			fclose(pgm);
			return(NULL);
		}
		NewImage->Vmax = maxval;
		NewImage->data = (unsigned char*)malloc((unsigned)(nr * nc * sizeof(unsigned char)));
		if (NewImage->data == NULL)
		{
			printf("IMGOPEN: Couldn't allocate space for image data\n");
			fclose(pgm);
			return(NULL);
		}
		for (i = 0; i < nr; i++) 
		{
			for (j = 0; j < nc; j++) 
			{
				k = 0;  found = 0;
				while (!found) 
				{
					ch = (char)fgetc(pgm);
					if (ch >= '0' && ch <= '9') 
					{
						intbuf[k] = ch;  k++;
					}
					else 
					{
						if (k != 0) 
						{
							intbuf[k] = '\0';
							found = 1;
						}
					}
				}
				setpixel_image(NewImage, i, j, atoi(intbuf));
			}
		}
		fclose(pgm);
		return (NewImage);	
}
int write_image(image_t *img, char *filename)//image导出成pgm文件
{
	int i, j, nr, nc, k, val;
	FILE *pgm;
	nr = img->rows;  nc = img->cols;
	pgm = fopen(filename, "w");
	fprintf(pgm, "P2\n");
	fprintf(pgm, "%d %d\n", nc, nr);
	fprintf(pgm, "%d\n",img->Vmax);
	k = 1;
	for (i = 0; i < nr; i++)
	{
		for (j = 0; j < nc; j++)
		{
			val = getpixel_image(img, i, j);
			if ((val < 0) || (val > MAX_BRIGHTNESS))
			{
				printf("IMGWRITE: Found value %d at row %d col %d\n", val, i, j);
				printf(" Setting it to zero\n");
				val = 0;
			}
			if (k % MAX_WRITELINE)
			{
				fprintf(pgm, "%d ", val);
			}
			else 
			{
				fprintf(pgm, "%d\n", val);
			}
			k++;
		}
	}
	fprintf(pgm, "\n");
	fclose(pgm);
	return (1);
}
image_t* adapt_contrast_image(image_t *img, float k)//对比度调整
{
	image_t *NewImage;
	int i, j, nc, nr,maxval;
	unsigned char m;
	NewImage = allocate_image();
	NewImage->cols=nc = img->cols;
	NewImage->rows=nr = img->rows;
	NewImage->Vmax =maxval= img->Vmax;
	NewImage->data = (unsigned char*)malloc((unsigned)(nr * nc * sizeof(unsigned char)));
	for (i = 0; i < nr; i++) 
	{
		for (j = 0; j < nc; j++) 
		{
			m = getpixel_image(img, i, j);
			if (k*m > 0 && k*m < MAX_BRIGHTNESS)
				m = k * m;
			else if (k*m < 0)
				m = 0;
			else
				m = MAX_BRIGHTNESS;
			setpixel_image(NewImage, i, j, m);
		}
	}
	return (NewImage);
}
histogram_t* histogram_image(image_t *img)//image转为histogram
{
	histogram_t *NewHistogram;
	int i,j,maxval;
	NewHistogram = allocate_histogram();
	NewHistogram->Vmax =maxval= img->Vmax;
	NewHistogram->data = (int*)malloc(maxval* sizeof(int));
	for (i = 0; i < NewHistogram->Vmax; i++) 
	{
		NewHistogram->data[i] = 0;
	}
	for (i = 0; i < img->rows; i++) 
	{
		for (j = 0; j < img->cols; j++)
		{
			if(img->data[i*img->cols + j]>0)
			NewHistogram->data[img->data[i*img->cols + j]]++;
		}
	}
	for (i = 0; i < NewHistogram->Vmax; i++)
	{
		NewHistogram->Hmax = NewHistogram->Hmax > NewHistogram->data[i] ? NewHistogram->Hmax : NewHistogram->data[i];
	}
	return(NewHistogram);
}
int write_histogram(histogram_t *hgm, char *filename) //histogram导出成pgm文件
{
	image_t *NewImage;
	int i, j;
	NewImage = allocate_image();
	NewImage->rows =NewImage->Vmax= hgm->Vmax;
	NewImage->cols = 255;
	NewImage->data = (unsigned char*)malloc((unsigned)(NewImage->rows * NewImage->cols * sizeof(unsigned char)));
	for (i = 0; i < NewImage->rows; i++)
	{
		for (j = 0; j < NewImage->cols; j++)
		{
			if(hgm->data[i]*255+j*hgm->Hmax<255*hgm->Hmax)
			    NewImage->data[j*NewImage->cols + i] = NewImage->Vmax;
			else
				NewImage->data[j*NewImage->cols + i] = 0;
		}
	}	
	return(write_image(NewImage, filename));
}
image_t* grew_to_bw(image_t *img, unsigned char k)//灰度图像黑白化
{
	image_t *NewImage;
	int i, j, nc, nr, maxval;
	unsigned char m;
	NewImage = allocate_image();
	NewImage->cols = nc = img->cols;
	NewImage->rows = nr = img->rows;
	NewImage->Vmax = maxval = img->Vmax;
	NewImage->data = (unsigned char*)malloc((unsigned)(nr * nc * sizeof(unsigned char)));
	for (i = 0; i < nr; i++) 
	{
		for (j = 0; j < nc; j++) 
		{
			m = getpixel_image(img, i, j);
			if (m < k)
				m = 0;
			else
				m = MAX_BRIGHTNESS;
			setpixel_image(NewImage, i, j, m);
		}
	}
	return (NewImage);
}
image_t* erosion_image(image_t *img)//侵蚀算法
{
	image_t *NewImage;
	int i, j,k,l, nc, nr, maxval;
	unsigned char m;
	NewImage = allocate_image();
	NewImage->cols = nc = img->cols;
	NewImage->rows = nr = img->rows;
	NewImage->Vmax = maxval = img->Vmax;
	NewImage->data = (unsigned char*)malloc((unsigned)(nr * nc * sizeof(unsigned char)));
	for (i = 0; i < nr; i++) 
	{
		for (j = 0; j < nc; j++) 
		{
			NewImage->data[(i*nc) + j] = MAX_BRIGHTNESS;
		}
	}
	for (i = 1; i < nr-1; i++) 
	{
		for (j = 1; j < nc-1; j++) 
		{
			m = getpixel_image(img, i, j);
			for (k = i - 1; k < i + 2; k++)
			{
				for (l = j - 1; l < j + 2; l++)
				{
					NewImage->data[k*nc + l] = NewImage->data[k*nc + l] < m ? NewImage->data[k*nc + l] : m;
				}
			}
		}
	}
	return (NewImage);
}
image_t* expansion_image(image_t *img)//膨胀算法
{
	image_t *NewImage;
	int i, j, k, l, nc, nr, maxval;
	unsigned char m;
	NewImage = allocate_image();
	NewImage->cols = nc = img->cols;
	NewImage->rows = nr = img->rows;
	NewImage->Vmax = maxval = img->Vmax;
	NewImage->data = (unsigned char*)malloc((unsigned)(nr * nc * sizeof(unsigned char)));
	for (i = 0; i < nr; i++) 
	{
		for (j = 0; j < nc; j++) 
		{
			NewImage->data[(i*nc) + j] = 0;
		}
	}
	for (i = 1; i < nr - 1; i++) 
	{
		for (j = 1; j < nc - 1; j++) 
		{
			m = getpixel_image(img, i, j);
			for (k = i - 1; k < i + 2; k++)
			{
				for (l = j - 1; l < j + 2; l++)
				{
					NewImage->data[k*nc + l] = NewImage->data[k*nc + l] > m ? NewImage->data[k*nc + l] : m;
				}
			}
		}
	}
	return (NewImage);
}
#endif