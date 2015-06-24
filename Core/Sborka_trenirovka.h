#pragma  once
#include <stdio.h>
#include <vector>
#include <iostream>

#include <opencv2/opencv.hpp>
#include "Preprocessing_img.h"
#include "Identification_Face.h"


// Для работы с текстовыми файлами.
#include <fstream>

class Sbor_trenirovka
{
private:
	// Устанавливаем разрешение изображения, получаемое с камеры
	static const int DESIRED_CAMERA_WIDTH = 640;
	static const int DESIRED_CAMERA_HEIGHT = 480;
	// Усатновка порогового значения для принятия решения алгоритмом, что текушее лицо является известным.
	const float IDENTIFICATION_PERSON() {return 0.37;}
	// Процент идентичности. Применяется при идентификации объекта. Чем выше значение, тем точнее идентификации, но тем больше снимков необходимо собрать
	// на этапе обучения.
	const int IDENTICHNOSTY_DEFAULT() { return 0;}
	// Параметры сохранения новых лиц в коллекции
	// Процент изменения текушего изображения с изображением лица в коллекции. Если текущее изображение отличается от сохраненного на 50% то оно будет сохранено в коллекции.
	const double CHANGE_IN_IMAGE_FOR_COLLECTION() { return 0.5; }
	// Временной интервал для вычисления различия двух изображений.
	const double CHANGE_IN_SECONDS_FOR_COLLECTION() { return 1.0; }
	
	double old_time; // текущее время.
	bool Persona;
	string name_Person;
	int num_img;
	int index_jurnal; // колличество строк в файле журнала
	VideoCapture cam;
	Mat frame_to_camera;
	Mat face_img; // изображение лица которое будет сохранено в коллекции
	Mat face_tmp; // Временное изображение лица.
	Identification_Face tmp_identification;
	Preprocessing_img preprocessing_face;
	vector<string> Name_Fase;
	vector<int> m_latestFaces;
	const char * FILE_LOG() { return "./Kollecxya.txt"; }
	const char * FOLDER_NAME()  { return "FACE"; }
	const char * MASHIN_TRENING() { return "./face_learning.xml"; }
	vector<Mat> face_collection;
	bool file_learning;
	void set_kamera(int num_kam)
	{
		try
		{

			cam.open(num_kam);

			if (!cam.isOpened())
				throw cv::Exception();

			// Устанавливаем разрешение камеры.
			// Данные параметры могут быть отключены.

			cam.set(CV_CAP_PROP_FRAME_WIDTH, DESIRED_CAMERA_WIDTH);
			cam.set(CV_CAP_PROP_FRAME_HEIGHT, DESIRED_CAMERA_HEIGHT);

		}
		catch (cv::Exception &e)
		{
			cerr << "Error: open web-camera!" << endl;
			this->~Sbor_trenirovka();

			system("PAUSE");
			exit(EXIT_FAILURE);


		}
	}

	// Добавляем текст на изображение
	Rect drawString(Mat &frame, string text, Point coord, Scalar color, float fondScale = 0.6f, int thickness = 1, int fontFace = FONT_HERSHEY_COMPLEX)
	{
		// Задаем размер текста
		int baseline = 0;
		Size textSize = getTextSize(text, fontFace, fondScale, thickness, &baseline);
		baseline += thickness;

		// Выравнивание текста
		if (coord.y >= 0)
			coord.y += textSize.height;
		else
			coord.y += frame.rows - baseline + 1;

		if (coord.x < 0)
			coord.x += frame.cols - textSize.width + 1;

		// Задаем координаты рамки вокруг текста
		Rect boundingRect = Rect(coord.x, coord.y - textSize.height, textSize.width, baseline + textSize.height);

		// Отображаем текст на изображении, применяем сглаживание.
		putText(frame, text, coord, fontFace, fondScale, color, thickness, CV_AA);

		// Возвращаем координаты прямоугольника с текстом.
		return boundingRect;
	}

	// Создаем папку со снимками лиц
	void create_folder_face()
	{
		try
		{
			// Создаем директорию.
			_mkdir(FOLDER_NAME());
			if (errno == ENOENT)
				throw std::exception("Puty ne nayden."); // Ошибка при создании/открытии каталога.

		}
		catch (std::exception &e)
		{
			cerr << e.what() << endl;
			system("PAUSE");
			exit(EXIT_FAILURE);
		}
	}

	// Сохраняем обнаруженное лицо в коллекцию для обучения.
	bool save_dst_Img(int &num_img, Mat &dstImg)
	{

		try
		{


			std::string file_name = string(FOLDER_NAME()) + "/" + to_string(static_cast<_Longlong>(num_img)) + ".jpg";

			if (!imwrite(file_name, dstImg))
				throw std::exception("error: create file img Face!");

			

			// Дозаписываем в контейнеры адреса снимков и их номера для создания модели
			Name_Fase.push_back(file_name);
			m_latestFaces.push_back(num_img);
			
			// Сохраняем лог-файл со зделанными снимками.
			create_file_log();
			num_img += 1;
			// Создаем матрицу с зеркальным отображением текушего лица, с целью увеличения обучающей выборки.
			file_name = string(FOLDER_NAME()) + "/" + to_string(static_cast<_Longlong>(num_img)) + ".jpg";
			
			Mat mirroredFace;
			flip(dstImg, mirroredFace, 1);
			
			if (!imwrite(file_name, mirroredFace))
				throw std::exception("create file!");

			
			// Дозаписываем в контейнеры адреса снимков и их номера для создания модели
			Name_Fase.push_back(file_name);
			m_latestFaces.push_back(num_img);

			create_file_log();
			num_img += 1;
			// Запускаем процедуру создания модели. По завершению будет создан файл-модели: MASHIN_TRENING()
			if (num_img == (index_jurnal+count_fase_img()))
			{
				if (tmp_identification.create_mashin_learning(Name_Fase, m_latestFaces, MASHIN_TRENING()))
				{
					// Очищаем контейнеры.
					Name_Fase.clear();
					m_latestFaces.clear();
				}
				else
					cout << "Mashin learning - stop!";
			}


			return true;
		}
		catch (std::exception &e)
		{
			cerr << e.what() << endl;
			return false;
		}


	}

	// Проверяем сушествует ли журнал коллекции. Если сушествует, то переопределяем индексы, переменные: index_jurnal, num_img
	bool read_jurnal()
	{
		try
		{
			// Читаем журнал. Открываем файла для вывода, указатель устанавливаем на начало файла.

			ifstream is;

			is.open(string(FILE_LOG()), std::ifstream::in | ios_base::beg); 

			// проверка открытия
			if ( (is.rdstate() & std::ifstream::failbit) != 0)
				throw std::exception("fail jurnala ne sushestvuet!");


			// Читаем строку из файла.
			for (string s; getline(is, s); index_jurnal++)
			{
				 // Для перестройки модели заполняем ветора коллекции.
				int num = stoi(s);
				m_latestFaces.push_back(stoi(s));
				getline(is,s);
				index_jurnal++;
				getline(is,s);
				Name_Fase.push_back(s);
				index_jurnal++;
			}


			is.close();

			// Устанавливаем значение num_img;
			num_img = index_jurnal/3;
			index_jurnal = num_img;

			return true;

		}
		catch (std::exception &e)
		{
			cout << e.what() << endl;

			num_img = 0;
			file_learning = true;

			return false;
		}


	}

	bool create_file_log()
	{
		// Создаем файл отчета о сохраненных снимках.
		try
		{
			ofstream file_write(FILE_LOG(), ios::app); // Добавляем записи в конец файла. Если файл не сушествует, то он будет создан

			file_write << num_img << "\n";
			file_write << name_Person << "\n";
			file_write << FOLDER_NAME() << "/" << num_img << ".jpg" << "\n";


			file_write.close();
			
			
			return true;

		}
		catch (std::exception &e)
		{
			cerr << "Error: file create" << endl;
			system("PAUSE");
			exit(EXIT_FAILURE);
			return false;
		}


	}

	// Получаем текущее время
	double get_ticket()
	{
		double current_time = (double)getTickCount();
		
		return current_time;
	}
	// Получаем кадр с камеры и сохраняем его в переменной frame_to_camera
	cv::Mat & return_frame()
	{

		cam >> frame_to_camera;


		return frame_to_camera;
	}

	// Отладка собираем модель
	void debug_sborka_modeli() {
		
		ifstream is;

		is.open(string(FILE_LOG()), std::ifstream::in | ios_base::beg); 

		// проверка открытия
		if ( (is.rdstate() & std::ifstream::failbit) != 0)
			throw std::exception("fail jurnala ne sushestvuet!");

		string s;
		vector<int> Num_img;
		vector<string> file_name;
		// Читаем строку из файла.
		while (getline(is, s))
		{
			Num_img.push_back(std::atoi(s.c_str()));
			getline(is,s);

			getline(is,s);
			file_name.push_back(s);
		}
		is.close();


		// Создаем файл машинного обучения
		tmp_identification.create_mashin_learning(file_name, Num_img, MASHIN_TRENING());
		
	}

	// Отладка: перестройка базы данных
	void edit_index_database() {
		try {
			ifstream file_log;
			const char *name_file_tmp = "jurnal_tmp";
			ofstream file_write(name_file_tmp, ios::app);
			file_log.open(string(FILE_LOG()), std::ifstream::in | ios_base::beg);
			
			if ((file_log.rdstate() & std::ifstream::failbit) != 0)
				throw std::exception("error: open file jurnal!");
			string s;
			
			for (int index = 0; getline(file_log, s); index++) {
				int index_tmp = stoi(s);
				
				if (index_tmp != index) {
					file_write << index << "\n";
					getline(file_log, s);
					file_write << s << "\n";
					getline(file_log, s);
					// Переименуем фото в коллекции
					string foto_rename = FOLDER_NAME() + string("/") + to_string(static_cast<_Longlong>(index)) + string(".jpg");
					
					Mat tmp = imread(s);
					imwrite(foto_rename, tmp);
					
					int del_resal = remove(s.c_str());
					if (del_resal) {
						throw std::exception("Error del file jurnal!");
					}
					
					file_write << foto_rename << "\n";

				} else
				{
					// Если индекс идет по порядку, то просто копируем строки из входного файла во временный файл
					file_write << index_tmp << "\n";
					getline(file_log, s);
					file_write << s << "\n";
					getline(file_log, s);
					file_write << s << "\n";

				}
				
			}

			file_log.close();
			file_write.close();
			// Переименовываем врменный файл
			const char *file_jurnal = FILE_LOG();
			int del_resal = remove(file_jurnal);
			if (del_resal)
				throw std::exception("Error del file jurnal!");
			
			int rc = rename(name_file_tmp, file_jurnal);
			if (rc)
				throw std::exception("Error rename file jurnal.");

		} 
		catch (std::exception &e)
		{
			cout << e.what() << endl;
			system("pause");
			exit(-1);
		}
	}

public:
	Sbor_trenirovka(int num_kam)
		:preprocessing_face(), tmp_identification(), Persona(false), num_img(0), index_jurnal(0), name_Person("Unknoun_Name"), old_time(0), file_learning(false)
	{
		// Отладка - пересборка индексов
		// edit_index_database();

		// Отладка собираем модель при запуске программы.
		// debug_sborka_modeli();

		set_kamera(num_kam);
		// при необходимости создаем каталог для лиц.
		create_folder_face();

	}
	// Колличество снимков лица, собираемых при посроении модели. Чем больше снимков, тем выше процент сходства при идентификации
	const int count_fase_img() { return 250; }
	bool set_person(string &name_persona)
	{
		if (name_persona.length() != 0)
		{
			Persona = true;
			
			name_Person = name_persona;
		
			read_jurnal();
			return true;
		}
		return false;

	}
/*	
	bool set_person(string &name_persona)
	{
		name_Person = name_persona;
		Persona = true;
		
		read_jurnal();
			return true;
		
		return false;
	}
*/
	int get_num_img()
	{
		return num_img;
	}
	int get_index_jurnal()
	{
		return index_jurnal;
	}
	bool save_face_jurnal()
	{
		// Число снимков для обучения, ограничем до 150 штук
		if (Persona && num_img < index_jurnal+count_fase_img())
		{
			if (face_tmp.data)
			{
				if (!face_img.data) // Если это первое изображение лица
				{
					face_img = face_tmp;
					// Сохраняем снимки лица в коллекцию
					save_dst_Img(num_img, face_img);

					old_time = get_ticket();

					// Для отладки
					//cout << "save first scrin!" << endl;

				}
				else
				{
					// Вычисляем время, когда было полученно лицо для сравнения.
					double current_time = get_ticket();
					double timeDiff_seconds = (current_time - old_time)/getTickFrequency();
					// Если с момента получения преведушего лица прошло более 1 секунды, то выполняем процедуру сравнения
					if (timeDiff_seconds > CHANGE_IN_SECONDS_FOR_COLLECTION())
					{
						// Сравниваем текущее лицо с преведущим.
						double imageDiff = tmp_identification.getSimilarity(face_tmp, face_img);
						// Если процент различия двух изображения больше 30%, то выполняем сохранение изображения в коллекции
						if (imageDiff > CHANGE_IN_IMAGE_FOR_COLLECTION())
						{
							face_img = face_tmp;
							// Сохраняем снимки лица в коллекцию
							save_dst_Img(num_img, face_img);

							old_time = current_time;
						}
					}
				}
			}
			
			return true;
		}
		return false;
	}

	
	// Сравнение двух лиц. tmpFace - лицо полученное с камеры. reconstructionFace - реконстркция лица из векторов. Данные для реконстркции предоставляет
	// модель, сформированная после операции обучения: mashin_learning_sborka_modeli()
	// Для алгоритмов идентификации: "FaceRecognizer.Eigenfaces", "FaceRecognizer.Fisherfaces"
	/*
	int sravnenie_ferstImg_and_reconstructionFace(const Mat &tmpFace)
	{
		
		float similarity = static_cast<float>(tmp_identification.reconstruction_and_similarity(tmpFace));
		// Если лица схожы то, similarity будет меньше 0.7 (т.е 70%)
		if (similarity < IDENTIFICATION_PERSON())
		{
			// Определим процент идентичности двух лиц
			return tmp_identification.get_identichnosty(tmpFace);
			
		}
		
		else
			return 0;
		
	
	}
	*/
	// Сравнение двух лиц для алгоритма: "FaceRecognizer.LBPH":
	double sravnenie_ferstImg_and_reconstructionFace(const Mat &tmpFace)
	{
		return tmp_identification.reconstruction_and_similarity(tmpFace);

	}
	bool mashin_learning_sborka_modeli()
	{
		
		try
		{
			ifstream is;

			is.open(string(FILE_LOG()), std::ifstream::in | ios_base::beg); // Открываем файл на чтенение и c начала.

			// проверка открытия
			if ( (is.rdstate() & std::ifstream::failbit) != 0)
				throw std::exception("nevozmojno proizvesti identificacxy - fail jurnala ne sushestvuet!");

			string addres_img;
			string name_fays;
			
			// Читаем строку из файла.
			for(string s; getline(is, s);)
			{
				getline(is,s);
				// Для выявление изображений, подходящих к идентификации, сохраняем имена и изображения лиц из папки - FACE в
				// отдельные контейнеры: Name_Face и face_collection
				// Добавдяем имена из файла журнала в контейнер Name_Face
				Name_Fase.push_back(s);
				getline(is, s);
				face_collection.push_back(imread(s, CV_LOAD_IMAGE_GRAYSCALE));
				
			}

			// Закрываем файл.
			is.close();

			// производим загрузку модели, созданной на этапе формирования коллекции.
			tmp_identification.start_learning(MASHIN_TRENING());
			
			// Отладка
			// tmp_identification.get_vectora();
			return true;
		}
		catch (std::invalid_argument &e)
		{
			// Для отладки
			// cout << "Nevozmojno proizvesti preobrazovanie: string - int" << endl;
			// system("PAUSE");
			return false;
		}
		catch (std::exception &e)
		{
			// Для отладки
			// cout << e.what() << endl;
			// system("PAUSE");
			return false;
		}
	}
	cv::Mat & return_frame(Mat &tmp, string text, Point coord, Scalar color)
	{

		Rect tmp_rect = drawString(tmp, text, coord, color);
		// Рисует прямоугольник на изображении.
		rectangle(tmp, tmp_rect, Scalar(200,200,200), 1);
		
		return tmp;
	}


	cv::Mat return_frame_form()
	{
		// 
		Mat &tmp = return_frame();

		// кадр который будет показан пользователю
		Mat &dst = preprocessing_face.preprocessing_frame(tmp, face_tmp);
		
		// fase_to_form = preprocessing_face.return_fase_region()
		// Создаем отдельное изображение с обнаруженным лицом, для отображения в графической форме.
		//fase_rect = preprocessing_face.return_rectangle_fase();
		//Mat fase_to_form123 = dst(preprocessing_face.return_rectangle_fase());

		return dst;

	}
	bool process_identification(int &identichnosty_to_form, string &name_persona_to_form, string &file_name)
	{

		// Режим идентификации
		if (tmp_identification.mashin_learnin() && face_tmp.data)
		{

			// Для алгоритма FaceRecognizer.LBPH
			int numer_img = tmp_identification.get_prediction(face_tmp);
			if (num_img >= 0) {
				string file = FOLDER_NAME() + string("/") + to_string(static_cast<_Longlong>(numer_img)) + string(".jpg");
				Mat B = imread(file, CV_8U);
				double l2 = tmp_identification.similarity_img(face_tmp, B);
				
				// qDebug() << l2;
				if ( l2  < IDENTIFICATION_PERSON()) {
					// Отладка
					// qDebug() << "Finish: " << l2;
					tmp_identification.save_model(MASHIN_TRENING());
					name_persona_to_form = Name_Fase[numer_img];
					identichnosty_to_form = (1.0 - l2) * 100;
					// qDebug() << identichnosty_to_form;
					file_name = file;
					return true;
				}
			}
			
			// Для алгоритмов: "FaceRecognizer.Eigenfaces", "FaceRecognizer.Fisherfaces"
			/*
			int identichnosth = sravnenie_ferstImg_and_reconstructionFace(face_tmp);
			
			if (identichnosth < IDENTICHNOSTY_DEFAULT())
			{
				return false;
			}
			
				for (unsigned i = 0; i < face_collection.size(); i++)
				{
					if (static_cast<float>(tmp_identification.getSimilarity(face_tmp, face_collection[i])) < IDENTIFICATION_PERSON())
					{
						string name = Name_Fase[i];
						name_persona_to_form = name;
						file_name = FOLDER_NAME() + string("/") + to_string(static_cast<_Longlong>(i)) + string(".jpg");
						identichnosty_to_form = identichnosth;
						return true;
					}
				}
				*/

			
			/*
			double rezultat = sravnenie_ferstImg_and_reconstructionFace(face_tmp);
			if (rezultat != 1e8)
			{
				// Поиск изображения в базе.
				for (unsigned i = 0; i < face_collection.size(); i++)
				{
					double shodstvo = tmp_identification.getSimilarity(face_tmp, face_collection[i]);
					if (shodstvo < IDENTIFICATION_PERSON())
					{
						
						name_persona_to_form = Name_Fase[i];
						file_name = FOLDER_NAME() + string("/") + to_string(static_cast<_Longlong>(i)) + string(".jpg");
						// Печатаем в окне процент сходство изображения в базе и изображения лица полученного с камеры.
						identichnosty_to_form = (1.0f - shodstvo) * 100;
						return true;
					}
				}
			}
			*/
		}
		
			return false;
}
	
	// Перечитываем журнал коллекции, при запуске идентификации
	bool read_jurnal_to_identification()
	{
		try
		{
			Name_Fase.clear();
			face_collection.clear();
			//face_collection.clear();
			// Читаем журнал. Открываем файла для вывода, указатель устанавливаем на начало файла.

			ifstream is;

			is.open(string(FILE_LOG()), std::ifstream::in | ios_base::beg); 

			// проверка открытия
			if ( (is.rdstate() & std::ifstream::failbit) != 0)
				throw std::exception("fail jurnala ne sushestvuet!");

			string s;
			// Читаем строку из файла.
			while (getline(is, s))
			{
				getline(is,s);
				// Заполняем контейнер именами из журнала коллекции
				Name_Fase.push_back(s);

				getline(is,s);
				face_collection.push_back(imread(s, CV_LOAD_IMAGE_GRAYSCALE));
			}
			is.close();

			return true;
		}
		catch (std::exception &e)
		{
			cout << e.what() << endl;

			return false;
		}

	}

	// Частота кадров - значение получаем с камеры.
	double camera_fps()
	{
		return cam.get(CV_CAP_PROP_FPS);
		
	}
	
	virtual ~Sbor_trenirovka()
	{
		
		
	}
};