#include "stdafx.h"
#include "Stage.h"
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
#include "../CameraFiles/Images.h"
#include <vector>

using namespace System;
using namespace System::IO;
using namespace System::Diagnostics;
using namespace System::Threading;
using namespace System::Collections;
using namespace System::Runtime::InteropServices;
using namespace System::ComponentModel;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;

String^ StageFunctions::StageLastError(int ErrorCd)
{
	BOOL Success;
	char ErrorMessage [100];

	Success = PI_TranslateError(ErrorCd, ErrorMessage, 100);
	if(!Success)
	{
		ErrorMessageString = "EVEN THE ERROR CODE TRANSLATOR FAILED!!!";
		return ErrorMessageString;
	}

	ErrorMessageString = gcnew String(ErrorMessage);
	return ErrorMessageString;
}

BOOL StageFunctions::StageInitialization()
{

	BOOL StageConnected = FALSE;
	char szUSBdevices[1024] = {};
	char USBController1 [48];		//Stage Controller box 1
	char USBController2 [48];		//Stage Controller box 2
	int err = 0;
	char szIDN [100];
	const BOOL flag = TRUE;
	BOOL bFlag1 = FALSE;
	BOOL bIsMoving1 = TRUE;
	BOOL bFlag2 = FALSE;
	BOOL bIsMoving2 = TRUE;
	const double VoltageForATZ = 0;
	const BOOL ATZUseDefaultVoltage = TRUE;
	char ZAxis []= "1";				//In controller box 1, the Z Axis is connected in the Motor&Sensor 1
	char XAxis []= "1";				//In controller box 2, the X Axis is connected in the Motor&Sensor 1
	char YAxis []= "2";				//In controller box 2, the Y Axis is connected in the Motor&Sensor 2
	char XYAxes[] = "1 2";			//X and Y axis can be commanded in simultaneous if required

	//Checking the number of controller boxes connected to the PC
	NrOfControllersConnected = PI_EnumerateUSB(
		szUSBdevices,
		static_cast<int>(sizeof(szUSBdevices)),
		""
	);

	for (int ii = 0; ii < 47; ii++)
	{
		USBController2[ii] = szUSBdevices[ii];
		USBController1[ii] = szUSBdevices[ii + 48];
	}

	Controller1Details = gcnew String(USBController1);	//Passing the information from the char variable into a string, controller box 1 SN 0117018375
	Controller2Details = gcnew String(USBController2);	//Passing the information from the char variable into a string, controller box 2 SN 0117014798

	//Getting the controller boxes respective Identifications (ID)
	ID1 = PI_ConnectUSBWithBaudRate(USBController1, 115200); // z axis SN 0117014798
	ID2 = PI_ConnectUSBWithBaudRate(USBController2, 115200); // x y axes SN 0117018375

	if(!PI_IsConnected(ID1) && !PI_IsConnected(ID2))
	{
		InitializationError = "One of/or the two controllers seem to not be connected!";
		return false;
	}
	else
	{
		ErrorCode1 = PI_GetError(ID1);
		ErrorMessageString1 = StageFunctions::StageLastError(ErrorCode1);

		ErrorCode2 = PI_GetError(ID2);
		ErrorMessageString2 = StageFunctions::StageLastError(ErrorCode1);
	}

	if(ID1 > -1 && ID2 > -1)
	{
		//Testing the commands, asking the controller its Identification (szIDN) should be equal to controller 1/2 details
		if(!PI_qIDN(ID1,szIDN,100))
		{
			InitializationError = "qIDN command of controller box 1 failed.";
			ErrorCode1 = PI_GetError(ID1);
			ErrorMessageString1 = StageFunctions::StageLastError(ErrorCode1);
			return false;
		}
		if(!PI_qIDN(ID2,szIDN,100))
		{
			InitializationError = "qIDN command of controller box 2 failed.";
			ErrorCode2 = PI_GetError(ID2);
			ErrorMessageString2 = StageFunctions::StageLastError(ErrorCode2);
			return false;
		}

		//Turning controllers stage servo: "ON" if flag = TRUE to operate stage in close-loop / "OFF" if FALSE to operate in open-loop
		if(!PI_SVO(ID1,ZAxis,&flag))
		{
			InitializationError = "SVO command of controller box 1 failed.";
			ErrorCode1 = PI_GetError(ID1);
			ErrorMessageString1 = StageFunctions::StageLastError(ErrorCode1);
			return false;
		}
		while(!bFlag1)
		{
			bFlag1 = StageFunctions::ControllerIsReadyForNewCommand(ID1);
		}

		if(!PI_SVO(ID2,XAxis,&flag))
		{
			InitializationError = "SVO command of controller box 2, X axis failed.";
			ErrorCode2 = PI_GetError(ID2);
			ErrorMessageString2 = StageFunctions::StageLastError(ErrorCode2);
			return false;
		}
		while(!bFlag2)
		{
			bFlag2 = StageFunctions::ControllerIsReadyForNewCommand(ID2);
		}

		if(!PI_SVO(ID2,YAxis,&flag))
		{
			InitializationError = "SVO command of controller box 2, Y axis failed.";
			ErrorCode2 = PI_GetError(ID2);
			ErrorMessageString2 = StageFunctions::StageLastError(ErrorCode2);
			return false;
		}
		bFlag2 = StageFunctions::ControllerIsReadyForNewCommand(ID2);
		while(!bFlag2)
		{
			bFlag2 = StageFunctions::ControllerIsReadyForNewCommand(ID2);
		}

		//Set the stage velocity to 5 mm/s for all the axis
		double Velocity = 5.0;
		double VelocityZ = 0.9;
		StageFunctions::SetAxisVelocity(ID1, ZAxis, VelocityZ);
		StageFunctions::SetAxisVelocity(ID2, XAxis, Velocity);
		StageFunctions::SetAxisVelocity(ID2, YAxis, Velocity);

		//Referencing the 3 axes
		if(!ReferenceAxis(ID1,ZAxis))
		{
			InitializationError = "Z axis referencing failed.";
			ErrorCode1 = PI_GetError(ID1);
			ErrorMessageString1 = StageFunctions::StageLastError(ErrorCode1);
			return false;
		}
		while(bIsMoving1 == TRUE)
		{
			bIsMoving1 = StageFunctions::StageIsMoving(ID1, ZAxis);
		}
		while(!bFlag1)
		{
			bFlag1 = StageFunctions::ControllerIsReadyForNewCommand(ID1);
		}

		//stage references at 23mm. to avoid hitting the objectives move to 0mm beforereferncing X and Y
		StageFunctions::MoveAxisToThisExactPosition(ID1, ZAxis, 0);

		while (StageFunctions::StageIsMoving(ID1, ZAxis))
		{
			StageFunctions::StageIsMoving(ID1, ZAxis);
		}

		if(!ReferenceAxis(ID2,XAxis))
		{
			InitializationError = "X axis referencing failed.";
			ErrorCode2 = PI_GetError(ID2);
			ErrorMessageString2 = StageFunctions::StageLastError(ErrorCode2);
			return false;
		}
		while(bIsMoving2 == TRUE)
		{
			bIsMoving2 = StageFunctions::StageIsMoving(ID2, XAxis);
		}
		while(!bFlag2)
		{
			bFlag2 = StageFunctions::ControllerIsReadyForNewCommand(ID2);
		}

		if(!ReferenceAxis(ID2,YAxis))
		{
			InitializationError = "Y axis referencing failed.";
			ErrorCode2 = PI_GetError(ID2);
			ErrorMessageString2 = StageFunctions::StageLastError(ErrorCode2);
			return false;
		}
		bIsMoving2 = StageFunctions::StageIsMoving(ID2, YAxis);
		while(bIsMoving2 == TRUE)
		{
			bIsMoving2 = StageFunctions::StageIsMoving(ID2, YAxis);
		}
		bFlag2 = StageFunctions::ControllerIsReadyForNewCommand(ID2);
		while(!bFlag2)
		{
			bFlag2 = StageFunctions::ControllerIsReadyForNewCommand(ID2);
		}

		//Auto Zero for all the axes

		if(!PI_ATZ(ID2, XAxis, &VoltageForATZ, &ATZUseDefaultVoltage))
		{
			InitializationError = "Auto Zero for X axis failed.";
			ErrorCode2 = PI_GetError(ID2);
			ErrorMessageString2 = StageFunctions::StageLastError(ErrorCode2);
			return false;
		}
		bIsMoving2 = StageFunctions::StageIsMoving(ID2, XAxis);
		while(bIsMoving2 == TRUE)
		{
 			bIsMoving2 = StageFunctions::StageIsMoving(ID2, XAxis);
		}
		bFlag2 = StageFunctions::ControllerIsReadyForNewCommand(ID2);
		while(!bFlag2)
		{
			bFlag2 = StageFunctions::ControllerIsReadyForNewCommand(ID2);
		}

		if(!PI_ATZ(ID2, YAxis, &VoltageForATZ, &ATZUseDefaultVoltage))
		{
			InitializationError = "Auto Zero for Y axis failed.";
			ErrorCode2 = PI_GetError(ID2);
			ErrorMessageString2 = StageFunctions::StageLastError(ErrorCode2);
			return false;
		}
		bIsMoving2 = StageFunctions::StageIsMoving(ID2, YAxis);
		while(bIsMoving2 == TRUE)
		{
			bIsMoving2 = StageFunctions::StageIsMoving(ID2, YAxis);
		}
		bFlag2 = StageFunctions::ControllerIsReadyForNewCommand(ID2);
		while(!bFlag2)
		{
			bFlag2 = StageFunctions::ControllerIsReadyForNewCommand(ID2);
		}

	}
	else
	{
		InitializationError = "None of the controllers was detected.";
		ErrorCode1 = PI_GetError(ID1);
		ErrorMessageString1 = StageFunctions::StageLastError(ErrorCode1);
		ErrorCode2 = PI_GetError(ID2);
		ErrorMessageString2 = StageFunctions::StageLastError(ErrorCode2);
		return false;
	}

	return true;
}

BOOL StageFunctions::ReferenceAxis(int ID, char * axis)
{
    BOOL bReferenced;
    BOOL bFlag = false;
	int Error;

    if(!PI_qFRF(ID, axis, &bReferenced)) //The reference state is given as Boolean value where 1 corresponds to a referenced axis and 0 corresponds to a not referenced axis
	{
		Error = PI_GetError(ID);
		ErrorMessageString = StageFunctions::StageLastError(Error);
		return false;
	}
	//If needed, reference the axis using the refence switch
	if(!bReferenced)
    {
        if(!PI_FRF(ID, axis))
		{
			Error = PI_GetError(ID);
			ErrorMessageString = StageFunctions::StageLastError(Error);
			return false;
		}
        //Wait until the stage finishes referencing and is able to receive a new command
        while(bFlag != TRUE)
        {
             bFlag = StageFunctions::ControllerIsReadyForNewCommand(ID);
        }
    }

    return TRUE;
}

BOOL StageFunctions::ControllerIsReadyForNewCommand(int ID)
{
	BOOL Flag = false;
	BOOL Success;
	int Error;

	Success = PI_IsControllerReady(ID, &Flag);
	if(!Success)
	{
		Error = PI_GetError(ID);
		ErrorMessageString = StageFunctions::StageLastError(Error);
		return false;
	}

	return Flag;
}

BOOL StageFunctions::StageIsMoving(int ID, char * axis)
{
	BOOL bIsMoving = TRUE;
	BOOL Success;
	int Error;

	Success = PI_IsMoving(ID, axis, &bIsMoving);
	if(!Success)
	{
		Error = PI_GetError(ID);
		ErrorMessageString = StageFunctions::StageLastError(Error);
		return false;
	}

	return bIsMoving;
}

double StageFunctions::GetAxisPosition(int ID, char * axis)
{
	BOOL Success;
	double Position;
	int Error;

	Success = PI_qPOS(ID, axis, &Position);

	if(!Success)
	{
		Error = PI_GetError(ID);
		ErrorMessageString = StageFunctions::StageLastError(Error);
		Position = 20;
	}

	return Position;
}

BOOL StageFunctions::MoveAxisToThisExactPosition(int ID, char * axis, double Position)
{
	BOOL Success;
	BOOL AxisIsreadyForNewCommand = FALSE;
	int Error;
	BOOL IsMoving = TRUE;

	Success = PI_MOV(ID, axis, &Position);
	if(!Success)
	{
		Error = PI_GetError(ID);
		ErrorMessageString = StageFunctions::StageLastError(Error);
		return false;
	}

	AxisIsreadyForNewCommand = StageFunctions::ControllerIsReadyForNewCommand(ID);
	while(!AxisIsreadyForNewCommand)
	{
		AxisIsreadyForNewCommand = StageFunctions::ControllerIsReadyForNewCommand(ID);
	}

	return TRUE;
}

BOOL StageFunctions::MoveAxisThisExactStep(int ID, char * axis, double Step)
{
	BOOL Success;
	BOOL AxisIsreadyForNewCommand = FALSE;
	int Error;
	BOOL IsMoving = TRUE;

	Success = PI_MVR(ID, axis, &Step);
	if(!Success)
	{
		Error = PI_GetError(ID);
		ErrorMessageString = StageFunctions::StageLastError(Error);
		return false;
	}

	//This command corrects the bad position error. However make movement very slow.

	return TRUE;
}

double StageFunctions::GetAxisVelocity(int ID, char * axis)
{
	BOOL Success;
	double Velocity;
	int Error;

	Success = PI_qVEL(ID, axis, &Velocity);
	if(!Success)
	{
		Error = PI_GetError(ID);
		ErrorMessageString = StageFunctions::StageLastError(Error);
		Velocity = 0;
	}

	return Velocity;
}

BOOL StageFunctions::SetAxisVelocity(int ID, char * axis, double Velocity)
{
	BOOL Success;
	int Error;

	Success = PI_VEL(ID, axis, &Velocity);
	if(!Success)
	{
		Error = PI_GetError(ID);
		ErrorMessageString = StageFunctions::StageLastError(Error);
		return FALSE;
	}

	return TRUE;
}

BOOL StageFunctions::ToggleOutputOnOff(int ID, int TriggerOutputID, BOOL TriggerState)
{

	if (!PI_TRO(ID, &TriggerOutputID, &TriggerState, 1))
	{
		return false;
	}
	return true;
}

BOOL StageFunctions::ToggleInputOnOff(int ID, int TriggerInputID, BOOL TriggerState)
{

	if (!PI_TRI(ID, &TriggerInputID, &TriggerState, 1))
	{
		return false;
	}
	return true;
}

BOOL StageFunctions::ConfigureTriggerOutput(int ID, int TriggerOutputID, int CTOPam_Axis, double CTOVal_XAxis)
{

	if (!PI_CTO(ID, &TriggerOutputID, &CTOPam_Axis, &CTOVal_XAxis, 1))
	{
		return false;
	}
	return true;
}

BOOL StageFunctions::ConfigureTriggerInput(int ID, int TriggerInputID, int CTIPam, char CTIVal_ZAxis)
{

	if (!PI_CTI(ID, &TriggerInputID, &CTIPam, &CTIVal_ZAxis, 1))

	{
		return false;
	}
	return true;
}

int CTOPam_Axis = 2;
double CTOVal_XAxis = 1.0;
