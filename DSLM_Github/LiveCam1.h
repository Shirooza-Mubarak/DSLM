#pragma once
#include"CameraFiles/Images.h"
#include"CameraFiles/Cameras.h"
#include "Zprojection.h"
#include <cmath>
#include <omp.h>
#include <iostream>
#include <windows.h>
#include <fstream>
#include <cmath>
#include <CameraFiles/Images.h>
#include <cstdlib>

#include <string>
#include <vector>
#include <chrono>
#include <msclr/marshal_cppstd.h>

namespace MyDSLM {

	using namespace System;
	using namespace System::IO;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;
	using namespace System::Diagnostics;

	/// <summary>
	/// Summary for LiveCam1
	/// </summary>

	public ref class LiveCam1 : public System::Windows::Forms::Form
	{
	public:
		char* BufferForImage;			//Pointer to receive the image
		char* BufferForImage_ch2;			//Pointer to receive the image
		HDCAM LiCAM;					//Handler for the camera
		DWORD	framebyte, dw, WaitMode;
		static Graphics^ g1;
		int ImageWidth;
		int ImageHeight;
		int noChannels;
		static int Yloop;
		static int yIndex;
		static int NumberOfScansPerTimePointCam1;			//Variable to recive the number of cross-sections acquired per time point
		static int HA_channel;
		Bitmap^ bitmap0;				//Bitmap to be used
		Bitmap^ bitmap1;				//Bitmap to be used
		int32 rowbytes;					//The total number of bytes allocated in a single line of pixels
		//typedef System::Drawing::Rectangle R;		//Rectangle to be used on image display
		IntPtr ptr0;					//IntPtr to be used in bitmapData->Scan0
		static String^ Folder = "";		//Folder to save experimental data
		static String^ Folder2 = "";		//Folder to save experimental data
		BOOL FunctionOK;				//Bool to receive the success or unsuccess of a function
		bool LiveImagingOnLoopIsON;			//Bool to know that live imaging is running
		static array <int>^ Cam1TimeForCaptureImageAndSaveBitmap = gcnew array <int>(6000);		//Array to save times in sequence image acquisition mode
		static array <int>^ Cam1TimeToDisplayBitmap = gcnew array <int>(60000);		//Second array to save times in sequence image acquisition mode
		unsigned char** mstack;			//3D stack, for running camera in sequence mode
		static  array <int>^ CamIndexAndTotalframes = gcnew array <int>(2);		//Variable to recieve camera buffer index and the total number of frames recorded by camera since the capture started
		static bool saveframestimes = false;	//Variable to define if user wants to save the time or not in the camera 1 info txt file
		static bool FolderWasSet = false;		//Variable to indicate if the folder to store the data was selected
		static bool dualChannelAcquisition;
		static int NumberOfTimePointsCam1;					//Number of Time Points to be acquired during the experiment
		static bool ExperimentStartedCam1 = false;			//Variable to receive the info from Form1 to start the experiment
		static bool AcquireNextTimePoint = false;			//variable to send the readiness status to Form1 for acquire next time point
		static bool CameraIsReadyToStartExperiment = false;	//Variable to make the scanner waiting to start
		static bool ScannerIsRunning = false;				//Variable for running experiment
		static int WaitingTimeExperiment = 1000;			//Variable for waiting time (or sleep) between time points
		static int CurrentTimePoint = 0;					//Variable for keeping the current time point running on experimental mode
		static int Image8Or16bit = 0;						//Variable to let know if image is 8 or 16 bit

		static int BrightLevelMin;
		static int BrightLevelMax;
		static int BrightLevelMin_ch2;
		static int BrightLevelMax_ch2;

		char* LutCh1;
		char* LutCh2;

		// for transformed image
		int maxXPointsOriginal;
		int maxXPointsInterpolated;
		int tranWidth;
		int tranHeight;
		static int magnification;
		static double stepX, sst, travelTime;
		double acquisitionAspectRatio;
		double sqrt2 = 1.4142135623730950488016887242097;
		double physicalPixelSize = 0.0065; // 0.0065 mm = 6.5 um
		static double stage_x =0;
		static double stage_y =0;

		static array <int>^ ImageHeightAdjustmentPixelsDifference = gcnew array <int>(6000);		//Array to store pixel lines height difference

		char** CStack;					//3D stack for channel 1, for receiving the mstack converted images with slected Bright levels
		char** CStack_ch2;					//3D stack for channel 2, for receiving the mstack converted images with slected Bright levels
		static int ImageReferenceHeight;		//Image reference height for height adjustment calculations
		static bool camera_rotated_90_degrees = false;
		//----DIAGNOTICS----//
		int Frame;

	public: System::ComponentModel::BackgroundWorker^ liveCam1_backgroundWorker;

	private: System::Windows::Forms::Label^ labelTimePoints;
	private: System::Windows::Forms::TextBox^ liveCam1_txtFrame;
	private: System::Windows::Forms::TextBox^ liveCam1_txtFolderPath2;
	private: System::Windows::Forms::RadioButton^ display_rdB_channel1;

	private: System::Windows::Forms::RadioButton^ display_rdB_channel2;

	private:
		Zprojection^ zprojection;

	private: System::Windows::Forms::Label^ labelscanspertimepoint;
	public:
		LiveCam1(void)
		{
			InitializeComponent();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~LiveCam1()
		{
			if (components)
			{
				delete components;
			}
			if (zprojection != nullptr && !zprojection->IsDisposed)
			{
				zprojection->Close();
				delete zprojection;
			}
		}
	private: System::Windows::Forms::CheckBox^ liveCam1_checkBoxRunExperiment;
	private: System::Windows::Forms::CheckBox^ liveCam1_checkBoxSaveSequenceOfImages;
	public: System::Windows::Forms::ProgressBar^ liveCam1_progressBar;
	private: System::Windows::Forms::Button^ liveCam1_btnStopLiveImaging;

	private: System::Windows::Forms::Button^ liveCam1_btnLiveImaging;

	private: System::Windows::Forms::Button^ liveCam1_btnSetFoldePath;

	private: System::Windows::Forms::Label^ lblCam1LiveImageFolderPath;
	private: System::Windows::Forms::TextBox^ liveCam1_txtFolderPath;
	private: System::Windows::Forms::CheckBox^ liveCam1_checkBoxSaveCapturedImage;
	private: System::Windows::Forms::Button^ liveCam1_btnCapture;
	private: System::Windows::Forms::FolderBrowserDialog^ liveCam1_FolderDialog;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->liveCam1_checkBoxRunExperiment = (gcnew System::Windows::Forms::CheckBox());
			this->liveCam1_checkBoxSaveSequenceOfImages = (gcnew System::Windows::Forms::CheckBox());
			this->liveCam1_progressBar = (gcnew System::Windows::Forms::ProgressBar());
			this->liveCam1_btnStopLiveImaging = (gcnew System::Windows::Forms::Button());
			this->liveCam1_btnLiveImaging = (gcnew System::Windows::Forms::Button());
			this->liveCam1_btnSetFoldePath = (gcnew System::Windows::Forms::Button());
			this->lblCam1LiveImageFolderPath = (gcnew System::Windows::Forms::Label());
			this->liveCam1_txtFolderPath = (gcnew System::Windows::Forms::TextBox());
			this->liveCam1_checkBoxSaveCapturedImage = (gcnew System::Windows::Forms::CheckBox());
			this->liveCam1_btnCapture = (gcnew System::Windows::Forms::Button());
			this->liveCam1_FolderDialog = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->liveCam1_backgroundWorker = (gcnew System::ComponentModel::BackgroundWorker());
			this->labelTimePoints = (gcnew System::Windows::Forms::Label());
			this->labelscanspertimepoint = (gcnew System::Windows::Forms::Label());
			this->liveCam1_txtFrame = (gcnew System::Windows::Forms::TextBox());
			this->liveCam1_txtFolderPath2 = (gcnew System::Windows::Forms::TextBox());
			this->display_rdB_channel1 = (gcnew System::Windows::Forms::RadioButton());
			this->display_rdB_channel2 = (gcnew System::Windows::Forms::RadioButton());
			this->SuspendLayout();
			// liveCam1_checkBoxRunExperiment
			this->liveCam1_checkBoxRunExperiment->AutoSize = true;
			this->liveCam1_checkBoxRunExperiment->Enabled = false;
			this->liveCam1_checkBoxRunExperiment->Location = System::Drawing::Point(522, 41);
			this->liveCam1_checkBoxRunExperiment->Name = L"liveCam1_checkBoxRunExperiment";
			this->liveCam1_checkBoxRunExperiment->Size = System::Drawing::Size(101, 17);
			this->liveCam1_checkBoxRunExperiment->TabIndex = 21;
			this->liveCam1_checkBoxRunExperiment->Text = L"Run Experiment";
			this->liveCam1_checkBoxRunExperiment->UseVisualStyleBackColor = true;
			// liveCam1_checkBoxSaveSequenceOfImages
			this->liveCam1_checkBoxSaveSequenceOfImages->AutoSize = true;
			this->liveCam1_checkBoxSaveSequenceOfImages->Enabled = false;
			this->liveCam1_checkBoxSaveSequenceOfImages->Location = System::Drawing::Point(413, 40);
			this->liveCam1_checkBoxSaveSequenceOfImages->Name = L"liveCam1_checkBoxSaveSequenceOfImages";
			this->liveCam1_checkBoxSaveSequenceOfImages->Size = System::Drawing::Size(103, 17);
			this->liveCam1_checkBoxSaveSequenceOfImages->TabIndex = 20;
			this->liveCam1_checkBoxSaveSequenceOfImages->Text = L"Save Sequence";
			this->liveCam1_checkBoxSaveSequenceOfImages->UseVisualStyleBackColor = true;
			// liveCam1_progressBar
			this->liveCam1_progressBar->Location = System::Drawing::Point(777, 37);
			this->liveCam1_progressBar->Name = L"liveCam1_progressBar";
			this->liveCam1_progressBar->Size = System::Drawing::Size(78, 25);
			this->liveCam1_progressBar->Step = 1;
			this->liveCam1_progressBar->TabIndex = 19;
			// liveCam1_btnStopLiveImaging
			this->liveCam1_btnStopLiveImaging->Enabled = false;
			this->liveCam1_btnStopLiveImaging->Location = System::Drawing::Point(658, 37);
			this->liveCam1_btnStopLiveImaging->Name = L"liveCam1_btnStopLiveImaging";
			this->liveCam1_btnStopLiveImaging->Size = System::Drawing::Size(112, 25);
			this->liveCam1_btnStopLiveImaging->TabIndex = 18;
			this->liveCam1_btnStopLiveImaging->Text = L"Stop Live Imaging";
			this->liveCam1_btnStopLiveImaging->UseVisualStyleBackColor = true;
			this->liveCam1_btnStopLiveImaging->Click += gcnew System::EventHandler(this, &LiveCam1::liveCam1_btnStopLiveImaging_Click);
			// liveCam1_btnLiveImaging
			this->liveCam1_btnLiveImaging->Enabled = false;
			this->liveCam1_btnLiveImaging->Location = System::Drawing::Point(329, 36);
			this->liveCam1_btnLiveImaging->Name = L"liveCam1_btnLiveImaging";
			this->liveCam1_btnLiveImaging->Size = System::Drawing::Size(78, 25);
			this->liveCam1_btnLiveImaging->TabIndex = 17;
			this->liveCam1_btnLiveImaging->Text = L"Live Imaging";
			this->liveCam1_btnLiveImaging->UseVisualStyleBackColor = true;
			this->liveCam1_btnLiveImaging->Click += gcnew System::EventHandler(this, &LiveCam1::liveCam1_btnLiveImaging_Click);
			// liveCam1_btnSetFoldePath
			this->liveCam1_btnSetFoldePath->Location = System::Drawing::Point(777, 7);
			this->liveCam1_btnSetFoldePath->Name = L"liveCam1_btnSetFoldePath";
			this->liveCam1_btnSetFoldePath->Size = System::Drawing::Size(78, 25);
			this->liveCam1_btnSetFoldePath->TabIndex = 16;
			this->liveCam1_btnSetFoldePath->Text = L"Open";
			this->liveCam1_btnSetFoldePath->UseVisualStyleBackColor = true;
			this->liveCam1_btnSetFoldePath->Click += gcnew System::EventHandler(this, &LiveCam1::liveCam1_btnSetFoldePath_Click);
			// lblCam1LiveImageFolderPath
			this->lblCam1LiveImageFolderPath->AutoSize = true;
			this->lblCam1LiveImageFolderPath->Location = System::Drawing::Point(10, 14);
			this->lblCam1LiveImageFolderPath->Name = L"lblCam1LiveImageFolderPath";
			this->lblCam1LiveImageFolderPath->Size = System::Drawing::Size(64, 13);
			this->lblCam1LiveImageFolderPath->TabIndex = 15;
			this->lblCam1LiveImageFolderPath->Text = L"Folder Path:";
			// liveCam1_txtFolderPath
			this->liveCam1_txtFolderPath->Location = System::Drawing::Point(80, 11);
			this->liveCam1_txtFolderPath->Name = L"liveCam1_txtFolderPath";
			this->liveCam1_txtFolderPath->Size = System::Drawing::Size(340, 20);
			this->liveCam1_txtFolderPath->TabIndex = 14;
			// liveCam1_checkBoxSaveCapturedImage
			this->liveCam1_checkBoxSaveCapturedImage->AutoSize = true;
			this->liveCam1_checkBoxSaveCapturedImage->Enabled = false;
			this->liveCam1_checkBoxSaveCapturedImage->Location = System::Drawing::Point(100, 41);
			this->liveCam1_checkBoxSaveCapturedImage->Name = L"liveCam1_checkBoxSaveCapturedImage";
			this->liveCam1_checkBoxSaveCapturedImage->Size = System::Drawing::Size(83, 17);
			this->liveCam1_checkBoxSaveCapturedImage->TabIndex = 13;
			this->liveCam1_checkBoxSaveCapturedImage->Text = L"Save Image";
			this->liveCam1_checkBoxSaveCapturedImage->UseVisualStyleBackColor = true;
			// liveCam1_btnCapture
			this->liveCam1_btnCapture->Enabled = false;
			this->liveCam1_btnCapture->Location = System::Drawing::Point(13, 37);
			this->liveCam1_btnCapture->Name = L"liveCam1_btnCapture";
			this->liveCam1_btnCapture->Size = System::Drawing::Size(78, 25);
			this->liveCam1_btnCapture->TabIndex = 12;
			this->liveCam1_btnCapture->Text = L"Capture";
			this->liveCam1_btnCapture->UseVisualStyleBackColor = true;
			this->liveCam1_btnCapture->Click += gcnew System::EventHandler(this, &LiveCam1::liveCam1_btnCapture_Click);
			// liveCam1_backgroundWorker
			this->liveCam1_backgroundWorker->WorkerReportsProgress = true;
			this->liveCam1_backgroundWorker->WorkerSupportsCancellation = true;
			this->liveCam1_backgroundWorker->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &LiveCam1::liveCam1_backgroundWorker_DoWork);
			this->liveCam1_backgroundWorker->ProgressChanged += gcnew System::ComponentModel::ProgressChangedEventHandler(this, &LiveCam1::liveCam1_backgroundWorker_ProgressChanged);
			this->liveCam1_backgroundWorker->RunWorkerCompleted += gcnew System::ComponentModel::RunWorkerCompletedEventHandler(this, &LiveCam1::liveCam1_backgroundWorker_RunWorkerCompleted);
			// labelTimePoints
			this->labelTimePoints->AutoSize = true;
			this->labelTimePoints->Location = System::Drawing::Point(861, 44);
			this->labelTimePoints->Name = L"labelTimePoints";
			this->labelTimePoints->Size = System::Drawing::Size(68, 13);
			this->labelTimePoints->TabIndex = 91;
			this->labelTimePoints->Text = L"Internal FPS:";
			// labelscanspertimepoint
			this->labelscanspertimepoint->AutoSize = true;
			this->labelscanspertimepoint->Location = System::Drawing::Point(861, 15);
			this->labelscanspertimepoint->Name = L"labelscanspertimepoint";
			this->labelscanspertimepoint->Size = System::Drawing::Size(105, 13);
			this->labelscanspertimepoint->TabIndex = 90;
			this->labelscanspertimepoint->Text = L"Scans / Total Scans";
			// liveCam1_txtFrame
			this->liveCam1_txtFrame->Location = System::Drawing::Point(935, 41);
			this->liveCam1_txtFrame->Name = L"liveCam1_txtFrame";
			this->liveCam1_txtFrame->Size = System::Drawing::Size(49, 20);
			this->liveCam1_txtFrame->TabIndex = 92;
			// liveCam1_txtFolderPath2
			this->liveCam1_txtFolderPath2->Location = System::Drawing::Point(426, 11);
			this->liveCam1_txtFolderPath2->Name = L"liveCam1_txtFolderPath2";
			this->liveCam1_txtFolderPath2->Size = System::Drawing::Size(340, 20);
			this->liveCam1_txtFolderPath2->TabIndex = 93;
			// display_rdB_channel1
			this->display_rdB_channel1->AutoSize = true;
			this->display_rdB_channel1->Checked = true;
			this->display_rdB_channel1->Location = System::Drawing::Point(1000, 14);
			this->display_rdB_channel1->Name = L"display_rdB_channel1";
			this->display_rdB_channel1->Size = System::Drawing::Size(73, 17);
			this->display_rdB_channel1->TabIndex = 94;
			this->display_rdB_channel1->TabStop = true;
			this->display_rdB_channel1->Text = L"Channel 1";
			this->display_rdB_channel1->UseVisualStyleBackColor = true;
			// display_rdB_channel2
			this->display_rdB_channel2->AutoSize = true;
			this->display_rdB_channel2->Location = System::Drawing::Point(1000, 44);
			this->display_rdB_channel2->Name = L"display_rdB_channel2";
			this->display_rdB_channel2->Size = System::Drawing::Size(73, 17);
			this->display_rdB_channel2->TabIndex = 95;
			this->display_rdB_channel2->Text = L"Channel 2";
			this->display_rdB_channel2->UseVisualStyleBackColor = true;
			// LiveCam1
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->ClientSize = System::Drawing::Size(1095, 462);
			this->Controls->Add(this->display_rdB_channel2);
			this->Controls->Add(this->display_rdB_channel1);
			this->Controls->Add(this->liveCam1_txtFolderPath2);
			this->Controls->Add(this->liveCam1_txtFrame);
			this->Controls->Add(this->labelTimePoints);
			this->Controls->Add(this->labelscanspertimepoint);
			this->Controls->Add(this->liveCam1_checkBoxRunExperiment);
			this->Controls->Add(this->liveCam1_checkBoxSaveSequenceOfImages);
			this->Controls->Add(this->liveCam1_progressBar);
			this->Controls->Add(this->liveCam1_btnStopLiveImaging);
			this->Controls->Add(this->liveCam1_btnLiveImaging);
			this->Controls->Add(this->liveCam1_btnSetFoldePath);
			this->Controls->Add(this->lblCam1LiveImageFolderPath);
			this->Controls->Add(this->liveCam1_txtFolderPath);
			this->Controls->Add(this->liveCam1_checkBoxSaveCapturedImage);
			this->Controls->Add(this->liveCam1_btnCapture);
			this->Margin = System::Windows::Forms::Padding(2);
			this->Name = L"LiveCam1";
			this->Text = L"LiveCam1";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: int GetSelectedChannel() {
		if (display_rdB_channel1->Checked == true)
				   return 1;
			   else if (display_rdB_channel2->Checked == true)
				   return 2;

			   return 1;
	}

	private: System::Void liveCam1_btnSetFoldePath_Click(System::Object^ sender, System::EventArgs^ e) {

		if (liveCam1_FolderDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			if (FolderWasSet == false)
			{
				liveCam1_btnCapture->Enabled = true;
				liveCam1_checkBoxSaveCapturedImage->Enabled = true;
				liveCam1_btnStopLiveImaging->Enabled = true;
				liveCam1_btnLiveImaging->Enabled = true;
				liveCam1_checkBoxSaveSequenceOfImages->Enabled = true;
				liveCam1_checkBoxRunExperiment->Enabled = true;

				liveCam1_txtFolderPath->Text = liveCam1_FolderDialog->SelectedPath;
				Folder = liveCam1_FolderDialog->SelectedPath;

				// Check if dualChannelAcquisition is true
				if (dualChannelAcquisition)
				{
					// Show second folder dialog if dualChannelAcquisition is true
					if (liveCam1_FolderDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
					{
						liveCam1_txtFolderPath2->Text = liveCam1_FolderDialog->SelectedPath;
						Folder2 = liveCam1_FolderDialog->SelectedPath;
					}
					else
					{
						liveCam1_txtFolderPath2->Text = "Error selecting second folder";
					}
				}
				else {
					liveCam1_txtFolderPath2->Enabled = false;
					display_rdB_channel2->Enabled = false;

				}
			}
			FolderWasSet = true;

		}

		else
		{
			liveCam1_txtFolderPath->Text = "Error selecting folder";
		}

	}

	private: System::Void liveCam1_btnCapture_Click(
		System::Object^ sender, System::EventArgs^ e)
	{
		if (liveCam1_backgroundWorker->IsBusy)
			return;

		if (LiCAM == 0)
			LiCAM = CameraFunctions::CAM1;

		if (LiCAM == 0)
			return;

		SIZE size = CameraFunctions::GetImageSize(LiCAM);
		DCAM_DATATYPE dataType = DCAM_DATATYPE_NONE;

		if (size.cx <= 0 || size.cy <= 0 ||
			!dcam_getdatatype(LiCAM, &dataType))
			return;

		if (dataType != DCAM_DATATYPE_UINT8 &&
			dataType != DCAM_DATATYPE_UINT16)
			return;

		bool sixteenBit = (dataType == DCAM_DATATYPE_UINT16);
		int low = BrightLevelMin;
		int high = BrightLevelMax;

		if (sixteenBit && high <= low)
			return;

		bool allocated = false;
		bool locked = false;
		Bitmap^ image = nullptr;
		System::Drawing::Imaging::BitmapData^ pixels = nullptr;
		Graphics^ graphics = nullptr;
		System::IO::FileStream^ output = nullptr;

		liveCam1_btnCapture->Enabled = false;
		liveCam1_btnLiveImaging->Enabled = false;

		try
		{
			// Capture one camera frame.
			if (!dcam_precapture(LiCAM, DCAM_CAPTUREMODE_SNAP) ||
				!dcam_allocframe(LiCAM, 1))
				return;

			allocated = true;
			DWORD eventCode = DCAM_EVENT_FRAMEEND;

			if (!dcam_capture(LiCAM) ||
				!dcam_wait(LiCAM, &eventCode, 2500, NULL) ||
				!dcam_idle(LiCAM))
				return;

			char* raw = nullptr;
			int32 rowBytes = 0;

			if (!dcam_lockdata(LiCAM, (void**)&raw, &rowBytes, -1))
				return;

			locked = true;

			if (raw == nullptr ||
				rowBytes < size.cx * (sixteenBit ? 2 : 1))
				return;

			// Convert into an 8-bit grayscale bitmap.
			image = FunctionsForImages::GetBitmapStructure(size.cx, size.cy);

			pixels = image->LockBits(
				System::Drawing::Rectangle(0, 0, size.cx, size.cy),
				System::Drawing::Imaging::ImageLockMode::WriteOnly,
				System::Drawing::Imaging::PixelFormat::Format8bppIndexed);

			unsigned char* destination =
				static_cast<unsigned char*>(pixels->Scan0.ToPointer());

			for (int y = 0; y < size.cy; y++)
			{
				const unsigned char* source =
					reinterpret_cast<const unsigned char*>(raw + y * rowBytes);

				unsigned char* target = destination + y * pixels->Stride;

				for (int x = 0; x < size.cx; x++)
				{
					int value = sixteenBit
						? reinterpret_cast<const unsigned short*>(source)[x]
						: source[x];

					if (sixteenBit)
						value = value <= low ? 0 : value >= high ? 255 :
						(value - low) * 255 / (high - low);

					target[x] = static_cast<unsigned char>(value);
				}
			}

			image->UnlockBits(pixels);
			pixels = nullptr;

			if (camera_rotated_90_degrees)
				image->RotateFlip(System::Drawing::RotateFlipType::Rotate90FlipNone);

			// Display the converted image.
			if (bitmap0 != nullptr)
				delete bitmap0;

			bitmap0 = image;
			image = nullptr;

			ImageWidth = bitmap0->Width;
			ImageHeight = bitmap0->Height;

			graphics = CreateGraphics();
			graphics->DrawImage(bitmap0, 0, 70);

			// Save without overwriting an earlier snapshot.
			Folder = liveCam1_txtFolderPath->Text;

			if (liveCam1_checkBoxSaveCapturedImage->Checked && Folder != "")
			{
				int number = 1;
				String^ path;

				do
				{
					path = System::IO::Path::Combine(
						Folder, String::Format("Snap_{0:D4}.tif", number++));
				} while (System::IO::File::Exists(path));

				output = gcnew System::IO::FileStream(
					path, System::IO::FileMode::CreateNew,
					System::IO::FileAccess::Write);

				bitmap0->Save(output, System::Drawing::Imaging::ImageFormat::Tiff);
			}
		}
		finally
		{
			if (output != nullptr)
				delete output;

			if (pixels != nullptr && image != nullptr)
				image->UnlockBits(pixels);

			if (image != nullptr)
				delete image;

			if (graphics != nullptr)
				delete graphics;

			if (allocated)
			{
				dcam_idle(LiCAM);

				if (locked)
					dcam_unlockdata(LiCAM);

				dcam_freeframe(LiCAM);
			}

			liveCam1_btnCapture->Enabled = true;
			liveCam1_btnLiveImaging->Enabled = true;
		}
	}

	private: System::Void liveCam1_btnLiveImaging_Click(System::Object^ sender, System::EventArgs^ e) {
		liveCam1_btnCapture->Enabled = false;;
		liveCam1_checkBoxSaveCapturedImage->Enabled = false;
		liveCam1_btnStopLiveImaging->Enabled = true;
		liveCam1_btnLiveImaging->Enabled = false;
		liveCam1_checkBoxSaveSequenceOfImages->Enabled = false;
		liveCam1_checkBoxRunExperiment->Enabled = false;

		g1 = CreateGraphics();

		liveCam1_backgroundWorker->RunWorkerAsync();
	}

	private: System::Void liveCam1_btnStopLiveImaging_Click(System::Object^ sender, System::EventArgs^ e) {
		liveCam1_backgroundWorker->CancelAsync();

		liveCam1_btnCapture->Enabled = true;
		liveCam1_checkBoxSaveCapturedImage->Enabled = true;

		LiveImagingOnLoopIsON = false;
		CameraFunctions::LiveImagingIsRunning = LiveImagingOnLoopIsON;

		liveCam1_btnStopLiveImaging->Enabled = false;
		liveCam1_btnLiveImaging->Enabled = true;

		liveCam1_checkBoxSaveSequenceOfImages->Enabled = true;
		liveCam1_checkBoxRunExperiment->Enabled = true;

		liveCam1_progressBar->Value = 0;
	}

	private: System::Void liveCam1_backgroundWorker_DoWork(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e) {

		if (LiCAM == 0)
			LiCAM = CameraFunctions::CAM1;

		if (camera_rotated_90_degrees == true)
		{
			SIZE ImageWidthAndHeight = CameraFunctions::GetImageSize(LiCAM);
			ImageWidth = ImageWidthAndHeight.cy;
			ImageHeight = ImageWidthAndHeight.cx;
		}
		else {
			SIZE ImageWidthAndHeight = CameraFunctions::GetImageSize(LiCAM);
			ImageWidth = ImageWidthAndHeight.cx;
			ImageHeight = ImageWidthAndHeight.cy;
		}

		if (ImageWidth <= 0 || ImageHeight <= 0)
		{
			e->Cancel = true;
			return;
		}
		else if (CameraFunctions::GetDataType(LiCAM) == "8-bit")
		{
			Image8Or16bit = 8;
			bitmap0 = FunctionsForImages::GetBitmapStructure(ImageWidth, ImageHeight);
			bitmap1 = FunctionsForImages::GetBitmapStructure(ImageWidth, ImageHeight);
		}
		else if (CameraFunctions::GetDataType(LiCAM) == "16-bit")
		{
			Image8Or16bit = 16;
			bitmap0 = FunctionsForImages::GetBitmapStructure(ImageWidth, ImageHeight);
			bitmap1 = FunctionsForImages::GetBitmapStructure(ImageWidth, ImageHeight);
		}

		// for HA window size variables
		double percHeightAdj = 0.25;
		int adjWindowWidth = static_cast<int>(floor(percHeightAdj * ImageWidth));
		int adjWindowCenter = static_cast<int>(floor(ImageWidth * 0.5));
		int adjWindowLeft = adjWindowCenter - static_cast<int>(floor(adjWindowWidth * 0.5));
		int adjWindowRight = adjWindowCenter + static_cast<int>(floor(adjWindowWidth * 0.5));
		adjWindowWidth = adjWindowRight - adjWindowLeft + 1;

		double* n_rec; //reciprocal of n! With n being the number of pixels in the height adjustment, 1 to adjWindowWidth
		n_rec = new double[adjWindowWidth];
		for (int i = 1; i <= adjWindowWidth; i++) {
			n_rec[i - 1] = 1.0f / i;
		}

		FunctionsForImages::adjWindowLeft = adjWindowLeft;
		FunctionsForImages::adjWindowRight = adjWindowRight;
		FunctionsForImages::n_rec = n_rec;

		//================ maximum projection

		typedef System::Drawing::Rectangle Recta; // Keep your typedef
		FunctionsForImages^ imgFunctions = gcnew FunctionsForImages();

		// Flattened zProjBuffer size

		int* ZProjInterpolationTable;

		ImageReferenceHeight = ImageHeight / 2;

		//===== Calculate LUT ======

		LutCh1 = (char*)malloc(65536 * sizeof(char));
		LutCh2 = (char*)malloc(65536 * sizeof(char));

		for (int k = 0; k < 65536; k++)  //awe: Compute the LUT for two channels?
		{
			if (k < BrightLevelMin)
				LutCh1[k] = 0;
			else if (k > BrightLevelMax)
				LutCh1[k] = static_cast<char>(255);
			else
				LutCh1[k] = (int)(k - BrightLevelMin) * 255 / (BrightLevelMax - BrightLevelMin);
			if (dualChannelAcquisition) {
				if (k < BrightLevelMin_ch2)
					LutCh2[k] = 0;
				else if (k > BrightLevelMax_ch2)
					LutCh2[k] = static_cast<char>(255);
				else
					LutCh2[k] = (int)(k - BrightLevelMin_ch2) * 255 / (BrightLevelMax_ch2 - BrightLevelMin_ch2);
			}
		}
		//==========================================

		// camera play
		if (liveCam1_checkBoxRunExperiment->Checked == false)
		{
			int frameLimit = 5000; //Amount of frames that will be saved when experiment isn't running.

			int n = 1;
			int i = 0;

			framebyte = CameraFunctions::GetDataFrameBytes(LiCAM);

			CStack = new char* [frameLimit];

			if (dualChannelAcquisition) {

				CStack_ch2 = new char* [frameLimit];
				for (int a = 0; a < frameLimit; ++a)
				{
					CStack[a] = new char[framebyte];
					memset(CStack[a], 0, framebyte);

					CStack_ch2[a] = new char[framebyte];
					memset(CStack_ch2[a], 0, framebyte);
					noChannels = 2;
				}

				BufferForImage = new char[framebyte];
				memset(BufferForImage, 0, framebyte);

				BufferForImage_ch2 = new char[framebyte];
				memset(BufferForImage_ch2, 0, framebyte);
			}
			else {
				for (int a = 0; a < frameLimit; ++a)
				{
					CStack[a] = new char[framebyte];
					memset(CStack[a], 0, framebyte);
					noChannels = 1;
				}
				BufferForImage = new char[framebyte];
				memset(BufferForImage, 0, framebyte);
			}

			FunctionOK = CameraFunctions::PreCaptureSequenceMode(LiCAM);  //prepare for capuring images; sets camera to stable status; images are captured continuously in this mode
			WaitMode = 1000;		// for infinite timeout				//WaitMode = DCAM_WAIT_INFINITE;
			dw = 0 | DCAM_EVENT_FRAMEEND; // waits for frame to be recorded
			int imagenr = 0;

			void** primaryframes;

			int nPrimaryBuffer = 2;		//Only one buffer is enough for the frame rate I require, you must look at c16_burst_copy code from Hamamatsu SDK
			primaryframes = CameraFunctions::PrepareImagesBuffer(primaryframes, nPrimaryBuffer, framebyte);
			FunctionOK = CameraFunctions::CameraAttachBuffer(LiCAM, primaryframes, nPrimaryBuffer);

			FunctionOK = CameraFunctions::Capture(LiCAM); // to begin capturing
			int32 index, total;
			int AllFramesContinuouslyAcquired = 0;

			Frame = 0;

			int32	iNextcopy;
			for (iNextcopy = 0; iNextcopy < frameLimit; )
			{
				FunctionOK = CameraFunctions::CameraWaitingForImage(LiCAM, dw, WaitMode); // confirms capturing of image data completed; waits for 1ms
				CamIndexAndTotalframes = CameraFunctions::GetCameraFrameIndexAndTotalNumberOfFramesAcquired(LiCAM); //gets frame no. and no. of frames captured in total
				AllFramesContinuouslyAcquired = CamIndexAndTotalframes[1]; //no. of frames captured in total
				index = CamIndexAndTotalframes[0]; // frame no.
				total = AllFramesContinuouslyAcquired;

				liveCam1_backgroundWorker->ReportProgress(iNextcopy); // The percentage, from 0 to 100, of the background operation that is complete.

				if (total > frameLimit)
					total = frameLimit;

				if (index != -1 && total != -1)
				{
					int p = 1;

					if (dualChannelAcquisition) {
						while (p <= noChannels) {

							if (p == 1) {

								int32	index = (iNextcopy*2) % nPrimaryBuffer;
								int32	nCopyFrame = 1;

								memcpy(BufferForImage, primaryframes[index], framebyte* nCopyFrame);
								bitmap0 = FunctionsForImages::ExportTheFinalBitmapWithHeightAdjustment(LiCAM, Image8Or16bit, bitmap0, BufferForImage, CStack[iNextcopy], ImageReferenceHeight, ImageWidth, ImageHeight, framebyte, LutCh1);

							}
							else if (p == 2) {

								int32	index = (iNextcopy * 2 + 1) % nPrimaryBuffer;
								int32	nCopyFrame = 1;

								memcpy(BufferForImage_ch2, primaryframes[index], framebyte* nCopyFrame);
								bitmap1 = FunctionsForImages::ExportTheFinalBitmapWithHeightAdjustment(LiCAM, Image8Or16bit, bitmap1, BufferForImage_ch2, CStack_ch2[iNextcopy], ImageReferenceHeight, ImageWidth, ImageHeight, framebyte, LutCh2);
								iNextcopy += nCopyFrame;
							}

							int selectedChannel = GetSelectedChannel();

							// Display image based on combo box selection
							if (selectedChannel == 1 && p == 1) {
								g1->DrawImage(bitmap0, 0, 70);
							}
							else if (selectedChannel == 2 && p == 2) {
								g1->DrawImage(bitmap1, 0, 70);
							}

							p++;
						}

					}
					else {

						int32	index = iNextcopy % nPrimaryBuffer;
						int32	nCopyFrame = 1;

						memcpy(BufferForImage, primaryframes[index], framebyte* nCopyFrame);
						bitmap0 = FunctionsForImages::ExportTheFinalBitmapWithHeightAdjustment(LiCAM, Image8Or16bit, bitmap0, BufferForImage, CStack[iNextcopy], ImageReferenceHeight, ImageWidth, ImageHeight, framebyte, LutCh1);
						g1->DrawImage(bitmap0, 0, 70);

					}

				}

				FunctionOK = CameraFunctions::FreeCamera(LiCAM);
				FunctionOK = CameraFunctions::CameraReleaseBuffer(LiCAM);
				FunctionOK = CameraFunctions::CameraAttachBuffer(LiCAM, primaryframes, nPrimaryBuffer);

				if (liveCam1_backgroundWorker->CancellationPending) // stop live imaging
				{
					Folder = liveCam1_txtFolderPath->Text;

					if (liveCam1_checkBoxSaveSequenceOfImages->Checked == true && Folder != "")
					{
						FunctionOK = FunctionsForImages::HeightAdjustemntMeasurementsFile(Folder, ImageHeightAdjustmentPixelsDifference, i, n);
						FunctionOK = CameraFunctions::Save3DConvertedStack(CStack, 1, i, ImageWidth * ImageHeight, Folder, 0);
					}

					saveframestimes = false;		//Change this variable if you want to save the information about the Cam1TimeToDisplayBitmap and Cam1TimeForCaptureImageAndSaveBitmap
					FunctionOK = CameraFunctions::CameraIdle(LiCAM);
					FunctionOK = CameraFunctions::CameraReleaseBuffer(LiCAM);
					FunctionOK = CameraFunctions::FreeCamera(LiCAM);

					if (primaryframes != nullptr)
					{
						delete[] reinterpret_cast<char*>(primaryframes);
						primaryframes = nullptr;
					}

					for (int j = 0; j < frameLimit; ++j)
						delete[] CStack[j];

					delete[] CStack;
					delete g1;
					delete[] n_rec;

					e->Cancel = true;
					break;
				}

			}
		}
		else
		{

			//The program will wait here until the start experiment button is clicked in Image Acquisition tab or the Stop Live imaging button
			while (ExperimentStartedCam1 != true)
			{
				if (liveCam1_backgroundWorker->CancellationPending)
				{
					e->Cancel = true;
					delete[] n_rec;
					break;
				}
			}

			//////////////// Zprojection ///////////////////////
			// nearest pixel z-projection interpolation table (from original coordinates to interpolated coordinates)

			acquisitionAspectRatio = magnification * stepX / 6.5;
			maxXPointsOriginal = (int)(acquisitionAspectRatio * sqrt2 * ((double)(NumberOfScansPerTimePointCam1 - 1)) + (double)ImageHeight + 0.5); // zero based indexing, add 0.5 for rounding
			maxXPointsInterpolated = (int)(((double)maxXPointsOriginal) / sqrt2 + 0.5); // zero based indexing, add 0.5 for rounding

			ZProjInterpolationTable = (int*)malloc(maxXPointsOriginal * sizeof(int));

			//  element value -1 indicates that the the corresponding x coordinate (index) does not contribute to the z-projection
			for (int i = 0; i < maxXPointsOriginal; i++)
			{
				ZProjInterpolationTable[i] = -1;
			}

			// fill in the actual values
			for (double i = 0; i < (double)maxXPointsInterpolated; i++)
			{
				ZProjInterpolationTable[(int)(i * sqrt2 + 0.5)] = (int)i; // add 0.5 for rounding

			}
			// fill in points that were not filled before
			if (ZProjInterpolationTable[0] == -1)
				ZProjInterpolationTable[0] = 0;
			for (int i = 1; i < maxXPointsOriginal; i++) {
				if (ZProjInterpolationTable[i] == -1)
					ZProjInterpolationTable[i] = ZProjInterpolationTable[i - 1];
			}

			tranWidth = maxXPointsInterpolated;
			tranHeight = (int)(ImageWidth);

			if (this->InvokeRequired) {
				this->Invoke(gcnew Action(this, &LiveCam1::CreateAndShowZprojection));
			}
			else {
				CreateAndShowZprojection();
			}

			unsigned char* zProjBuffer;
			zProjBuffer = (unsigned char*)malloc(tranWidth * tranHeight);

			////////////////////////////////////////////////

			framebyte = CameraFunctions::GetDataFrameBytes(LiCAM);

			CStack = new char* [NumberOfScansPerTimePointCam1];
			CStack_ch2 = new char* [NumberOfScansPerTimePointCam1];

			for (int a = 0; a < NumberOfScansPerTimePointCam1; ++a)
			{
				CStack[a] = new char[framebyte];
				memset(CStack[a], 0, framebyte);

				CStack_ch2[a] = new char[framebyte];
				memset(CStack_ch2[a], 0, framebyte);

			}
			BufferForImage = new char[framebyte];
			memset(BufferForImage, 0, framebyte);

			BufferForImage_ch2 = new char[framebyte];
			memset(BufferForImage_ch2, 0, framebyte);

			if (dualChannelAcquisition) {

				noChannels = 2;

			}
			else {

				noChannels = 1;

			}

			FunctionOK = CameraFunctions::PreCaptureSequenceMode(LiCAM);
			WaitMode = 1000;
			dw = 0 | DCAM_EVENT_FRAMEEND;
			int imagenr = 0;
			CurrentTimePoint = 1;

			void** primaryframes;

			int nPrimaryBuffer = 2;		//Only one buffer is enough for the frame rate I require, you must look at c16_burst_copy code from Hamamatsu SDK
			primaryframes = CameraFunctions::PrepareImagesBuffer(primaryframes, nPrimaryBuffer, framebyte);
			FunctionOK = CameraFunctions::CameraAttachBuffer(LiCAM, primaryframes, nPrimaryBuffer);

			FunctionOK = CameraFunctions::Capture(LiCAM);
			int32 index, total, nCopyFrame;

			/////////////// write z- projection file/////////////////////////////////

			FileInfo^ fileInfo = gcnew FileInfo(Folder + "\\zProj.raw");
			FileStream^ fileStream = fileInfo->Open(FileMode::Create, FileAccess::Write, FileShare::None);
			BinaryWriter^ writer = gcnew BinaryWriter(fileStream);

			////////////////////// write info file ////////////////////////////////////////

			StreamWriter^ infoWriter;
			FileInfo^ infoFile = gcnew FileInfo(Folder + "\\_info.txt");
			FileStream^ infoSave = infoFile->Open(FileMode::Append, FileAccess::Write, FileShare::Read);
			infoWriter = gcnew StreamWriter(infoSave);
			infoWriter->WriteLine(" ");
			infoWriter->WriteLine("\t\t {0} --- Information from Camera Images!", DateTime::Now);

			infoWriter->WriteLine("Camera Model: " + CameraFunctions::Camera1Model);
			infoWriter->WriteLine("Camera Serial Number: " + CameraFunctions::Camera1SN);
			infoWriter->WriteLine("Camera Connection: " + CameraFunctions::Camera1ConnectionType);
			infoWriter->WriteLine(" ");

			infoWriter->WriteLine("Camera Trigger mode: " + CameraFunctions::Camera1TriggerMode);
			infoWriter->WriteLine("Camera Trigger mode polarity: " + CameraFunctions::Camera1TriggerModePolarity);
			if (CameraFunctions::Camera1TriggerMode == "Internal")
				infoWriter->WriteLine("Camera exposure (ms): {0}", CameraFunctions::Cam1Exposure);
			else {
				infoWriter->WriteLine("Camera exposure is dependent on scanner rate.");
				infoWriter->WriteLine("Single scan time: {0}", sst);
				infoWriter->WriteLine("travel time: {0}", travelTime);
			}

			infoWriter->WriteLine(" ");
			infoWriter->WriteLine(" ");
			infoWriter->WriteLine(" ");
			infoWriter->WriteLine(" ");

			infoWriter->WriteLine("Step X: {0} um \n", stepX);
			infoWriter->WriteLine("Magnification: {0}\n", magnification);
			infoWriter->WriteLine("No Steps: {0}\n", NumberOfScansPerTimePointCam1);
			infoWriter->WriteLine("Recording type: Z-{0}bit\n ", Image8Or16bit);
			infoWriter->WriteLine("Image Setting:");
			infoWriter->WriteLine("Width: {0}", tranWidth);
			infoWriter->WriteLine("Height: {0}", tranHeight);
			infoWriter->WriteLine("Org Width: {0}", ImageWidth);
			infoWriter->WriteLine("Org Height: {0}", ImageHeight);
			infoWriter->WriteLine("{0} Dark level: {1}, Bright level: {2}, Height Adjustment level: {3}\n", DateTime::Now.ToString("HH:mm:ss.fff"), BrightLevelMin, BrightLevelMax, ImageReferenceHeight);

			// Flush the writer to ensure data is written to the file
			infoWriter->Flush();

			//////////////////////////////start experiment ////////////////////////////

			CameraIsReadyToStartExperiment = true;
			while (CurrentTimePoint <= NumberOfTimePointsCam1 && ExperimentStartedCam1 == true) // TP loop
			{
				int iNextcopy = 0;
				liveCam1_backgroundWorker->ReportProgress(0, CurrentTimePoint);

				memset(zProjBuffer, 0, tranWidth * tranHeight); // Initialize zProjBuffer to zero

				infoWriter->WriteLine("\\\\\\\\\\\\");
				infoWriter->WriteLine("{0} Scan Ready for time point: {1}", DateTime::Now.ToString("HH:mm:ss.fff"), CurrentTimePoint);

				//start time point collection

				while (iNextcopy < NumberOfScansPerTimePointCam1 && ExperimentStartedCam1 == true) // #scans loop
				{
					CamIndexAndTotalframes = CameraFunctions::GetCameraFrameIndexAndTotalNumberOfFramesAcquired(LiCAM);
					index = CamIndexAndTotalframes[0];
					total = CamIndexAndTotalframes[1];

					if (total > NumberOfScansPerTimePointCam1)
						total = NumberOfScansPerTimePointCam1;

					if (index != -1 && total != -1)
					{
						AcquireNextTimePoint = false;
						int p = 1;

						if (dualChannelAcquisition) {
							while (p <= noChannels) {

								if (CameraFunctions::CameraWaitingForImage(LiCAM, dw, WaitMode))
								{
									if (p == 1) {

										index = (iNextcopy * 2) % nPrimaryBuffer;
										nCopyFrame = 1;

										memcpy(BufferForImage, primaryframes[index], framebyte * nCopyFrame);
										bitmap0 = FunctionsForImages::ExportTheFinalBitmapWithHeightAdjustment(LiCAM, Image8Or16bit, bitmap0, BufferForImage, CStack[iNextcopy], ImageReferenceHeight, ImageWidth, ImageHeight, framebyte, LutCh1);

										//Image saved with correct bright levels
										if (HA_channel == 1) {
											ImageHeightAdjustmentPixelsDifference[iNextcopy] = FunctionsForImages::ShiftFromReferenceHeight;			//Saving pixels to adjust for surface focusing
										}

									}
									else if (p == 2) {

										index = (iNextcopy * 2 + 1) % nPrimaryBuffer;
										nCopyFrame = 1;

										memcpy(BufferForImage_ch2, primaryframes[index], framebyte * nCopyFrame);
										bitmap1 = FunctionsForImages::ExportTheFinalBitmapWithHeightAdjustment(LiCAM, Image8Or16bit, bitmap1, BufferForImage_ch2, CStack_ch2[iNextcopy], ImageReferenceHeight, ImageWidth, ImageHeight, framebyte, LutCh2);

										//Image saved with correct bright levels

										if (HA_channel == 2) {
											ImageHeightAdjustmentPixelsDifference[iNextcopy] = FunctionsForImages::ShiftFromReferenceHeight;			//Saving pixels to adjust for surface focusing
										}

										iNextcopy += nCopyFrame;
										liveCam1_backgroundWorker->ReportProgress(iNextcopy, CurrentTimePoint);
									}

									int selectedChannel = GetSelectedChannel();

									// Display image based on combo box selection
									if (selectedChannel == 1 && p == 1) {
										g1->DrawImage(bitmap0, 0, 70);
									}
									else if (selectedChannel == 2 && p == 2) {
										g1->DrawImage(bitmap1, 0, 70);
									}

									p++;
								}

								else {
									if (iNextcopy == 0)
									{
										infoWriter->WriteLine("{0} Waiting for image: {1}", DateTime::Now.ToString("HH:mm:ss.fff"), CurrentTimePoint);

										iNextcopy = 0;
									}
									else {
										infoWriter->WriteLine("{0} Last frame of timepoint {1} is: {2}", DateTime::Now.ToString("HH:mm:ss.fff"), CurrentTimePoint, iNextcopy);

										iNextcopy = NumberOfScansPerTimePointCam1 + 1;
										p = 3;

									}

								}
							}

						}
						else {
							if (CameraFunctions::CameraWaitingForImage(LiCAM, dw, WaitMode)) {
								index = iNextcopy % nPrimaryBuffer;
								nCopyFrame = 1;

								memcpy(BufferForImage, primaryframes[index], framebyte * nCopyFrame);
								bitmap0 = FunctionsForImages::ExportTheFinalBitmapWithHeightAdjustment(LiCAM, Image8Or16bit, bitmap0, BufferForImage, CStack[iNextcopy], ImageReferenceHeight, ImageWidth, ImageHeight, framebyte, LutCh1);

								g1->DrawImage(bitmap0, 0, 70);

								// Image saved with correct bright levels
								ImageHeightAdjustmentPixelsDifference[iNextcopy] = FunctionsForImages::ShiftFromReferenceHeight;

								iNextcopy += nCopyFrame;
								liveCam1_backgroundWorker->ReportProgress(iNextcopy, CurrentTimePoint);
							}
							else {
								if (iNextcopy == 0) {
									infoWriter->WriteLine("{0} Waiting for image: {1}", DateTime::Now.ToString("HH:mm:ss.fff"), CurrentTimePoint);
									iNextcopy = 0;
								}
								else {
									infoWriter->WriteLine("{0} Last frame of timepoint {1} is: {2}", DateTime::Now.ToString("HH:mm:ss.fff"), CurrentTimePoint, iNextcopy);
									iNextcopy = NumberOfScansPerTimePointCam1 + 1;
								}
							}
						}

					}

					if (liveCam1_backgroundWorker->CancellationPending)
					{
						CameraIsReadyToStartExperiment = false;
						e->Cancel = true;
						break;

						infoWriter->Close();
						infoSave->Close();
					}

					ScannerIsRunning;
				} // frame capturing end

				infoWriter->WriteLine("{0} Acquisition Done, Start Saving: {1}", DateTime::Now.ToString("HH:mm:ss.fff"), CurrentTimePoint);

				///////////////// start Z projection /////////////////////////
				if (ExperimentStartedCam1 == true)
				{

					infoWriter->WriteLine("{0} Start Zproj calculations: {1}", DateTime::Now.ToString("HH:mm:ss.fff"), CurrentTimePoint);

					int currentImageNoz = 0;
					while (currentImageNoz < NumberOfScansPerTimePointCam1)
					{
						// Get the number of processors in this system
						int iCPU = omp_get_num_procs();
						// Now set the number of threads
						omp_set_num_threads((iCPU));

#pragma omp parallel
						{
#pragma omp for schedule(guided,52) nowait
							for (int o = 0; o <= ImageHeight - 1; o++)
							{
								int xInd = ZProjInterpolationTable[(int)(currentImageNoz * (acquisitionAspectRatio * sqrt2)) + (ImageHeight - o - 1)];

								for (int p = 0; p < ImageWidth; ++p) {
									int result8bitZ = static_cast<unsigned char>(CStack[currentImageNoz][p + o * ImageWidth]);

									// use look up table to get interpolated image
									int ZProjInd = xInd + tranWidth * (tranHeight - (p + 1));
									zProjBuffer[ZProjInd] = (zProjBuffer[ZProjInd] < result8bitZ) ? result8bitZ : zProjBuffer[ZProjInd];
								}
							}
						}

						currentImageNoz++;
					}

					for (int y = 0; y < tranHeight; ++y)
					{
						for (int x = 0; x < tranWidth; ++x)
						{
							// Calculate the index in the zProjBuffer
							int index = x + y * tranWidth;
							writer->Write(zProjBuffer[index]);
						}
					}

					// Convert zProjBuffer to a Bitmap
					Bitmap^ bitmapZ =FunctionsForImages::GetBitmapStructure(tranWidth, tranHeight);

					// Lock the bitmap bits
					Recta rect(0, 0, tranWidth, tranHeight);
					BitmapData^ bitmapData = bitmapZ->LockBits(rect, ImageLockMode::ReadWrite, Imaging::PixelFormat::Format8bppIndexed);
					IntPtr ptrZ = bitmapData->Scan0; // Use ptrZ for the address of the first line

					// Use the pointer to write the zProjBuffer data directly to the bitmap
					unsigned char* p = static_cast<unsigned char*>(ptrZ.ToPointer());

					for (int y = 0; y < tranHeight; ++y)
					{
						for (int x = 0; x < tranWidth; ++x)
						{
							int index = x + y * tranWidth;
							p[x + y * bitmapData->Stride] = zProjBuffer[index];
						}
					}

					bitmapZ->UnlockBits(bitmapData);
					try
					{
						if (this->zprojection != nullptr && !this->zprojection->IsDisposed)
						{
							this->zprojection->UpdateZprojectionImage(bitmapZ);
						}
					}
					finally
					{
						delete bitmapZ;
					}
				}
				
				///////////////// Z projection finish. Save the data///////////////////////////////////

				FunctionOK = FunctionsForImages::HeightAdjustemntMeasurementsFile(Folder, ImageHeightAdjustmentPixelsDifference, iNextcopy, CurrentTimePoint); //Saving Heights in the correct file

				FunctionOK = CameraFunctions::Save3DConvertedStack(CStack, 1, iNextcopy, ImageWidth * ImageHeight, Folder, CurrentTimePoint);  //saving the captured image

				if (dualChannelAcquisition) {
					FunctionOK = CameraFunctions::Save3DConvertedStack(CStack_ch2, 1, iNextcopy, ImageWidth * ImageHeight, Folder2, CurrentTimePoint);  //saving the captured image
				}

				infoWriter->WriteLine("{0} Saving Done, Clearing Buffers {1}", DateTime::Now.ToString("HH:mm:ss.fff"), CurrentTimePoint);

				FunctionOK = CameraFunctions::CameraIdle(LiCAM);				// aborts acquisition. This sets the camera from busy state to ready state
				FunctionOK = CameraFunctions::CameraReleaseBuffer(LiCAM);		// releases attachedbuffer from DCAM
				FunctionOK = CameraFunctions::FreeCamera(LiCAM);				// frees the image buffer

				infoWriter->WriteLine("{0} Buffers Cleared {1}", DateTime::Now.ToString("HH:mm:ss.fff"), CurrentTimePoint);

				if (liveCam1_backgroundWorker->CancellationPending)
				{
					CameraIsReadyToStartExperiment = false;

					infoWriter->Close();
					infoSave->Close();

					writer->Close();
					fileStream->Close();

					if (primaryframes != nullptr)
					{
						delete[] reinterpret_cast<char*>(primaryframes);
						primaryframes = nullptr;
					}

					for (int j = 0; j < NumberOfScansPerTimePointCam1; ++j) {
						delete[] CStack[j];
					}

					if (dualChannelAcquisition) {
						delete[] BufferForImage_ch2;

						for (int j = 0; j < NumberOfScansPerTimePointCam1; ++j) {
							delete[] CStack_ch2[j];
						}
					}

					e->Cancel = true;

					delete[] BufferForImage;
					std::free(zProjBuffer);
					zProjBuffer = nullptr;
					delete[] n_rec;
					break;
				}

				FunctionOK = CameraFunctions::PreCaptureSequenceMode(LiCAM);										//prepare for capturing images
				primaryframes = CameraFunctions::PrepareImagesBuffer(primaryframes, nPrimaryBuffer, framebyte);		//specify a image buffer , not a DCAM function
				FunctionOK = CameraFunctions::CameraAttachBuffer(LiCAM, primaryframes, nPrimaryBuffer);				//DCAM module assigns user specified memory as image capturing buffer

				FunctionOK = CameraFunctions::Capture(LiCAM);														// start capturing images

  				CurrentTimePoint++;

				infoWriter->WriteLine("{0} buffer attached for next image {1}\n", DateTime::Now.ToString("HH:mm:ss.fff"), CurrentTimePoint);
				infoWriter->Flush();

				AcquireNextTimePoint = true;

			} // TP capturing while loop end

			FunctionOK = CameraFunctions::CameraIdle(LiCAM);
			FunctionOK = CameraFunctions::CameraReleaseBuffer(LiCAM);
			FunctionOK = CameraFunctions::FreeCamera(LiCAM);

			CameraIsReadyToStartExperiment = false;

			writer->Close();
			fileStream->Close();

			if (primaryframes != nullptr)
			{
				delete[] reinterpret_cast<char*>(primaryframes);
				primaryframes = nullptr;
			}

			for (int j = 0; j < NumberOfScansPerTimePointCam1; ++j)
				delete[] CStack[j];

			delete[] CStack;

			if (dualChannelAcquisition) {

				for (int j = 0; j < NumberOfScansPerTimePointCam1; ++j) {
					delete[] CStack_ch2[j];
				}

				delete[] CStack_ch2;
			}

			delete[] BufferForImage;
			std::free(zProjBuffer);
			zProjBuffer = nullptr;
			delete[] n_rec;
			delete g1;

		}
	}

	private: void CreateAndShowZprojection() {
		if (this->zprojection == nullptr || this->zprojection->IsDisposed) {
			this->zprojection = gcnew Zprojection(tranWidth, tranHeight);
			this->zprojection->Show();
		}
	}

	private: System::Void liveCam1_backgroundWorker_ProgressChanged(System::Object^ sender, System::ComponentModel::ProgressChangedEventArgs^ e) {

		if (e->UserState == nullptr)
			return;

		Frame = e->ProgressPercentage;

		liveCam1_progressBar->Value = 0;
		liveCam1_progressBar->Maximum = NumberOfScansPerTimePointCam1;
		liveCam1_progressBar->Value = Frame;

		labelscanspertimepoint->Text =
			"Scans: " + Frame.ToString() + "/" +
			NumberOfScansPerTimePointCam1.ToString();

	}

	private: System::Void liveCam1_backgroundWorker_RunWorkerCompleted(System::Object^ sender, System::ComponentModel::RunWorkerCompletedEventArgs^ e) {
		liveCam1_progressBar->Value = 0;
		labelscanspertimepoint->Text = "Scans: " + liveCam1_progressBar->Value.ToString() + "/" + NumberOfScansPerTimePointCam1.ToString();

		liveCam1_btnCapture->Enabled = true;
		liveCam1_checkBoxSaveCapturedImage->Enabled = true;

		LiveImagingOnLoopIsON = false;
		CameraFunctions::LiveImagingIsRunning = LiveImagingOnLoopIsON;

		liveCam1_btnStopLiveImaging->Enabled = false;
		liveCam1_btnLiveImaging->Enabled = true;

		liveCam1_checkBoxSaveSequenceOfImages->Enabled = true;
		liveCam1_checkBoxRunExperiment->Enabled = true;

	}
};
}
