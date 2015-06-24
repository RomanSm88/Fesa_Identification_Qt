#ifndef SBORKA_DANNXH_H
#define SBORKA_DANNXH_H

#include <QDialog>
#include "ui_Sbor_dannxh.h"
#include <./Core/Sborka_trenirovka.h>

class Sborka_dannxh : public QDialog
{
	Q_OBJECT    
private:
	std::string face_name;
	Sbor_trenirovka &jurnal;
	bool set_persona;
public:
	Sborka_dannxh(Sbor_trenirovka &jur, QWidget *parent = 0, Qt::WFlags flags = 0);
	~Sborka_dannxh();
	void show_form(); // Отобразить форму.
	void start_safe_person();
	void set_progress_bar(int i);
	bool get_face_name();
	void clear_form();
	bool sborka_modeli;
	// Разделитель между именем и фамилией в журнале коллекции.
	const char RASDELITEL() { return '_'; }
private slots:
	void close_form();
	

public:
	Ui::Form *ui;
};

#endif // FESA_IDENTIFICATION_QT_H