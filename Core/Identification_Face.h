#pragma  once
#include <Qt/qdebug.h>
class Identification_Face
{
private:
	// Контейнер для хранения модуль машинного обучения из библиотеки OpenCV
	// Шаблонный класс, описывает указатель типа, переданного в углавых скобках.
	Ptr<FaceRecognizer> model;
	Mat face_vector;
	vector<Mat> preprocessedFace; // контейнер с лицами.
	vector<int> faceLabels; // контейнер с метками.
	// Mat *face_vector; // Матрица содержит реконструкцию лица из векторов.
	double Similarity; // средняя квадратическая погрешность.
	bool statrt_learning;
	
	// Метод производит реконструкцию лица из векторов, полученных на этапе обработки лица.
	// Метод принимает на обработку изображение лица, полученное после преобрзований, реализованных в классе Preprocessing_img
	Mat reconstructFace(const Mat preprocessedFace)
	{
		// Так как выполняется реконструкция для модулей Eigenfaces или Fisherfaces, тогда в целях проверки получаемых результатов
		// будем использовать обработку исключинеий.
		try
		{
			// Получаем данные из модели
			//Mat eigenvectors = model->get<Mat>("eigenvectors");
			//Mat averageFaceRow = model->get<Mat>("mean");
			Mat eigenvectors = model->get<Mat>("opencv_storage");
			Mat averageFaceRow = model->get<Mat>("histograms");

			int faceHeight = preprocessedFace.rows;

			// Перенаправляем входное изображение на пространство PCA
			Mat projection = subspaceProject(eigenvectors, averageFaceRow,preprocessedFace.reshape(1,1));

			// printMatInfo(projection, "projection");

			// Выполняем реконструкцию лица, по данным полученным из пространства PCA
			Mat reconstructionRow = subspaceReconstruct(eigenvectors, averageFaceRow, projection);
			// printMatInfo(reconstructionRow, "reconstructionRow");


			// Преобразуем форму изображения в прямоугольное.
			Mat reconstructionMat = reconstructionRow.reshape(1, faceHeight);
			
			// Преобразование изображения в 8-битное
			face_vector = Mat(reconstructionMat.size(), CV_8U);
			
			reconstructionMat.convertTo(face_vector, CV_8U, 1, 0);
			// printMatInfo(reconstructedFace, "reconstructuinM");
			return face_vector;

		}
		catch (cv::Exception e)
		{
			face_vector = Mat();
			// Если ошибка возвращаем пустую матрицу.
			return face_vector;

		}
	}

public:
	// Устанавливает алгоритм машинного обучения.
	void algoritm_mashin_learning(const char *name_algoritm = "FaceRecognizer.LBPH")
	{
		try
		{
			// проверка загрузки модуля contrib из библиотеки OpenCV
			if (!initModule_contrib())
				throw "no is not module contrib - OpenCV!";


			// Алгоритм машинного обучения - FaceRecognizer.Eigenfaces
			// Создаем модель машинного обучения по алгоритму LBPH
			// Радиус при создании двоичного кода: 1
			// Точность модели (neighbors)- средняя (10), значение по умолчанию - 8. С ростам параметра увеличивается файл машиного обучения.
			// Файл более 100 Мб, невозможно использовать на ПК, в котором 4 Гб оперативной памяти.
			// Колличество клтеток по осям x,y: 10;10, значение в научных публикация и по умолчанию 8
			// Колличество клеток должно быть на порядок меньше точности - уменьшает размер файла машинного обучения, следовательно ускоряет работу идентификации
			model = createLBPHFaceRecognizer(1, 12, 3, 3, DBL_MAX);
			
			if (model.empty())
				throw "no is not load model, name ";


		}
		catch (string &e)
		{
			cerr << e << endl;
			this->~Identification_Face();
			system("PAUSE");
			exit(EXIT_FAILURE);
		}
	}
	Identification_Face()
		: Similarity(0.0), statrt_learning(false)
	{
		
			algoritm_mashin_learning();

	}

	void set_preprocessed_face_and_face_label(string &img, int &num_img)
	{
		preprocessedFace.push_back(imread(img, CV_LOAD_IMAGE_GRAYSCALE));
		faceLabels.push_back(num_img);

	}

	
	// очистка контейнеров
	void erase_vector()
	{
		preprocessedFace.clear();
		faceLabels.clear();
	}
	bool create_mashin_learning (const vector<String> &file_name, const vector<int> &label, const char *file_learning)
	{
		vector<Mat> tmp_fase;

		for (unsigned i = 0; i < file_name.size() && file_name.size() == label.size(); i++)
			tmp_fase.push_back(imread(file_name[i], CV_LOAD_IMAGE_GRAYSCALE));
		
		try
		{
			if (tmp_fase.empty())
				throw std::exception("Error: vector<int>Label = NULL.");
			// Тренируем модель.
			model->train(tmp_fase, label);

			// Сохраняем результат
			model->save(file_learning);
		}
		catch (std::exception &e)
		{
			cerr << e.what() << endl;
			return false;

		}

		return true;
	}

	void save_model(const char *file_leaarning) {
		model->save(file_leaarning);
	}
	void start_learning(const char *file_learning)
	{
		/* 
				В метд FaceRecognizer::train(контейнер_с_изображением_лиц, контейнер_с_метками) загружвается контейнер с изображением лиц и контейнер с метками этих лиц.
				Например, в контейнере лиц, содержится по три изображения двух разных людей. Изображениям первого человека присваивается метка
				0, изображениям второго человека присваивается метка 1. Реализуется, данная операция, следующим образом:
				vector<Mat> images;
				vector<int> label;
				images.push_back(imread("persona0/0.jpg", CV_LOAD_IMAGE_GRAYSCALE));
				label.push_back(0)... и таким образом еще два изображения, с метками 0
				Изображение второго лица
				images.push_back(imread("persona1/0.jpg", CV_LOAD_IMAGE_GRAYSCALE));
				label.push_back(1)... и таким образом еще два изображения с метками 1

				Фактическое время обучения может быть несколько секунд или минут, в зависимости от размера коллекции.
			*/
		try
		{
			model->load(file_learning);

		}
		catch (cv::Exception &e)
		{
			cerr << "Error: learning mashin!" << endl;
			system("PAUSE");
			statrt_learning =  false;
		}

		statrt_learning = true;
	}
	
	bool mashin_learnin()
	{
		return statrt_learning;
	}

	// Средняя квадратическая погрешность двух сравниваемых величин, в данном случаии двух матриц.
	double getSimilarity(const Mat A, const Mat B)
	{
		if (A.rows > 0 && A.rows == B.rows && A.cols > 0 && A.cols == B.cols)
		{
			// Расчет относительной погрешности L2 при сравнении двух изображений. 
			// переменная errorL2 содершит сумму погрешностей в каждом пикселе изображения.
			double errorL2 = norm(A, B, CV_L2);
			// Находим значение погрешности в одном пикселе изображении
			Similarity = errorL2 / (double) (A.rows * A.cols);
			return Similarity;

		} else
			// Матрица A и B различны.
			// cout << "Warning: image A and B different. " << endl;
		Similarity = 100000000.0; // Метод возвращает значение, которое интерпритируется как ошибка.
		
		return Similarity;

	}

	// Выполняем предсказание, есть ли педставленное лицо в коллекции
	int get_prediction(const Mat A) {
		
		double procent_unicalynosti = 0.0;
		int label_img;
		model->predict(A, label_img, procent_unicalynosti);
		double porog_threshold = model->getDouble("threshold"); // Получаем пороговое значение уникальности из модели.

		//// Если процент уникальности меньше заданного порога, значит представленый объек может быть идентифицирован
		if (procent_unicalynosti < porog_threshold) {
			return label_img;
			//return -1;
		}
	
		return -1;
	}
	// LBPH
	double similarity_img(const Mat &A, const Mat &B) {

		
		// Вычисляем схожесть двух изображения. Изображение А - поступило с камеры, изображение B - реконстуркция из данных модели
		if (A.rows > 0 && A.rows == B.rows && A.cols > 0 && A.cols == B.cols)
		{
			// Расчет относительной погрешности L2 при сравнении двух изображений. 
			// переменная errorL2 содершит сумму погрешностей в каждом пикселе изображения.
			double errorL2 = norm(A, B, CV_L2);
			// Находим значение погрешности в одном пикселе изображении
			Similarity = errorL2 / (double) (A.rows * A.cols);

			return Similarity;

		} else
		{
			// Матрица A и B различны. Отладка
			// cout << "Warning: image A and B different. " << endl;
			Similarity = 100000000.0; // Метод возвращает значение, которое интерпритируется как ошибка.

		}

		return Similarity;
	}

	// Метод выполняет векторную реконстркцию лица из данных модели (model) и сравивает два изображения:
	// векторную реконструкцию B и изображение лица полученное с камеры - А
	double reconstruction_and_similarity(const Mat A)
	{
		// Производим реконстркцию лица
		Mat B = reconstructFace(A);
		// Вычисляем схожесть двух изображения. Изображение А - поступило с камеры, изображение B - реконстуркция из данных модели
		if (A.rows > 0 && A.rows == B.rows && A.cols > 0 && A.cols == B.cols)
		{
			// Расчет относительной погрешности L2 при сравнении двух изображений. 
			// переменная errorL2 содершит сумму погрешностей в каждом пикселе изображения.
			double errorL2 = norm(A, B, CV_L2);
			// Находим значение погрешности в одном пикселе изображении
			Similarity = errorL2 / (double) (A.rows * A.cols);

			return Similarity;

		} else
		{
			// Матрица A и B различны. Отладка
			// cout << "Warning: image A and B different. " << endl;
			Similarity = 100000000.0; // Метод возвращает значение, которое интерпритируется как ошибка.
			
		}

		return Similarity;

	}


	// Создаем переменную матрицу, (конвертирование текушей матрицы путем преобразования строк и столбцов) для прямоугольного 8-битного 
	// изображения. Значения элементов прямоугольной 8-ми битной матрицы в диапозоне от 0 до 255
	Mat getImageFrom1DFloatMat (const Mat matrixRow, int height)
	{
		// Создаем прямоугольную матрицу
		Mat rectangularMat = matrixRow.reshape(1, height);

		// Создаем 8-битное изображение из прямоугольной матрицы.
		Mat dst;

		normalize(rectangularMat, dst, 0, 255, NORM_MINMAX, CV_8UC1);
		
		return dst;
	}

	// Метод возвращает значение идентичности лиц, одно из которых находиться в модели, друго переданное как аргумент метода
	int get_identichnosty(const Mat &face_to_camera)
	{
		//int label_index;
		//model->predict(face_to_camera, label_index, similarity);
		// В переменной сохраняется значение идентичности в процентах.
		return model->predict(face_to_camera);
		// Метод возвращает номер снимка в коллекции который идентичен кадру поступившему с камеры - face_to_camera
		// return label_index;
	}
//
//// Отладка
//	void get_vectora()
//	{
//		for (unsigned i = 0; i < faceLabels.size(); i++)
//			imshow("Test_vector", preprocessedFace[faceLabels[i]]);
//
//		if (faceLabels.size() == preprocessedFace.size())
//			cout << "Razmerx sovpadayt: " << preprocessedFace.size() << endl;
//
//	}
//

	virtual ~Identification_Face()
	{
		/*
		if (preprocessedFace)
			delete preprocessedFace;

		if (faceLabels)
			delete faceLabels;
		*/
		
	}



};