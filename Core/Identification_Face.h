#pragma  once
#include <Qt/qdebug.h>
class Identification_Face
{
private:
	// ��������� ��� �������� ������ ��������� �������� �� ���������� OpenCV
	// ��������� �����, ��������� ��������� ����, ����������� � ������� �������.
	Ptr<FaceRecognizer> model;
	Mat face_vector;
	vector<Mat> preprocessedFace; // ��������� � ������.
	vector<int> faceLabels; // ��������� � �������.
	// Mat *face_vector; // ������� �������� ������������� ���� �� ��������.
	double Similarity; // ������� �������������� �����������.
	bool statrt_learning;
	
	// ����� ���������� ������������� ���� �� ��������, ���������� �� ����� ��������� ����.
	// ����� ��������� �� ��������� ����������� ����, ���������� ����� �������������, ������������� � ������ Preprocessing_img
	Mat reconstructFace(const Mat preprocessedFace)
	{
		// ��� ��� ����������� ������������� ��� ������� Eigenfaces ��� Fisherfaces, ����� � ����� �������� ���������� �����������
		// ����� ������������ ��������� �����������.
		try
		{
			// �������� ������ �� ������
			//Mat eigenvectors = model->get<Mat>("eigenvectors");
			//Mat averageFaceRow = model->get<Mat>("mean");
			Mat eigenvectors = model->get<Mat>("opencv_storage");
			Mat averageFaceRow = model->get<Mat>("histograms");

			int faceHeight = preprocessedFace.rows;

			// �������������� ������� ����������� �� ������������ PCA
			Mat projection = subspaceProject(eigenvectors, averageFaceRow,preprocessedFace.reshape(1,1));

			// printMatInfo(projection, "projection");

			// ��������� ������������� ����, �� ������ ���������� �� ������������ PCA
			Mat reconstructionRow = subspaceReconstruct(eigenvectors, averageFaceRow, projection);
			// printMatInfo(reconstructionRow, "reconstructionRow");


			// ����������� ����� ����������� � �������������.
			Mat reconstructionMat = reconstructionRow.reshape(1, faceHeight);
			
			// �������������� ����������� � 8-������
			face_vector = Mat(reconstructionMat.size(), CV_8U);
			
			reconstructionMat.convertTo(face_vector, CV_8U, 1, 0);
			// printMatInfo(reconstructedFace, "reconstructuinM");
			return face_vector;

		}
		catch (cv::Exception e)
		{
			face_vector = Mat();
			// ���� ������ ���������� ������ �������.
			return face_vector;

		}
	}

public:
	// ������������� �������� ��������� ��������.
	void algoritm_mashin_learning(const char *name_algoritm = "FaceRecognizer.LBPH")
	{
		try
		{
			// �������� �������� ������ contrib �� ���������� OpenCV
			if (!initModule_contrib())
				throw "no is not module contrib - OpenCV!";


			// �������� ��������� �������� - FaceRecognizer.Eigenfaces
			// ������� ������ ��������� �������� �� ��������� LBPH
			// ������ ��� �������� ��������� ����: 1
			// �������� ������ (neighbors)- ������� (10), �������� �� ��������� - 8. � ������ ��������� ������������� ���� �������� ��������.
			// ���� ����� 100 ��, ���������� ������������ �� ��, � ������� 4 �� ����������� ������.
			// ����������� ������� �� ���� x,y: 10;10, �������� � ������� ���������� � �� ��������� 8
			// ����������� ������ ������ ���� �� ������� ������ �������� - ��������� ������ ����� ��������� ��������, ������������� �������� ������ �������������
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

	
	// ������� �����������
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
			// ��������� ������.
			model->train(tmp_fase, label);

			// ��������� ���������
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
				� ���� FaceRecognizer::train(���������_�_������������_���, ���������_�_�������) ������������ ��������� � ������������ ��� � ��������� � ������� ���� ���.
				��������, � ���������� ���, ���������� �� ��� ����������� ���� ������ �����. ������������ ������� �������� ������������� �����
				0, ������������ ������� �������� ������������� ����� 1. �����������, ������ ��������, ��������� �������:
				vector<Mat> images;
				vector<int> label;
				images.push_back(imread("persona0/0.jpg", CV_LOAD_IMAGE_GRAYSCALE));
				label.push_back(0)... � ����� ������� ��� ��� �����������, � ������� 0
				����������� ������� ����
				images.push_back(imread("persona1/0.jpg", CV_LOAD_IMAGE_GRAYSCALE));
				label.push_back(1)... � ����� ������� ��� ��� ����������� � ������� 1

				����������� ����� �������� ����� ���� ��������� ������ ��� �����, � ����������� �� ������� ���������.
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

	// ������� �������������� ����������� ���� ������������ �������, � ������ ������� ���� ������.
	double getSimilarity(const Mat A, const Mat B)
	{
		if (A.rows > 0 && A.rows == B.rows && A.cols > 0 && A.cols == B.cols)
		{
			// ������ ������������� ����������� L2 ��� ��������� ���� �����������. 
			// ���������� errorL2 �������� ����� ������������ � ������ ������� �����������.
			double errorL2 = norm(A, B, CV_L2);
			// ������� �������� ����������� � ����� ������� �����������
			Similarity = errorL2 / (double) (A.rows * A.cols);
			return Similarity;

		} else
			// ������� A � B ��������.
			// cout << "Warning: image A and B different. " << endl;
		Similarity = 100000000.0; // ����� ���������� ��������, ������� ���������������� ��� ������.
		
		return Similarity;

	}

	// ��������� ������������, ���� �� ������������� ���� � ���������
	int get_prediction(const Mat A) {
		
		double procent_unicalynosti = 0.0;
		int label_img;
		model->predict(A, label_img, procent_unicalynosti);
		double porog_threshold = model->getDouble("threshold"); // �������� ��������� �������� ������������ �� ������.

		//// ���� ������� ������������ ������ ��������� ������, ������ ������������� ����� ����� ���� ���������������
		if (procent_unicalynosti < porog_threshold) {
			return label_img;
			//return -1;
		}
	
		return -1;
	}
	// LBPH
	double similarity_img(const Mat &A, const Mat &B) {

		
		// ��������� �������� ���� �����������. ����������� � - ��������� � ������, ����������� B - ������������� �� ������ ������
		if (A.rows > 0 && A.rows == B.rows && A.cols > 0 && A.cols == B.cols)
		{
			// ������ ������������� ����������� L2 ��� ��������� ���� �����������. 
			// ���������� errorL2 �������� ����� ������������ � ������ ������� �����������.
			double errorL2 = norm(A, B, CV_L2);
			// ������� �������� ����������� � ����� ������� �����������
			Similarity = errorL2 / (double) (A.rows * A.cols);

			return Similarity;

		} else
		{
			// ������� A � B ��������. �������
			// cout << "Warning: image A and B different. " << endl;
			Similarity = 100000000.0; // ����� ���������� ��������, ������� ���������������� ��� ������.

		}

		return Similarity;
	}

	// ����� ��������� ��������� ������������ ���� �� ������ ������ (model) � ��������� ��� �����������:
	// ��������� ������������� B � ����������� ���� ���������� � ������ - �
	double reconstruction_and_similarity(const Mat A)
	{
		// ���������� ������������ ����
		Mat B = reconstructFace(A);
		// ��������� �������� ���� �����������. ����������� � - ��������� � ������, ����������� B - ������������� �� ������ ������
		if (A.rows > 0 && A.rows == B.rows && A.cols > 0 && A.cols == B.cols)
		{
			// ������ ������������� ����������� L2 ��� ��������� ���� �����������. 
			// ���������� errorL2 �������� ����� ������������ � ������ ������� �����������.
			double errorL2 = norm(A, B, CV_L2);
			// ������� �������� ����������� � ����� ������� �����������
			Similarity = errorL2 / (double) (A.rows * A.cols);

			return Similarity;

		} else
		{
			// ������� A � B ��������. �������
			// cout << "Warning: image A and B different. " << endl;
			Similarity = 100000000.0; // ����� ���������� ��������, ������� ���������������� ��� ������.
			
		}

		return Similarity;

	}


	// ������� ���������� �������, (��������������� ������� ������� ����� �������������� ����� � ��������) ��� �������������� 8-������� 
	// �����������. �������� ��������� ������������� 8-�� ������ ������� � ��������� �� 0 �� 255
	Mat getImageFrom1DFloatMat (const Mat matrixRow, int height)
	{
		// ������� ������������� �������
		Mat rectangularMat = matrixRow.reshape(1, height);

		// ������� 8-������ ����������� �� ������������� �������.
		Mat dst;

		normalize(rectangularMat, dst, 0, 255, NORM_MINMAX, CV_8UC1);
		
		return dst;
	}

	// ����� ���������� �������� ������������ ���, ���� �� ������� ���������� � ������, ����� ���������� ��� �������� ������
	int get_identichnosty(const Mat &face_to_camera)
	{
		//int label_index;
		//model->predict(face_to_camera, label_index, similarity);
		// � ���������� ����������� �������� ������������ � ���������.
		return model->predict(face_to_camera);
		// ����� ���������� ����� ������ � ��������� ������� ��������� ����� ������������ � ������ - face_to_camera
		// return label_index;
	}
//
//// �������
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