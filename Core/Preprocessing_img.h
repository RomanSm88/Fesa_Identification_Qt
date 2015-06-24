#pragma once
#include "Detect_Object.h"
#include <opencv2/gpu/gpu.hpp>
/*
�������� ����������: ��� Windows
#include <Windows.h> // ����� CreateDirectory("c:\\dir1\\dir2", NULL)
#include <ShlObj.h> // ��������� �������� ��������� SHCreateDirectoryEx(NULL, "C:\\dir1\\dir2", NULL)
*/
// �������� ��������� � UNIX
#include <direct.h> // ����� _mkdir("\\dir1");

class Preprocessing_img : public Detect_Object
{
private:
	// ���������� ������� ��� ������ ����. ������� ����� ������� ��� �������.
	Rect *searchedRightEye;
	Rect *searchedLeftEye;
	Mat faceImg; // �� ������������ ����������� ����.
	Mat dstImg; // ������������ ����������� ����, ��� ��������� ��������� ��������.
	bool doLeftAndRightSeparately; 
	Rect *storeFaceRect;
	// ���������� ������������ ���� �� �������� �����������
	Point *storeLeftEye;
	Point *storeRightEye;
	// ��������� ���������� ������ ������������ ���� �� ����, ����� ������������� ���������
	const double DESIRED_LEFT_EYE_X() {return 0.16; }
	const double DESIRED_LEFT_EYE_Y() {return 0.14; }
	const double FACE_ELLIPSE_CY() {return 0.40; }
	// ����������� �������� 0.5
	const double FACE_ELLIPSE_W() { return 0.50; }
	const double FACE_ELLIPSE_H() { return 0.80; }
	// ������������� ������ ����, � ���������� dstImg
	const int desiredFaceWidth()  { return 70;  }
	const int desiredFaceHeight() { return 70; }
	

void drawing_eye_circle(Mat &img)
{
	// ���������� ������-����� ����� ��� ���� ����.
	Scalar eyeColor = CV_RGB(0,255,255);
	
	if (storeLeftEye->x >= 0) {
		// ������ ���� ������ ������.
		circle(img, Point(storeFaceRect->x + storeLeftEye->x, storeFaceRect->y + storeLeftEye->y), 6, eyeColor, 1, CV_AA);
	}
	if (storeRightEye->x >= 0) {
		// ������ ���� ������ ������.
		circle(img, Point(storeFaceRect->x + storeRightEye->x, storeFaceRect->y + storeRightEye->y), 6, eyeColor, 1, CV_AA);
	}
}
/*
��������� �����������:
	1. �������������� ���������������, �������� � ������� � �������������� ������� ������������� ����
	2. ����������� �����, ���������� ������������ ������;
	3. ����������� ���������� (������������) �� ����� � ������ ����� ����
	4. �������� ���� � �����, � ������� ������������ �����;
	����� ���������� ������������ ����������� ������� � ����������� ������� ��� NULL, ���� �� ����� ���������� ���� � ��� ����� �� ���.
	���� ���� � ����� ����������, �� ���������� ������������� ��������� � ����������: storeFaceRect, storeLeftEye, storeRightEye.
	����� ���� �������������� � ���������� ��������: searchedRightEye, searchedLeftEye
*/
	// ������ ����� ��������� ������� ������ Preprocessing_img
bool getPreprocessedFace(Mat srcImg)
{
	
	// �������� ��������� ������� ����, ���� ����� �� ���� ����������
	if (storeFaceRect)
		storeFaceRect->width = -1;
	if (storeLeftEye)
		storeLeftEye->x = -1;
	if (storeRightEye)
		storeRightEye->x = -1;
	if (searchedLeftEye)
		searchedLeftEye->width = -1;
	if (searchedRightEye)
		searchedRightEye->width = -1;
	
	// ����� ���� ������� ������
	this->return_rect_object(srcImg);
	Rect &faceRect = this->get_largets_object();

	// ��������: ���� �� ���������� ����
	if (faceRect.width > 0)
	{
		if (storeFaceRect)
			delete storeFaceRect;
		// �������� ������������ ���� � �������������
		storeFaceRect = new Rect(faceRect);

		// ��������� ������ �� ������� �����������, �� ��������������� � �����-����� �������� �����.
		Mat &tmp = return_gray_img(); 
		
		// �������� ����������� � ������������ �����
		faceImg = tmp(faceRect);

		// ���������� ��� ���������� ��������� ������ � ������� ����� �� �������� ����������� - faceImg
		Point leftEye, rightEye;
		
		// ���� �� ���� ���������� ��� �����, ����� ����� ������ false.
		detectBothEyes(faceImg, return_eye1(), return_eye2(), leftEye, rightEye, searchedLeftEye, searchedRightEye);
			

		// ���� ������ ��� ���������� storeLeftEye, soreRightEye ���� ��������, �� ����������� �� ���������� ����: leftEye, rightEye
		
		if (storeLeftEye)
		{
			delete storeLeftEye;
		}
			storeLeftEye = new Point(leftEye);
		
		
		if (storeRightEye)
		{
			delete storeRightEye;
		}
			storeRightEye = new Point (rightEye);
		

		// ��������: ���� �� ���������� �����
		if (storeLeftEye->x >= 0 && storeRightEye->x >= 0)
		{
			// ������ ������ ����������� ����� �� ��� ����������� ������� �������������� � �������� ��������.
			// ���������� ��� ����� � ��������� ������� ��������������� ������������� ������.
			// � ������� �������� ����������� ����� � �������������� ���������.
			// ������� ����������� ����� ����� ����� ����.

			Point2f eyesCenter = Point2f((storeLeftEye->x + storeRightEye->x) * 0.5f, (storeRightEye->y + storeLeftEye->y) * 0.5f);
			
			// ��������� ���� ����� ����� ����.
			double dy = (storeRightEye->y - storeLeftEye->y);
			double dx = (storeRightEye->x - storeLeftEye->x);
			// �� ������� �������� ������� ������ ����������
			double len = sqrt(dx*dx + dy*dy);
			// ������������ �������� ���� �� ������ � �������
			double angle = atan2(dy, dx) * 180.0/CV_PI; 

			// � ������, ���������� ������ ����� �� ��������������� ����������� ������ ���� (0.19, 0.14)
			const double DESIRED_RIGHT_EYE_X = (1.0f - DESIRED_LEFT_EYE_X());
			// ��������� �������� ��������������� ��������� �����������
			double desiredLen = (DESIRED_RIGHT_EYE_X - DESIRED_LEFT_EYE_X()) * desiredFaceWidth();
			double scale = desiredLen/len;

			// �������� ������� �������������� ��� �������� � ��������������� ���� ��� ������ ����� � ��������.
			Mat rot_mat = getRotationMatrix2D(eyesCenter, angle, scale);

			// ������� ����� ����, ��� ���������� ����������� �����
			rot_mat.at<double>(0,2) += desiredFaceWidth() * 0.5f - eyesCenter.x;
			rot_mat.at<double>(1,2) += desiredFaceWidth() * DESIRED_LEFT_EYE_Y() - eyesCenter.y;

			// ����������� ������� �����������: ������ ������, � ���� ��������.
			// ��� ��� ������� ����������� ����� ����������� 1:1, �� ��� ������ ������������ ���������� 'w'
			
			Mat warped = Mat(desiredFaceHeight(), desiredFaceWidth(), CV_8U, Scalar(128)); // ����������� �������� ����������� � ������� ��������.
			warpAffine(faceImg, warped, rot_mat, warped.size());
			
			// ����� ����������� ����� ������� ��������������
			// imshow("warped", warped);

			// ��������� � ��� ������������� ������ ����������� �������� � ���������, ���� ����������� ������� ������ ��� ������� �������
			if (!doLeftAndRightSeparately)
			{
				equalize_Img(warped, warped);
				equalize_Img(warped, warped);
			}
			else
			{
				// ��������� �������������� �������� ��� ����� � ������ ����� ����
				equalizeLeftAndRightHalves(warped);
				
				
			}
			// ����� ����� ��������������
			// imshow("equalized", warped);

			// ��� �������� ������ ���� ���������� ������������ ������, �� �� ����������� ����.
			// Mat filtered_tmp = Mat(warped.size(), CV_8U);
			// bilateralFilter(warped, filtered_tmp, 0, 20.0, 2.0);
			// bilateralFilter(filtered_tmp, warped, 0, 20.0, 2.0);
			Mat filtered = Mat(warped.size(), CV_8U);
			bilateralFilter(warped, filtered, 0, 20.0, 2.0);

			// ����� ����� ��������������
			// imshow("filtered", filtered);

			// ����������� ����� ����.
			// ���������� ���� � �������� ����.
			Mat mask = Mat(warped.size(), CV_8U, Scalar(0)); // ������ ������� ����������� �����-�����

			Point faceCenter = Point(desiredFaceWidth()/2, cvRound(desiredFaceHeight() * FACE_ELLIPSE_CY()) );
			Size size = Size( cvRound(desiredFaceWidth() * FACE_ELLIPSE_W()), cvRound(desiredFaceHeight() * FACE_ELLIPSE_H()) );
			ellipse(mask, faceCenter, size, 0, 0, 360, Scalar(255), CV_FILLED);
			
			// ���������� ����� ����������� ����� ���� ��� � ����� ���� ��� ��������� �����
			// imshow("mask", mask);

			// ������� ��������� � ������� ����� �������� ����������� ����� ����������
			dstImg = Mat(warped.size(), CV_8U, Scalar(128));

			// ���������� �����
			/*
            namedWindow("filtered");
            imshow("filtered", filtered);
            namedWindow("dstImg");
            imshow("dstImg", dstImg);
            namedWindow("mask");
            imshow("mask", mask);
            */

			// ����� ��������� ���� �����. � ������� ������ ����� ������� ������� ������� �.� ����� ����������� ������� ���������� ������� ������.
			// ��������� ������.
			filtered.copyTo(dstImg, mask);
			//filtered.copyTo(dstImg, mask);
			//filtered.copyTo(dstImg, mask);
			
			// ���������� ����� ����� ����������.
			// imshow("dstImg", dstImg);
		} 
		
		return true;
	}

	// ���� ���� �� ���� �������, �� ������ ������ �����������
	return false;
}


/*
	����� ���� �� ����. ����� �������� ������ ������ eyeCascade, ��� ������ �����.
	���������� ������� �������� ����������� � ����������: leftEye, rightEye.
*/
bool detectBothEyes(const Mat &face, CascadeClassifier &eyeCascade1, CascadeClassifier &eyeCascade2, Point &leftEye, Point &reghtEye, Rect *searchedRightEye /* = NULL */, Rect *searchedLeftEye /* = NULL */)
{
	// ������� ��� �����, � ������������ 40%, �� �� �������� ������� ����� ��� ���.
	const float EYE_SX = 0.16f;
	const float EYE_SY = 0.26f;
	const float EYE_SW = 0.30f;
	const float EYE_SH = 0.28f;
	
	// ������� cvRound() ���������� ���������� ������������� ����� �� ���������� ������.
	int leftX = cvRound(face.cols * EYE_SX);
	int topY = cvRound(face.rows * EYE_SY);
	int widthX = cvRound(face.cols * EYE_SW);
	int heightY = cvRound(face.rows * EYE_SH);
	
	// ��������� ���������� �� X ������� �����.
	int rightX = cvRound(face.cols * (1.0-EYE_SX-EYE_SW) );
	/* �������� ���� �� ����� � ������ �����.
	   ������ �������� �������� � ��������� ����������: topLeftOfFace, topRightOfFace.
	*/
	Mat topLeftOfFace = face(Rect(leftX, topY, widthX, heightY));

	Mat topRightOfFace = face(Rect(rightX, topY, widthX, heightY));
	// �������� ���������� ��� �������� ���������������, ���������� ������ � ����� ���� �� �����������.
	Rect leftEyeRect, rightEyeRect;

	// �������� � ���������� ���������� ��������������� ���������� ����� � ������ ����
		if (searchedLeftEye)
			delete searchedLeftEye;

		searchedLeftEye = new Rect(leftX, topY, widthX, heightY);
	
		if (searchedRightEye)
			delete searchedRightEye;

		searchedRightEye = new Rect(rightX, topY, widthX, heightY);
	
	
	// ����� �������� ������� � ������ � ����� ������� ����������� ��� ������ 1-�� ���������
	leftEyeRect = this->return_object_eyeCascade1(topLeftOfFace, topLeftOfFace.cols);
	
	// ���� � ����� �������, ���� �� ���� ������� 1-��� ����������, �� ������� ������.
	if (leftEyeRect.width <= 0)
	{
		leftEyeRect = this->return_object_eyeCascade2(topLeftOfFace, topLeftOfFace.cols);
	}
	
	rightEyeRect = this->return_object_eyeCascade1(topRightOfFace,  topRightOfFace.cols);
	
	// ���� � ������ ������� ���� �� ��� ������, �� ������� 2-�� ��������
	if (rightEyeRect.width <= 0)
	{
		rightEyeRect = this->return_object_eyeCascade2(topRightOfFace, topRightOfFace.cols);
		/*
		if (rightEyeRect.width <= 0)
		{
			cout << "Problem: no is not detector Right eye = eyeCascade2!" << endl;
			system("PAUSE");
		}
		*/
	}


	// �������� - ���� �� ���������� �����
	// �������� ����������� ������ �����
	if (leftEyeRect.width > 0)
	{
		// ��� ��� ������� ���� ���� �������, ���������� ��������� ���������� ������ �������������� ��� ������ �����
		leftEyeRect.x += leftX;
		leftEyeRect.y += topY;
		leftEye = Point(leftEyeRect.x + leftEyeRect.width/2, leftEyeRect.y + leftEyeRect.height/2);
	} else
		// ���� ����� �� ���� ���������, ���������� ����� � ������������ -1,-1
		leftEye = Point(-1, -1);

	// �������� ����������� ������� �����
	if (rightEyeRect.width > 0) 
	{ 
		// ���������� ��������� ������ �������������� ��� ������� �����.
		rightEyeRect.x += rightX; 
		rightEyeRect.y += topY;  
		reghtEye = Point(rightEyeRect.x + rightEyeRect.width/2, rightEyeRect.y + rightEyeRect.height/2);
	}
	else
	// ���� ������ ���� �� ��� ������, ����� - reghtEye, ����������� ���������� (-1, -1)
		reghtEye = Point(-1, -1);

	if ((leftEye.x == -1 && leftEye.y == -1)  || (reghtEye.x == -1 && reghtEye.y == -1))
		return false;
	else
		return true;

}

// ������������ ����������� ��� ����� � ������ ����� ����
void equalizeLeftAndRightHalves(Mat &faceImg)
{
	// ���� �� ������� ������������ ������������ ��� ����� �����������, �� �������� ���� ����� ����� ��������, � ������ �������� ����� ���������.
	// �� ����� ���������� ������������ ��������� ��� ����� � ������ ����� ���� �� �����������.
	// ��� ����������� ����� ����� ���������� ����, ���������� ������������ ������������ ��� ����� ����������� � 
	// ���������� ���������� ������������ ���� ����������� � ������ ��������� �����������. ���������� ���������� ���������� ������ � ����� ����� �� 
	// ��������������� �� ������������ �������� �����������.

	int w = faceImg.cols;
	int h = faceImg.rows;

	// 1. �������� ������������ ��� ����� ����
	Mat wholeFace;
	equalize_Img(faceImg, wholeFace);

	// 2. ����� �������� ����������� �� ��� ����� � ����������� ���������� ��� ����� � ������ ����� ���� �� �����������
	int midX = w/2;
	Mat leftSide = faceImg(Rect(0,0, midX,h));
	Mat rightSide = faceImg(Rect(midX,0, w-midX,h));

	equalize_Img(leftSide, leftSide);
	equalize_Img(rightSide, rightSide);
	

	// 3. ���������� ���������� ���� �����������: wholeFace, leftSide, reghtSide
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			int v;
			// ����� � ���� 25%
			if (x < w/4) 
				v = leftSide.at<uchar>(y, x);
			else if (x < w*2/4) // ���������� ���������� 25% ����� ����� �������� �� ���� ������������
			{
				int lv = leftSide.at<uchar>(y,x);
				int wv = wholeFace.at<uchar>(y,x);
				// ���������� ����������
				float f = (x-w*1/4)/static_cast<float>(w*0.25f);
				v = cvRound((1.0f -f) * lv + (f) * wv);

			}
			else if (x < w * 3/4) // ���������� ���������� 25% ����� ������ �������� �� ���� ������������
			{
				int rv = rightSide.at<uchar>(y, x-midX);
				int wv = wholeFace.at<uchar>(y, x);
				// ����������
				float f = (x - w * 2/4)/static_cast<float>(w*0.25f);
				v = cvRound((1.0f - f) * wv + (f) * rv);
			} else // ���������� ������ ����� ���� ��� ����������
				v = rightSide.at<uchar>(y, x-midX);

			faceImg.at<uchar>(y, x) = v;
		} 
	} // ����� ����� y
	// ���� �� ������� ������������ ������������ ��� ����� �����������, �� �������� ���� ����� ����� ��������, � ������ �������� ����� ���������.
	// �� ����� ���������� ������������ ��������� ��� ����� � ������ ����� ���� �� �����������.
	// ��� ����������� ����� ����� ���������� ����, ���������� ������������ ������������ ��� ����� ����������� � 
	// ���������� ���������� ������������ ���� ����������� � ������ ��������� �����������. ���������� ���������� ���������� ������ � ����� ����� �� 
	// ��������������� �� ������������ �������� �����������.


	// �������
	// imshow("leftSide", leftSide);
	// imshow("rightSide", rightSide);

}

public:

		Preprocessing_img()
			: Detect_Object(), storeFaceRect(NULL), storeLeftEye(NULL), storeRightEye(NULL), searchedLeftEye(NULL), searchedRightEye(NULL)
		{

		}

		// ��������� �� ��������� ����, ��������� �������������� ���� � ���������� ������������ ����
		Mat & preprocessing_frame(Mat &frm, Mat &dst_mashin_learning)
		{

			// ���� ���� � ����� ���� ���������� 
			if (getPreprocessedFace(frm))
			{
				dst_mashin_learning = return_frame_mashin_learning();
				// �������� ����
				rectangle(frm, *storeFaceRect, Scalar(250,250,200), 1);
				//�������� �����
				drawing_eye_circle(frm);
				// ��� ������������, ���� ��� ������ ��� ������, �� ���� ���������� ����� �������.
				Mat displayedFaceRegion = frm(*storeFaceRect);
				displayedFaceRegion+= CV_RGB(90,90,90);
				// ������� ������������ ����.
				return frm;
				
			}
			// ���� ���� �� ���� ����������.
				dst_mashin_learning.data = 0;
				
				return frm;
		}
		Mat & return_frame_mashin_learning()
		{
			return dstImg;
		}

virtual ~Preprocessing_img()
	{
		if (storeFaceRect)
			delete storeFaceRect;
		if (storeLeftEye)
			delete storeLeftEye;
		if (storeRightEye)
			delete storeRightEye;
		if (searchedRightEye)
			delete searchedRightEye;
		if (searchedLeftEye)
			delete searchedLeftEye;
		
	}
};