// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// Defines the entry point for the console application.
//

#include "stdafx.h"

#include "CommonFunctions.h"
#include "StringHandle.h"

// Device enumeration
DWORD EnumerateAllDevices();
void ChooseDevice(_Outptr_result_maybenull_ IPortableDevice** device);

// Content enumeration
void EnumerateAllContent(_In_ IPortableDevice* device);
void ReadHintLocations(_In_ IPortableDevice* device);

// Content transfer
void TransferContentFromDevice(_In_ IPortableDevice* device);
int TransferContentFromDeviceAuto(
	_In_ IPortableDevice* device, WCHAR* pObjID);
void TransferContentToDevice(
    _In_ IPortableDevice* device,
    _In_ REFGUID          contentType,
    _In_ PCWSTR           fileTypeFilter,
    _In_ PCWSTR           defaultFileExtension);
void TransferContactToDevice(_In_ IPortableDevice* device);
void CreateFolderOnDevice(_In_ IPortableDevice* device);
void CreateContactPhotoResourceOnDevice(_In_ IPortableDevice* device);

// Content deletion
void DeleteContentFromDevice(_In_ IPortableDevice* device);

// Content moving
void MoveContentAlreadyOnDevice(_In_ IPortableDevice* device);

// Content update (properties and data simultaneously)
void UpdateContentOnDevice(
    _In_ IPortableDevice* device,
    _In_ REFGUID          contentType,
    _In_ PCWSTR           fileTypeFilter,
    _In_ PCWSTR           defaultFileExtension);
    
// Content properties
void ReadContentProperties(_In_ IPortableDevice* device);
void WriteContentProperties(_In_ IPortableDevice* device);
void ReadContentPropertiesBulk(_In_ IPortableDevice* device);
void WriteContentPropertiesBulk(_In_ IPortableDevice* device);
void ReadContentPropertiesBulkFilteringByFormat(_In_ IPortableDevice* device);

// Functional objects
void ListFunctionalObjects(_In_ IPortableDevice* device);
void ListFunctionalCategories(_In_ IPortableDevice* device);
void ListSupportedContentTypes(_In_ IPortableDevice* device);
void ListRenderingCapabilityInformation(_In_ IPortableDevice* device);

// Device events
void ListSupportedEvents(_In_ IPortableDevice* device);
void RegisterForEventNotifications(_In_ IPortableDevice* device, _Inout_ PWSTR* eventCookie);
void UnregisterForEventNotifications(_In_opt_ IPortableDevice* device, _In_opt_ PCWSTR eventCookie);

// Misc.
void GetObjectIdentifierFromPersistentUniqueIdentifier(_In_ IPortableDevice* device);

void DoMenu()
{
    HRESULT hr                  = S_OK;
    UINT    selectionIndex      = 0;
    PWSTR   eventCookie         = nullptr;
    WCHAR   selectionString[SELECTION_BUFFER_SIZE] = {0};
    ComPtr<IPortableDevice> device;

    ChooseDevice(&device);

    if (device == nullptr)
    {
        // No device was selected, so exit immediately.
        return;
    }

    while (selectionIndex != 99)
    {
        ZeroMemory(selectionString, sizeof(selectionString));
        wprintf(L"\n\n");
        wprintf(L"WPD Sample Application \n");
        wprintf(L"=======================================\n\n");
        wprintf(L"0.  Enumerate all Devices\n");
        wprintf(L"1.  Choose a Device\n");
        wprintf(L"2.  Enumerate all content on the device\n");
        wprintf(L"3.  Transfer content from the device\n");
        wprintf(L"4.  Delete content from the device\n");
        wprintf(L"5.  Move content already on the device to another location on the device\n");
        wprintf(L"6   Transfer Image content to the device\n");
        wprintf(L"7.  Transfer Music content to the device\n");
        wprintf(L"8.  Transfer Contact (VCARD file) content to the device\n");
        wprintf(L"9.  Transfer Contact (Defined by Properties Only) to the device\n");
        wprintf(L"10. Create a folder on the device\n");
        wprintf(L"11. Add a Contact Photo resource to an object\n");
        wprintf(L"12. Read properties on a content object\n");
        wprintf(L"13. Write properties on a content object\n");
        wprintf(L"14. Get an object identifier from a Persistent Unique Identifier (PUID)\n");
        wprintf(L"15. List all functional categories supported by the device\n");
        wprintf(L"16. List all functional objects on the device\n");
        wprintf(L"17. List all content types supported by the device\n");
        wprintf(L"18. List rendering capabilities supported by the device\n");
        wprintf(L"19. Register to receive device event notifications\n");
        wprintf(L"20. Unregister from receiving device event notifications\n");
        wprintf(L"21. List all events supported by the device\n");
        wprintf(L"22. List all hint locations supported by the device\n");
        wprintf(L"==(Advanced BULK property operations)==\n");
        wprintf(L"23. Read properties on multiple content objects\n");
        wprintf(L"24. Write properties on multiple content objects\n");
        wprintf(L"25. Read properties on multiple content objects using object format\n");
        wprintf(L"==(Update content operations)==\n");
        wprintf(L"26. Update Image content (properties and data) on the device\n");
        wprintf(L"27. Update Music content (properties and data) on the device\n");
        wprintf(L"28. Update Contact content (properties and data) on the device\n");
        wprintf(L"99. Exit\n");
        hr = StringCchGetsW(selectionString, ARRAYSIZE(selectionString));
        if (SUCCEEDED(hr))
        {
            selectionIndex = static_cast<UINT>(_wtoi(selectionString));
            switch (selectionIndex)
            {
                case 0:
                    EnumerateAllDevices();
                    break;
                case 1:
                    // Unregister any device event registrations before
                    // creating a new IPortableDevice
                    UnregisterForEventNotifications(device.Get(), eventCookie);
                    CoTaskMemFree(eventCookie);
                    eventCookie = nullptr;

                    // Release the old IPortableDevice interface before
                    // obtaining a new one.
                    device = nullptr;
                    ChooseDevice(&device);
                    break;
                case 2:
                    EnumerateAllContent(device.Get());
                    break;
                case 3:
                    TransferContentFromDevice(device.Get());
                    break;
                case 4:
                    DeleteContentFromDevice(device.Get());
                    break;
                case 5:
                    MoveContentAlreadyOnDevice(device.Get());
                    break;
                case 6:
                    TransferContentToDevice(device.Get(),
                                            WPD_CONTENT_TYPE_IMAGE,
                                            L"JPEG (*.JPG)\0*.JPG\0JPEG (*.JPEG)\0*.JPEG\0JPG (*.JPE)\0*.JPE\0JPG (*.JFIF)\0*.JFIF\0\0",
                                            L"JPG");
                    break;
                case 7:
                    TransferContentToDevice(device.Get(),
                                            WPD_CONTENT_TYPE_AUDIO,
                                            L"MP3 (*.MP3)\0*.MP3\0\0",
                                            L"MP3");
                    break;
                case 8:
                    TransferContentToDevice(device.Get(),
                                            WPD_CONTENT_TYPE_CONTACT,
                                            L"VCARD (*.VCF)\0*.VCF\0\0",
                                            L"VCF");
                    break;
                case 9:
                    TransferContactToDevice(device.Get());
                    break;
                case 10:
                    CreateFolderOnDevice(device.Get());
                    break;
                case 11:
                    CreateContactPhotoResourceOnDevice(device.Get());
                    break;
                case 12:
                    ReadContentProperties(device.Get());
                    break;
                case 13:
                    WriteContentProperties(device.Get());
                    break;
                case 14:
                    GetObjectIdentifierFromPersistentUniqueIdentifier(device.Get());
                    break;
                case 15:
                    ListFunctionalCategories(device.Get());
                    break;
                case 16:
                    ListFunctionalObjects(device.Get());
                    break;
                case 17:
                    ListSupportedContentTypes(device.Get());
                    break;
                case 18:
                    ListRenderingCapabilityInformation(device.Get());
                    break;
                case 19:
                    RegisterForEventNotifications(device.Get(), &eventCookie);
                    break;
                case 20:
                    UnregisterForEventNotifications(device.Get(), eventCookie);
                    CoTaskMemFree(eventCookie);
                    eventCookie = nullptr;
                    break;
                case 21:
                    ListSupportedEvents(device.Get());
                    break;
                case 22:
                    ReadHintLocations(device.Get());
                    break;
                case 23:
                    ReadContentPropertiesBulk(device.Get());
                    break;
                case 24:
                    WriteContentPropertiesBulk(device.Get());
                    break;
                case 25:
                    ReadContentPropertiesBulkFilteringByFormat(device.Get());
                    break;
                case 26:
                    UpdateContentOnDevice(device.Get(),
                                          WPD_CONTENT_TYPE_IMAGE,
                                          L"JPEG (*.JPG)\0*.JPG\0JPEG (*.JPEG)\0*.JPEG\0JPG (*.JPE)\0*.JPE\0JPG (*.JFIF)\0*.JFIF\0\0",
                                          L"JPG");
                    break;
                case 27:
                    UpdateContentOnDevice(device.Get(),
                                          WPD_CONTENT_TYPE_AUDIO,
                                          L"MP3 (*.MP3)\0*.MP3\0\0",
                                          L"MP3");
                    break;
                case 28:
                    UpdateContentOnDevice(device.Get(),
                                          WPD_CONTENT_TYPE_CONTACT,
                                          L"VCARD (*.VCF)\0*.VCF\0\0",
                                          L"VCF");
                    break;
                default:
                    break;
            }
        }
        else
        {
            wprintf(L"! Failed to read menu selection string input, hr = 0x%lx\n", hr);
        }
    }
    CoTaskMemFree(eventCookie);
}

/*************************************/

/*************************************/
void showUI()
{
	wprintf(L"\n\n");
	wprintf(L"WPD Sample Application \n");
	wprintf(L"===================================================\n");
	wprintf(L"*********Tranfer photos from iPhone to PC**********\n");
	wprintf(L"***************************************************\n");
	wprintf(L"***************************************************\n");
	wprintf(L"***************************************************\n");
	wprintf(L"***************************************************\n");
	wprintf(L"===================================================\n\n");
}

WCHAR  wcPathSelection[PATH_SELECTION_BUFFER_SIZE] = { 0 };
char   cPathSelArr[PATH_SELECTION_BUFFER_SIZE] = { 0 };

void DoTransfer()
{
	HRESULT hr = S_OK;
	UINT    selectionIndex = 0;
	PWSTR   eventCookie = nullptr;
	WCHAR   selectionString[SELECTION_BUFFER_SIZE] = { 0 };
	ComPtr<IPortableDevice> device;

	UINT    numberStartX = 0;
	UINT    numberEndY = 0;

	while (1)
	{
		objCount = 0;

		showUI();

		ChooseDevice(&device);
		if (device == nullptr)
		{
			// No device was selected, so exit immediately.
			return;
		}

		//Prompt user to enter a destinationDir to transfer.
		wprintf(L"Enter the destination directory you wish to transfer.\n>");
		hr = StringCchGetsW(wcPathSelection, ARRAYSIZE(wcPathSelection));
		if (FAILED(hr))
		{
			wprintf(L"An invalid object identifier was specified, aborting content transfer\n");
		}
		wprintf(L"pathSelection : %ws\n", wcPathSelection);

		wprintf(L"EnumerateAllContent begin ...\n");
		EnumerateAllContent(device.Get());
		wprintf(L"\n There are [%d] photos on this device!\n\n", objCount);

		UINT cnt = 0;
		std::vector<PWCHAR> wcObjArr;
		for ( auto& newStr : newObjArr)
		{
			std::wcout << "cnt = "<< cnt++ << ", newStr:" << newStr << std::endl;
			wchar_t* wcNewObj = (wchar_t*)newStr.c_str();
			wcObjArr.push_back(wcNewObj);
			wprintf(L"+++wcNewObj = %ws\n", wcNewObj);
		}
		wprintf(L"\n The latest [%d] photos will be tranferred!\n", cnt);
		wprintf(L"\n +++The latest [%d] photos will be tranferred!\n", wcObjArr.size());

		wprintf(L"\n Do you confirm start to trasfer?  0:yes \n>");
		ZeroMemory(selectionString, sizeof(selectionString));
		hr = StringCchGetsW(selectionString, ARRAYSIZE(selectionString));
		if (SUCCEEDED(hr))
		{
			selectionIndex = static_cast<UINT>(_wtoi(selectionString));
		}
		if (selectionIndex == 0)
		{
			int retTrans = -1;
			std::wstring tbWtObj;
			std::vector <std::wstring> toBeWriteObjArr;
			for (auto& newWcObj : wcObjArr)
			{
				wprintf(L"\n Transfer newWcObj = %ws\n\n", newWcObj);
				retTrans = TransferContentFromDeviceAuto(device.Get(), newWcObj);
				if (retTrans == 0)
				{
					tbWtObj = newWcObj;
					toBeWriteObjArr.push_back(tbWtObj);
				}
			}
			wprintf(L"\n +++There are [%d] photos have been tranferred!\n\n", toBeWriteObjArr.size());
			writeArrayToFile(toBeWriteObjArr, L"ObjArray.txt");
		}



		/*********************************************************/
		//wprintf(L"\n Photos numbered X through Y will be transferred. X < Y\n");
		//wprintf(L"\n Please enter the starting number X\n>");
		//ZeroMemory(selectionString, sizeof(selectionString));
		//hr = StringCchGetsW(selectionString, ARRAYSIZE(selectionString));
		//if (SUCCEEDED(hr))
		//{
		//	selectionIndex = static_cast<UINT>(_wtoi(selectionString));
		//	numberStartX = selectionIndex;
		//}

		//wprintf(L"\n Please enter the ending number Y\n>");
		//ZeroMemory(selectionString, sizeof(selectionString));
		//hr = StringCchGetsW(selectionString, ARRAYSIZE(selectionString));
		//if (SUCCEEDED(hr))
		//{
		//	selectionIndex = static_cast<UINT>(_wtoi(selectionString));
		//	numberEndY = selectionIndex;
		//}
		/*******************************************************/

		//if (numberStartX < numberEndY)
		//{
		//	wprintf(L"TransferContentFromDeviceAuto begin ...objCount = %d\n", objCount);
		//	for (int j = numberStartX; j < numberEndY; j++)        //Transfer all photo to PC  , j = 4, j = 27358
		//	{
		//		wprintf(L"\n\n\nStart Transfer wcObjIDArr[%d] = %ws\n", j, wcObjIDArr[j]);
		//		retTrans = TransferContentFromDeviceAuto(device.Get(), wcObjIDArr[j]);
		//		if (retTrans == 0)
		//		{
		//			tbWtObj = wcObjIDArr[j];
		//			toBeWriteObjArr.push_back(tbWtObj);
		//		}
		//	}
		//	writeArrayToFile(toBeWriteObjArr, L"ObjArray.txt");
		//}
		//else
		//{
		//	wprintf(L"! X = %d, Y = %d\n", numberStartX, numberEndY);
		//}

		wprintf(L"Transfer completed!\n");

	}

	/*************************************/

}



int _cdecl wmain(int /*argc*/, _In_ wchar_t* /*argv[]*/)
{

	//wchar2wstring_test();
	//file_read_write_test();
	//system("pause");

    // Enable the heap manager to terminate the process on heap error.
    HeapSetInformation(nullptr, HeapEnableTerminationOnCorruption, nullptr, 0);

    // Initialize COM for COINIT_MULTITHREADED
    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    if (SUCCEEDED(hr))
    {
        // Enter the menu processing loop
        //DoMenu();
		DoTransfer();

        // Uninitialize COM
        CoUninitialize();
    }

    return 0;
}
