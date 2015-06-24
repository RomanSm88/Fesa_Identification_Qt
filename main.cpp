#include "fesa_identification_qt.h"
#include <QtGui/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Fesa_Identification_Qt w;

	//w.return_pixmap_img();

	w.show();

	
	


	return a.exec();
}
