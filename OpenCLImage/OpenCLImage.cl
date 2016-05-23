
__kernel void scale(__global const int* srcR,__global const int* srcG,__global const int* srcB,
__global  int* dstR,__global  int* dstG,__global  int* dstB,
const int srcWidth,const int srcHeight,const int dstWidth,const float scaleNum)
{
    int i=get_global_id(0);
	//存储总加权像素
	int wPixelR;
	int wPixelG;
	int wPixelB;
	
	int x = i % dstWidth;
	int y = i / dstWidth;
	//映射回的原坐标
	float Xsrc = x / scaleNum;
	float Ysrc = y / scaleNum;

	int X = Xsrc;
	int Y = Ysrc;

	wPixelR = 0;
	wPixelG = 0;
	wPixelB = 0;
	//计算加权像素值
	for (int k = Y - 1;k <= Y + 2;++k)
	{
		for (int j = X - 1;j <= X + 2;++j)
		{
			//16个点落在图像内的点才计算，落在外面的点权重为0
			if (!(j < 0 || k < 0 || j >= srcWidth || k >= srcHeight))
			{
			//给像素加权
				float weight;
				float weightX;
				float weightY;
				float tempX =j - Xsrc;
				float tempY =k - Ysrc;
				tempX=tempX>=0?tempX:(-1.0*tempX);
				tempY=tempY>=0?tempY:(-1.0*tempY);

				//权重公式
				if (tempX >= 0 && tempX <= 1)
				{
					weightX = tempX*tempX*tempX - 2 * tempX*tempX + 1;
				}
				else
				{
					weightX = -1 * tempX*tempX*tempX + 5 * tempX*tempX - 8 * tempX + 4;
				}

				if (tempY >= 0 && tempY <= 1)
				{
					weightY = tempY*tempY*tempY - 2 * tempY*tempY + 1;
				}
				else
				{
					weightY = -1 * tempY*tempY*tempY + 5 * tempY*tempY - 8 * tempY + 4;
				}

				weight = weightX*weightY;

				
					int pixelR = srcR[k*srcWidth+j];
					int pixelG =srcG[k*srcWidth+j];
					int pixelB = srcB[k*srcWidth+j];
					wPixelR += (int)(weight*pixelR);
					wPixelG += (int)(weight*pixelG);
					wPixelB += (int)(weight*pixelB);
				
			}
		}
	}
		//防止像素越界
		wPixelR = wPixelR > 255 ? 255 : wPixelR;
		wPixelR = wPixelR < 0 ? 0 : wPixelR;
		wPixelG = wPixelG > 255 ? 255 : wPixelG;
		wPixelG = wPixelG < 0 ? 0 : wPixelG;
		wPixelB = wPixelB > 255 ? 255 : wPixelB;
		wPixelB = wPixelB < 0 ? 0 : wPixelB;

		dstR[i] = wPixelR;
		dstG[i] = wPixelG;
		dstB[i] = wPixelB;
}