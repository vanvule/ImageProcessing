#include "stdafx.h"
#include "Kmean.h"
struct pixel {
	float R = 0;
	float G = 0;
	float B = 0;
};
static int channel;
static vector<pixel> centroids;// vector luu cac tam  cum 
static bool Stop = false;//bien ktra dung thuat toan khi khong tim duoc tam moi

Kmean::Kmean()
{
	
	this->_numClusters = 0;
}

int Duplicate(vector<pixel> centroid,pixel x)
{
	/*
	Ham kiem tra pixel x co nam trong vector chua cac pixel
	0: Neu khong trung
	1: Neu trung
	*/
	if (channel == 3)
	{
		for (int i = 0; i < centroid.size(); i++)
		{

			if (centroid[i].R == x.R && centroid[i].G == x.G && centroid[i].B == x.B)
				return 1;
		}
	}
	return 0;
}
float euclidianDistance(pixel a, pixel b)
{
	/*
	Ham tinh khoang cach 2 pixel: chinh la khoang cach 3 phan tu R,G,B
	Tra ve so thuc khong cach
	*/
	float d = 0;
	if (channel == 3)
	{
		
		d += pow((a.R - b.R), 2);
		d += pow((a.G - b.G), 2);
		d += pow((a.B - b.B), 2);
		
	}
	return sqrt(d);
}
int updateCentroid(vector<pixel> centroid, vector<pixel> cur_centroid)
{
	/*
	Ham ktra cac tam moi co khac biet voi tam cu hay khong
	Cap nhat ai tam moi neu gia tri tam cu khac voi gia tri tam moi
	0 Neu cap nhat tam moi
	1 Khong khac biet, khog cap nhat
	*/
	Stop = true;//Bien kiem tra dieu kien dung thuat toan
	for (int i = 0; i < centroid.size();i++)
	{
		if (channel == 3)
		{
			if (centroid[i].R != cur_centroid[i].R || centroid[i].G != cur_centroid[i].G || centroid[i].B != cur_centroid[i].B)
			{
				Stop = false;
				centroids.clear();
				centroids = cur_centroid;//cap nhat lai tam moi
				return 0;
			}
		}
	}
	return 1;
}

int IndexMin(vector<float> dlist)
{
	/* Ham tim vi tri be nhat
	Tra ve vi tri: int
	*/
	float min_value = dlist[0];
	int index = 0;
	for (int i = 1; i < dlist.size(); i++)
	{
		if (dlist[i] < min_value)
		{
			min_value = dlist[i];
			index = i;
		}
	}
	return index;
}

int Kmean::Apply(const Mat& srcImage, Mat &dstImage)
{
	Mat image = srcImage;
	channel = image.channels();
	int row = image.rows;
	int col = image.cols;	
	if (channel == 3)
	{
		//Khoi tao K tam ngau nhien va khong trung
		while (centroids.size() < _numClusters)
		{
			srand(time(NULL));
			int a = rand() % (row-1);
			int b = rand() % (col-1);
			uchar *p = image.ptr<uchar>(a, b);	
			pixel P;
			P.R = p[0];
			P.G = p[1];
			P.B = p[2];
			if (Duplicate(centroids, P) == 0)
				centroids.push_back(P);
		}
		//Lay data diem dua vao 1 vector de tang toc do xu ly
		vector <pixel> DataPixel;
		for (int i = 0; i < row; i++)
		{
			uchar *data = image.ptr<uchar>(i);
			for (int j = 0; j < col; j++, data += channel)
			{

				pixel P;
				P.R = data[0];
				P.G = data[1];
				P.B = data[2];
				DataPixel.push_back(P);
			}
		}
		int size = DataPixel.size();
		while (Stop == false)
		{
			cout << ".";
			vector<pixel> cur_centroid;//luu cac tam moi tim duoc
			for (int k = 0; k < _numClusters; k++)//Xet tung cum
			{
				vector<pixel>sample;//Mau chua cac pixel thuoc 1 lan chon tam
				float Sum_R_inSample = 0;
				float Sum_G_inSample = 0;
				float Sum_B_inSample = 0;
				
				for(int i = 0; i < size; i++)
				{
					vector <float> d_list;
					for (int ki = 0; ki < _numClusters; ki++)
					{
						d_list.push_back(euclidianDistance(centroids[ki], DataPixel[i]));//Khoang cach tu  diem xet dn k tam
					}
					int c = IndexMin(d_list);//Tim vi tri co hong cahch den tam ngan nhat
					if (c == k)//Neu thuoc tam dang xet thif them vao mau samole
					{
						sample.push_back(DataPixel[i]);
						Sum_R_inSample += DataPixel[i].R;
						Sum_G_inSample += DataPixel[i].G;
						Sum_B_inSample += DataPixel[i].B;
					}
				}
				
				//Tim mean tam moi trong sample
				pixel new_centroid;
				new_centroid.R = Sum_R_inSample / sample.size();
				new_centroid.G = Sum_G_inSample / sample.size();
				new_centroid.B = Sum_B_inSample / sample.size();
				cur_centroid.push_back(new_centroid);//Them diem tam moi vua lay trung binh vao vector tam hien tai
			}
			//ktra update tam cum
			updateCentroid(centroids, cur_centroid);
		}
		//Chuan hoa cac pixel ve cum gan nhat
		dstImage = Mat(row,col,CV_8UC3);
		for (int i = 0; i < row; i++)
		{
			uchar *data = image.ptr<uchar>(i);
			uchar *data_dst = dstImage.ptr<uchar>(i);
			for (int j = 0; j < col; j++, data += channel, data_dst += channel)
			{
				pixel P;
				P.R = data[0];
				P.G = data[1];
				P.B = data[2];
				vector <float> d_list;
				for (int ki = 0; ki < _numClusters; ki++)
				{
					d_list.push_back(euclidianDistance(centroids[ki], P));
				}
				int c = IndexMin(d_list);
				data_dst[0] = roundf(centroids[c].R);
				data_dst[1] = roundf(centroids[c].G);
				data_dst[2] = roundf(centroids[c].B);
			}
		}
		return 1;
	}
	
	return 0;
}
