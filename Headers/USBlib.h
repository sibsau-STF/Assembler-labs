#pragma once
#include "../Libs/libusb-1.0/libusb.h";
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
using namespace std;

namespace USBlib {
	namespace USBClass {

/*
Коды классов USB устройств
00h - код отсутствует (информацию о классе нужно получать в дескрипторе интерфейса)
01h - аудиоустройство (если код получен из дескриптора интерфейса, а не устройства)
02h - коммуникационное устройство (сетевой адаптер)
03h - устройство пользовательского интерфейса 
05h - физическое устройство
06h - изображени¤
07h - принтер
08h - устройство хранени¤ данных
09h - концентратор
0Ah - CDC-Data 
0Bh - Smart Card 
0Dh - ContentSecurity
0Eh - видеоустройство
0Fh - персональное медицинское устройство
10h - аудио-и видеоустройства
DCh - диагностическое устройство
E0h - беспроводный контроллер
EFh - различные устройства
FE	- специфическое устройство
*/
	//Класс устройства
	enum USBClass {
		None = 0x00,
		Audio = 0x01,
		Communication = 0x02,
		HID = 0x03,
		Physical = 0x05,
		Image = 0x06,
		Printer = 0x07,
		Storage = 0x08,
		Concentrator = 0x09,
		CDCdata = 0x0A,
		SmartCard = 0x0B,
		ContentSecurity = 0x0D,
		VideoDevice = 0x0E,
		MedicalDevice = 0x0F,
		AudioVideoDevice = 0x10,
		DiagnosticTool = 0xDC,
		RemoteController = 0xE0,
		VariousDevice = 0xEF,
		SpecificDevice = 0xFE,
	};

	//Получает класс подключенного устройства
	string parsedUSBClass(USBClass cls) {
		switch (cls)
		{
		case USBClass::None:
			return "No code";
		case USBClass::Audio:
			return "Audio Device";
		case USBClass::Communication:
			return "Communication Device";;
		case USBClass::HID:
			return "Human Interface Device";
		case USBClass::Physical:
			return "Physical Device";
		case USBClass::Image:
			return "Image Device";
		case USBClass::Printer:
			return "Printer";
		case USBClass::Storage:
			return "Storage";
		case USBClass::Concentrator:
			return "Concentrator";
		case USBClass::CDCdata:
			return "CDC-Data";
		case USBClass::SmartCard:
			return "SmartCard";
		case USBClass::ContentSecurity:
			return "ContentSecurity";
		case USBClass::VideoDevice:
			return "Video Device";
		case USBClass::MedicalDevice:
			return "Medical Device";
		case USBClass::AudioVideoDevice:
			return "Audio-Video Device";
		case USBClass::DiagnosticTool:
			return "Diagnostic Tool";
		case USBClass::RemoteController:
			return "Remote Controller";
		case USBClass::VariousDevice:
			return "Various Device";
		case USBClass::SpecificDevice:
			return "Specific Device";
		default:
			break;
		}
	}
	}

	//Инициализирует libusb
	libusb_context* libInit() {
		libusb_device **devices;
		libusb_context *context = NULL; // контекст сессии libusb 
		int r;          // дл¤ возвращаемых значений
		r = libusb_init(&context);
		if (r < 0)
			throw exception("Не удалось инициализировать libusb");
		libusb_set_debug(context, 3);
		return context;
	}

	//Высвобождает ресурсы libusb
	void libExit(libusb_context *context, libusb_device** &devices) {
		libusb_free_device_list(devices, 1);
		libusb_exit(context);
	}

	//Получает список устройств
	void getDevices(libusb_context *context, libusb_device** &devices, ssize_t &count) {
		count = libusb_get_device_list(context, &devices);
	}

	//Получает дескриптор устройства
	libusb_device_descriptor deviceDescriptor(libusb_device *device) {
		libusb_device_descriptor desc;
		int r = libusb_get_device_descriptor(device, &desc);
		if (r < 0) {
			stringstream str = stringstream();
			str << "Ошибка: не удалось получить дескриптор устройства. Код:";
			str << r;
			throw exception(str.str().c_str());
		}
		return desc;
	}

	//Получает конфигурацию устройства(функци¤ не используется)
	libusb_config_descriptor* deviceConfiguration(libusb_device *device) {
		libusb_config_descriptor *config;
		int r = r = libusb_get_config_descriptor(device, 0, &config);
		if (r < 0) {
			stringstream str = stringstream();
			str << "Ошибка: конфигурация устройства не получена. Код:";
			str << r;
			throw exception(str.str().c_str());
		}
		return config;
	}

	//Позвращает серийный номер устройства
	string deviceSerialNum(libusb_device *device) {
		libusb_device_handle *handler;
		libusb_device_descriptor desc;
		int r = libusb_get_device_descriptor(device, &desc);
		r = libusb_open(device, &handler);
		if (r != LIBUSB_SUCCESS)
		{
			stringstream str = stringstream();
			str << "Ошибка: не удаётся получить доступ к устройству. Код:";
			str << r;
			throw exception(str.str().c_str());
		}
		unsigned char* data = new unsigned char[256];
		size_t length=0;
		//получаю строковое представление серийника
		libusb_get_string_descriptor_ascii(handler, desc.iSerialNumber, data, length);
		return string((char*)data, length);
	}

	//Получает сведения об устройстве
	string deviceDescription(libusb_device *device) {
		libusb_device_descriptor desc; // дескриптор устройства
		try
		{
			desc = deviceDescriptor(device);
		}
		catch (const std::exception& e)
		{
			return string(e.what());
		}

		stringstream str = stringstream();
		str << "USB Class: \t" << USBClass::parsedUSBClass((USBClass::USBClass)desc.bDeviceClass) << "(0x" << uppercase << hex << setprecision(2) << (USBClass::USBClass)desc.bDeviceClass << ")\n";
		str << "Vendor id:\t0x" << uppercase << hex << setprecision(4) << desc.idVendor << endl;
		str << "Product id:\t0x" << uppercase << hex << setprecision(4) << desc.idProduct << endl;
		try
		{
			str << "Serial:\t" << uppercase << deviceSerialNum(device) << endl;
		}
		catch (const std::exception&)
		{
			str << "No serial" << endl;
		}
		str << endl;
		return str.str();
	}

}
