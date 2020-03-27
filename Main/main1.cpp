#include "../Libs/libusb-1.0/libusb.h"
#include "../Headers/USBlib.h"
#include <iostream>
#include <locale.h>

using namespace std;

int main() {
	setlocale(LC_ALL, "");

	libusb_device **devices;
	ssize_t cnt;
	//получаю контекст библиотеки
	libusb_context *context = USBlib::libInit();
	//получаю подключенные устройства
	USBlib::getDevices(context, devices, cnt);

	cout << "Device count: " << cnt << endl;
	//вывожу данные об устройствах
	for (ssize_t i = 0; i < cnt; i++)
		cout << USBlib::deviceToString(devices[i]) << endl;

	USBlib::libExit(context, devices);
	system("pause");
}