#include "stdafx.h"
#include "ScanHead.h"
#include "RTC5impl.h" //import trc5
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
#include <tchar.h>
#include <string>

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

BOOL ScanHeadFunctions::ScanheadInitialization()
{
	short CommandError = true;
	short InitErrorNo = true;
	// load the correction and dat files when executable running 
	char executablePath[32768];

	DWORD pathLength = GetModuleFileNameA(
		nullptr, executablePath, 32768);

	if (pathLength == 0 || pathLength >= 32768)
		return FALSE;

	std::string scannerFolder(executablePath, pathLength);
	size_t separator = scannerFolder.find_last_of("\\/");

	if (separator == std::string::npos)
		return FALSE;

	scannerFolder.resize(separator + 1);
	scannerFolder += "..\\..\\ScannerFiles\\";

	std::string correctionFile =
		scannerFolder + "Cor_1to1.ct5";

	// initialise scanner
	InitErrorNo = init_rtc5_dll();
	CommandError = load_correction_file(correctionFile.c_str(), 1, 2);

	if(CommandError) {
        printf("Correction file loading error.");
        return FALSE;
    }

	CommandError = load_program_file(scannerFolder.c_str());
    if(CommandError) {
        printf("Program file loading error.");
        return FALSE;
    }

	set_laser_mode(0U);						//CO2 mode
	set_laser_control(0x18U);				//Initialising TTL LASERON pin

	set_control_mode(1);					//Enable the START input; external trigger by stage
	config_list(-1, 0);						// set all 2^20 memory to list_1 and 0 to list_2 (can be changed accordingly)
	set_extstartpos(0);						//Select list 1 to be executed by external trigger for rtc5

	return TRUE;
}
BOOL ScanHeadFunctions::SetScanGain(int gain) {

	set_hi(1, gain, gain, 0, 0); // set x and y gain to 10%
	return TRUE;
}

BOOL ScanHeadFunctions::SetScanSpeed(double markspeed, double jumpspeed)
{
	set_mark_speed(markspeed);
	set_jump_speed(jumpspeed);

	return TRUE;

}

unsigned short ScanHeadFunctions::RTCVersion()
{
	unsigned short RTCVersion = get_rtc_version(); //  gives version of RTC5RBF.rbf file
	return RTCVersion;
}

unsigned short ScanHeadFunctions::HEXVersion()
{
	unsigned short HexVersion = get_hex_version();  // in RTC% gives the version of the RTC5OUT.out file; option 3D not enabled
	return HexVersion;
}

unsigned short ScanHeadFunctions::DLLVersion()
{
	unsigned short DllVersion = get_dll_version(); // Returns the version number of the RTC5 DLL
	return DllVersion;
}

BOOL ScanHeadFunctions::SingleScanLine(double marktime, double jumptime, double delaytime, short X1para, short X2para, short Y1para, short Y2para)
{
	double timetoscan = 0;
	UINT busy = 0;
	UINT position = 0; // for rtc5
	// Define constants for setting DIGITAL OUT1
	const unsigned int DIGITAL_OUT1 = 0x01; // Bit #0
	const unsigned int DIGITAL_OUT2 = 0x02; // Bit #0

	unsigned int Pins = 0; // Initialize Pins to 0 or any other initial value

	set_start_list(1);
	set_scanner_delays(static_cast<UINT>(delaytime), 0, 0);

		// Laser OFF during reposition; Set DIGITAL OUT1 low
		Pins &= ~(DIGITAL_OUT1 | DIGITAL_OUT2);
		set_laser_pin_out_list(Pins);
		timed_jump_abs(Y1para, X1para, jumptime);

		// Laser ON during marking/exposure ; Set DIGITAL OUT1 high
		Pins = (Pins & ~DIGITAL_OUT2) | DIGITAL_OUT1;
		set_laser_pin_out_list(Pins);
		timed_mark_abs(Y2para, X2para, marktime);
		set_end_of_list();

	execute_list(1);				// start marking

   // wait as long the execution is finished
	do {
		get_status(&busy, &position);

	} while (busy);

	return TRUE;
}
BOOL ScanHeadFunctions::SingleScanLineDualCh(double marktime, double jumptime, double delaytime, short X1para, short X2para, short Y1para, short Y2para)
{
	double timetoscan = 0;
	UINT busy = 0;
	UINT position = 0; // for rtc5
	// Define constants for setting DIGITAL OUT1
	const unsigned int DIGITAL_OUT1 = 0x01; // Bit #0
	const unsigned int DIGITAL_OUT2 = 0x02; // Bit #1

	unsigned int Pins = 0; // Initialize Pins to 0 or any other initial value

	set_start_list(1);
	set_scanner_delays(static_cast<UINT>(delaytime), 0, 0);

		// Jump: both OFF
		Pins &= ~(DIGITAL_OUT1 | DIGITAL_OUT2);
		set_laser_pin_out_list(Pins);
		timed_jump_abs(Y1para, X1para, jumptime);

		// Mark: 488 ON, 561 OFF
		Pins = (Pins & ~DIGITAL_OUT2) | DIGITAL_OUT1;
		set_laser_pin_out_list(Pins);
		timed_mark_abs(Y2para, X2para, marktime);

		// Jump: both OFF
		Pins &= ~(DIGITAL_OUT1 | DIGITAL_OUT2);
		set_laser_pin_out_list(Pins);
		timed_jump_abs(Y2para, X2para, jumptime);

		// Mark: 561 ON, 488 OFF
		Pins = (Pins & ~DIGITAL_OUT1) | DIGITAL_OUT2;
		set_laser_pin_out_list(Pins);
		timed_mark_abs(Y1para, X1para, marktime);

    set_end_of_list();

     execute_list(1);				// start marking

	// wait as long the execution is finished
    do{
		get_status(&busy, &position);

    } while(busy);

	return TRUE;
}

BOOL ScanHeadFunctions::BeamJumpToInitialPosition(double jumpspeed, short jumpabsx, short jumpabsy)
{
	double timetoscan = 0;
	UINT busy = 0; // for rtc5
	UINT position = 0; // for rtc5

	//unsigned short busy = 0;
	//unsigned short position = 0; // for rtc4

	set_start_list(1);			// timing, delay and speed preset

		set_jump_speed(jumpspeed);	// jump speed in bits per milliseconds

		// jump "SCANLAB", move the laser beam to the initial position
		jump_abs(jumpabsx, jumpabsy);

    set_end_of_list();

    execute_list(1);				// start jumping

	// wait as long the execution is finished
    do
	{
		get_status(&busy, &position);
    } while(busy);

	return TRUE;
}

BOOL ScanHeadFunctions::ExternalTriggerOn()
{
	set_control_mode(1);	//Enable the START input
	set_extstartpos(0); //Select list 1 to be executed by external trigger for rtc5. sets the start position to the beginning of List 1.
	return TRUE;
}
