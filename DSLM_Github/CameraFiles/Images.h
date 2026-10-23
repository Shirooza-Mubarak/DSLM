#ifndef _Images_h
#define _Images_h

#include "windows.h"
#include "dcamapi.h"
#include "dcamapi3.h"
#include "ccDefine.h"
#include "dcamapix.h"
#include "dcamprop.h"
#include "dcimgapi.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;
using namespace System::Runtime::InteropServices;

namespace MyDSLM {
	using namespace System;
	using namespace System::Drawing;

}

ref class FunctionsForImages{
public:

	//Bitmap structure initialization and information
	static Bitmap^ FunctionsForImages::GetBitmapStructure(int imagewidth, int imageheight);

	static Bitmap^ FunctionsForImages::ExportTheFinalBitmapWithHeightAdjustment(HDCAM h, int ImageType8Or16bit, Bitmap^ bitmapexp, char * ImageBuffer, char* CStack, int ReferenceHeight, int IWidth, int IHeight, DWORD framebyte, char* LUT);

	static BOOL FunctionsForImages::HeightAdjustemntMeasurementsFile(String ^Folder, array <int> ^ HAMeasuements, int numberofframesacquired, int timepoint);

	static int BrightLevelMin;
	static int BrightLevelMax;
	static int BrightLevelMin_ch2;
	static int BrightLevelMax_ch2;
	static unsigned char* LookUT8bit;				//This look up table doesn't change anything on the 16-bit image minimum and maximum bright levels, it only turn the values into 0-255, so the image can be displayed
	static int ShiftFromReferenceHeight;	//Variable to store the pixels difference from brightest line to reference line
	static BOOL camera_rotated_90_degrees;

	static double HACalculationTime; //Variable to hold calculation time
	static int adjWindowLeft;
	static int adjWindowRight;
	static double* n_rec;

	static char * Image8bitWithBrightLevels;

};

#endif
