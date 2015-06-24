#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>

using namespace std;
using namespace cv;

class Detect_Object
{
private:
	// Частное от колличества столбцов в исходной матрицы на переменную scaleWidth. Используется для восстановления изображения к исходному размеру.
	float scale;
	Size minFeatureSize;
	// искать только одно лицо изображенное крупным планом
	int flags;
	int minNeighboard;
	float searchScaleFactor;
	int minFeatureSize_a;
	int minFeatureSize_b;  
	int minNeigh; 
	float scale_factor;
	Rect largestObject; // Координаты искомого объекта.
	Mat finish_frame; // Обработанный кадр
	Mat gray_frame;
	CascadeClassifier faceCascade;
	CascadeClassifier eyeCascade;
	CascadeClassifier eyeCascade2;
	
	// Метод восстанавливает изображение до исходного размера и проверяет, находиться ли искомый объект в центре изображения, если нет то производиться
	// преобрабзование координат, так чтобы искомый объект был в центре изображения
	void resize_img_and_edit_koordinatx(const Mat &img, int scaleWidth, vector<Rect> &object)
	{
		// Восстанавливаем исходный размер изображения и выделяем область с объектом, прямоугольником.
		if (img.cols > scaleWidth)
		{
			for (int i = 0; i < (int)object.size(); i++)
			{
				object[i].x = cvRound(object[i].x * scale);
				object[i].y = cvRound(object[i].y * scale);
				object[i].width = cvRound(object[i].width * scale);
				object[i].height = cvRound(object[i].height * scale);

			}

		}
		// Проверяем находится ли обнаруженный объект в центре изображения, если нет выполняем преобразования координат так чтобы объект был в центре изображения

		for (int i = 0; i < (int)object.size(); i++)
		{
			if (object[i].x < 0)
				object[i].x = 0;

			if (object[i].y < 0)
				object[i].y = 0;

			// Если на изображении только половина искомого объекта
			if (object[i].x + object[i].width > img.cols)
				object[i].x = img.cols - object[i].width;

			if (object[i].y + object[i].height > img.rows)
				object[i].y = img.rows - object[i].height;
		}
	}
	// Предварительная обработка входного кадра.
void obrabotka_kadra(const Mat &img, int scaleWidth)
	{
		// Проверяем цветаваю  схему изображения и при необходимости преобразуем его в оттенки серого.

		// Определяем цветовую схему по колличеству каналов. Если изображение черно-белое (оттенки серого) то колличество каналов у такого изображения = 2
		if (img.channels() == 3 || img.channels() == 4)
			cvtColor(img, gray_frame, CV_BGR2GRAY);
		else
			// Так как входное изображение уже полутоновое, то можно с ним работать без дополнительных преобразований.
			gray_frame = img;
		

		// Уменьщаем размер изображения, для ускорения работы алгоритма.
		resize_img(gray_frame, finish_frame, scaleWidth);
		
		// Выравниваем значения яркости и контрастности входного изображения.
		equalize_Img(finish_frame, finish_frame);
	}
void set_minFeatureSize(int a, int b)
	{
		if (a == b && (a+b) >= 40 && (a + b) <= 58)
			minFeatureSize = Size(a, b);
		else
			minFeatureSize = Size(20,20);
	}
void set_searchScaleFactor(float a)
	{
		// Значение детализации изображения при поиске лица. Значение должно быть больше 1
		if (a > 1.0)
			searchScaleFactor = a;
		else
			searchScaleFactor = 1.0;
	}
void set_minNeighboard(int a)
	{
		// Задаем порог детектирования. Значение должно быть в пределе 2-6
		if (a >= 2 && a <= 6)
			minNeighboard = a;
		else
			minNeighboard = 4;
	}
// Загружаем файлы каскадов.
void load_file_kascade_face(const char *faceCascadeFilename)
{
	if (!faceCascade.load(faceCascadeFilename))
	{
		cout << "Ne udalosy zagruzity: " << faceCascadeFilename << endl;
		system("PAUSE");
	}	
}
void load_file_kaskade_eye_1(const char *eyeCascadeFilename1)
{
	if (!eyeCascade.load(eyeCascadeFilename1))
	{
		cout << "Ne udalosy zagruzity: " << eyeCascadeFilename1 << endl;
		system("PAUSE");
	}	

}
void load_file_kaskade_eye_2(const char *eyeCascadeFilename2)
{
	if (!eyeCascade2.load(eyeCascadeFilename2))
	{
		cout << "Ne udalosy zagruzity: " << eyeCascadeFilename2 << endl;
		system("PAUSE");

	}
}
protected:
	CascadeClassifier & return_faceCascade()
	{
		return faceCascade;
	}
	CascadeClassifier & return_eye1()
	{
		return eyeCascade;
	}
	CascadeClassifier & return_eye2()
	{
		return eyeCascade2;
	}
	
	// Получаем ссылку на черно-белый кадр
	Mat & return_gray_img()
	{
		return gray_frame;
	}
	void get_minFeatureSize()
	{
		cout << "Skaniruyeshee okno: " << minFeatureSize.height << "x" << minFeatureSize.width << endl;
	}

	
void get_searchScaleFactor()
	{
		cout << "searchScaleFactor: " << searchScaleFactor << endl;
	}
	
void get_minNeighboard()
	{
		cout << "minNeighbors: "  << minNeighboard << endl;
	}

	
	// Выравнивание значение яркости и контрастности. Результат сохраняется в equalizeImg
void equalize_Img(Mat &src, Mat &dst)
	{
		equalizeHist(src, dst);
	}
void set_scale(const Mat &img, int scaleWidth)
	{
			
		scale = img.cols/(float)scaleWidth;

	}
void copy_frame(Mat &src, Mat &dst)
	{
		src.copyTo(dst);
	}
	// Уменьшение размера изображения если необходимо, для ускорения работы алгоритма.
void resize_img(Mat &src, Mat &dst, int scaleWidth)
	{
		set_scale(src, scaleWidth);

		if (src.cols > scaleWidth)
		{
			// Умьщаем размер изображения но сохраняем пропорцию
			int scaledHeight = cvRound(src.rows/scale);
			resize(src, dst, Size(scaleWidth, scaledHeight));
			
		}
		
	}

Rect & get_largets_object()
	{
		return largestObject;
	}
	
	// Поиск объекта, метод возвращает координаты искомого объекта на изображении.
void search_object(const Mat &img, CascadeClassifier &cascade, int scaleWidth)
	{
		vector<Rect> object; // Контейнер для хранения координат лица на изображении.

		// Запускаем процедуру обнаружения объектов в небольщом черно-белом изображении.
		/*
		Поиснение параметров:
		minFeatureSize: если лицо распологается близко к камере (смартфон, веб-камера), то оптимальным является значение 80x80, что ускоряет работу алгоритма; 
		если искомый объект удален от камеры и на изображении много посторонних объектов, то оптимальное значение лежит в диапозоне 20x20 или 30x30.
		searchScaleFactor: поиск признаков лица (искомого объекта). Поиск всех признаков 1.1 (медленная работа) или поиск основных признаков 1.2 (быстрая работа алгоритма)
		большее число ложных срабатываний.
		minNeighbors: решение о том, что обнаруженный объект является искомым. Детальный анали, тогда значение должно быть больше 3 (замедляет работу алгоритма);
		для большинства случаев, оптимальное значение равно 3.
		flags: статическая константа задает параметр поиска; поиск всех лиц - значение по умолчанию; CASCADE_FIND_BIGGEST_OBJECT - детектирования только самого
		крупного лица (алгоритм работает быстрее).
		Для усорения работы алгоритма детектирования, возможно добавить параметры CASCADE_DO_ROUGH_SEARCH или CASCADE_SCALE_IMAGE
		*/
		cascade.detectMultiScale(finish_frame, object, searchScaleFactor, minNeighboard, flags, minFeatureSize);

		if (object.size() > 0)
		{
			// Выполняем обработку полученных координат, находяшихся в контейнере object
			resize_img_and_edit_koordinatx(img, scaleWidth, object);

			largestObject = (Rect)object.at(0);
			
		}
		else
			// Если объект не обнаружен, сохраняем координаты неправильного многоульника.
			largestObject = Rect(-1,-1,-1,-1);

		
		
	}
Rect search_object_eye(Mat &img, CascadeClassifier &cascade, int scaleWidth)
{
	vector<Rect> object; // Контейнер для хранения координат лица на изображении.
	
	cascade.detectMultiScale(img, object, searchScaleFactor, minNeighboard, flags, minFeatureSize);

	if (object.size() > 0)
		return (Rect)object.at(0);
	else
		// Если объект не обнаружен, сохраняем координаты неправильного многоульника.
		return Rect(-1,-1,-1,-1);
}
Rect & return_rect_object(Mat &img)
{
	int scaleWidth = 320;
	// подготовить кадр для обработки ...
	obrabotka_kadra(img,scaleWidth);
	
	set_scale(img, scaleWidth);
	
	search_object(img, faceCascade, scaleWidth);

	return get_largets_object();


}
Rect return_object_eyeCascade1(Mat &img, int scaleWidth)
{
	set_scale(img, scaleWidth);

	return search_object_eye(img, eyeCascade, scaleWidth);
	
}

Rect return_object_eyeCascade2(Mat &img, int scaleWidth)
{
	set_scale(img, scaleWidth);

	return search_object_eye(img, eyeCascade2, scaleWidth);
}
// explicit запрещает неявное преведение типов, переданных в конструктор.
explicit Detect_Object(const Mat &img, CascadeClassifier &cascade, int minFeatureSize_a = 20, int minFeatureSize_b = 20, int flg = CASCADE_FIND_BIGGEST_OBJECT, int scaleWidth = 320, int minNeigh = 4, float scale_factor = 1.1f)
	:flags(flg)
{
	// подготовить кадр для обработки ...
	obrabotka_kadra(img,scaleWidth);
	// Устанавливаем параметры поиска
	flags = flg;
	set_searchScaleFactor(scale_factor); // Значение детализации изображения при поиске лица. Значение должно быть больше 1
	set_minFeatureSize(minFeatureSize_a, minFeatureSize_b); // Размер сканирующего окна - минимальный, для детального поиска
	set_minNeighboard(minNeigh); // Задаем порог детектирования. Значение должно быть в пределе 2-6
	set_scale(img, scaleWidth);
	// Производим поиск.
	search_object(img, cascade, scaleWidth);

}

Detect_Object()
	: minFeatureSize_a(20), minFeatureSize_b(20), minNeigh(4), scale_factor(1.1f), flags(CASCADE_FIND_BIGGEST_OBJECT)
{
	set_searchScaleFactor(scale_factor); // Значение детализации изображения при поиске лица. Значение должно быть больше 1
	set_minFeatureSize(minFeatureSize_a, minFeatureSize_b); // Размер сканирующего окна - минимальный, для детального поиска
	set_minNeighboard(minNeigh); // Задаем порог детектирования. Значение должно быть в пределе 2-6
	
	load_file_kascade_face("lbpcascade_frontalface.xml");
	load_file_kaskade_eye_1("haarcascade_eye.xml");
	load_file_kaskade_eye_2("haarcascade_eye_tree_eyeglasses.xml");
	
}
virtual ~Detect_Object()
	{

	}
};