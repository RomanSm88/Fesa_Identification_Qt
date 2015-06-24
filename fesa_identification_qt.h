#ifndef FESA_IDENTIFICATION_QT_H
#define FESA_IDENTIFICATION_QT_H

#include <QtGui/QWidget>
#include "ui_fesa_identification_qt.h"
#include "Sborka_dannxh.h"
#include "Core/Sborka_trenirovka.h"
#include <QTimer>

class Fesa_Identification_Qt : public QWidget
{
	Q_OBJECT
private:
	QTimer *fps_timer; // ������ � ���������� ������������ ������ ������� ������. ������� ������ ������� � ������.
	QPixmap cv_mat_to_qpixmap();
	QPixmap cv_mat_to_qpixmap(std::string file_name);
	void name_familiy(std::string &name_fam, std::string &name, std::string &fam);
	bool identification;
	bool loop_identification; // ����������� ����� �������������.
public:
	Fesa_Identification_Qt(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Fesa_Identification_Qt();
	Sbor_trenirovka *core_fase_identification;
	Sborka_dannxh *sborka;
public slots: 
		void identification_fase(); // ���� ����������� ������������� ��������
		void close_program(); // ���� ����������� ����� �� ����������.
		//void retunr_identification(); // ���� ����������� ����� ����������� �������������.
		void add_person(); // ���� ����������� �������� ���������� ���� ��� ���������� ������� � ���������.
		void return_pixmap_img();// ���� ����������� ����� ����������� � ������
		void set_name_persona();
private:
	Ui::Fesa_Identification_QtClass ui;
};

#endif // FESA_IDENTIFICATION_QT_H
