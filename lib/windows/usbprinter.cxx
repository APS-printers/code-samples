/*
Copyright (c) 2016 APS Trading EOOD

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/   

#include "usbprinter.hxx"
#include <QDebug>

USBPrinter::USBPrinter()
{
}

bool USBPrinter::open_first_usb_printer()
{
#define	MAX_SIZE				256
#define USB_HEADER				"\\\\?\\"
#define USB_DEVICE_KEY			"#{28d78fad-5a12-11d1-ae5b-0000f803a8c2}"
#define USB_PRINT_REGISTER_KEY  L"SYSTEM\\CurrentControlSet\\Services\\usbprint\\Enum"

        TCHAR PortName[MAX_SIZE];
        TCHAR p[MAX_SIZE];
        DWORD size = MAX_SIZE;
        HKEY hUSBPRN;

        if (RegOpenKey(HKEY_LOCAL_MACHINE, USB_PRINT_REGISTER_KEY, &hUSBPRN) == ERROR_SUCCESS) {
                if (ERROR_SUCCESS == RegQueryValueEx(hUSBPRN, L"0", NULL, NULL, (LPBYTE)p, &size)) {
                        //QMessageBox::information(0, "", QByteArray(p, size).toPercentEncoding());
                        for (int i = 0; i < size; i++)
                                if (p[i] == '\\') p[i] = '#';
                        swprintf(PortName, L"%s%ls%s", USB_HEADER, p, USB_DEVICE_KEY);
                        if (0) QMessageBox::information(0, "", QString::fromWCharArray(PortName));
                        qDebug() << "found printer: " << QString::fromWCharArray(PortName);

                        if((hPort = CreateFile(PortName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0)) == INVALID_HANDLE_VALUE) {
                                qDebug() << "failed to open printer";
                                return false;
                        }
                        if (0) QMessageBox::information(0, "", "createfile succeeded");
                }
                else {
                        qDebug() << "USB printer not found.";
                        RegCloseKey(hUSBPRN);
                        return false;
                }
                RegCloseKey(hUSBPRN);
        }
        else
                return false;
        return true;
}

int USBPrinter::write(const QByteArray &data)
{
DWORD x;
        WriteFile(hPort, data.data(), data.length(), & x, 0);
        return x;
}

void USBPrinter::hard_reset()
{
#define USBPRINT_IOCTL_INDEX  0x0000
#define IOCTL_USBPRINT_VENDOR_SET_COMMAND CTL_CODE(FILE_DEVICE_UNKNOWN,  \
						   USBPRINT_IOCTL_INDEX+14,\
						   METHOD_BUFFERED,  \
						   FILE_ANY_ACCESS)   
	
		BYTE inbuf[]={2, 0 >> 8, 0 & 0xFF};
		DWORD returned;
		DeviceIoControl(handle(), IOCTL_USBPRINT_VENDOR_SET_COMMAND, inbuf, sizeof inbuf, 0, 0, & returned, 0);
}
