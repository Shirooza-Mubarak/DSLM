#ifndef _Cameras_h
#define _Cameras_h

#include "windows.h"
#include "dcamapi.h"
#include "dcamapi3.h"
#include "ccDefine.h"
#include "dcamapix.h"
#include "dcamprop.h"
#include "dcimgapi.h"

using namespace System;

ref class CameraFunctions{
public:

	//Cameras initialization, information, connection and close
	static int CameraFunctions::CameraInitialization();	//Initialization of the camera(s) connected to the PC
	static HDCAM CameraFunctions::CameraConnectedWithSuccess(long Device_nr);	//Open the cameras to start controlling them and return the camera Handle
	static String^ CameraFunctions::GetCameraModel(long Device_nr);	//Get camera model
	static String^ CameraFunctions::GetCameraSN(long Device_nr);		//Get camera Serial Number (SN)
	static String^ CameraFunctions::GetCameraConnectionType(long Device_nr);	//Get camera connection Type: USB3 or Camera Link
	static String^ CameraFunctions::GetCameraTriggerMode(HDCAM h);	//Get Camera Trigger Mode
	static BOOL CameraFunctions::SetCameraTriggerMode(HDCAM h, String^ TriggerM);	//Set Camera Trigger Mode
	static String^ CameraFunctions::GetCameraTriggerModePolarity(HDCAM h);	//Get Camera Trigger Mode Polarity
	static BOOL CameraFunctions::SetCameraTriggerModePolarity(HDCAM h, String^ TriggerMPol);	//Set Camera Trigger Mode Polarity

	//Bitmap and data type
	static String^ CameraFunctions::GetBitsType(HDCAM h);	//Get bitmap type: none, 8, 16, 24 or 32-bits
	static String^ CameraFunctions::GetDataType(HDCAM h);	//Get image data type: 8 or 16-bit or more
	static BOOL CameraFunctions::SetBitsType(HDCAM h, int spType);		//Set bitmap type: 8, 16, 24 or 32-bits
	static BOOL CameraFunctions::SetDataType(HDCAM h, int sdType);		//Set image data type: 8 or 16-bit or more

	//Get images and images info from camera
	static char* CameraFunctions::CaptureImage(HDCAM h,int max, int min);
	static SIZE CameraFunctions::GetImageSize(HDCAM h);
	static BOOL CameraFunctions::FreeCamera(HDCAM h);
	static BOOL CameraFunctions::SetImageFrame(HDCAM h, int xwidth, int xoff, int yheight, int yoff );
	static array <int> ^ CameraFunctions::GetImageFrame(HDCAM h);
	static DWORD CameraFunctions::GetDataFrameBytes(HDCAM h);
	static void** CameraFunctions::PrepareImagesBuffer(void**& StackWithFrames, int32 NumberOfFrames, int32 Framebyte);
	static BOOL CameraFunctions::PreCaptureSequenceMode(HDCAM h);
	static BOOL CameraFunctions::Capture(HDCAM h);
	static BOOL CameraFunctions::CameraIdle(HDCAM h);
	static BOOL CameraFunctions::CameraAttachBuffer(HDCAM h, void** StackWithFrames, int32 numberofframes);
	static BOOL CameraFunctions::CameraWaitingForImage(HDCAM h, DWORD codetowait, DWORD waitingmode);
	static array <int> ^ CameraFunctions::GetCameraFrameIndexAndTotalNumberOfFramesAcquired(HDCAM h);
	static BOOL CameraFunctions::CameraReleaseBuffer(HDCAM h);
	static double CameraFunctions::GetCameraExposureTime(HDCAM h);
	static BOOL CameraFunctions::SetCameraExposureTime(HDCAM h, double exposuretimetoset);
	static BOOL CameraFunctions::SaveImage(HDCAM h, int camera1or2, DWORD dwFramebytes, char* imageaddress, String^ Folder);

	static BOOL CameraFunctions::Save3DConvertedStack(char **stack, int camera1or2, int numberofframes, DWORD Fbyte, String^ Folder, int timepoint);

	static double CameraFunctions::GetInternalFrameRate(HDCAM h);

	static BOOL camera_rotated_90_degrees;
	char* BufferForImage;			//Pointer to receive the image

	static BOOL LiveImagingIsRunning;		//Variable to indicate if the live mode is running for cam 1
	static HDCAM CAM1;		//Camera Handle, for camera 1

	static String^ Camera1TriggerMode;
	static String^ Camera1TriggerModePolarity;

	static array <int> ^ Camera1ImagesSizesAndOffsets;

	static double Cam1Exposure;			//In case internal trigger is used, camera 1

	static String^	Camera1Model;			//String to capture camera 1 Model
	static String^	Camera1SN;				//String to capture camera 1 SN
	static String^	Camera1ConnectionType;	//String to capture camera 1 connection type

	static String^ Cam1DataType;			//String to keep camera 1 data type
	static String^ Cam1BitmapBits;			//String to keep camera 1 bitmap bits

private:
	static int32 m_nDevice;	//Will receive the indication of the number of cameras connected to the PC

	static String^	CameraModel;			//String to capture camera Model
	static String^	CameraSN;				//String to capture camera SN
	static String^	CameraConnectionType;	//String to capture camera connection type
};

#endif
