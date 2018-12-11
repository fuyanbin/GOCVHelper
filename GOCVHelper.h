	//���ƣ�GOCVHelper0.7b.cpp
	//���ܣ�ͼ�����MFC��ǿ
	//���ߣ�jsxyhelu(1755311380@qq.com http://jsxyhelu.cnblogs.com)
	//��֯��GREENOPEN
	//���ڣ�2016-09-24
	#include "stdafx.h"
	#include <windows.h>
	#include <iostream>
	#include <fstream>
	#include <cstdlib>
	#include <io.h>
	#include <stdlib.h>
	#include <stdio.h>
	#include <vector>

	using namespace std;
	using namespace cv;

	#define  DIRECTION_X 0
	#define  DIRECTION_Y 1
	//�����㷨������Opencv��Mfc��ȷ���õĻ����¡�
	//�������� ��Ŀ-����-��������-����-�ַ��� ����Ϊ ʹ�ö��ֽ��ַ���
	//�� ��Ŀ-����-��������-c/c++-Ԥ������-Ԥ���������� ���� _CRT_SECURE_NO_WARNINGS
	namespace GO{
		//��ȡ�ҶȻ��ɫͼƬ���Ҷ�
		Mat imread2gray(string path);
		//���������޵�threshold
		Mat threshold2(Mat src,int minvalue,int maxvalue);
		//����Ӧ���޵�canny�㷨 
		Mat canny2(Mat src);
		void AdaptiveFindThreshold( Mat src,double *low,double *high,int aperture_size=3);
		void _AdaptiveFindThreshold(CvMat *dx, CvMat *dy, double *low, double *high);
		//���׶�
		/*ʹ������
		Mat src = imread2gray("E:\\sandbox\\pcb.png");
		Mat dst ;
		threshold(src,dst,100,255,THRESH_BINARY);
		dst = fillHoles(dst);
		imshow("src",src);
		imshow("dst",dst);
		waitKey();*/
		Mat fillHoles(Mat src);
		float getWhiteRate(Mat src);
		Mat getInnerHoles(Mat src);
		//��ñȥ���,radiusΪģ��뾶
		Mat moveLightDiff(Mat src,int radius = 40);
		//�� DEPTH_8U�Ͷ�ֵͼ�����ϸ��  �����Zhang���п���ϸ���㷨
		void thin(const Mat &src, Mat &dst, const int iterations=100);
		//ʹ��rect�����͸��
		Mat translucence(Mat src,Rect rect,int idepth = 90);
		//ʹ��rect�������������
		Mat mosaic(Mat src,Rect rect,int W = 18,int H = 18);
		//������ɫֱ��ͼ�ľ������
		double GetHsVDistance(Mat src_base,Mat src_test1);
		/*ʹ�÷���
		//�������Ҷȵ�mix
		Mat src = imread("E:\\sandbox\\lena.jpg");
		Mat mask = imread("E:\\sandbox\\star.png");
		Mat maskF(src.size(),CV_32FC3);
		Mat srcF(src.size(),CV_32FC3);
		Mat dstF(src.size(),CV_32FC3);
		src.convertTo(srcF,CV_32FC3);
		mask.convertTo(maskF,CV_32FC3);
		srcF = srcF /255;
		maskF = maskF/255;
		Mat dst(srcF);
		//��Ƭ����
		Multiply(srcF,maskF,dstF);
		dstF = dstF *255;
		dstF.convertTo(dst,CV_8UC3);
		imshow("��Ƭ����.jpg",dst);
		// Color_Burn ��ɫ����
		Color_Burn(srcF,maskF,dstF);
		dstF = dstF *255;
		dstF.convertTo(dst,CV_8UC3);
		imshow("��ɫ����.jpg",dst);
		// ������ǿ
		Linear_Burn(srcF,maskF,dstF);
		dstF = dstF *255;
		dstF.convertTo(dst,CV_8UC3);
		imshow("������ǿ.jpg",dst);
		waitKey();*/
		// Multiply ��Ƭ����
		void Multiply(Mat& src1, Mat& src2, Mat& dst);
		// Color_Burn ��ɫ����
		void Color_Burn(Mat& src1, Mat& src2, Mat& dst);
		// ������ǿ
		void Linear_Burn(Mat& src1, Mat& src2, Mat& dst);
		//----------------------------------------------------------------------------------------------------------------------------------------//
		//ʹ�÷���    ACE(src);
		//��˷� elementWiseMultiplication
		Mat EWM(Mat m1,Mat m2);
		//ͼ��ֲ��Աȶ���ǿ�㷨
		Mat ACE(Mat src,int C = 4,int n=20,int MaxCG = 5);
		//LocalNormalization�㷨
		Mat LocalNormalization(Mat float_gray,float sigma1,float sigma2);
		//----------------------------------------------------------------------------------------------------------------------------------------//
		//Ѱ����������
		VP FindBigestContour(Mat src);
		//Ѱ�Ҳ����Ƴ���ɫ��ͨ����
		vector<VP> connection2(Mat src,Mat& draw);
		vector<VP> connection2(Mat src);
		//���������������С����ѡ��
		/*ʹ�÷���
		Mat src = imread2gray("E:\\sandbox\\connection.png");
		Mat dst;
		vector<VP> contours;
		vector<VP> results;
		threshold(src,src,100,255,THRESH_BINARY);
		contours = connection2(src);
		results = selectShapeArea(src,dst,contours,1,9999);
		imshow("src",src);
		imshow("dst",dst);
		waitKey();
		*/
		vector<VP>  selectShapeArea(Mat src,Mat& draw,vector<VP> contours,int minvalue,int maxvalue);
		vector<VP>  selectShapeArea(vector<VP> contours,int minvalue,int maxvalue);
		//����������Բ�����Խ���ѡ��
		vector<VP>  selectShapeArea(Mat src,Mat& draw,vector<VP> contours,int minvalue,int maxvalue);
		vector<VP>  selectShapeArea(vector<VP> contours,int minvalue,int maxvalue);
		//����������Բ������
		float calculateCircularity(VP contour);
		//��������֮��ľ���
		float getDistance(Point2f f1,Point2f f2);
		//ͶӰ��x��Y����,�ϲ���Ϊvup,�²���Ϊvdown,gapΪ�����
		void projection2(Mat src,vector<int>& vup,vector<int>& vdown,int direction = DIRECTION_X,int gap = 10);
		//�������
		/*
		int main(int argc, char* argv[])
		{
		string FileName_S="e:/template/input.png";
		Mat src = imread(FileName_S,0);
		Mat dst;
		imshow("src",src);
		bitwise_not(src,src);
		SmoothEdgeSingleChannel(src,dst,2.5,1.0,254);
		imshow("dst",dst);
		waitKey();
		}
		*/
		bool SmoothEdgeSingleChannel( Mat mInput,Mat &mOutput, double amount, double radius, uchar Threshold) ;
		//----------------------------------------------------------------------------------------------------------------------------------------//
		//�ݹ��ȡĿ¼��ȫ���ļ�
		void getFiles(string path, vector<string>& files,string flag ="r"/*�������ݹ����ﲻдr�Ϳ���*/);
		//�ݹ��ȡĿ¼��ȫ��ͼƬ
		void getFiles(string path, vector<Mat>& files,string flag = "r");
		//�ݹ��ȡĿ¼��ȫ��ͼƬ������
		void getFiles(string path, vector<pair<Mat,string>>& files,string flag="r");
		//ɾ��Ŀ¼�µ�ȫ���ļ�
		void deleteFiles(string path,string flag = "r");
		//��������дĿ¼�µ�csv�ļ�,��д���ļ�λ��-���ࡱ��
		int writeCsv(const string& filename,const vector<pair<string,string>>srcVect,char separator=';');
		//��ȡĿ¼�µ�csv�ļ�,��á��ļ�λ��-���ࡱ��
		vector<pair<string,string>> readCsv(const string& filename, char separator = ';') ;
		//----------------------------------------------------------------------------------------------------------------------------------------//
		//C++��spilt����
		void SplitString(const string& s, vector<string>& v, const string& c);
		//! ͨ���ļ������ƻ�ȡ�ļ�������������׺
		void getFileName(const string& filepath, string& name,string& lastname);
		void getFileName(const string& filepath, string& name);
		//-----------------------------------------------------------------------------------------------------------------------------------------//
		//ini ����
		CString  GetInitString( CString Name1 ,CString Name2);
		void WriteInitString( CString Name1 ,CString Name2 ,CString strvalue);
		//-----------------------------------------------------------------------------------------------------------------------------------------//
		//excel����
		CString ExportListToExcel(CString  sExcelFile,CListCtrl* pList, CString strTitle);
		BOOL GetDefaultXlsFileName(CString& sExcelFile);
	}