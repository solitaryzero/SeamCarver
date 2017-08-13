#include <iostream>
#include "stdafx.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <cv.h>

using namespace cv;
const int inf = 1000000000;

int cal(Vec3b a, Vec3b b)
{
	int ans = int(sqrt((a[0] - b[0])*(a[0] - b[0]) + (a[1] - b[1])*(a[1] - b[1]) + (a[2] - b[2])*(a[2] - b[2])));
	if ((a[1] > 250) && (a[0] < 10) && (a[2] < 10)) ans = ans - 100000;
	return ans;
}

Mat ComputeEnergy(Mat m, int para)
{
	Mat m_gray = Mat(m.rows, m.cols, CV_8U);
	cvtColor(m, m_gray, CV_RGB2GRAY);
	Mat tmp = Mat(m.rows, m.cols, CV_32S);

	if (para == 5)  //sobel
	{
		Mat sobel = Mat(m.rows, m.cols, CV_8U);
		Sobel(m_gray, sobel, -1, 1, 1);
		m_gray = sobel;
		para = 1;
	}

	if (para == 0) //removal
	{
		for (int i = 0; i < m.rows; i++)
		{
			for (int j = 0; j < m.cols; j++)
			{
				if (i == 0)
				{
					tmp.at<int>(i, j) = cal(m.at<Vec3b>(i, j), m.at<Vec3b>(i + 1, j));
				}
				else
				{
					tmp.at<int>(i, j) = cal(m.at<Vec3b>(i, j), m.at<Vec3b>(i - 1, j));
				}

				if (j == 0)
				{
					tmp.at<int>(i, j) += cal(m.at<Vec3b>(i, j), m.at<Vec3b>(i, j + 1));
				}
				else
				{
					tmp.at<int>(i, j) += cal(m.at<Vec3b>(i, j), m.at<Vec3b>(i, j - 1));
				}
			}
		}
	}

	if (para == 1)   // operator 1
	{
		for (int i = 0; i < m_gray.rows; i++)
		{
			for (int j = 0; j < m_gray.cols; j++)
			{
				if (i == 0)
				{
					tmp.at<int>(i, j) = abs(m_gray.at<uchar>(i + 1, j) - m_gray.at<uchar>(i, j));
				}
				else
				{
					tmp.at<int>(i, j) = abs(m_gray.at<uchar>(i, j) - m_gray.at<uchar>(i - 1, j));
				}

				if (j == 0)
				{
					tmp.at<int>(i, j) += abs(m_gray.at<uchar>(i, j + 1) - m_gray.at<uchar>(i, j));
				}
				else
				{
					tmp.at<int>(i, j) += abs(m_gray.at<uchar>(i, j) - m_gray.at<uchar>(i, j - 1));
				}
			}
		}
	}

	if (para == 2) //operator 2
	{
		for (int i = 0; i < m_gray.rows; i++)
		{
			for (int j = 0; j < m_gray.cols; j++)
			{
				if (i == m.rows - 1)
				{
					tmp.at<int>(i, j) = abs(m_gray.at<uchar>(i, j) - m_gray.at<uchar>(i - 1, j));
				}
				else
				{
					tmp.at<int>(i, j) = abs(m_gray.at<uchar>(i + 1, j) - m_gray.at<uchar>(i, j));
				}

				if (j == m.cols - 1)
				{
					tmp.at<int>(i, j) += abs(m_gray.at<uchar>(i, j + 1) - m_gray.at<uchar>(i, j));
				}
				else
				{
					tmp.at<int>(i, j) += abs(m_gray.at<uchar>(i, j + 1) - m_gray.at<uchar>(i, j - 1)) / 2;
				}
			}
		}
	}

	if (para == 3) //operator 3
	{
		for (int i = 0; i < m.rows; i++)
		{
			for (int j = 0; j < m.cols; j++)
			{
				if (i == 0)
				{
					tmp.at<int>(i, j) = abs(m_gray.at<uchar>(i + 1, j) - m_gray.at<uchar>(i, j));
				}
				else if (i == m.rows - 1)
				{
					tmp.at<int>(i, j) = abs(m_gray.at<uchar>(i, j) - m_gray.at<uchar>(i - 1, j));
				}
				else
				{
					tmp.at<int>(i, j) = abs(m_gray.at<uchar>(i + 1, j) - m_gray.at<uchar>(i - 1, j)) / 2;
				}

				if (j == 0)
				{
					tmp.at<int>(i, j) += abs(m_gray.at<uchar>(i, j + 1) - m_gray.at<uchar>(i, j));
				}
				else if (j == m.cols - 1)
				{
					tmp.at<int>(i, j) += abs(m_gray.at<uchar>(i, j) - m_gray.at<uchar>(i, j - 1));
				}
				else
				{
					tmp.at<int>(i, j) += abs(m_gray.at<uchar>(i, j + 1) - m_gray.at<uchar>(i, j - 1)) / 2;
				}
			}
		}
	}

	if (para == 4) //sobel operator 
	{
		Mat tmp1(m_gray.rows, m_gray.cols, CV_32SC1);
		cvSobel(&m_gray, &tmp1, 1, 1, 3);

		for (int i = 0; i < tmp1.rows; i++)
		{
			for (int j = 0; j < tmp1.cols; j++)
			{
				if (i == 0)
				{
					tmp.at<int>(i, j) = abs(tmp1.at<int>(i + 1, j) - tmp1.at<int>(i, j));
				}
				else if (i == m.rows - 1)
				{
					tmp.at<int>(i, j) = abs(tmp1.at<int>(i, j) - tmp1.at<int>(i - 1, j));
				}
				else
				{
					tmp.at<int>(i, j) = abs(tmp1.at<int>(i + 1, j) - tmp1.at<int>(i - 1, j)) / 2;
				}

				if (j == 0)
				{
					tmp.at<int>(i, j) += abs(tmp1.at<int>(i, j + 1) - tmp1.at<int>(i, j));
				}
				else if (j == tmp.cols - 1)
				{
					tmp.at<int>(i, j) += abs(tmp1.at<int>(i, j) - tmp1.at<int>(i, j - 1));
				}
				else
				{
					tmp.at<int>(i, j) += abs(tmp1.at<int>(i, j + 1) - tmp1.at<int>(i, j - 1)) / 2;
				}
			}
		}
	}

	return tmp;
}

int GetEnergy(Mat &energy, int i, int j)
{
	if ((i < 0) || (i >= energy.rows) || (j < 0) || (j >= energy.cols)) return -1;
	return energy.at<int>(i, j);
}

vector<int> FindVSeam(Mat &m, Mat &energy)
{
	vector<int> seam(m.rows);
	vector<vector<int>> dist(m.rows,vector<int>(m.cols));
	vector<vector<int>> father(m.rows, vector<int>(m.cols));
	
	for (int i = 0; i < m.rows; i++)
	{
		for (int j = 0; j < m.cols; j++)
		{
			if (i == 0) dist[i][j] = 0;
			else dist[i][j] = inf;
		}
	}

	for (int i = 0; i < m.rows-1; i++)
	{
		for (int j = 0; j < m.cols; j++)
		{
			if ((GetEnergy(energy, i + 1, j - 1) != -1) && (GetEnergy(energy, i + 1, j - 1) + dist[i][j] < dist[i + 1][j - 1]))
			{
				dist[i + 1][j - 1] = GetEnergy(energy, i + 1, j - 1) + dist[i][j];
				father[i + 1][j - 1] = 1;
			}
			if ((GetEnergy(energy, i + 1, j) != -1) && (GetEnergy(energy, i + 1, j) + dist[i][j] < dist[i + 1][j]))
			{
				dist[i + 1][j] = GetEnergy(energy, i + 1, j) + dist[i][j];
				father[i + 1][j] = 0;
			}
			if ((GetEnergy(energy, i + 1, j + 1) != -1) && (GetEnergy(energy, i + 1, j + 1) + dist[i][j] < dist[i + 1][j + 1]))
			{
				dist[i + 1][j + 1] = GetEnergy(energy, i + 1, j + 1) + dist[i][j];
				father[i + 1][j + 1] = -1;
			}
		}
	}

	int min = inf;
	int index = -1;
	for (int i = 0; i < m.cols; i++)
	{
		if (dist[m.rows - 1][i] < min)
		{
			min = dist[m.rows - 1][i];
			index = i;
		}
	}

	for (int i = m.rows - 1; i >= 0; i--)
	{
		seam[i] = index;
		index += father[i][index];
	}

	return seam;
}

vector<int> FindHSeam(Mat &m, Mat &energy)
{
	transpose(m, m);
	transpose(energy, energy);
	vector<int> seam = FindVSeam(m, energy);
	transpose(m, m);
	transpose(energy, energy);
	return seam;
}

void RemoveVSeam(Mat &m, Mat &energy, vector<int> seam)
{
	for (int i = 0; i < m.rows; i++)
		for (int j = seam[i]; j < m.cols-1; j++)
		{
			m.at<Vec3b>(i, j) = m.at<Vec3b>(i, j + 1);
		}
	transpose(m, m);
	m.pop_back(1);
	transpose(m, m);
	energy = ComputeEnergy(m,5);
}

void RemoveHSeam(Mat &m, Mat &energy, vector<int> seam)
{
	transpose(m, m);
	transpose(energy, energy);
	RemoveVSeam(m, energy, seam);
	transpose(m, m);
	transpose(energy, energy);
}

void InsertHSeam(Mat &m, Mat &energy, vector<int> seam, int colored)
{
	Mat tmp(1, m.cols, CV_8UC3);
	m.push_back(tmp);

	for (int i = 0; i < m.cols; i++)
		for (int j = m.rows-1; j > seam[i]; j--)
		{
			m.at<Vec3b>(j, i) = m.at<Vec3b>(j-1, i);
		}
	
	if (colored == 1)
	{
		for (int i = 0; i < m.cols; i++)
		{
			m.at<Vec3b>(seam[i],i) = Vec3b(0, 0, 255);
		}
		return;
	}

	energy = ComputeEnergy(m,5);
}

void InsertVSeam(Mat &m, Mat &energy, vector<int> seam, int colored)
{
	transpose(m, m);
	transpose(energy, energy);
	InsertHSeam(m, energy, seam, colored);
	transpose(m, m);
	transpose(energy, energy);
}

void ShowVSeam(Mat &m, vector<int> seam)
{
	Mat tmp = m;

	for (int i = 0; i < m.rows; i++)
		tmp.at<Vec3b>(i, seam[i]) = Vec3b(0, 0, 255);

	IplImage Ipl_tmp = tmp;
	cvShowImage("result", &Ipl_tmp);
	waitKey(0);
}

void ShowHSeam(Mat &m, vector<int> seam)
{
	transpose(m, m);
	Mat tmp = m;

	for (int i = 0; i < m.rows; i++)
		tmp.at<Vec3b>(i, seam[i]) = Vec3b(0, 0, 255);

	transpose(tmp, tmp);
	IplImage Ipl_tmp = tmp;
	cvShowImage("result", &Ipl_tmp);
	waitKey(0);
	transpose(m, m);
}

void Init(Mat &m, Mat &energy, char* filepath)
{
	IplImage *tmp = cvLoadImage(filepath);
	m = Mat(tmp, true);
	energy = ComputeEnergy(m,5);
}

void Work(Mat &m, Mat &energy, char* savepath, int VCarvnum, int HCarvnum)
{
	vector<int> seam;
	vector<vector<int>> seams;
	seams.clear();
	
	Mat m_gray = Mat(m.rows, m.cols, CV_8UC1);
	cvtColor(m, m_gray, CV_RGB2GRAY);
	Mat sobel = Mat(m.rows, m.cols, CV_8UC1);
	Sobel(m_gray, sobel, -1, 1, 1);
	imwrite("C:/Users/sam/Documents/Visual Studio 2015/Projects/ConsoleApplication5/Debug/sobel/s10.png", sobel);

	for (int i = 0; i < VCarvnum; i++)
	{
		seam = FindVSeam(m, energy);
		RemoveVSeam(m, energy, seam);
		seams.push_back(seam);
	}

	for (int i = 0; i < HCarvnum; i++)
	{
		seam = FindHSeam(m, energy);
		RemoveHSeam(m, energy, seam);
		seams.push_back(seam);
	}

	//for (int i = 0; i < VCarvnum; i++)
	//{
	//	seam = FindVSeam(m, energy);
	//	RemoveVSeam(m, energy, seam);
	//	seams.push_back(seam);
	//}

	//for (int i = 0; i < HCarvnum; i++)
	//{
	//	seam = FindHSeam(m, energy);
	//	InsertHSeam(m, energy, seam, 0);
	//	seams.push_back(seam);
	//}

	IplImage tmp = m;
	cvSaveImage(savepath, &tmp);

	for (int i = 0; i < HCarvnum; i++)
	{
		//printf("Hseam%d\n", i);
		seam = seams[seams.size() - 1];
		seams.pop_back();
		InsertHSeam(m, energy, seam, 1);
	}
	for (int i = 0; i < VCarvnum; i++)
	{
		//printf("Vseam%d\n", i);
		seam = seams[seams.size() - 1];
		seams.pop_back();
		InsertVSeam(m, energy, seam, 1);
	}

	IplImage tmp1 = m;
	cvSaveImage("C:/Users/sam/Documents/Visual Studio 2015/Projects/ConsoleApplication5/Debug/sobel/b10.png", &tmp1);
}

int main()
{
	Mat m,energy,m_gray;
	int VCarvnum, HCarvnum;
	freopen("C:/Users/sam/Documents/Visual Studio 2015/Projects/ConsoleApplication5/Debug/para.txt", "r",stdin);
	scanf("%d %d", &VCarvnum, &HCarvnum);
	Init(m, energy,"C:/Users/sam/Documents/Visual Studio 2015/Projects/ConsoleApplication5/Debug/10.jpg");
	Work(m, energy, "C:/Users/sam/Documents/Visual Studio 2015/Projects/ConsoleApplication5/Debug/sobel/a10.png", VCarvnum, HCarvnum);
	fclose(stdin);
	return 0;
}