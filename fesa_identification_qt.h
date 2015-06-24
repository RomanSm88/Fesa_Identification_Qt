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
	QTimer *fps_timer; // Таймер с интервалом срабатывания равной частоте кадров. Частоту кадров полуаем с камеры.
	QPixmap cv_mat_to_qpixmap();
	QPixmap cv_mat_to_qpixmap(std::string file_name);
	void name_familiy(std::string &name_fam, std::string &name, std::string &fam);
	bool identification;
	bool loop_identification; // Непрерывный режим идентификации.
public:
	Fesa_Identification_Qt(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Fesa_Identification_Qt();
	Sbor_trenirovka *core_fase_identification;
	Sborka_dannxh *sborka;
public slots: 
		void identification_fase(); // Слот реализующий идентификацию человека
		void close_program(); // Слот реализующий выход из приложения.
		//void retunr_identification(); // Слот реализующий вывод результатов идентификации.
		void add_person(); // Слот реализующий создание далогового окна при добавлении объекта в коллекцию.
		void return_pixmap_img();// Слот реализующий вывод изображения с камеры
		void set_name_persona();
private:
	Ui::Fesa_Identification_QtClass ui;
};

#endif // FESA_IDENTIFICATION_QT_H
