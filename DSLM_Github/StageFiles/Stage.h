#ifndef _Stage_h
#define _Stage_h

#include "windows.h"
#include "PI_GCS2_DLL.h"
#include <vector>

using namespace System;

ref class StageFunctions{
public:

	//Stage initialization, information, connection
	static BOOL StageFunctions::StageInitialization();	//Initialization of the 3D stage axes XYZ connected to the PC through a box controller
	static BOOL StageFunctions::ReferenceAxis(int ID, char* axis);		//Function to reference the "axis" attached to the controller with that specific "ID"
	static BOOL StageFunctions::ControllerIsReadyForNewCommand(int ID);	//Function to ask the controller if it can receive a new command or not
	static BOOL StageFunctions::StageIsMoving(int ID, char * axis);		//Function to ask the controller with a specific "ID" if the referenced "axis"/axes is/are moving
	static double StageFunctions::GetAxisPosition(int ID, char * axis);	//Function to read axis current position
	static BOOL StageFunctions::MoveAxisToThisExactPosition(int ID, char * axis, double Position);	//Function to move the axis to that exact position
	static BOOL StageFunctions::MoveAxisThisExactStep(int ID, char * axis, double Step);	//Function to move the axis this exact Step relative to current position
	static double StageFunctions::GetAxisVelocity(int ID, char * axis);	//Function to get axis velocity
	static BOOL StageFunctions::SetAxisVelocity(int ID, char * axis, double Velocity);	//Function to set axis velocity

	//Output Triggering
	static BOOL StageFunctions::ToggleOutputOnOff(int ID, int TriggerOutputID, BOOL TriggerState);
	static BOOL StageFunctions::ConfigureTriggerOutput(int ID, int TriggerOutputID, int CTOPam_Axis, double CTOVal_XAxis);

	//Input Triggering
	static BOOL StageFunctions::ConfigureTriggerInput(int ID, int TriggerInputID, int CTIPam, char CTIVal_ZAxis);
	static BOOL StageFunctions::ToggleInputOnOff(int ID, int TriggerInputID, BOOL TriggerState);

	//Get Controller boxes errors
	static String^ StageFunctions::StageLastError(int ErrorCd);

	static int NrOfControllersConnected;	//Number of controller boxes connected to the PC
	static int ID1;							//Controller box 1 ID, this ID allows to command directly the controller box 1
	static int ID2;							//Controller box 2 ID, this ID allows to command directly the controller box 2
	static String^ Controller1Details;		//Details of controller 1 connected to the PC
	static String^ Controller2Details;		//Details of controller 2 connected to the PC
	static String^ InitializationError;		//String to output error during Initialization of the stage
	static String^ ErrorMessageString;		//Variable to store the last error catched by one of the controller boxes
	static String^ ErrorMessageString1;		//Variable to store the last error catched by the controller box 1
	static String^ ErrorMessageString2;		//Variable to store the last error catched by the controller box 2
	static int ErrorCode1;					//Each error have an "identity" number, for controller box 1
	static int ErrorCode2;					//Each error have an "identity" number, for controller box 2

};

#endif
