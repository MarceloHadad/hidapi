/*******************************************************
 HIDAPI - Multi-Platform library for
 communication with HID devices.

 Alan Ott
 Signal 11 Software

 libusb/hidapi Team

 Copyright 2022.

 This contents of this file may be used by anyone
 for any reason without any conditions and may be
 used as a starting point for your own applications
 which use HIDAPI.
********************************************************/

#include "pch.hpp"

// Headers needed for sleeping.
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// Fallback/example
#ifndef HID_API_MAKE_VERSION
#define HID_API_MAKE_VERSION(mj, mn, p) (((mj) << 24) | ((mn) << 8) | (p))
#endif
#ifndef HID_API_VERSION
#define HID_API_VERSION HID_API_MAKE_VERSION(HID_API_VERSION_MAJOR, HID_API_VERSION_MINOR, HID_API_VERSION_PATCH)
#endif

//
// Sample using platform-specific headers
#if defined(__APPLE__) && HID_API_VERSION >= HID_API_MAKE_VERSION(0, 12, 0)
#include <hidapi_darwin.h>
#endif

#if defined(_WIN32) && HID_API_VERSION >= HID_API_MAKE_VERSION(0, 12, 0)
#include <hidapi_winapi.h>
#endif

#if defined(USING_HIDAPI_LIBUSB) && HID_API_VERSION >= HID_API_MAKE_VERSION(0, 12, 0)
#include <hidapi_libusb.h>
#endif
//

hid_device *Handle;
unsigned char Buffer[64];

void getHidDeviceHandle(struct hid_device_info *cur_dev, std::string manufacturer)
{
	std::wstring wstr = std::wstring(manufacturer.begin(), manufacturer.end());
	const wchar_t *searchedManufacturer = wstr.c_str();
	for (; cur_dev; cur_dev = cur_dev->next)
	{
		if (cur_dev->vendor_id == 2765)
		{
			if (wcscmp(cur_dev->manufacturer_string, searchedManufacturer) == 0)
			{
				hid_device *handle;

				handle = hid_open_path(cur_dev->path);
				Handle = handle;
				return;
			}
		}
	}

	throw std::runtime_error("[Error] Device not found!");
}

static std::string ConvertWideCharToString(const wchar_t *wstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.to_bytes(wstr);
}

int main()
{
	try
	{
		struct hid_device_info *devices;

		devices = hid_enumerate(0x0, 0x0);
		getHidDeviceHandle(devices, "ID TECH");
		if (!Handle)
		{
			std::cout << "!Handle\n";
			hid_exit();
		}
		struct hid_device_info *info = hid_get_device_info(Handle);

		std::cout << "info:"
				  << "\nmanufacturer_string: " << ConvertWideCharToString(info->manufacturer_string) << "\nproduct_string: " << ConvertWideCharToString(info->product_string) << "\nserial_number: " << ConvertWideCharToString(info->serial_number) << "\n";

		hid_free_enumeration(devices);
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	catch (...)
	{
		std::cerr << "Unknown exception!" << std::endl;
		return 2;
	}
	return 0;
}