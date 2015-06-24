#pragma once
#include "Detect_Object.h"
#include <opencv2/gpu/gpu.hpp>
/*
Создание директорий: для Windows
#include <Windows.h> // Метод CreateDirectory("c:\\dir1\\dir2", NULL)
#include <ShlObj.h> // Каскадное создание каталогов SHCreateDirectoryEx(NULL, "C:\\dir1\\dir2", NULL)
*/
// Создание каталогов в UNIX
#include <direct.h> // Метод _mkdir("\\dir1");

class Preprocessing_img : public Detect_Object
{
private:
	// Переданные регионы для поиска глаз. Регионы можно вывести при отладки.
	Rect *searchedRightEye;
	Rect *searchedLeftEye;
	Mat faceImg; // Не обработанное изображение лица.
	Mat dstImg; // Обработанное изображение лица, для алгоритма машинного обучения.
	bool doLeftAndRightSeparately; 
	Rect *storeFaceRect;
	// Координаты расположения глаз на исходном изображении
	Point *storeLeftEye;
	Point *storeRightEye;
	// Константы определяет процет различимости лица на фоне, после дополнительно обработки
	const double DESIRED_LEFT_EYE_X() {return 0.16; }
	const double DESIRED_LEFT_EYE_Y() {return 0.14; }
	const double FACE_ELLIPSE_CY() {return 0.40; }
	// Минимальное значение 0.5
	const double FACE_ELLIPSE_W() { return 0.50; }
	const double FACE_ELLIPSE_H() { return 0.80; }
	// Устанавливаем размер лица, в контейнере dstImg
	const int desiredFaceWidth()  { return 70;  }
	const int desiredFaceHeight() { return 70; }
	

void drawing_eye_circle(Mat &img)
{
	// Отображаем светло-синие круги для двух глаз.
	Scalar eyeColor = CV_RGB(0,255,255);
	
	if (storeLeftEye->x >= 0) {
		// Рисуем круг вокруг зрочка.
		circle(img, Point(storeFaceRect->x + storeLeftEye->x, storeFaceRect->y + storeLeftEye->y), 6, eyeColor, 1, CV_AA);
	}
	if (storeRightEye->x >= 0) {
		// Рисуем круг вокруг зрочка.
		circle(img, Point(storeFaceRect->x + storeRightEye->x, storeFaceRect->y + storeRightEye->y), 6, eyeColor, 1, CV_AA);
	}
}
/*
Обработка изображения:
	1. Геометрическое масштабирование, врашение и перевод с использованием функции распознования глаз
	2. Сглаживание шумов, используем двусторонний фильтр;
	3. Выравниваем гистрограм (освещенности) на левой и правой части лица
	4. Удаление фона и волос, с помошью Элиптической маски;
	Метод возвращает обработанное изображение объекта с выделенными глазами или NULL, если не может обнаружить лицо и два глаза на нем.
	Если лицо и глаза обнаружены, то координаты прямоуольника храняться в переменных: storeFaceRect, storeLeftEye, storeRightEye.
	Поиск глаз осушествляется в переданных регионах: searchedRightEye, searchedLeftEye
*/
	// Данный метод управляет работой класса Preprocessing_img
bool getPreprocessedFace(Mat srcImg)
{
	
	// Отменяем поисковую область лица, если глаза не были обнаржуены
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
	
	// Найти лицо крупным планом
	this->return_rect_object(srcImg);
	Rect &faceRect = this->get_largets_object();

	// Проверка: было ли обнаружено лицо
	if (faceRect.width > 0)
	{
		if (storeFaceRect)
			delete storeFaceRect;
		// Передаем обнаруженное лицо в прямоугольник
		storeFaceRect = new Rect(faceRect);

		// Сохраняем ссылку на исходно изображение, но преобразованное в черно-белую цветавую схему.
		Mat &tmp = return_gray_img(); 
		
		// Получаем изображение с обнаруженным лицом
		faceImg = tmp(faceRect);

		// Переменные для сохранения координат левого и правого глаза на исходном изображении - faceImg
		Point leftEye, rightEye;
		
		// Если не были обнаружены оба глаза, тогда метод вернет false.
		detectBothEyes(faceImg, return_eye1(), return_eye2(), leftEye, rightEye, searchedLeftEye, searchedRightEye);
			

		// Если память под переменные storeLeftEye, soreRightEye была выделена, то присваиваем им координаты глаз: leftEye, rightEye
		
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
		

		// Проверка: были ли обнаружены глаза
		if (storeLeftEye->x >= 0 && storeRightEye->x >= 0)
		{
			// Делаем размер изображения таким же как изображение которое использовалось в процессе обучения.
			// Выставляем оба глаза в положении идельно соответствующим оригинальному снимку.
			// С помощью врашения распологаем глаза в горизонтальном положении.
			// Находим центральную точку между парой глаз.

			Point2f eyesCenter = Point2f((storeLeftEye->x + storeRightEye->x) * 0.5f, (storeRightEye->y + storeLeftEye->y) * 0.5f);
			
			// Вычисляем угол между парой глаз.
			double dy = (storeRightEye->y - storeLeftEye->y);
			double dx = (storeRightEye->x - storeLeftEye->x);
			// По теореме Пифагора находим длинну гипотенузы
			double len = sqrt(dx*dx + dy*dy);
			// Конвертируем значение угла из радиан в градусы
			double angle = atan2(dy, dx) * 180.0/CV_PI; 

			// В теории, координаты левого глаза на полномасштабном изображении должны быть (0.19, 0.14)
			const double DESIRED_RIGHT_EYE_X = (1.0f - DESIRED_LEFT_EYE_X());
			// Вычисляем значение масштабирования исходного изображения
			double desiredLen = (DESIRED_RIGHT_EYE_X - DESIRED_LEFT_EYE_X()) * desiredFaceWidth();
			double scale = desiredLen/len;

			// Получаем матрицу преобразования для врашения и масштабирования лица под нужным углом и размером.
			Mat rot_mat = getRotationMatrix2D(eyesCenter, angle, scale);

			// Смещаем центр глаз, для вычисления центральной точки
			rot_mat.at<double>(0,2) += desiredFaceWidth() * 0.5f - eyesCenter.x;
			rot_mat.at<double>(1,2) += desiredFaceWidth() * DESIRED_LEFT_EYE_Y() - eyesCenter.y;

			// Преобразуем входное изображение: задаем размер, и угол поворота.
			// Так как входное изображение имеет соотношение 1:1, то для высоты используется переменная 'w'
			
			Mat warped = Mat(desiredFaceHeight(), desiredFaceWidth(), CV_8U, Scalar(128)); // Преобразуем выходное изображение в оттенки серового.
			warpAffine(faceImg, warped, rot_mat, warped.size());
			
			// Вывод изображения после Афинных преобразований
			// imshow("warped", warped);

			// Проверяем и при необходимости задаем стандартную ярокость и контрасть, если изображение слишком темное или слишком светлое
			if (!doLeftAndRightSeparately)
			{
				equalize_Img(warped, warped);
				equalize_Img(warped, warped);
			}
			else
			{
				// Выполняем преобразования отдельно для левой и правой части лица
				equalizeLeftAndRightHalves(warped);
				
				
			}
			// Вывод после преобразования
			// imshow("equalized", warped);

			// Для снижения уровня шума используем двусторонний фильтр, но не скглаживаем края.
			// Mat filtered_tmp = Mat(warped.size(), CV_8U);
			// bilateralFilter(warped, filtered_tmp, 0, 20.0, 2.0);
			// bilateralFilter(filtered_tmp, warped, 0, 20.0, 2.0);
			Mat filtered = Mat(warped.size(), CV_8U);
			bilateralFilter(warped, filtered, 0, 20.0, 2.0);

			// Вывод после преобразования
			// imshow("filtered", filtered);

			// Сглаживание краев лица.
			// Нарисовать элип в серидине лица.
			Mat mask = Mat(warped.size(), CV_8U, Scalar(0)); // Делаем входное изображение черно-белым

			Point faceCenter = Point(desiredFaceWidth()/2, cvRound(desiredFaceHeight() * FACE_ELLIPSE_CY()) );
			Size size = Size( cvRound(desiredFaceWidth() * FACE_ELLIPSE_W()), cvRound(desiredFaceHeight() * FACE_ELLIPSE_H()) );
			ellipse(mask, faceCenter, size, 0, 0, 360, Scalar(255), CV_FILLED);
			
			// Отладочный вывод изображения после того как в цетре лица был нарисован элипс
			// imshow("mask", mask);

			// Создаем контейнер в который будет помешено изображение после фильтрации
			dstImg = Mat(warped.size(), CV_8U, Scalar(128));

			// Отладочный вывод
			/*
            namedWindow("filtered");
            imshow("filtered", filtered);
            namedWindow("dstImg");
            imshow("dstImg", dstImg);
            namedWindow("mask");
            imshow("mask", mask);
            */

			// Элипс выполняет роль маски. С помошью данной маски удаляем внешние пиксели т.е часть изображения которое находиться снаружи элипса.
			// Применяем фильтр.
			filtered.copyTo(dstImg, mask);
			//filtered.copyTo(dstImg, mask);
			//filtered.copyTo(dstImg, mask);
			
			// Отладочный вывод после фильтрации.
			// imshow("dstImg", dstImg);
		} 
		
		return true;
	}

	// Если лицо не было найдено, то вернем пустое изображение
	return false;
}


/*
	Поиск глаз на лице. Можно передать второй каскад eyeCascade, для поиска очков.
	Координаты искомых объектов сохраняются в переменных: leftEye, rightEye.
*/
bool detectBothEyes(const Mat &face, CascadeClassifier &eyeCascade1, CascadeClassifier &eyeCascade2, Point &leftEye, Point &reghtEye, Rect *searchedRightEye /* = NULL */, Rect *searchedLeftEye /* = NULL */)
{
	// Находим оба глаза, с вероятностью 40%, но не выявляем открыты глаза или нет.
	const float EYE_SX = 0.16f;
	const float EYE_SY = 0.26f;
	const float EYE_SW = 0.30f;
	const float EYE_SH = 0.28f;
	
	// Функция cvRound() производит округление вещественного числа до ближайшего целого.
	int leftX = cvRound(face.cols * EYE_SX);
	int topY = cvRound(face.rows * EYE_SY);
	int widthX = cvRound(face.cols * EYE_SW);
	int heightY = cvRound(face.rows * EYE_SH);
	
	// Вычисляем координаты по X правого глаза.
	int rightX = cvRound(face.cols * (1.0-EYE_SX-EYE_SW) );
	/* Разделим лицо на левую и правую часть.
	   Каждую половину сохраним в отдельных переменных: topLeftOfFace, topRightOfFace.
	*/
	Mat topLeftOfFace = face(Rect(leftX, topY, widthX, heightY));

	Mat topRightOfFace = face(Rect(rightX, topY, widthX, heightY));
	// Создадим переменные для хранения прямоугольников, выдедяюших правый и левый глаз на изображении.
	Rect leftEyeRect, rightEyeRect;

	// Сохраним в указателях координаты прямоугольников выделяющих левый и правый глаз
		if (searchedLeftEye)
			delete searchedLeftEye;

		searchedLeftEye = new Rect(leftX, topY, widthX, heightY);
	
		if (searchedRightEye)
			delete searchedRightEye;

		searchedRightEye = new Rect(rightX, topY, widthX, heightY);
	
	
	// Поиск искомого объекта в правой и левой области изображения при помоши 1-го детектора
	leftEyeRect = this->return_object_eyeCascade1(topLeftOfFace, topLeftOfFace.cols);
	
	// Если в левой области, глаз не были найдены 1-вым детектором, то пробуем второй.
	if (leftEyeRect.width <= 0)
	{
		leftEyeRect = this->return_object_eyeCascade2(topLeftOfFace, topLeftOfFace.cols);
	}
	
	rightEyeRect = this->return_object_eyeCascade1(topRightOfFace,  topRightOfFace.cols);
	
	// Если в правой области глаз не был найден, то пробуем 2-ой детектор
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


	// Проверка - были ли обнаружены глаза
	// Проверка обнаружения левого глаза
	if (leftEyeRect.width > 0)
	{
		// Так как границы лица были удалены, необходимо вычислить координаты вершин прямоугольника для левого глаза
		leftEyeRect.x += leftX;
		leftEyeRect.y += topY;
		leftEye = Point(leftEyeRect.x + leftEyeRect.width/2, leftEyeRect.y + leftEyeRect.height/2);
	} else
		// Если глаза не были обнаржены, возвращаем точку с координатами -1,-1
		leftEye = Point(-1, -1);

	// Проверка обнаружения правого глаза
	if (rightEyeRect.width > 0) 
	{ 
		// Вычисление координат вершин прямоугольника для правого глаза.
		rightEyeRect.x += rightX; 
		rightEyeRect.y += topY;  
		reghtEye = Point(rightEyeRect.x + rightEyeRect.width/2, rightEyeRect.y + rightEyeRect.height/2);
	}
	else
	// Если правый глаз не был найден, точки - reghtEye, присваиваем координаты (-1, -1)
		reghtEye = Point(-1, -1);

	if ((leftEye.x == -1 && leftEye.y == -1)  || (reghtEye.x == -1 && reghtEye.y == -1))
		return false;
	else
		return true;

}

// Нормализация изображения для левой и правой части лица
void equalizeLeftAndRightHalves(Mat &faceImg)
{
	// Если мы сделает варавнивание освешенности для всего изображения, то половина лица будет слабо освешено, а вторая половина будет засвечина.
	// По этому производим выравнивание освешения для левой и правой части лица по отдельности.
	// Для сглаживания грани между половинами лица, производим выравнивание освешенности для всего изображения и 
	// производим смешивание освешенности трех изображения в центре исходного изображения. Смешивание производим наложением правых и левых чисте на 
	// нормализованное по освешенности исходное изображение.

	int w = faceImg.cols;
	int h = faceImg.rows;

	// 1. Варавним освешенность для всего лица
	Mat wholeFace;
	equalize_Img(faceImg, wholeFace);

	// 2. Делим исходное изображение на две части и выравниваем осшенность для левой и правой части лица по отдельности
	int midX = w/2;
	Mat leftSide = faceImg(Rect(0,0, midX,h));
	Mat rightSide = faceImg(Rect(midX,0, w-midX,h));

	equalize_Img(leftSide, leftSide);
	equalize_Img(rightSide, rightSide);
	

	// 3. Произведем смешивание трех изображения: wholeFace, leftSide, reghtSide
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			int v;
			// Берем с лева 25%
			if (x < w/4) 
				v = leftSide.at<uchar>(y, x);
			else if (x < w*2/4) // произведем смешивание 25% части левой половины со всем изображением
			{
				int lv = leftSide.at<uchar>(y,x);
				int wv = wholeFace.at<uchar>(y,x);
				// Произведем смешивание
				float f = (x-w*1/4)/static_cast<float>(w*0.25f);
				v = cvRound((1.0f -f) * lv + (f) * wv);

			}
			else if (x < w * 3/4) // произведем смешивание 25% части правой половины со всем изображением
			{
				int rv = rightSide.at<uchar>(y, x-midX);
				int wv = wholeFace.at<uchar>(y, x);
				// Смешивание
				float f = (x - w * 2/4)/static_cast<float>(w*0.25f);
				v = cvRound((1.0f - f) * wv + (f) * rv);
			} else // Используем правую часть лица без смешивания
				v = rightSide.at<uchar>(y, x-midX);

			faceImg.at<uchar>(y, x) = v;
		} 
	} // конец цикла y
	// Если мы сделает варавнивание освешенности для всего изображения, то половина лица будет слабо освешено, а вторая половина будет засвечина.
	// По этому производим выравнивание освешения для левой и правой части лица по отдельности.
	// Для сглаживания грани между половинами лица, производим выравнивание освешенности для всего изображения и 
	// производим смешивание освешенности трех изображения в центре исходного изображения. Смешивание производим наложением правых и левых чисте на 
	// нормализованное по освешенности исходное изображение.


	// Отладка
	// imshow("leftSide", leftSide);
	// imshow("rightSide", rightSide);

}

public:

		Preprocessing_img()
			: Detect_Object(), storeFaceRect(NULL), storeLeftEye(NULL), storeRightEye(NULL), searchedLeftEye(NULL), searchedRightEye(NULL)
		{

		}

		// Принимаем на обработку кадр, выполняем детектирование глаз и возвращаем обработанный кадр
		Mat & preprocessing_frame(Mat &frm, Mat &dst_mashin_learning)
		{

			// Если лицо и глаза были обнаружены 
			if (getPreprocessedFace(frm))
			{
				dst_mashin_learning = return_frame_mashin_learning();
				// Выделить лицо
				rectangle(frm, *storeFaceRect, Scalar(250,250,200), 1);
				//Выделить глаза
				drawing_eye_circle(frm);
				// Для сигнализации, того что снимок был сделан, на лице отображаем белую вспышку.
				Mat displayedFaceRegion = frm(*storeFaceRect);
				displayedFaceRegion+= CV_RGB(90,90,90);
				// Вернуть обработанный кадр.
				return frm;
				
			}
			// Если лицо не было обнаружено.
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