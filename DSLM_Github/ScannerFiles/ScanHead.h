#ifndef _ScanHead_h
#define _ScanHead_h

#include <windows.h>
#include "RTC5impl.h" //import rtc5

using namespace System;

ref class ScanHeadFunctions{
public:

	//Scanner controller card RTC5 functions
	static BOOL ScanHeadFunctions::ScanheadInitialization();
	static BOOL ScanHeadFunctions::SetScanGain(int gain);
	static BOOL ScanHeadFunctions::SetScanSpeed(double markspeed, double jumpspeed);
	static unsigned short ScanHeadFunctions::RTCVersion();
	static unsigned short ScanHeadFunctions::HEXVersion();
	static unsigned short ScanHeadFunctions::DLLVersion();
	static BOOL ScanHeadFunctions::SingleScanLine(double markstime, double jumptime, double delaytime, short X1para, short X2para, short Y1para, short Y2para);
	static BOOL ScanHeadFunctions::SingleScanLineDualCh(double markstime, double jumptime, double delaytime, short X1para, short X2para, short Y1para, short Y2para);
	static BOOL ScanHeadFunctions::ExternalTriggerOn();
	static BOOL ScanHeadFunctions::BeamJumpToInitialPosition(double jumpTime, short jumpabsx, short jumpabsy);

};

#endif
