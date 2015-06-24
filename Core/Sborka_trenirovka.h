#pragma  once
#include <stdio.h>
#include <vector>
#include <iostream>

#include <opencv2/opencv.hpp>
#include "Preprocessing_img.h"
#include "Identification_Face.h"


// ��� ������ � ���������� �������.
#include <fstream>

class Sbor_trenirovka
{
private:
	// ������������� ���������� �����������, ���������� � ������
	static const int DESIRED_CAMERA_WIDTH = 640;
	static const int DESIRED_CAMERA_HEIGHT = 480;
	// ��������� ���������� �������� ��� �������� ������� ����������, ��� ������� ���� �������� ���������.
	const float IDENTIFICATION_PERSON() {return 0.37;}
	// ������� ������������. ����������� ��� ������������� �������. ��� ���� ��������, ��� ������ �������������, �� ��� ������ ������� ���������� �������
	// �� ����� ��������.
	const int IDENTICHNOSTY_DEFAULT() { return 0;}
	// ��������� ���������� ����� ��� � ���������
	// ������� ��������� �������� ����������� � ������������ ���� � ���������. ���� ������� ����������� ���������� �� ������������ �� 50% �� ��� ����� ��������� � ���������.
	const double CHANGE_IN_IMAGE_FOR_COLLECTION() { return 0.5; }
	// ��������� �������� ��� ���������� �������� ���� �����������.
	const double CHANGE_IN_SECONDS_FOR_COLLECTION() { return 1.0; }
	
	double old_time; // ������� �����.
	bool Persona;
	string name_Person;
	int num_img;
	int index_jurnal; // ����������� ����� � ����� �������
	VideoCapture cam;
	Mat frame_to_camera;
	Mat face_img; // ����������� ���� ������� ����� ��������� � ���������
	Mat face_tmp; // ��������� ����������� ����.
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

			// ������������� ���������� ������.
			// ������ ��������� ����� ���� ���������.

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

	// ��������� ����� �� �����������
	Rect drawString(Mat &frame, string text, Point coord, Scalar color, float fondScale = 0.6f, int thickness = 1, int fontFace = FONT_HERSHEY_COMPLEX)
	{
		// ������ ������ ������
		int baseline = 0;
		Size textSize = getTextSize(text, fontFace, fondScale, thickness, &baseline);
		baseline += thickness;

		// ������������ ������
		if (coord.y >= 0)
			coord.y += textSize.height;
		else
			coord.y += frame.rows - baseline + 1;

		if (coord.x < 0)
			coord.x += frame.cols - textSize.width + 1;

		// ������ ���������� ����� ������ ������
		Rect boundingRect = Rect(coord.x, coord.y - textSize.height, textSize.width, baseline + textSize.height);

		// ���������� ����� �� �����������, ��������� �����������.
		putText(frame, text, coord, fontFace, fondScale, color, thickness, CV_AA);

		// ���������� ���������� �������������� � �������.
		return boundingRect;
	}

	// ������� ����� �� �������� ���
	void create_folder_face()
	{
		try
		{
			// ������� ����������.
			_mkdir(FOLDER_NAME());
			if (errno == ENOENT)
				throw std::exception("Puty ne nayden."); // ������ ��� ��������/�������� ��������.

		}
		catch (std::exception &e)
		{
			cerr << e.what() << endl;
			system("PAUSE");
			exit(EXIT_FAILURE);
		}
	}

	// ��������� ������������ ���� � ��������� ��� ��������.
	bool save_dst_Img(int &num_img, Mat &dstImg)
	{

		try
		{


			std::string file_name = string(FOLDER_NAME()) + "/" + to_string(static_cast<_Longlong>(num_img)) + ".jpg";

			if (!imwrite(file_name, dstImg))
				throw std::exception("error: create file img Face!");

			

			// ������������ � ���������� ������ ������� � �� ������ ��� �������� ������
			Name_Fase.push_back(file_name);
			m_latestFaces.push_back(num_img);
			
			// ��������� ���-���� �� ���������� ��������.
			create_file_log();
			num_img += 1;
			// ������� ������� � ���������� ������������ �������� ����, � ����� ���������� ��������� �������.
			file_name = string(FOLDER_NAME()) + "/" + to_string(static_cast<_Longlong>(num_img)) + ".jpg";
			
			Mat mirroredFace;
			flip(dstImg, mirroredFace, 1);
			
			if (!imwrite(file_name, mirroredFace))
				throw std::exception("create file!");

			
			// ������������ � ���������� ������ ������� � �� ������ ��� �������� ������
			Name_Fase.push_back(file_name);
			m_latestFaces.push_back(num_img);

			create_file_log();
			num_img += 1;
			// ��������� ��������� �������� ������. �� ���������� ����� ������ ����-������: MASHIN_TRENING()
			if (num_img == (index_jurnal+count_fase_img()))
			{
				if (tmp_identification.create_mashin_learning(Name_Fase, m_latestFaces, MASHIN_TRENING()))
				{
					// ������� ����������.
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

	// ��������� ���������� �� ������ ���������. ���� ����������, �� �������������� �������, ����������: index_jurnal, num_img
	bool read_jurnal()
	{
		try
		{
			// ������ ������. ��������� ����� ��� ������, ��������� ������������� �� ������ �����.

			ifstream is;

			is.open(string(FILE_LOG()), std::ifstream::in | ios_base::beg); 

			// �������� ��������
			if ( (is.rdstate() & std::ifstream::failbit) != 0)
				throw std::exception("fail jurnala ne sushestvuet!");


			// ������ ������ �� �����.
			for (string s; getline(is, s); index_jurnal++)
			{
				 // ��� ����������� ������ ��������� ������ ���������.
				int num = stoi(s);
				m_latestFaces.push_back(stoi(s));
				getline(is,s);
				index_jurnal++;
				getline(is,s);
				Name_Fase.push_back(s);
				index_jurnal++;
			}


			is.close();

			// ������������� �������� num_img;
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
		// ������� ���� ������ � ����������� �������.
		try
		{
			ofstream file_write(FILE_LOG(), ios::app); // ��������� ������ � ����� �����. ���� ���� �� ����������, �� �� ����� ������

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

	// �������� ������� �����
	double get_ticket()
	{
		double current_time = (double)getTickCount();
		
		return current_time;
	}
	// �������� ���� � ������ � ��������� ��� � ���������� frame_to_camera
	cv::Mat & return_frame()
	{

		cam >> frame_to_camera;


		return frame_to_camera;
	}

	// ������� �������� ������
	void debug_sborka_modeli() {
		
		ifstream is;

		is.open(string(FILE_LOG()), std::ifstream::in | ios_base::beg); 

		// �������� ��������
		if ( (is.rdstate() & std::ifstream::failbit) != 0)
			throw std::exception("fail jurnala ne sushestvuet!");

		string s;
		vector<int> Num_img;
		vector<string> file_name;
		// ������ ������ �� �����.
		while (getline(is, s))
		{
			Num_img.push_back(std::atoi(s.c_str()));
			getline(is,s);

			getline(is,s);
			file_name.push_back(s);
		}
		is.close();


		// ������� ���� ��������� ��������
		tmp_identification.create_mashin_learning(file_name, Num_img, MASHIN_TRENING());
		
	}

	// �������: ����������� ���� ������
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
					// ����������� ���� � ���������
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
					// ���� ������ ���� �� �������, �� ������ �������� ������ �� �������� ����� �� ��������� ����
					file_write << index_tmp << "\n";
					getline(file_log, s);
					file_write << s << "\n";
					getline(file_log, s);
					file_write << s << "\n";

				}
				
			}

			file_log.close();
			file_write.close();
			// ��������������� �������� ����
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
		// ������� - ���������� ��������
		// edit_index_database();

		// ������� �������� ������ ��� ������� ���������.
		// debug_sborka_modeli();

		set_kamera(num_kam);
		// ��� ������������� ������� ������� ��� ���.
		create_folder_face();

	}
	// ����������� ������� ����, ���������� ��� ��������� ������. ��� ������ �������, ��� ���� ������� �������� ��� �������������
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
		// ����� ������� ��� ��������, ��������� �� 150 ����
		if (Persona && num_img < index_jurnal+count_fase_img())
		{
			if (face_tmp.data)
			{
				if (!face_img.data) // ���� ��� ������ ����������� ����
				{
					face_img = face_tmp;
					// ��������� ������ ���� � ���������
					save_dst_Img(num_img, face_img);

					old_time = get_ticket();

					// ��� �������
					//cout << "save first scrin!" << endl;

				}
				else
				{
					// ��������� �����, ����� ���� ��������� ���� ��� ���������.
					double current_time = get_ticket();
					double timeDiff_seconds = (current_time - old_time)/getTickFrequency();
					// ���� � ������� ��������� ����������� ���� ������ ����� 1 �������, �� ��������� ��������� ���������
					if (timeDiff_seconds > CHANGE_IN_SECONDS_FOR_COLLECTION())
					{
						// ���������� ������� ���� � ����������.
						double imageDiff = tmp_identification.getSimilarity(face_tmp, face_img);
						// ���� ������� �������� ���� ����������� ������ 30%, �� ��������� ���������� ����������� � ���������
						if (imageDiff > CHANGE_IN_IMAGE_FOR_COLLECTION())
						{
							face_img = face_tmp;
							// ��������� ������ ���� � ���������
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

	
	// ��������� ���� ���. tmpFace - ���� ���������� � ������. reconstructionFace - ������������ ���� �� ��������. ������ ��� ������������ �������������
	// ������, �������������� ����� �������� ��������: mashin_learning_sborka_modeli()
	// ��� ���������� �������������: "FaceRecognizer.Eigenfaces", "FaceRecognizer.Fisherfaces"
	/*
	int sravnenie_ferstImg_and_reconstructionFace(const Mat &tmpFace)
	{
		
		float similarity = static_cast<float>(tmp_identification.reconstruction_and_similarity(tmpFace));
		// ���� ���� ����� ��, similarity ����� ������ 0.7 (�.� 70%)
		if (similarity < IDENTIFICATION_PERSON())
		{
			// ��������� ������� ������������ ���� ���
			return tmp_identification.get_identichnosty(tmpFace);
			
		}
		
		else
			return 0;
		
	
	}
	*/
	// ��������� ���� ��� ��� ���������: "FaceRecognizer.LBPH":
	double sravnenie_ferstImg_and_reconstructionFace(const Mat &tmpFace)
	{
		return tmp_identification.reconstruction_and_similarity(tmpFace);

	}
	bool mashin_learning_sborka_modeli()
	{
		
		try
		{
			ifstream is;

			is.open(string(FILE_LOG()), std::ifstream::in | ios_base::beg); // ��������� ���� �� �������� � c ������.

			// �������� ��������
			if ( (is.rdstate() & std::ifstream::failbit) != 0)
				throw std::exception("nevozmojno proizvesti identificacxy - fail jurnala ne sushestvuet!");

			string addres_img;
			string name_fays;
			
			// ������ ������ �� �����.
			for(string s; getline(is, s);)
			{
				getline(is,s);
				// ��� ��������� �����������, ���������� � �������������, ��������� ����� � ����������� ��� �� ����� - FACE �
				// ��������� ����������: Name_Face � face_collection
				// ��������� ����� �� ����� ������� � ��������� Name_Face
				Name_Fase.push_back(s);
				getline(is, s);
				face_collection.push_back(imread(s, CV_LOAD_IMAGE_GRAYSCALE));
				
			}

			// ��������� ����.
			is.close();

			// ���������� �������� ������, ��������� �� ����� ������������ ���������.
			tmp_identification.start_learning(MASHIN_TRENING());
			
			// �������
			// tmp_identification.get_vectora();
			return true;
		}
		catch (std::invalid_argument &e)
		{
			// ��� �������
			// cout << "Nevozmojno proizvesti preobrazovanie: string - int" << endl;
			// system("PAUSE");
			return false;
		}
		catch (std::exception &e)
		{
			// ��� �������
			// cout << e.what() << endl;
			// system("PAUSE");
			return false;
		}
	}
	cv::Mat & return_frame(Mat &tmp, string text, Point coord, Scalar color)
	{

		Rect tmp_rect = drawString(tmp, text, coord, color);
		// ������ ������������� �� �����������.
		rectangle(tmp, tmp_rect, Scalar(200,200,200), 1);
		
		return tmp;
	}


	cv::Mat return_frame_form()
	{
		// 
		Mat &tmp = return_frame();

		// ���� ������� ����� ������� ������������
		Mat &dst = preprocessing_face.preprocessing_frame(tmp, face_tmp);
		
		// fase_to_form = preprocessing_face.return_fase_region()
		// ������� ��������� ����������� � ������������ �����, ��� ����������� � ����������� �����.
		//fase_rect = preprocessing_face.return_rectangle_fase();
		//Mat fase_to_form123 = dst(preprocessing_face.return_rectangle_fase());

		return dst;

	}
	bool process_identification(int &identichnosty_to_form, string &name_persona_to_form, string &file_name)
	{

		// ����� �������������
		if (tmp_identification.mashin_learnin() && face_tmp.data)
		{

			// ��� ��������� FaceRecognizer.LBPH
			int numer_img = tmp_identification.get_prediction(face_tmp);
			if (num_img >= 0) {
				string file = FOLDER_NAME() + string("/") + to_string(static_cast<_Longlong>(numer_img)) + string(".jpg");
				Mat B = imread(file, CV_8U);
				double l2 = tmp_identification.similarity_img(face_tmp, B);
				
				// qDebug() << l2;
				if ( l2  < IDENTIFICATION_PERSON()) {
					// �������
					// qDebug() << "Finish: " << l2;
					tmp_identification.save_model(MASHIN_TRENING());
					name_persona_to_form = Name_Fase[numer_img];
					identichnosty_to_form = (1.0 - l2) * 100;
					// qDebug() << identichnosty_to_form;
					file_name = file;
					return true;
				}
			}
			
			// ��� ����������: "FaceRecognizer.Eigenfaces", "FaceRecognizer.Fisherfaces"
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
				// ����� ����������� � ����.
				for (unsigned i = 0; i < face_collection.size(); i++)
				{
					double shodstvo = tmp_identification.getSimilarity(face_tmp, face_collection[i]);
					if (shodstvo < IDENTIFICATION_PERSON())
					{
						
						name_persona_to_form = Name_Fase[i];
						file_name = FOLDER_NAME() + string("/") + to_string(static_cast<_Longlong>(i)) + string(".jpg");
						// �������� � ���� ������� �������� ����������� � ���� � ����������� ���� ����������� � ������.
						identichnosty_to_form = (1.0f - shodstvo) * 100;
						return true;
					}
				}
			}
			*/
		}
		
			return false;
}
	
	// ������������ ������ ���������, ��� ������� �������������
	bool read_jurnal_to_identification()
	{
		try
		{
			Name_Fase.clear();
			face_collection.clear();
			//face_collection.clear();
			// ������ ������. ��������� ����� ��� ������, ��������� ������������� �� ������ �����.

			ifstream is;

			is.open(string(FILE_LOG()), std::ifstream::in | ios_base::beg); 

			// �������� ��������
			if ( (is.rdstate() & std::ifstream::failbit) != 0)
				throw std::exception("fail jurnala ne sushestvuet!");

			string s;
			// ������ ������ �� �����.
			while (getline(is, s))
			{
				getline(is,s);
				// ��������� ��������� ������� �� ������� ���������
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

	// ������� ������ - �������� �������� � ������.
	double camera_fps()
	{
		return cam.get(CV_CAP_PROP_FPS);
		
	}
	
	virtual ~Sbor_trenirovka()
	{
		
		
	}
};