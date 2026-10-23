#include "stdafx.h"
#include "Cameras.h"
#include "Images.h"
#include "LiveCam1.h"
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
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;
using namespace System::Runtime::InteropServices;
using namespace System::IO;
using namespace System::Diagnostics;
using namespace MyDSLM;

Bitmap^ FunctionsForImages::GetBitmapStructure(int imagewidth, int imageheight)
{
	Bitmap^ B = gcnew Bitmap(
		imagewidth,
		imageheight,
		System::Drawing::Imaging::PixelFormat::Format8bppIndexed);

	System::Drawing::Imaging::ColorPalette^ palette = B->Palette;

	for (int i = 0; i < 256; i++)
	{
		palette->Entries[i] =
			System::Drawing::Color::FromArgb(i, i, i);
	}

	B->Palette = palette;

	return B;
}

Bitmap^ FunctionsForImages::ExportTheFinalBitmapWithHeightAdjustment(HDCAM h, int ImageType8Or16bit, Bitmap^ bitmapexp, char * ImageBuffer, char* CStack, int ReferenceHeight, int IWidth, int IHeight, DWORD framebyte, char* LUT)
{
	SYSTEMTIME HAtimestart, HAtimestop; //Set up values to hold timings

	GetSystemTime(&HAtimestart);

	int byts;						//Bytes in the image
	array <Byte> ^ bitval;		//Array to store the individual bytes values
	typedef System::Drawing::Rectangle Recta;		//Rectangle to be used on image display
	IntPtr ptr0;					//IntPtr to be used in bitmapData->Scan0
	int min =0;
	int max =0;
	int bitpos=0;
	int j = 0;

	// ********Height adjustment variables*************
	int n;
	double	mean_run, M2, delta;
	int peakPosition = 0;
	int peakValue = 0;
	int peakValueNow = 0;

	Recta rect = Recta(0, 0, IWidth, IHeight); //rectange to specify portion of bipmap to be locked
	BitmapData^ bitmapDa0 = bitmapexp->LockBits(rect, ImageLockMode::ReadWrite, bitmapexp->PixelFormat);
	ptr0 = bitmapDa0->Scan0; //get address of the first line

	if (ImageType8Or16bit == 8)
	{
		byts = abs(bitmapDa0->Stride) * bitmapDa0->Height; //Declare array to hold the bytes of the bitmap
		bitval = gcnew array <Byte>(byts);
		Marshal::Copy(ptr0, bitval, 0, byts);

		//Write lookup table values into array
		bitpos = ((IHeight)*IWidth) - 1;

		if (camera_rotated_90_degrees != FALSE)
		{
			SIZE IWAndH = CameraFunctions::GetImageSize(h);
			int IWidth = IWAndH.cy; //512
			int IHeight = IWAndH.cx; //2048
		}

		if (camera_rotated_90_degrees != FALSE)
		{
			for (int y = 0; y < IHeight; y++)//(int y = IHeight; y-- > 0; )
			{
				for (int x = 0; x < IWidth; x++)//(int x = IWidth; x-- > 0; )
				{

					bitval[bitpos] = ImageBuffer[x * IHeight + (IHeight - 1 - y)];
					bitpos--;
				}
			}
		}
		else
		{
			for (int y = IHeight; y-- > 0; )
			{
				for (int x = IWidth; x-- > 0; )
				{

					bitval[bitpos] = ImageBuffer[x + y * IWidth];
					bitpos--;
				}
			}
		}

		// Save the converted 8-bit image to CStack
		Marshal::Copy(bitval, 0, IntPtr(CStack), byts);

		//Marking reference line in the image, with static line

		int k = ReferenceHeight;
		for (int i = k; i <= k + 1; i++)
		{
			for (j = 0; j <= IWidth; j++)
			{
				bitval[i * IWidth + j] = 255;
			}
		}

		// marking vertical line
		int s = IWidth / 2;
		for (int i = s; i <= s + 1; i++)
		{
			for (j = 0; j <= IHeight - 1; j++)
			{
				bitval[IWidth / 2 + IWidth * j] = 255;;

			}
		}

		//Height Adjustment calculations, find the line of pixels with high intensity
		for (int i = 0; i <= IHeight - 1; i++)
		{
			n = 0;
			mean_run = 0;
			M2 = 0;

			for (j = adjWindowLeft; j <= adjWindowRight; j += 2)	// computes running variance
			{
				n = n + 1;
				delta = bitval[i * IWidth + j] - mean_run;
				mean_run = mean_run + delta * n_rec[n - 1];
				M2 = M2 + delta * (bitval[i * IWidth + j] - mean_run);
			}

			peakValueNow = static_cast<int>(M2 / (n - 1)); //sample variance

			if (peakValueNow > peakValue)
			{
				peakValue = peakValueNow;
				peakPosition = i;
			}

		}

		if (peakPosition == 0)
		{
			for (j = 0; j < IWidth; j++)
			{
				bitval[peakPosition * IWidth + j] = 255;
				bitval[(peakPosition + 1) * IWidth + j] = 255;
			}
		}
		else
		{
			for (j = 0; j < IWidth; j++)
			{
				bitval[(peakPosition - 1) * IWidth + j] = 255;
				bitval[peakPosition * IWidth + j] = 255;
			}

		}

		ShiftFromReferenceHeight = ReferenceHeight - peakPosition;

	}

	else if(ImageType8Or16bit == 16)
	{
		byts = abs(bitmapDa0->Stride) * bitmapDa0->Height; //Declare array to hold the bytes of the bitmap
		bitval = gcnew array <Byte>(byts);
		Marshal::Copy(ptr0, bitval, 0, byts);

		//Write lookup table values into array
		bitpos = ((IHeight)*IWidth) - 1;

		if (camera_rotated_90_degrees != FALSE)
		{
			SIZE IWAndH = CameraFunctions::GetImageSize(h);
			int IWidth = IWAndH.cy; //512
			int IHeight = IWAndH.cx; //2048
		}

		if (camera_rotated_90_degrees != FALSE)
		{
			for (int y = 0; y < IHeight; y++)//(int y = IHeight; y-- > 0; )
			{
				for (int x = 0; x < IWidth; x++)//(int x = IWidth; x-- > 0; )
				{

					bitval[bitpos] = LUT[*((unsigned short*)(&ImageBuffer[x * IHeight + (IHeight - 1 - y)]))];

					bitpos--;
				}
			}
		}
		else
		{
			for (int y = IHeight; y-- > 0; )
			{
				for (int x = IWidth; x-- > 0; )
				{

					bitval[bitpos] = LUT[*((unsigned short*)(&ImageBuffer[(x + y * IWidth) * 2]))];
					bitpos--;
				}
			}
		}

		// Save the converted 8-bit image to CStack
		Marshal::Copy(bitval, 0, IntPtr(CStack), byts);

		//Marking reference line in the image, with static line

		int k = ReferenceHeight;
		for (int i = k; i <= k + 1; i++)
		{
			for (j = 0; j <= IWidth; j++)
			{
				bitval[i * IWidth + j] = 255;
			}
		}

		// marking vertical line

		int s = IWidth / 2;
		for (int i = s; i <= s + 1; i++)
		{
			for (j = 0; j <= IHeight - 1; j++)
			{
				bitval[IWidth / 2 + IWidth * j] = 255;;

			}
		}

		//Height Adjustment calculations, find the line of pixels with high intensity
		for (int i = 0; i <= IHeight - 1; i++)
		{
			n = 0;
			mean_run = 0;
			M2 = 0;

			for (j = adjWindowLeft; j <= adjWindowRight; j += 2)	// computes running variance
			{
				n = n + 1;
				delta = bitval[i * IWidth + j] - mean_run;
				mean_run = mean_run + delta * n_rec[n - 1];
				M2 = M2 + delta * (bitval[i * IWidth + j] - mean_run);
			}

			peakValueNow = static_cast<int>(M2 / (n - 1)); //sample variance

			if (peakValueNow > peakValue)
			{
				peakValue = peakValueNow;
				peakPosition = i;
			}

		}

		if (peakPosition == 0)
		{
			for (j = 0; j < IWidth; j++)
			{
				bitval[peakPosition * IWidth + j] = 255;
				bitval[(peakPosition + 1) * IWidth + j] = 255;
			}
		}
		else
		{
			for (j = 0; j < IWidth; j++)
			{
				bitval[(peakPosition - 1) * IWidth + j] = 255;
				bitval[peakPosition * IWidth + j] = 255;
			}

		}

		ShiftFromReferenceHeight = ReferenceHeight - peakPosition;

	}

	ColorPalette^ palette = bitmapexp->Palette;
	for (int i = 1; i < 255; i++)
	{
		palette->Entries[i] = Color::FromArgb(i, i, i);
	}
	palette->Entries[0] = Color::Blue;
	palette->Entries[255] = Color::Red;
	bitmapexp->Palette = palette;

	Marshal::Copy(bitval, 0, ptr0, byts);

	bitmapexp->UnlockBits(bitmapDa0);

	GetSystemTime(&HAtimestop);

	HACalculationTime = (HAtimestop.wHour - HAtimestart.wHour) * 60 * 60 * 1000 + (HAtimestop.wMinute - HAtimestart.wMinute) * 60 * 1000 + (HAtimestop.wSecond - HAtimestart.wSecond) * 1000 + (HAtimestop.wMilliseconds - HAtimestart.wMilliseconds);

	return bitmapexp;

}

BOOL FunctionsForImages::HeightAdjustemntMeasurementsFile(String ^Folder, array <int> ^ HAMeasuements, int numberofframesacquired, int timepoint)
{

	// File to record all the data from the code
	StreamWriter ^ HAWriter;
	FileInfo ^HAFile = gcnew FileInfo(Folder + "\\HA_all_timePoints.txt");
	FileStream ^HASave = HAFile->Open(FileMode::Append, FileAccess::Write, FileShare::Read);
	HAWriter = gcnew StreamWriter(HASave);

	HAWriter->WriteLine("Time point: {0} \t {1}  ", timepoint, DateTime::Now.ToString("HH:mm:ss.fff"));
	for(int u=0; u<= numberofframesacquired-1; u++)
	{
		int pixelsforHA = HAMeasuements[u];
		HAWriter->WriteLine(pixelsforHA);
	}

	HAWriter->Close();

	return TRUE;

}

