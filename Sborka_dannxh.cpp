#include "Sborka_dannxh.h"
#include <QMessageBox>
#include <QTextCodec>

Sborka_dannxh::Sborka_dannxh(Sbor_trenirovka &jur, QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags), ui(new Ui::Form), face_name(""), jurnal(jur), set_persona(false), sborka_modeli(false)
{
	ui->setupUi(this);
	
}


void Sborka_dannxh::show_form()
{
	this->show();
	
}

void Sborka_dannxh::start_safe_person()
{
	// Стоки сохраняются с корректным русскими символами.
	std::string name_2 = (*ui->lineEdit).text().toUtf8();
	std::string name_1 = (*ui->lineEdit_2).text().toUtf8();
	face_name = name_1 + RASDELITEL() + name_2;

	set_persona = jurnal.set_person(face_name);
}
void Sborka_dannxh::clear_form()
{
	
	// Задаем кодировку для русскоязычных сообщений, прописанных в исходном коде.
	// По правилам необходимо писать сообщения на английском и создавать в папке ресурсов, файл перевода, в нужной кодировки.
	// Для создания файла перевода, читать Internationalization with Qt.
	QTextCodec *codec = QTextCodec::codecForName("CP1251");
	QTextCodec::setCodecForTr(codec);

	// QTextCodec::setCodecForCStrings(codec);
	// QTextCodec::setCodecForLocale(codec);

	// Собираем модель исккуственного интелекта.
	sborka_modeli = jurnal.mashin_learning_sborka_modeli();

	QMessageBox::information(this, tr("Сохранение объекта..."), tr("Объект успешно сохранен."), QMessageBox::Cancel);
	set_persona = false;
	// Перечитываем журнал коллекции, и заполняем контейнер новыми снимками.
	jurnal.read_jurnal_to_identification();

	// Очищаем поля ввода
	(*ui->lineEdit).clear();
	(*ui->lineEdit_2).clear();
	(*ui->progressBar_2).setValue(0);
	
}
bool Sborka_dannxh::get_face_name()
{
	return set_persona;
}
void Sborka_dannxh::set_progress_bar(int i)
{
	
	float  index = jurnal.get_index_jurnal(); // Значение индекса в файле журнала
	float count_fase =jurnal.count_fase_img();  // Колличество образцов для каждой персоны.
	float wan_procent = (i-index)/count_fase; // Вещественное значение
	float procent_vxpolneniya = wan_procent * 100; // Перевод вещественного значения в проценты.
	
	(*ui->progressBar_2).setValue(procent_vxpolneniya);
	
}
void Sborka_dannxh::close_form()
{
	this->~Sborka_dannxh();
}

Sborka_dannxh::~Sborka_dannxh()
{
	delete ui;
}