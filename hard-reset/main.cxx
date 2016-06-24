#include <QCoreApplication>
#include <QDebug>

#include "usbprinter.hxx"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	USBPrinter p;
    
	if (!p.open_first_usb_printer())
		qDebug() << "no usb printers found";
	else
	{
		qDebug() << "resetting first usb printer...";
		p.hard_reset();
	}
		
	return 0;
}
