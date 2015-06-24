#include "fesa_identification_qt.h"
#include <QTextCodec>


Fesa_Identification_Qt::Fesa_Identification_Qt(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags), core_fase_identification(new Sbor_trenirovka(0)), sborka(new Sborka_dannxh(*core_fase_identification, this)), fps_timer(new QTimer(this)), identification(false)
{
	ui.setupUi(this);
	
	// ��������� ���� ���������� �������.
	connect(ui.commandLinkButton, SIGNAL(clicked(bool)), this, SLOT(add_person()));

	// ��������� ��������� �������������
	connect(ui.commandLinkButton_2, SIGNAL(clicked(bool)), this, SLOT(identification_fase()));

	// ����� �� ���������.
	connect(ui.commandLinkButton_3, SIGNAL(clicked(bool)), this, SLOT(close()));
	
	// ��������� ���������� � ������� �����, ���������� �� �������������.
	this->ui.groupBox->setVisible(false);
	
	connect(fps_timer, SIGNAL(timeout()), this, SLOT(return_pixmap_img()));
	// ������������� ��������� �������� ������ �������, ������ ������� ������
	fps_timer->start(core_fase_identification->camera_fps());
	
	// ��������� ������ � ������� ���������.
	connect ((*sborka).ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(set_name_persona()));

	sborka->sborka_modeli = core_fase_identification->mashin_learning_sborka_modeli();
}

QPixmap Fesa_Identification_Qt::cv_mat_to_qpixmap()
{
	int num_img;
	cv::Mat src = core_fase_identification->return_frame_form();
	// ���������� ����������� ����������� �� ������� OpenCV, � ������� Qt.
	QImage img = QImage((const uchar *)src.data, src.cols, src.rows, QImage::Format_RGB888).rgbSwapped();
	QPixmap pixmap = QPixmap::fromImage(img);

	return pixmap;
}

QPixmap Fesa_Identification_Qt::cv_mat_to_qpixmap(std::string file_name)
{
		QImage img2;
		if (img2.load(file_name.c_str()))
		{
			QPixmap fase = QPixmap::fromImage(img2);
			return fase;
		}
		
		QPixmap fase;
			return fase;
}
void Fesa_Identification_Qt::return_pixmap_img()
{

	// �������� ����������� � ������.
	QPixmap pixmap = cv_mat_to_qpixmap();

	// ������� ����������� � ������� ���� ���������.
	ui.label->setPixmap(pixmap);
	
	
	// ���������� ������y, ���� ���� ������� �����.
	if (sborka->get_face_name())
	{
		if (core_fase_identification->save_face_jurnal())
			sborka->set_progress_bar(core_fase_identification->get_num_img());
		else
			sborka->clear_form();
	}
	
	
	// ��������� ������� �������������, ���� identification = true
	if (identification)
	{
		string name_fase;
		string name;
		string familiy;
		string file_name;
		int identich_progressBar = 0;
		cv::Mat foto_face;
		if (core_fase_identification->process_identification(identich_progressBar, name_fase, file_name))
		{
			
			// ������� 
			// qDebug() << identich_progressBar;
			name_familiy(name_fase, name, familiy);
			// �������� ��������� ��������� �� ���� ������� ����, ��� ��������� ����������� ������� ��������.
			QTextCodec *codec = QTextCodec::codecForName("UTF-8");
			QTextCodec::setCodecForCStrings(codec);
			
			ui.label_3->setText(QString(familiy.c_str()));
			ui.label_4->setText(QString(name.c_str()));
			// �������� ����������� �� ���������.
			ui.label_5->setPixmap(cv_mat_to_qpixmap(file_name));
			ui.progressBar->setValue(identich_progressBar);

			// ��� ����������� �������������
			identification = false;
			
			
		}
		/*
		else
		{
			// ��� ����������� �������������.
			ui.label_3->clear();
			ui.label_4->clear();
			ui.label_5->clear();
			ui.progressBar->setValue(0);
		}
		*/
	}
}
// ����� ������������ ������ �� �����, � ������ � ��������. ��� ����������� � ���������� name, ������� � ���������� fam
void Fesa_Identification_Qt::name_familiy(std::string &name_fam, std::string &name, std::string &fam)
{
	int index;
	for (int i = 0; i < name_fam.size(); i++)
	{
		if (name_fam[i] == sborka->RASDELITEL())
		{
			index = i;
		}

	}

	for (int i = 0; i < name_fam.size(); i++)
	{
		if (i < index)
			fam += name_fam[i];
		if (i > index)
			name += name_fam[i];
	}
}
void Fesa_Identification_Qt::add_person()
{
	// ���� ��� ������� ����� �������������, �� ��������� ���
	if (identification) {
		
		identification = false;
	}
		
	// ���� ���������� ������������� �� ����� ���� ��������, �� ��������� ��.
	if (this->ui.groupBox->isVisible())
	{
		this->ui.groupBox->setVisible(false);
	}
	sborka->show_form();
}

void Fesa_Identification_Qt::set_name_persona()
{
	sborka->start_safe_person();
}

void Fesa_Identification_Qt::identification_fase()
{	
	// ���� ������ ������������� ��������� �� ���������� �����, �� ���������� �� ������.
	if (!sborka->sborka_modeli)
	{
		sborka->sborka_modeli = core_fase_identification->mashin_learning_sborka_modeli();
		
	}
	ui.label_3->clear();
	ui.label_4->clear();
	ui.label_5->clear();
	ui.progressBar->setValue(0);
	
		// ���� ���� ��������� ������, ������, ������������� � ������ ������� ������ �� �����������.
	if (!this->ui.groupBox->isVisible())
	{	

		this->ui.groupBox->setVisible(true);
	}

	// ��������� ����� �������������
	identification = true;
		
	

}

void Fesa_Identification_Qt::close_program()
{

	this->~Fesa_Identification_Qt();

}


Fesa_Identification_Qt::~Fesa_Identification_Qt()
{
	delete core_fase_identification;
	delete sborka;
	delete fps_timer;
}
