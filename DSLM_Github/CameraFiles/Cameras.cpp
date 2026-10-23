#include "stdafx.h"
#include "Cameras.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include <ctime>
#include <time.h>
#include <conio.h>

using namespace System;
using namespace System::IO;
using namespace System::Diagnostics;
using namespace System::Threading;
using namespace System::Collections;
using namespace System::Runtime::InteropServices;

#define	_secure_bufsnap(bufsnap)		bufsnap,sizeof(bufsnap)

int CameraFunctions::CameraInitialization()
{
	BOOL CameraCommandError = FALSE;	//Boolean variable to receive the success or insuccess of camera command
	int32 mm_nDevice;

	//Initiallizing the cameras connected to the computer
	CameraCommandError = dcam_init(NULL, &mm_nDevice, "NULL");
	if(CameraCommandError != TRUE)
		return 0;

	m_nDevice = mm_nDevice;
	return m_nDevice;
}

HDCAM CameraFunctions::CameraConnectedWithSuccess(long Device_nr)
{
	BOOL CameraCommandError = FALSE;	//Boolean variable to receive the success or insuccess of camera command
	HDCAM CAM;		//Handle for camera

	CameraCommandError = dcam_open(&CAM, Device_nr, "NULL" );
		if(CameraCommandError != TRUE)
			return FALSE;

	return CAM;
}

String^ CameraFunctions::GetCameraModel(long Device_nr)
{
	char ReceiverForString[256];

	dcam_getmodelinfo(Device_nr, DCAM_IDSTR_MODEL, ReceiverForString, sizeof(ReceiverForString));
	String^	strModel = gcnew String(ReceiverForString);

	CameraModel = strModel;

	return CameraModel;
}

String^ CameraFunctions::GetCameraSN(long Device_nr)
{
	char ReceiverForString[256];

	dcam_getmodelinfo(Device_nr, DCAM_IDSTR_CAMERAID, ReceiverForString, sizeof(ReceiverForString));
	String^	strCameraId = gcnew String(ReceiverForString);

	CameraSN = strCameraId;

	return CameraSN;
}

String^ CameraFunctions::GetCameraConnectionType(long Device_nr)
{
	char ReceiverForString[256];

	dcam_getmodelinfo(Device_nr, DCAM_IDSTR_BUS, ReceiverForString, sizeof(ReceiverForString));
	String^	strBus = gcnew String(ReceiverForString);

	CameraConnectionType = strBus;

	return CameraConnectionType;
}

String^ CameraFunctions::GetBitsType(HDCAM h)
{
	BOOL CameraCommandError = FALSE;	//Boolean variable to receive the success or insuccess of camera command
	DCAM_BITSTYPE pType;

	CameraCommandError = dcam_getbitstype(h, &pType);
	if(CameraCommandError != TRUE)
		return "Error in command";

	String^ str;
	switch( pType )
		{
		case DCAM_BITSTYPE_INDEX8:
			str = "8-bit";
			break;
		case DCAM_BITSTYPE_RGB24:
			str = "24-bit";
			break;
		case DCAM_BITSTYPE_RGB16:
			str = "16-bit";
			break;
		case DCAM_BITSTYPE_RGB32:
			str = "32-bit";
			break;
		default:
			str = "Unknown";
			break;
		}

	return str;
}

String^ CameraFunctions::GetDataType(HDCAM h)
{
	BOOL CameraCommandError = FALSE;	//Boolean variable to receive the success or insuccess of camera command
	DCAM_DATATYPE pType;

	CameraCommandError = dcam_getdatatype(h, &pType);
	if(CameraCommandError != TRUE)
		return "Error in command";

	String^ str;
	switch( pType )
		{
		case DCAM_DATATYPE_UINT8:
			str = "8-bit"; //unsigned 8-bit
			break;
		case DCAM_DATATYPE_UINT16:
			str = "16-bit"; //unsigned 16-bit
			break;
		default:
			str = "Unknown";
			break;
		}

	return str;
}

BOOL CameraFunctions::SetBitsType(HDCAM h, int spType)
{
	BOOL CameraCommandError = FALSE;	//Boolean variable to receive the success or insuccess of camera command

	if(spType == 8)
	{
		CameraCommandError = dcam_setbitstype(h, DCAM_BITSTYPE_INDEX8);
			if(CameraCommandError != TRUE)
				return FALSE;
	}
	else if (spType == 24)
	{
		CameraCommandError = dcam_setbitstype(h, DCAM_BITSTYPE_RGB24);
			if(CameraCommandError != TRUE)
				return FALSE;
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CameraFunctions::SetDataType(HDCAM h, int sdType)
{
	BOOL CameraCommandError = FALSE;	//Boolean variable to receive the success or insuccess of camera command

	if(sdType == 8)
	{
		CameraCommandError = dcam_setdatatype(h, DCAM_DATATYPE_UINT8);
			if(CameraCommandError != TRUE)
				return FALSE;
	}
	else if (sdType == 16)
	{
		CameraCommandError = dcam_setdatatype(h, DCAM_DATATYPE_UINT16);
			if(CameraCommandError != TRUE)
				return FALSE;
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

char* CameraFunctions::CaptureImage(HDCAM h,int max, int min)
{
	BOOL CameraCommandError = FALSE;	//Boolean variable to receive the success or insuccess of camera command
	int32 Error;
	char Errorbuffer[70];

	// --- preparing to capture ---
	CameraCommandError = dcam_precapture(h, DCAM_CAPTUREMODE_SNAP);
	if( !CameraCommandError )
		{
			Error = dcam_getlasterror(h, Errorbuffer, sizeof(Errorbuffer));
			return 0;
		}

	CameraCommandError = dcam_allocframe(h, 1);
	if( !CameraCommandError )
		{
			Error = dcam_getlasterror(h, Errorbuffer, sizeof(Errorbuffer));
			return 0;
		}

	// --- start capturing ---
	CameraCommandError = dcam_capture(h);
	if( !CameraCommandError )
		{
			Error = dcam_getlasterror(h, Errorbuffer, sizeof(Errorbuffer));
			return 0;
		}

	// --- wait coming N frames ---
	DWORD code = DCAM_EVENT_FRAMEEND;

	CameraCommandError = dcam_wait(h, &code, 2500, NULL);
	if( !CameraCommandError )
		{
			Error = dcam_getlasterror(h, Errorbuffer, sizeof(Errorbuffer));
			return 0;
		}

	// --- stop capturing ---

	dcam_idle(h);	// this is eliminatable at DCAM_CAPTUREMODE_SNAP

	// --- grabbing the images info and details ---
	char*		top;
	DCAM_DATATYPE	DataType = DCAM_DATATYPE_NONE;
	DCAM_SIZE	datasize;
	DWORD		dwFramebytes;
	int32		rowbytes, BytesPerPixel = 1;

	CameraCommandError = dcam_getdatatype(h, &DataType);
	if( !CameraCommandError )
		{
			Error = dcam_getlasterror(h, Errorbuffer, sizeof(Errorbuffer));
			return 0;
		}

	switch (DataType)
		{
			case DCAM_DATATYPE_UINT8:
			case DCAM_DATATYPE_INT8:
				BytesPerPixel = 1;  ///
				break;
			case DCAM_DATATYPE_UINT16:
			case DCAM_DATATYPE_INT16:
				BytesPerPixel = 2;
				break;
			default:
				BytesPerPixel = 1;
		}

	CameraCommandError = dcam_getdatasizeex(h, &datasize);
	if( !CameraCommandError )
		{
			Error = dcam_getlasterror(h, Errorbuffer, sizeof(Errorbuffer));
			return 0;
		}

	CameraCommandError = dcam_getdataframebytes(h ,&dwFramebytes);
	if( !CameraCommandError )
		{
			Error = dcam_getlasterror(h, Errorbuffer, sizeof(Errorbuffer));
			return 0;
		}

	// --- store images into file ---
	CameraCommandError = dcam_lockdata(h, (void**)&top, &rowbytes, -1);
	if( !CameraCommandError )
		{
			Error = dcam_getlasterror(h, Errorbuffer, sizeof(Errorbuffer));
			return 0;
		}

	return top;

}

SIZE CameraFunctions::GetImageSize(HDCAM h)  //get the width and height of the image data
{
	BOOL CameraCommandError = FALSE;	//Boolean variable to receive the success or insuccess of camera command
	SIZE sz, EmptySize;
	EmptySize.cx = 0;
	EmptySize.cy = 0;

	CameraCommandError = dcam_getdatasize(h, &sz);

	if(!CameraCommandError)
		return EmptySize;
	else
		return sz;

}

BOOL CameraFunctions::FreeCamera(HDCAM h)
{
	BOOL CameraCommandError = FALSE;	//Boolean variable to receive the success or insuccess of camera command
	int32 Error;
	char Errorbuffer[70];

	CameraCommandError = dcam_freeframe(h);
	if( !CameraCommandError )
	{
		Error = dcam_getlasterror(h, Errorbuffer, sizeof(Errorbuffer));
		return 0;
	}

	return CameraCommandError;
}

BOOL CameraFunctions::SetImageFrame(HDCAM h, int xwidth, int xoff, int yheight, int yoff )
{
	BOOL CameraCommandError = FALSE;	//Boolean variable to receive the success or insuccess of camera command

	DCAM_PARAM_SUBARRAY		param;
	memset( &param, 0, sizeof( param ) );
	param.hdr.cbSize	= sizeof( param );
	param.hdr.id		= DCAM_IDPARAM_SUBARRAY;
	param.hdr.iFlag		= dcamparam_subarray_hpos
						| dcamparam_subarray_vpos
						| dcamparam_subarray_hsize
						| dcamparam_subarray_vsize
						;

	param.hpos	= xoff;   //offset in pixels from x direction
	param.vpos	= yoff;   //offset in pixels from y direction
	param.hsize	= xwidth; //width of frame after x offset;
	param.vsize	= yheight; //height of frame after y offset;

	CameraCommandError = dcam_extended(h, DCAM_IDMSG_SETPARAM, &param, sizeof( param ) );

	return CameraCommandError;

}

array <int> ^ CameraFunctions::GetImageFrame(HDCAM h)
{
	BOOL CameraCommandError = FALSE;	//Boolean variable to receive the success or insuccess of camera command

	array <int> ^ CameraSizes = gcnew array <int>(4);

	DCAM_PARAM_SUBARRAY		param;
	memset( &param, 0, sizeof( param ) );
	param.hdr.cbSize	= sizeof( param );
	param.hdr.id		= DCAM_IDPARAM_SUBARRAY;
	param.hdr.iFlag		= dcamparam_subarray_hpos
						| dcamparam_subarray_vpos
						| dcamparam_subarray_hsize
						| dcamparam_subarray_vsize
						;

	CameraCommandError = dcam_extended(h, DCAM_IDMSG_GETPARAM, &param, sizeof( param ) );

	CameraSizes[0] = param.hsize; //width (X)
	CameraSizes[1] = param.hpos;  //offset of width (X Offset)
	CameraSizes[2] = param.vsize; //height (Y)
	CameraSizes[3] = param.vpos;   //offset of height (Y Offset)

	return CameraSizes;

}

BOOL CameraFunctions::SaveImage(HDCAM h, int camera1or2, DWORD dwFramebytes, char* imageaddress, String^ Folder)
{
	FILE* pFile = NULL;
	char OutputImageFileName[100];

	if(camera1or2 == 1)
	{
		const char* AddressAndTitle = (const char*)(void*)Marshal::StringToHGlobalAnsi(Folder + "\\Snap_%d");

		sprintf_s( _secure_bufsnap(OutputImageFileName), AddressAndTitle, 0 );
		fopen_s( &pFile, OutputImageFileName, "w+b");
		if(!pFile)
		{
			printf("Error opening file : %s\n", OutputImageFileName);
			return 0;
		}

		fwrite(imageaddress, 1, dwFramebytes, pFile);
		fclose(pFile);
	}
	else if(camera1or2 == 2)
	{
		const char* AddressAndTitle = (const char*)(void*)Marshal::StringToHGlobalAnsi(Folder + "\\Snap_Cam2_%d");

		sprintf_s( _secure_bufsnap(OutputImageFileName), AddressAndTitle, 0 );
		fopen_s( &pFile, OutputImageFileName, "w+b");
		if(!pFile)
		{
			printf("Error opening file : %s\n", OutputImageFileName);
			return 0;
		}

		fwrite(imageaddress, 1, dwFramebytes, pFile);
		fclose(pFile);
	}

	return 1;
}

String^ CameraFunctions::GetCameraTriggerMode(HDCAM h)
{
	BOOL CameraCommandError = FALSE;
	String^ CamTriggerMode;
	int32 CurrentTM;
	int32 Error;
	char Errorbuffer[70];

	CameraCommandError = dcam_gettriggermode(h, &CurrentTM);
	if( !CameraCommandError )
	{
		Error = dcam_getlasterror(h, Errorbuffer, sizeof(Errorbuffer));
		return CamTriggerMode = "Error in camara get trigger mode command!";
	}

	switch (CurrentTM)
	{
	case 1:
		CamTriggerMode = "Internal";
		break;
	case 2:
		CamTriggerMode = "External - Edge";
		break;
	case 4:
		CamTriggerMode = "External - Level";
		break;
	case 8:
		CamTriggerMode = "Multishot Sensitive";
		break;
	case 10:
		CamTriggerMode = "Cycle Delay";
		break;
	case 20:
		CamTriggerMode = "Software";
		break;
	case 40:
		CamTriggerMode = "Fast Repetition";
		break;
	case 80:
		CamTriggerMode = "TDI";
		break;
	case 100:
		CamTriggerMode = "TDI Internal";
		break;
	case 200:
		CamTriggerMode = "Start";
		break;
	case 400:
		CamTriggerMode = "Syncreadout";
		break;
	}

	return CamTriggerMode;
}

BOOL CameraFunctions::SetCameraTriggerMode(HDCAM h, String^ TriggerM)
{
	BOOL CameraCommandError = FALSE;
	int32 Error;
	int32 TM = 0;
	char Errorbuffer[70];

	if(TriggerM == "Internal")
	{
		TM = 1;
	}
	else if(TriggerM == "External - Edge")
	{
		TM = 2;
	}
	else if(TriggerM == "External - Level")
	{
		TM = 4;
	}
	else if(TriggerM == "Multishot Sensitive")
	{
		TM = 8;
	}
	else if(TriggerM == "Cycle Delay")
	{
		TM = 10;
	}
	else if(TriggerM == "Software")
	{
		TM = 20;
	}
	else if(TriggerM == "Fast Repetition")
	{
		TM = 40;
	}
	else if(TriggerM == "TDI")
	{
		TM = 80;
	}
	else if(TriggerM == "TDI Internal")
	{
		TM = 100;
	}
	else if(TriggerM == "Start")
	{
		TM = 200;
	}
	else if(TriggerM == "Syncreadout")
	{
		TM = 400;
	}

	CameraCommandError = dcam_settriggermode(h, TM);
	if( !CameraCommandError )
	{
		Error = dcam_getlasterror(h, Errorbuffer, sizeof(Errorbuffer));
		return CameraCommandError;
	}

	return CameraCommandError;
}

String^ CameraFunctions::GetCameraTriggerModePolarity(HDCAM h)
{
	BOOL CameraCommandError = FALSE;
	String^ CamTriggerModePolarity;
	int32 CurrentTMPol;
	int32 Error;
	char Errorbuffer[70];

	CameraCommandError = dcam_gettriggerpolarity(h, &CurrentTMPol);
	if( !CameraCommandError )
	{
		Error = dcam_getlasterror(h, Errorbuffer, sizeof(Errorbuffer));
		return CamTriggerModePolarity = "Error occuring in get trigger polarity command.";
	}

	switch (CurrentTMPol)
	{
	case 0:
		CamTriggerModePolarity = "Negative";
		break;
	case 1:
		CamTriggerModePolarity = "Positive";
		break;
	}

	return CamTriggerModePolarity;

}

BOOL CameraFunctions::SetCameraTriggerModePolarity(HDCAM h, String^ TriggerMPol)
{
	BOOL CameraCommandError = FALSE;
	int32 TMPol;
	int32 Error;
	char Errorbuffer[70];

	if(TriggerMPol == "Negative")
	{
		TMPol = DCAM_TRIGPOL_NEGATIVE;	
	}
	else if(TriggerMPol == "Positive")
	{
		TMPol = 1;
	}

	CameraCommandError = dcam_settriggerpolarity(h, TMPol);
	if( !CameraCommandError )
	{
		Error = dcam_getlasterror(h, Errorbuffer, sizeof(Errorbuffer));
		return 0;
	}

	return TRUE;
}

DWORD CameraFunctions::GetDataFrameBytes(HDCAM h)
{
	DWORD dwFBytes;
	BOOL CameraCommandError = FALSE;	//Boolean variable to receive the success or insuccess of camera command
	int32 Error;
	char Errorbuffer[70];

	CameraCommandError = dcam_getdataframebytes(h ,&dwFBytes);
	if( !CameraCommandError )
	{
		Error = dcam_getlasterror(h, Errorbuffer, sizeof(Errorbuffer));
		return 0;
	}

	return dwFBytes;
}

BOOL CameraFunctions::PreCaptureSequenceMode(HDCAM h)
{
	BOOL CameraCommandError = FALSE;	//Boolean variable to receive the success or insuccess of camera command
	int32 Error;
	char Errorbuffer[70];

	CameraCommandError = dcam_precapture(h, DCAM_CAPTUREMODE_SEQUENCE); // called to prepare to capture images; sets camera to stable status
	if( !CameraCommandError )
		{
			Error = dcam_getlasterror(h, Errorbuffer, sizeof(Errorbuffer));
			return 0;
		}

	return TRUE;

}

BOOL CameraFunctions::Capture(HDCAM h)
{
	BOOL CameraCommandError = FALSE;	//Boolean variable to receive the success or insuccess of camera command
	int32 Error;
	char Errorbuffer[70];

	CameraCommandError = dcam_capture(h);
	if( !CameraCommandError )
		{
			Error = dcam_getlasterror(h, Errorbuffer, sizeof(Errorbuffer));
			return 0;
		}

	return TRUE;
}

BOOL CameraFunctions::CameraIdle(HDCAM h)
{
	BOOL CameraCommandError = FALSE;	//Boolean variable to receive the success or insuccess of camera command
	int32 Error;
	char Errorbuffer[70];

	CameraCommandError = dcam_idle(h);
	if( !CameraCommandError )
		{
			Error = dcam_getlasterror(h, Errorbuffer, sizeof(Errorbuffer));
			return 0;
		}

	return TRUE;
}

void** CameraFunctions::PrepareImagesBuffer(void**& StackWithFrames, int32 NumberOfFrames, int32 Framebyte)
{
	int	bufsize = sizeof(*StackWithFrames)*(NumberOfFrames + 1) + Framebyte * NumberOfFrames;
	char*	buf = new char[bufsize];

	if(buf == NULL)
		return 0;

	memset(buf, 0, bufsize);

	StackWithFrames = (void**) buf;
	StackWithFrames[0] = buf + sizeof(*StackWithFrames)*(NumberOfFrames + 1);

	int	i;
	for(i = 1; i < NumberOfFrames; i++)
	{
		StackWithFrames[i] = (char*)StackWithFrames[i-1] + Framebyte;
	}

	return StackWithFrames;
}

BOOL CameraFunctions::CameraAttachBuffer(HDCAM h, void** StackWithFrames, int32 numberofframes)
{
	BOOL CameraCommandError = FALSE;	//Boolean variable to receive the success or insuccess of camera command
	int32 Error;
	char Errorbuffer[70];

	CameraCommandError = dcam_attachbuffer(h, StackWithFrames, sizeof(*StackWithFrames)*numberofframes);
	if( !CameraCommandError )
		{
			Error = dcam_getlasterror(h, Errorbuffer, sizeof(Errorbuffer));
			return 0;
		}

	return TRUE;
}

BOOL CameraFunctions::CameraWaitingForImage(HDCAM h, DWORD codetowait, DWORD waitingmode)
{
	BOOL CameraCommandError = FALSE;	//Boolean variable to receive the success or insuccess of camera command
	int32 Error;
	char Errorbuffer[70];

	CameraCommandError = dcam_wait(h, &codetowait, waitingmode, NULL);
	if( !CameraCommandError )
		{
			Error = dcam_getlasterror(h, Errorbuffer, sizeof(Errorbuffer));
			return 0;
		}

	return TRUE;
}

array <int> ^ CameraFunctions::GetCameraFrameIndexAndTotalNumberOfFramesAcquired(HDCAM h)
{
	BOOL CameraCommandError = FALSE;	//Boolean variable to receive the success or insuccess of camera command
	int32 Error;
	char Errorbuffer[70];
	array <int> ^ CameraFrameIndexAndTotalNrOfFrames = gcnew array <int>(2);
	CameraFrameIndexAndTotalNrOfFrames[0] = 10000;
	CameraFrameIndexAndTotalNrOfFrames[1] = 10000;
	int32 index = 0;
	int32 total = 0;

	CameraCommandError = dcam_gettransferinfo(h, &index, &total);
	if( !CameraCommandError )
	{
		Error = dcam_getlasterror(h, Errorbuffer, sizeof(Errorbuffer));
		return CameraFrameIndexAndTotalNrOfFrames;
	}

	CameraFrameIndexAndTotalNrOfFrames[0] = index;
	CameraFrameIndexAndTotalNrOfFrames[1] = total;

	return CameraFrameIndexAndTotalNrOfFrames;
}

BOOL CameraFunctions::CameraReleaseBuffer(HDCAM h)
{
	BOOL CameraCommandError = FALSE;	//Boolean variable to receive the success or insuccess of camera command
	int32 Error;
	char Errorbuffer[70];

	CameraCommandError = dcam_releasebuffer(h);
	if( !CameraCommandError )
		{
			Error = dcam_getlasterror(h, Errorbuffer, sizeof(Errorbuffer));
			return 0;
		}

	return TRUE;
}

double CameraFunctions::GetCameraExposureTime(HDCAM h)
{
	BOOL CameraCommandError = FALSE;	//Boolean variable to receive the success or insuccess of camera command
	int32 Error;
	char Errorbuffer[70];
	double expt = 0.0;

	CameraCommandError = dcam_getexposuretime(h, &expt);
	if( !CameraCommandError )
		{
			Error = dcam_getlasterror(h, Errorbuffer, sizeof(Errorbuffer));
			return 0;
		}

	return expt;
}

BOOL CameraFunctions::SetCameraExposureTime(HDCAM h, double exposuretimetoset)
{
	BOOL CameraCommandError = FALSE;	//Boolean variable to receive the success or insuccess of camera command
	int32 Error;
	char Errorbuffer[70];

	CameraCommandError = dcam_setexposuretime(h, exposuretimetoset);
	if( !CameraCommandError )
		{
			Error = dcam_getlasterror(h, Errorbuffer, sizeof(Errorbuffer));
			return 0;
		}

	return TRUE;
}

BOOL CameraFunctions::Save3DConvertedStack(char** stack, int camera1or2, int numberofframes, DWORD Fbyte, String^ Folder, int timepoint)
{
	char FileName[100];
	const char* CamInUse;

	if (camera1or2 == 1)
		CamInUse = (const char*)(void*)Marshal::StringToHGlobalAnsi(Folder + "\\image_%04d");
	else
		CamInUse = (const char*)(void*)Marshal::StringToHGlobalAnsi(Folder + "\\image_%04d");

	sprintf_s(FileName, sizeof(FileName), CamInUse, timepoint);

	std::ofstream outputImageFile(FileName, std::ofstream::binary);

	// supplies the converted 8-bit frame size.
	int frameSize = static_cast<int>(Fbyte);

	for (int i = 0; i < numberofframes; ++i)
		outputImageFile.write(stack[i], frameSize);

	outputImageFile.close();

	return TRUE;
}

double CameraFunctions::GetInternalFrameRate(HDCAM h)
{
	BOOL CameraCommandError = FALSE;	//Boolean variable to receive the success or insuccess of camera command
	int32 Error;
	char Errorbuffer[70];
	double Value = 0.0;

	CameraCommandError = dcam_getpropertyvalue(h, DCAM_IDPROP_INTERNALFRAMERATE, &Value);
	if (!CameraCommandError)
	{
		Error = dcam_getlasterror(h, Errorbuffer, sizeof(Errorbuffer));
		return 0.0;
	}

	return Value;
}
