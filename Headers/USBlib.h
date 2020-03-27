#pragma once
#include "../Libs/libusb-1.0/libusb.h";
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

namespace USBlib {
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
	string getUSBClass(USBClass cls) {
		switch (cls)
		{
		case USBlib::None:
			return "No code";
		case USBlib::Audio:
			return "Audio Device";
		case USBlib::Communication:
			return "Communication Device";;
		case USBlib::HID:
			return "Human Interface Device";
		case USBlib::Physical:
			return "Physical Device";
		case USBlib::Image:
			return "Image Device";
		case USBlib::Printer:
			return "Printer";
		case USBlib::Storage:
			return "Storage";
		case USBlib::Concentrator:
			return "Concentrator";
		case USBlib::CDCdata:
			return "CDC-Data";
		case USBlib::SmartCard:
			return "SmartCard";
		case USBlib::ContentSecurity:
			return "ContentSecurity";
		case USBlib::VideoDevice:
			return "Video Device";
		case USBlib::MedicalDevice:
			return "Medical Device";
		case USBlib::AudioVideoDevice:
			return "Audio-Video Device";
		case USBlib::DiagnosticTool:
			return "Diagnostic Tool";
		case USBlib::RemoteController:
			return "Remote Controller";
		case USBlib::VariousDevice:
			return "Various Device";
		case USBlib::SpecificDevice:
			return "Specific Device";
		default:
			break;
		}
	}
	//Инициализирует libusb
	libusb_context* libInit() {
		libusb_device **devices;
		libusb_context *context = NULL; // контекст сессии libusb 
		int r;          // дл¤ возвращаемых значений
		r = libusb_init(&context);
		if (r < 0)
			throw exception("Ќе удалось инициализировать libusb");
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
	libusb_device_descriptor getDeviceDescriptor(libusb_device *device) {
		libusb_device_descriptor desc;
		int r = libusb_get_device_descriptor(device, &desc);
		if (r < 0) {
			stringstream str = stringstream();
			str << "ќшибка: не удалось получить дескриптор устройства.  од:";
			str << r;
			throw exception(str.str().c_str());
		}
		return desc;
	}

	//Получает конфигурацию устройства(функци¤ не используетс¤)
	libusb_config_descriptor* getDeviceConfiguration(libusb_device *device) {
		libusb_config_descriptor *config;
		int r = r = libusb_get_config_descriptor(device, 0, &config);
		if (r < 0) {
			stringstream str = stringstream();
			str << "ќшибка: конфигураци¤ устройства не получена.  од:";
			str << r;
			throw exception(str.str().c_str());
		}
		return config;
	}

	//Позвращает серийный номер устройства
	string deviceSerialNum(libusb_device *device) {
		libusb_device_handle *handler;
		int r = libusb_open(device, &handler);
		if (r != 0)
		{
			stringstream str = stringstream();
			str << "ќшибка: не удаЄтс¤ получить доступ к устройству.  од:";
			str << r;
			throw exception(str.str().c_str());
		}
		unsigned char* data = new unsigned char[256];
		size_t length=0;
		//получаю строковое представление серийника
		libusb_get_string_descriptor_ascii(handler, 0, data, length);
		return string((char*)data, length);
	}

	//Получает сведени¤ об устройстве
	string deviceToString(libusb_device *device) {
		libusb_device_descriptor desc; // дескриптор устройства
		try
		{
			desc = getDeviceDescriptor(device);
		}
		catch (const std::exception& e)
		{
			return string(e.what());
		}

		stringstream str = stringstream();
		str << "USB Class: \t"<<getUSBClass((USBClass)desc.bDeviceClass) << "(0x" << uppercase << hex << (USBClass)desc.bDeviceClass << ")\n";
		str << "Vendor id:\t0x"<<uppercase <<hex << desc.idVendor << endl;
		str << "Product id:\t0x" << uppercase <<hex << desc.idProduct <<endl;
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
