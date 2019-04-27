// vimba_cpp_1.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"


#include <iostream>

#include <cstring>
#include <string>
#include <sstream>

#include <vector>
#include <algorithm>
#include "ApiController.h"
#include "Bitmap.h"
#include "MakoCam.h"

#include <ctime> 
#include <time.h>
//#include "ListCameras.h"
using namespace std;

unsigned t0, t1;
//Flags
std::string  pCameraID_Global;
bool flag_camera_init = false;
bool flag_camera_connected = false;
bool flag_cameraID_selected = false;
//bool flag_init_camera_again = false;





class CamObserver : public AVT::VmbAPI::ICameraListObserver
{

public:
	void CameraListChanged(AVT::VmbAPI::CameraPtr pCam, AVT::VmbAPI::UpdateTriggerType reason)
	{
		std::string pID;
		pCam->GetID(pID);
		// Next to the camera pointer a reason why the observer 's function was triggered
		// is passed in. Possible values are:
		// UpdateTriggerPluggedIn (0), a new camera was discovered
		// UpdateTriggerPluggedOut (1), a known camera disappeared from the bus
		// UpdateTriggerOpenStateChanged (3), a known camera was opened or closed
		// by another application
		if (AVT::VmbAPI::UpdateTriggerPluggedIn == reason)
		{
			std::cout << " Camera " << pID << " connected \n";
			if (!flag_cameraID_selected)
				flag_camera_init = true;
			


		}
		if (AVT::VmbAPI::UpdateTriggerPluggedOut == reason) {

			std::cout << " Camera " << pID << " disconnected \n";

			if (flag_cameraID_selected) {
				if (pCameraID_Global == pID) {
					flag_cameraID_selected = false;
					flag_camera_connected = false;
					
					cout << "Camera en uso desconectada";
				}

			}
		}
		std::cout << " \n Event Open state changed: " << reason << endl;

	}
};
class EventObserver : public AVT::VmbAPI::IFeatureObserver
{
public:
	void FeatureChanged(const AVT::VmbAPI::FeaturePtr &feature)
	{
		if (feature != NULL)
		{
			VmbError_t res;
			std::string strName("");
			res = feature->GetDisplayName(strName);
			std::cout << "Event: '" << strName << "' occurred" << std::endl;

			
			
		}
	}
};
class FrameObserver : public AVT::VmbAPI::IFrameObserver
{

public:
	// In your contructor call the constructor of the base class
	// and pass a camera object

	FrameObserver(AVT::VmbAPI::CameraPtr pCamera) : IFrameObserver(pCamera)
	{
		// Put your initialization code here
	}
	void FrameReceived(const AVT::VmbAPI::FramePtr pFrame)
	{
		VmbFrameStatusType eReceiveStatus;
		if (VmbErrorSuccess == pFrame->GetReceiveStatus(eReceiveStatus))
		{
			if (VmbFrameStatusComplete == eReceiveStatus)
			{
				cout << "Se recibe Frame";
			}
			else
			{
				// Put your code here to react on an unsuccessfully received frame
			}
		}
		// When you are finished copying the frame , re-queue it
		m_pCamera->QueueFrame(pFrame);
	}
};




int _tmain(int argc, _TCHAR* argv[])
{

#pragma region Declaracíón de variables



	VmbErrorType    err = VmbErrorSuccess;
	string			errString;
	string          pParameter;                     // The command line parameter


	// The ID of the camera to use
	string  pCameraID;
	string pCameraName;
	string pCameraModel;
	string pCameraSerialNumber;
	string pInterfaceID;
	VmbInterfaceType    interfaceType;
	string flags;

	// Chars Camera
	char pCamID[30];
	char PCAmName[30];
	char pCamModel[30];
	char pCamSerialNumber[30];
	char pInterID[30];

	// sub index Camera Avalible
	int cam_avalible;

	int contadorTrigger = 0;
	MakoCam::Ptr cam;
	std::string linestatus;


	bool status_line_2;

	// Bitmap
	const char *    pFileName = "SingleImage - Mako.bmp";
	const char *pfile[5] = { "t01.bmp","t02.bmp","t03.bmp","t04.bmp","t05.bmp" };

	time_t timer;
	struct tm * timeinfo;


	VmbFrameStatusType status = VmbFrameStatusIncomplete;

	std::string				strCameraID;
	AVT::VmbAPI::FramePtr	pFrame;
	AVT::VmbAPI::CameraPtr	pCamera;
	AVT::VmbAPI::FeaturePtr Feature;

	//	std::stringstream stream;
	//std:string s_timestamp;
	std::string path = "c:/utilities/";


#pragma endregion



#pragma region Init_Api



	std::cout << "\n";
	std::cout << "***************************************\n";
	std::cout << "*  Vimba API Camera adquisition  OX   * \n";
	std::cout << "***************************************\n\n";
	AVT::VmbAPI::Recursos::ApiController	 apiController;
	std::cout << "Vimba C++ API Version " << apiController.GetVersion() << "\n\n";
	err = apiController.StartUp();

#pragma endregion

	//Show all cameras connected
	apiController.ShowCameraList();
	//Choice an avalible camera
	
	// Send pCamera for Init configuration

	if (VmbErrorSuccess == apiController.Giveme_Avalible_Cameras(pCamera))
	{
		apiController.Camera_default_config(pCamera);

		if (VmbErrorSuccess == pCamera->GetID(pCameraID_Global)) {
			pCameraID = pCameraID_Global;
			flag_cameraID_selected = true;
		}
		else flag_cameraID_selected = false;
		
		flag_camera_init = false;
		flag_camera_connected = true;
	}
	else {
		flag_camera_init = false;
		flag_camera_connected = false;
	}


	//getchar();
#pragma region  Registro de Eventos
	//Register Camera List Observer

	apiController.m_system.RegisterCameraListObserver(AVT::VmbAPI::ICameraListObserverPtr(new CamObserver));

	//Register the observer before queuing the frame
	//	pFrame->RegisterObserver(AVT::VmbAPI::IFrameObserverPtr(new FrameObserver(pCamera)));
	//----------------------------------------------------------

	////---------------------
	////CamObserver CamObs;
		//-----------------------

		// 2. register the observer for the camera event
	//AVT::VmbAPI::FeaturePtr pFeature;
	//pCamera->GetFeatureByName("AcquisitionStart", pFeature);
	//pFeature->RegisterObserver(AVT::VmbAPI::IFeatureObserverPtr(new EventObserver()));
	//// 3. select "AcquisitionStart" (or a different) event
	//pCamera->GetFeatureByName("EventSelector", pFeature);
	//pFeature->SetValue("AcquisitionStart");
	//// 4. switch on the event notification (or switch it off with "Off")
	//pCamera->GetFeatureByName("EventNotification", pFeature);
	//pFeature->SetValue("On");
#pragma endregion



	int contador = 0;



	while (1) {
		if (flag_camera_connected) {


			cout << "Waiting Trigger \n" << endl;
			// wait until Line2 set 1
			do {
				err = pCamera->GetFeatureByName("LineStatus", Feature);
				Feature->GetValue(status_line_2);
				cout << endl << contador << " " << apiController.ErrorCodeToMessage(err);
				contador++;
				Sleep(100);// temporal, quitarlo

			} while (!status_line_2 && flag_camera_connected);
			//wait until Line2 set 0
			do {
				err = pCamera->GetFeatureByName("LineStatus", Feature);
				Feature->GetValue(status_line_2);
			} while (status_line_2 && flag_camera_connected);

			if (flag_camera_connected) {
				t0 = clock();

				err = pCamera->AcquireSingleImage(pFrame, 5000); //Init Capture
				//cout << "\n pase por aquí";
				if (VmbErrorSuccess == err)
				{
					pCamera->EndCapture(); // End Capture

					err = pFrame->GetReceiveStatus(status);
					if (VmbErrorSuccess == err
						&& VmbFrameStatusComplete == status)
					{
						VmbPixelFormatType ePixelFormat = VmbPixelFormatMono8;
						err = pFrame->GetPixelFormat(ePixelFormat);
						if (VmbErrorSuccess == err)
						{

							if ((VmbPixelFormatMono8 != ePixelFormat)
								&& (VmbPixelFormatRgb8 != ePixelFormat))
							{
								err = VmbErrorInvalidValue;
							}
							else
							{
								VmbUint32_t nImageSize = 0;
								err = pFrame->GetImageSize(nImageSize);
								if (VmbErrorSuccess == err)
								{
									VmbUint32_t nWidth = 0;
									err = pFrame->GetWidth(nWidth);
									if (VmbErrorSuccess == err)
									{
										VmbUint32_t nHeight = 0;
										err = pFrame->GetHeight(nHeight);
										if (VmbErrorSuccess == err)
										{
											VmbUchar_t *pImage = NULL;
											err = pFrame->GetImage(pImage);

											if (VmbErrorSuccess == err)
											{

												AVTBitmap bitmap;

												if (VmbPixelFormatRgb8 == ePixelFormat)
												{
													bitmap.colorCode = ColorCodeRGB24;
												}
												else
												{
													bitmap.colorCode = ColorCodeMono8;
												}

												bitmap.bufferSize = nImageSize;
												bitmap.width = nWidth;
												bitmap.height = nHeight;

												// Create the bitmap

												if (0 == AVTCreateBitmap(&bitmap, pImage))
												{
													std::cout << "Could not create bitmap.\n";
													err = VmbErrorResources;
												}
												else
												{
													// Save the bitmap
													if (contadorTrigger > 4) contadorTrigger = 0;
													std::stringstream stream;
													std::string s_timestamp;
													std::time(&timer);
													timeinfo = localtime(&timer);

													stream << path << timeinfo->tm_year - 100 << "_" << timeinfo->tm_yday
														<< "_" << timeinfo->tm_hour << "_" << timeinfo->tm_min
														<< "_" << timeinfo->tm_sec << timeinfo->tm_isdst << ".bmp";
													s_timestamp = stream.str();

													char * cstr = new char[s_timestamp.length() + 1];
													std::strcpy(cstr, s_timestamp.c_str());


													if (0 == AVTWriteBitmapToFile(&bitmap, cstr))
													{
														std::cout << "Could not write bitmap to file.\n";
														err = VmbErrorOther;
													}
													else
													{

														std::cout << "Bitmap successfully written to file \"" << cstr << "\"\n";
														//std::cout << "Bitmap successfully written to file \"" << pfile[contadorTrigger] << "\"\n";

														contadorTrigger++;
														//Time
														t1 = clock();
														double time = (double(t1 - t0) / CLOCKS_PER_SEC);
														cout << "Execution Time: " << time << endl;

														// Release the bitmap's buffer
														if (0 == AVTReleaseBitmap(&bitmap))
														{
															std::cout << "Could not release the bitmap.\n";
															err = VmbErrorInternalFault;
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
				else std::cout << "\nError Acquisition: " << apiController.ErrorCodeToMessage(err);

			}


			//cameras[cam_avalible]->Close();
		}

		if (flag_camera_init) {
			if (VmbErrorSuccess == apiController.Giveme_Avalible_Cameras(pCamera))
			{
				pCamera->GetID(pCameraID);
				pCameraID_Global = pCameraID;
				apiController.Camera_default_config(pCamera);

				flag_cameraID_selected = true;				
				flag_camera_connected = true;
				flag_camera_init = false;

			}
			else { flag_camera_connected = false; 
			flag_camera_init = false;
			}
		}
	}
	//getchar();

	apiController.ShutDown();

	return 0;
}