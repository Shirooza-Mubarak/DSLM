#pragma once
#include"CameraFiles/Cameras.h"
#include"StageFiles/Stage.h"
#include"ScannerFiles/ScanHead.h"
#include"LiveCam1.h"
#include"ImageSizeForm.h"
#include<cmath>
#include"StageFiles/PI_GCS2_DLL.h"
#include <memory>
#include <vector>
#include "stdafx.h"
#include "stdio.h"
#include "string.h"

namespace MyDSLM {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Diagnostics;
	using namespace System::IO;

	/// <summary>
	/// Summary for MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{

			InitializeComponent();
			this->Shown += gcnew System::EventHandler(this, &MainForm::MainForm_Shown);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		//-------------Camera Variables-------------//
		int NrOfCameras; //Variable to store the number of cameras connected
		HDCAM FCAM1; //variable to store the ID of camera 1, this is referenced in most functions
		BOOL CameraFunctionOK; //variable to check if the camera function completed successfully or not
		BOOL ninety_degree_rotation = false;

		//-------------Stage Variables-------------//
		BOOL StageFunctionsOK; //Variable to check if the stage function completed successfully or not
		String^ ErrorFromStageFunctions; //variable to store error string if the function fails

		//Controller Boxes Information//
		int NrOfAxisControllers; //variable to store the number of controllers connected to the computer
		String^ ControllerBox1;	//variable to store the name of controller box 1
		String^ ControllerBox2; //variable to store the name of controller box 2
		int ControllerBox1ID; //variable to store the ID of controller box 1
		int ControllerBox2ID;//variable to store the ID of controller box 2
		static char *XAxis = "1"; //In controller box 2, the X Axis is connected in the Motor&Sensor 1
		static char *YAxis = "2"; //In controller box 2, the Y Axis is connected in the Motor&Sensor 2
		char *ZAxis = "1"; //In controller box 1, the Z Axis is connected in the Motor&Sensor 1

		//Axis Information//
		double XAxisVelocity; //variable to store to velocity of the X axis stage
		double YAxisVelocity; //variable to store to velocity of the Y axis stage
		double ZAxisVelocity; //variable to store to velocity of the Z axis stage
		double AxisPosition; //variable to store the position of the axis, one variable can be used for all axes
		double StepSize; //variable to store the size of the axes when moving axis one by one
		double XPos; //variable to store x position
		double ZPos; //variable to store Y position
		double YPos; //variable to store Z position
		int n_XPositions = 5; //variable to to store number of X positions
		array<double>^ XPositions = gcnew array<double>(n_XPositions); //list of X positions 
		array<double>^ ZPositions = gcnew array<double>(n_XPositions); //list of Z positions 
		array<double>^ YPositions = gcnew array<double>(2); //list of Y positions, only 2 positions required
		int Yloop; //Number of loops required if using 2 different Y positions

		double XStepSize;  //variable to store the step size entered by user
		int NumberOfScansPerTimePoint;	//variable to store the number of scans performed in each time point
		array<double>^ ZStepSize = gcnew array<double>(4); //array to store the Z step size in each region
		
		//-------------Scanner Variables-------------//
		BOOL ScannerFunctionOK;	//Variable to store if function is valid
		static array <short>^ ScannerMirrorsCurrentPos = gcnew array <short>(2);	//Array to store current position of mirrors (x,y)
		double MarkTime; //variable to store the mark time of the scanner
		double JumpTime; //variable to store the jump time of the scanner
		double DelayTime; //variable to store the jump delay time of the scanner
		short X1para; //variable to store the x position of the mark segment of the signle scan
		short X2para; //variable to store the Y position of the mark segment of the signle scan
		short Y1para; //variable to store the x position of the jump segment of the signle scan
		short Y2para; //variable to store the Y position of the jump segment of the signle scan
		static int WaitScan; //variable to store the wait time between scans
		short InitPosX; //variable to store the initial X position of the scanner
		short InitPosY; //variable to store the initial Y position of the scanner
		int gain;
		BOOL ExternalTrigger = false; //Variable to hold if using external trigger (default false)

		//-------------Live Image Variables-------------//
		static unsigned char* LookUpTable; //Variable to hold the look up table every time the bright levels, min and or max, are changed
		static int Cam1BrightLevelMin = 0; //Variable to indicate the minimum value for image bright in a scale of 0-65535, Camera 1
		static int Cam1BrightLevelMax = 0; //Variable to indicate the maximum value for image bright in a scale of 0-65535, Camera 1
		static int Cam1BrightLevelMin_ch2 = 0; //Variable to indicate the minimum value for image bright in a scale of 0-65535, Camera 1
		static int Cam1BrightLevelMax_ch2 = 0; //Variable to indicate the maximum value for image bright in a scale of 0-65535, Camera 1

		static int ImageRefHeightValue = 200; //Value to be considered for height adjustment calculations

		//-------------Experiment Setup Variables-------------//
		int NumberOfTimePoints; //Number of Time Points to be acquired during the experiment
		int WaitingTimeBetweenTimePoints; //Variable with user input value for resting time between time points
		bool ExperimentStarted = false; //Variable to send to Form2 and Form3 to indicate the start of the experiment
		bool scanningcont; //Boolean variable to state if continuous scanning is running or not
		int numberofscans = 0; //Variable to indicate the number of scans performed by scanner while running
		String^ Folder; //String to save folder path
		static array <int>^ TimeForStepPlusScan; //Variable to hold the time measured between stage step + scan
		
		//-------------Height Adjustment Variables-------------//
		array <int>^ HeightAdjustSection = gcnew array<int>(4); //Array to store height adjustment average of region

private: System::Windows::Forms::ToolStrip^ stripLiveCameras;
private: System::Windows::Forms::ToolStripButton^ camera1_btnLiveImage;

	//-------Stage members -----//

private: System::Windows::Forms::Label^ lblStageInfo;

private: System::ComponentModel::BackgroundWorker^ RunWorker;

private: System::ComponentModel::BackgroundWorker^ background_StartExperiment;
private: System::Windows::Forms::TabPage^ ExperimentTab;
private: System::Windows::Forms::NumericUpDown^ experiment_numSST;
private: System::Windows::Forms::Label^ label_SingleScanTime;
private: System::Windows::Forms::NumericUpDown^ ExperimentSetup_travelTime;
private: System::Windows::Forms::NumericUpDown^ ExperimentSetup_NoOfSteps;
private: System::Windows::Forms::Label^ label_travelTime;
private: System::Windows::Forms::Label^ label_NoOfScans;
private: System::Windows::Forms::TextBox^ experiment_txtActionBox;
private: System::Windows::Forms::TextBox^ experiment_txtInfoBox;

private: System::Windows::Forms::NumericUpDown^ experiment_nudWaitingTime;
private: System::Windows::Forms::Label^ labelWaitingTime;
private: System::Windows::Forms::Label^ labelscansperformed;
private: System::Windows::Forms::ProgressBar^ experiment_progressBarNumberOfScansPerTimePoint;
private: System::Windows::Forms::Label^ labeltimepointsperformed;
private: System::Windows::Forms::ProgressBar^ experiment_progressBarExperiment;
private: System::Windows::Forms::NumericUpDown^ experiment_numNumberOfTimePointsValue;
private: System::Windows::Forms::Label^ labelNumberOfTimePoints;
private: System::Windows::Forms::Button^ experiment_btnStopExperiment;
private: System::Windows::Forms::Button^ experiment_btnStartExperiment;
private: System::Windows::Forms::Button^ experiment_btnSetupExperiment;
private: System::Windows::Forms::Label^ Noteslabel;
private: System::Windows::Forms::TabPage^ StageTab;

private: System::Windows::Forms::TextBox^ stage_txtInfoBox;
private: System::Windows::Forms::TextBox^ stage_txtZVelocity;
private: System::Windows::Forms::TextBox^ stage_txtXPosition;
private: System::Windows::Forms::TextBox^ stage_txtYVelocity;
private: System::Windows::Forms::TextBox^ stage_txtZPosition;
private: System::Windows::Forms::TextBox^ stage_txtXVelocity;
private: System::Windows::Forms::TextBox^ stage_txtYPosition;
private: System::Windows::Forms::TextBox^ stage_txtActionBox;
private: System::Windows::Forms::GroupBox^ groupExperiementSetup;
private: System::Windows::Forms::Button^ stage_btnSetY1;
private: System::Windows::Forms::NumericUpDown^ stage_nudY1Stage;
private: System::Windows::Forms::NumericUpDown^ stage_nudY2Stage;
private: System::Windows::Forms::Label^ label2;
private: System::Windows::Forms::NumericUpDown^ stage_zLimit;
private: System::Windows::Forms::Label^ stage_zlimit_text;


private: System::Windows::Forms::Button^ stage_btnSetStageExpParams;
private: System::Windows::Forms::NumericUpDown^ stage_numObjectiveMagnification;
private: System::Windows::Forms::NumericUpDown^ stage_numWaveXStep;
private: System::Windows::Forms::Button^ stage_btnSetY2;
private: System::Windows::Forms::Button^ stage_btnSetX4Z4;
private: System::Windows::Forms::Button^ stage_btnSetX3Z3;
private: System::Windows::Forms::Button^ stage_btnSetX2Z2;
private: System::Windows::Forms::Button^ stage_btnSetX1Z1;
private: System::Windows::Forms::Button^ stage_btnSetX0Z0;
private: System::Windows::Forms::Label^ lblY1;
private: System::Windows::Forms::Label^ lblX4Z4;
private: System::Windows::Forms::Label^ lblX3Z3;
private: System::Windows::Forms::Label^ lblX2Z2;
private: System::Windows::Forms::Label^ lblObjective;
private: System::Windows::Forms::Label^ lblX1Z1;
private: System::Windows::Forms::Label^ lblX0Z0;
private: System::Windows::Forms::Label^ lblWaveXStep;
private: System::Windows::Forms::TextBox^ stage_txtZ4Stage;
private: System::Windows::Forms::TextBox^ stage_txtX4Stage;
private: System::Windows::Forms::TextBox^ stage_txtZ3Stage;
private: System::Windows::Forms::TextBox^ stage_txtX3Stage;
private: System::Windows::Forms::TextBox^ stage_txtZ2Stage;
private: System::Windows::Forms::TextBox^ stage_txtX2Stage;
private: System::Windows::Forms::TextBox^ stage_txtZ1Stage;
private: System::Windows::Forms::TextBox^ stage_txtX1Stage;
private: System::Windows::Forms::TextBox^ stage_txtZ0Stage;
private: System::Windows::Forms::TextBox^ stage_txtX0Stage;
private: System::Windows::Forms::Button^ stage_btnInitialiseStage;
private: System::Windows::Forms::Button^ stage_btnLeft;
private: System::Windows::Forms::Button^ stage_btnSetZVelocity;
private: System::Windows::Forms::NumericUpDown^ stage_numXStep;
private: System::Windows::Forms::Button^ stage_btnSetYVelocity;
private: System::Windows::Forms::Label^ lblXStepSize;
private: System::Windows::Forms::Label^ lblZVelocity;
private: System::Windows::Forms::NumericUpDown^ stage_numZVelocity;
private: System::Windows::Forms::Label^ label1;
private: System::Windows::Forms::Button^ stage_btnDown;
private: System::Windows::Forms::Label^ lblZPosition;
private: System::Windows::Forms::Button^ stage_btnFront;
private: System::Windows::Forms::Label^ lblYVelocity;
private: System::Windows::Forms::NumericUpDown^ stage_numYVelocity;
private: System::Windows::Forms::NumericUpDown^ stage_numYStep;
private: System::Windows::Forms::Button^ stage_btnBack;
private: System::Windows::Forms::Label^ lblZStepSize;
private: System::Windows::Forms::Label^ lblYStepSize;
private: System::Windows::Forms::Label^ lblXVelocity;
private: System::Windows::Forms::NumericUpDown^ stage_numZStep;
private: System::Windows::Forms::NumericUpDown^ stage_numXVelocity;
private: System::Windows::Forms::Button^ stage_btnSetXVelocity;
private: System::Windows::Forms::Button^ stage_btnUp;
private: System::Windows::Forms::Label^ lblYPosition;
private: System::Windows::Forms::Button^ stage_btnRight;
private: System::Windows::Forms::TabPage^ CameraTab;
private: System::Windows::Forms::TextBox^ txt_camera_infoFile;
private: System::Windows::Forms::TextBox^ camera_ActionBox;
private: System::Windows::Forms::GroupBox^ groupBox2;
private: System::Windows::Forms::RadioButton^ rdB_HA_ch2;
private: System::Windows::Forms::RadioButton^ rdB_HA_ch1;
private: System::Windows::Forms::Label^ label5;
private: System::Windows::Forms::NumericUpDown^ nudReferenceHeight;
private: System::Windows::Forms::Button^ SetReferenceHighet;
private: System::Windows::Forms::Label^ lblReferenceHeight;
private: System::Windows::Forms::GroupBox^ groupBox1;

private: System::Windows::Forms::NumericUpDown^ nudBrightMax_ch2;
private: System::Windows::Forms::NumericUpDown^ nudBrightMin_ch2;
private: System::Windows::Forms::Label^ label4;
private: System::Windows::Forms::Label^ label3;
private: System::Windows::Forms::Label^ lblBrightMax;
private: System::Windows::Forms::NumericUpDown^ nudBrightMax_ch1;
private: System::Windows::Forms::Label^ lblBrightMin;
private: System::Windows::Forms::NumericUpDown^ nudBrightMin_ch1;
private: System::Windows::Forms::Button^ SetBright;
private: System::Windows::Forms::GroupBox^ GroupCamera1;
private: System::Windows::Forms::CheckBox^ Camera_90degreeRotation_checkBox;
private: System::Windows::Forms::NumericUpDown^ camera1_numCurrentTMExposure;
private: System::Windows::Forms::NumericUpDown^ camera1_numSetTMExposure;
private: System::Windows::Forms::Button^ camera1_btnSetTMInternalExposure;
private: System::Windows::Forms::Label^ lblCam1InternalExposure;
private: System::Windows::Forms::ComboBox^ camera1_comboTMPolarity;
private: System::Windows::Forms::ComboBox^ camera1_comboTM;
private: System::Windows::Forms::Label^ lblCam1TMPolarity;
private: System::Windows::Forms::Label^ lblCam1TriggerMode;
private: System::Windows::Forms::GroupBox^ groupCam1ImageSize;
private: System::Windows::Forms::Button^ camera1_btnImageSetSize;
private: System::Windows::Forms::TextBox^ camera1_txtImageYOffSet;
private: System::Windows::Forms::Label^ lblCam1ImageYOffSet;
private: System::Windows::Forms::TextBox^ camera1_txtImageYSize;
private: System::Windows::Forms::Label^ lblCam1ImageYSize;
private: System::Windows::Forms::TextBox^ camera1_txtImageXOffSet;
private: System::Windows::Forms::Label^ lblCam1ImageXOffSet;
private: System::Windows::Forms::TextBox^ camera1_txtImageXSize;
private: System::Windows::Forms::Label^ lblCam1ImageXSize;
private: System::Windows::Forms::Button^ camera1_btnSetImageDataBits;
private: System::Windows::Forms::ComboBox^ camera1_comboImageDataBits;
private: System::Windows::Forms::Label^ lblCam1ImageDataBits;
private: System::Windows::Forms::TextBox^ camera1_txtImageDataBits;
private: System::Windows::Forms::Button^ camera1_btnSetBitMapBits;
private: System::Windows::Forms::ComboBox^ camera1_comboBitMapBits;
private: System::Windows::Forms::Label^ lblCam1BitMapBits;
private: System::Windows::Forms::TextBox^ camera1_txtBitMapBitsType;
private: System::Windows::Forms::TextBox^ camera1_txtInfoBox;



















private: System::Windows::Forms::TabControl^ TabControls;
private: System::Windows::Forms::TabPage^ ScannerTab;
private: System::Windows::Forms::Button^ scanner_btnSetGain;
private: System::Windows::Forms::NumericUpDown^ scanner_numGain;
private: System::Windows::Forms::NumericUpDown^ scanner_numSetJumpDelay;
private: System::Windows::Forms::Label^ lblJumpDelay;
private: System::Windows::Forms::GroupBox^ GroupSingleScanCycleTime;
private: System::Windows::Forms::RadioButton^ scanner_radioScanJobDualChannel;
private: System::Windows::Forms::RadioButton^ scanner_radioScanJobSingleChannel;
private: System::Windows::Forms::TextBox^ scanner_txtCalculateSST;
private: System::Windows::Forms::Button^ scanner_btnSSTCalculate;
private: System::Windows::Forms::GroupBox^ GroupScanJob;
private: System::Windows::Forms::NumericUpDown^ scanner_numRepeat;
private: System::Windows::Forms::CheckBox^ Scanner_Loop_checkBox;
private: System::Windows::Forms::Label^ labelScannerRepeat;
private: System::Windows::Forms::Label^ labelScannerY2;
private: System::Windows::Forms::Label^ labelScannerY1;
private: System::Windows::Forms::Label^ labelScannerX2;
private: System::Windows::Forms::Label^ labelScannerX1;
private: System::Windows::Forms::NumericUpDown^ scanner_numX2;
private: System::Windows::Forms::NumericUpDown^ scanner_numX1;
private: System::Windows::Forms::NumericUpDown^ scanner_numY1;
private: System::Windows::Forms::NumericUpDown^ scanner_numY2;
private: System::Windows::Forms::Button^ scanner_btnRun;
private: System::Windows::Forms::Button^ scanner_btnStop;
private: System::Windows::Forms::TextBox^ scanner_txtActionBox;
private: System::Windows::Forms::TextBox^ scanner_txtInfoBox;
private: System::Windows::Forms::Label^ lblMarkSpeed;
private: System::Windows::Forms::Label^ lblJumpSpeed;
private: System::Windows::Forms::NumericUpDown^ scanner_numSetMarkTime;
private: System::Windows::Forms::NumericUpDown^ scanner_numSetJumpTime;
private: System::Windows::Forms::Button^ scanner_btnSetSpeed;
private: System::Windows::Forms::Button^ btnInitialiseScanner;
private: System::Windows::Forms::Button^ SetBright_ch2;



private:

internal:

//private: System::Windows::Forms::Button^ btn_measureVelocity;

private:

internal:

private: System::ComponentModel::IContainer^ components;

	   /// <summary>
		/// Required designer variable.
		/// </summary>

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->stripLiveCameras = (gcnew System::Windows::Forms::ToolStrip());
			this->camera1_btnLiveImage = (gcnew System::Windows::Forms::ToolStripButton());
			this->lblStageInfo = (gcnew System::Windows::Forms::Label());
			this->RunWorker = (gcnew System::ComponentModel::BackgroundWorker());
			this->background_StartExperiment = (gcnew System::ComponentModel::BackgroundWorker());
			this->ExperimentTab = (gcnew System::Windows::Forms::TabPage());
			this->experiment_numSST = (gcnew System::Windows::Forms::NumericUpDown());
			this->label_SingleScanTime = (gcnew System::Windows::Forms::Label());
			this->ExperimentSetup_travelTime = (gcnew System::Windows::Forms::NumericUpDown());
			this->ExperimentSetup_NoOfSteps = (gcnew System::Windows::Forms::NumericUpDown());
			this->label_travelTime = (gcnew System::Windows::Forms::Label());
			this->label_NoOfScans = (gcnew System::Windows::Forms::Label());
			this->experiment_txtActionBox = (gcnew System::Windows::Forms::TextBox());
			this->experiment_txtInfoBox = (gcnew System::Windows::Forms::TextBox());
			this->experiment_nudWaitingTime = (gcnew System::Windows::Forms::NumericUpDown());
			this->labelWaitingTime = (gcnew System::Windows::Forms::Label());
			this->labelscansperformed = (gcnew System::Windows::Forms::Label());
			this->experiment_progressBarNumberOfScansPerTimePoint = (gcnew System::Windows::Forms::ProgressBar());
			this->labeltimepointsperformed = (gcnew System::Windows::Forms::Label());
			this->experiment_progressBarExperiment = (gcnew System::Windows::Forms::ProgressBar());
			this->experiment_numNumberOfTimePointsValue = (gcnew System::Windows::Forms::NumericUpDown());
			this->labelNumberOfTimePoints = (gcnew System::Windows::Forms::Label());
			this->experiment_btnStopExperiment = (gcnew System::Windows::Forms::Button());
			this->experiment_btnStartExperiment = (gcnew System::Windows::Forms::Button());
			this->experiment_btnSetupExperiment = (gcnew System::Windows::Forms::Button());
			this->Noteslabel = (gcnew System::Windows::Forms::Label());
			this->StageTab = (gcnew System::Windows::Forms::TabPage());
			this->stage_txtInfoBox = (gcnew System::Windows::Forms::TextBox());
			this->stage_txtZVelocity = (gcnew System::Windows::Forms::TextBox());
			this->stage_txtXPosition = (gcnew System::Windows::Forms::TextBox());
			this->stage_txtYVelocity = (gcnew System::Windows::Forms::TextBox());
			this->stage_txtZPosition = (gcnew System::Windows::Forms::TextBox());
			this->stage_txtXVelocity = (gcnew System::Windows::Forms::TextBox());
			this->stage_txtYPosition = (gcnew System::Windows::Forms::TextBox());
			this->stage_txtActionBox = (gcnew System::Windows::Forms::TextBox());
			this->groupExperiementSetup = (gcnew System::Windows::Forms::GroupBox());
			this->stage_btnSetY1 = (gcnew System::Windows::Forms::Button());
			this->stage_nudY1Stage = (gcnew System::Windows::Forms::NumericUpDown());
			this->stage_nudY2Stage = (gcnew System::Windows::Forms::NumericUpDown());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->stage_zLimit = (gcnew System::Windows::Forms::NumericUpDown());
			this->stage_zlimit_text = (gcnew System::Windows::Forms::Label());
			this->stage_btnSetStageExpParams = (gcnew System::Windows::Forms::Button());
			this->stage_numObjectiveMagnification = (gcnew System::Windows::Forms::NumericUpDown());
			this->stage_numWaveXStep = (gcnew System::Windows::Forms::NumericUpDown());
			this->stage_btnSetY2 = (gcnew System::Windows::Forms::Button());
			this->stage_btnSetX4Z4 = (gcnew System::Windows::Forms::Button());
			this->stage_btnSetX3Z3 = (gcnew System::Windows::Forms::Button());
			this->stage_btnSetX2Z2 = (gcnew System::Windows::Forms::Button());
			this->stage_btnSetX1Z1 = (gcnew System::Windows::Forms::Button());
			this->stage_btnSetX0Z0 = (gcnew System::Windows::Forms::Button());
			this->lblY1 = (gcnew System::Windows::Forms::Label());
			this->lblX4Z4 = (gcnew System::Windows::Forms::Label());
			this->lblX3Z3 = (gcnew System::Windows::Forms::Label());
			this->lblX2Z2 = (gcnew System::Windows::Forms::Label());
			this->lblObjective = (gcnew System::Windows::Forms::Label());
			this->lblX1Z1 = (gcnew System::Windows::Forms::Label());
			this->lblX0Z0 = (gcnew System::Windows::Forms::Label());
			this->lblWaveXStep = (gcnew System::Windows::Forms::Label());
			this->stage_txtZ4Stage = (gcnew System::Windows::Forms::TextBox());
			this->stage_txtX4Stage = (gcnew System::Windows::Forms::TextBox());
			this->stage_txtZ3Stage = (gcnew System::Windows::Forms::TextBox());
			this->stage_txtX3Stage = (gcnew System::Windows::Forms::TextBox());
			this->stage_txtZ2Stage = (gcnew System::Windows::Forms::TextBox());
			this->stage_txtX2Stage = (gcnew System::Windows::Forms::TextBox());
			this->stage_txtZ1Stage = (gcnew System::Windows::Forms::TextBox());
			this->stage_txtX1Stage = (gcnew System::Windows::Forms::TextBox());
			this->stage_txtZ0Stage = (gcnew System::Windows::Forms::TextBox());
			this->stage_txtX0Stage = (gcnew System::Windows::Forms::TextBox());
			this->stage_btnInitialiseStage = (gcnew System::Windows::Forms::Button());
			this->stage_btnLeft = (gcnew System::Windows::Forms::Button());
			this->stage_btnSetZVelocity = (gcnew System::Windows::Forms::Button());
			this->stage_numXStep = (gcnew System::Windows::Forms::NumericUpDown());
			this->stage_btnSetYVelocity = (gcnew System::Windows::Forms::Button());
			this->lblXStepSize = (gcnew System::Windows::Forms::Label());
			this->lblZVelocity = (gcnew System::Windows::Forms::Label());
			this->stage_numZVelocity = (gcnew System::Windows::Forms::NumericUpDown());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->stage_btnDown = (gcnew System::Windows::Forms::Button());
			this->lblZPosition = (gcnew System::Windows::Forms::Label());
			this->stage_btnFront = (gcnew System::Windows::Forms::Button());
			this->lblYVelocity = (gcnew System::Windows::Forms::Label());
			this->stage_numYVelocity = (gcnew System::Windows::Forms::NumericUpDown());
			this->stage_numYStep = (gcnew System::Windows::Forms::NumericUpDown());
			this->stage_btnBack = (gcnew System::Windows::Forms::Button());
			this->lblZStepSize = (gcnew System::Windows::Forms::Label());
			this->lblYStepSize = (gcnew System::Windows::Forms::Label());
			this->lblXVelocity = (gcnew System::Windows::Forms::Label());
			this->stage_numZStep = (gcnew System::Windows::Forms::NumericUpDown());
			this->stage_numXVelocity = (gcnew System::Windows::Forms::NumericUpDown());
			this->stage_btnSetXVelocity = (gcnew System::Windows::Forms::Button());
			this->stage_btnUp = (gcnew System::Windows::Forms::Button());
			this->lblYPosition = (gcnew System::Windows::Forms::Label());
			this->stage_btnRight = (gcnew System::Windows::Forms::Button());
			this->CameraTab = (gcnew System::Windows::Forms::TabPage());
			this->txt_camera_infoFile = (gcnew System::Windows::Forms::TextBox());
			this->camera_ActionBox = (gcnew System::Windows::Forms::TextBox());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->rdB_HA_ch2 = (gcnew System::Windows::Forms::RadioButton());
			this->rdB_HA_ch1 = (gcnew System::Windows::Forms::RadioButton());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->nudReferenceHeight = (gcnew System::Windows::Forms::NumericUpDown());
			this->SetReferenceHighet = (gcnew System::Windows::Forms::Button());
			this->lblReferenceHeight = (gcnew System::Windows::Forms::Label());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->SetBright_ch2 = (gcnew System::Windows::Forms::Button());
			this->nudBrightMax_ch2 = (gcnew System::Windows::Forms::NumericUpDown());
			this->nudBrightMin_ch2 = (gcnew System::Windows::Forms::NumericUpDown());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->lblBrightMax = (gcnew System::Windows::Forms::Label());
			this->nudBrightMax_ch1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->lblBrightMin = (gcnew System::Windows::Forms::Label());
			this->nudBrightMin_ch1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->SetBright = (gcnew System::Windows::Forms::Button());
			this->GroupCamera1 = (gcnew System::Windows::Forms::GroupBox());
			this->Camera_90degreeRotation_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->camera1_numCurrentTMExposure = (gcnew System::Windows::Forms::NumericUpDown());
			this->camera1_numSetTMExposure = (gcnew System::Windows::Forms::NumericUpDown());
			this->camera1_btnSetTMInternalExposure = (gcnew System::Windows::Forms::Button());
			this->lblCam1InternalExposure = (gcnew System::Windows::Forms::Label());
			this->camera1_comboTMPolarity = (gcnew System::Windows::Forms::ComboBox());
			this->camera1_comboTM = (gcnew System::Windows::Forms::ComboBox());
			this->lblCam1TMPolarity = (gcnew System::Windows::Forms::Label());
			this->lblCam1TriggerMode = (gcnew System::Windows::Forms::Label());
			this->groupCam1ImageSize = (gcnew System::Windows::Forms::GroupBox());
			this->camera1_btnImageSetSize = (gcnew System::Windows::Forms::Button());
			this->camera1_txtImageYOffSet = (gcnew System::Windows::Forms::TextBox());
			this->lblCam1ImageYOffSet = (gcnew System::Windows::Forms::Label());
			this->camera1_txtImageYSize = (gcnew System::Windows::Forms::TextBox());
			this->lblCam1ImageYSize = (gcnew System::Windows::Forms::Label());
			this->camera1_txtImageXOffSet = (gcnew System::Windows::Forms::TextBox());
			this->lblCam1ImageXOffSet = (gcnew System::Windows::Forms::Label());
			this->camera1_txtImageXSize = (gcnew System::Windows::Forms::TextBox());
			this->lblCam1ImageXSize = (gcnew System::Windows::Forms::Label());
			this->camera1_btnSetImageDataBits = (gcnew System::Windows::Forms::Button());
			this->camera1_comboImageDataBits = (gcnew System::Windows::Forms::ComboBox());
			this->lblCam1ImageDataBits = (gcnew System::Windows::Forms::Label());
			this->camera1_txtImageDataBits = (gcnew System::Windows::Forms::TextBox());
			this->camera1_btnSetBitMapBits = (gcnew System::Windows::Forms::Button());
			this->camera1_comboBitMapBits = (gcnew System::Windows::Forms::ComboBox());
			this->lblCam1BitMapBits = (gcnew System::Windows::Forms::Label());
			this->camera1_txtBitMapBitsType = (gcnew System::Windows::Forms::TextBox());
			this->camera1_txtInfoBox = (gcnew System::Windows::Forms::TextBox());
			this->TabControls = (gcnew System::Windows::Forms::TabControl());
			this->ScannerTab = (gcnew System::Windows::Forms::TabPage());
			this->scanner_btnSetGain = (gcnew System::Windows::Forms::Button());
			this->scanner_numGain = (gcnew System::Windows::Forms::NumericUpDown());
			this->scanner_numSetJumpDelay = (gcnew System::Windows::Forms::NumericUpDown());
			this->lblJumpDelay = (gcnew System::Windows::Forms::Label());
			this->GroupSingleScanCycleTime = (gcnew System::Windows::Forms::GroupBox());
			this->scanner_radioScanJobDualChannel = (gcnew System::Windows::Forms::RadioButton());
			this->scanner_radioScanJobSingleChannel = (gcnew System::Windows::Forms::RadioButton());
			this->scanner_txtCalculateSST = (gcnew System::Windows::Forms::TextBox());
			this->scanner_btnSSTCalculate = (gcnew System::Windows::Forms::Button());
			this->GroupScanJob = (gcnew System::Windows::Forms::GroupBox());
			this->scanner_numRepeat = (gcnew System::Windows::Forms::NumericUpDown());
			this->Scanner_Loop_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->labelScannerRepeat = (gcnew System::Windows::Forms::Label());
			this->labelScannerY2 = (gcnew System::Windows::Forms::Label());
			this->labelScannerY1 = (gcnew System::Windows::Forms::Label());
			this->labelScannerX2 = (gcnew System::Windows::Forms::Label());
			this->labelScannerX1 = (gcnew System::Windows::Forms::Label());
			this->scanner_numX2 = (gcnew System::Windows::Forms::NumericUpDown());
			this->scanner_numX1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->scanner_numY1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->scanner_numY2 = (gcnew System::Windows::Forms::NumericUpDown());
			this->scanner_btnRun = (gcnew System::Windows::Forms::Button());
			this->scanner_btnStop = (gcnew System::Windows::Forms::Button());
			this->scanner_txtActionBox = (gcnew System::Windows::Forms::TextBox());
			this->scanner_txtInfoBox = (gcnew System::Windows::Forms::TextBox());
			this->lblMarkSpeed = (gcnew System::Windows::Forms::Label());
			this->lblJumpSpeed = (gcnew System::Windows::Forms::Label());
			this->scanner_numSetMarkTime = (gcnew System::Windows::Forms::NumericUpDown());
			this->scanner_numSetJumpTime = (gcnew System::Windows::Forms::NumericUpDown());
			this->scanner_btnSetSpeed = (gcnew System::Windows::Forms::Button());
			this->btnInitialiseScanner = (gcnew System::Windows::Forms::Button());
			this->stripLiveCameras->SuspendLayout();
			this->ExperimentTab->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->experiment_numSST))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ExperimentSetup_travelTime))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ExperimentSetup_NoOfSteps))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->experiment_nudWaitingTime))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->experiment_numNumberOfTimePointsValue))->BeginInit();
			this->StageTab->SuspendLayout();
			this->groupExperiementSetup->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->stage_nudY1Stage))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->stage_nudY2Stage))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->stage_zLimit))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->stage_numObjectiveMagnification))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->stage_numWaveXStep))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->stage_numXStep))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->stage_numZVelocity))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->stage_numYVelocity))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->stage_numYStep))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->stage_numZStep))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->stage_numXVelocity))->BeginInit();
			this->CameraTab->SuspendLayout();
			this->groupBox2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nudReferenceHeight))->BeginInit();
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nudBrightMax_ch2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nudBrightMin_ch2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nudBrightMax_ch1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nudBrightMin_ch1))->BeginInit();
			this->GroupCamera1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->camera1_numCurrentTMExposure))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->camera1_numSetTMExposure))->BeginInit();
			this->groupCam1ImageSize->SuspendLayout();
			this->TabControls->SuspendLayout();
			this->ScannerTab->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->scanner_numGain))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->scanner_numSetJumpDelay))->BeginInit();
			this->GroupSingleScanCycleTime->SuspendLayout();
			this->GroupScanJob->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->scanner_numRepeat))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->scanner_numX2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->scanner_numX1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->scanner_numY1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->scanner_numY2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->scanner_numSetMarkTime))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->scanner_numSetJumpTime))->BeginInit();
			this->SuspendLayout();
			// 
			// stripLiveCameras
			// 
			this->stripLiveCameras->ImageScalingSize = System::Drawing::Size(20, 20);
			this->stripLiveCameras->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->camera1_btnLiveImage });
			this->stripLiveCameras->Location = System::Drawing::Point(0, 0);
			this->stripLiveCameras->Name = L"stripLiveCameras";
			this->stripLiveCameras->Size = System::Drawing::Size(725, 25);
			this->stripLiveCameras->TabIndex = 2;
			this->stripLiveCameras->Text = L"toolStrip1";
			// 
			// camera1_btnLiveImage
			// 
			this->camera1_btnLiveImage->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->camera1_btnLiveImage->Name = L"camera1_btnLiveImage";
			this->camera1_btnLiveImage->Size = System::Drawing::Size(71, 22);
			this->camera1_btnLiveImage->Text = L"Live Image ";
			this->camera1_btnLiveImage->Click += gcnew System::EventHandler(this, &MainForm::camera1_btnLiveImage_Click);
			// 
			// lblStageInfo
			// 
			this->lblStageInfo->AutoSize = true;
			this->lblStageInfo->Location = System::Drawing::Point(85, 4);
			this->lblStageInfo->Name = L"lblStageInfo";
			this->lblStageInfo->Size = System::Drawing::Size(56, 13);
			this->lblStageInfo->TabIndex = 6;
			this->lblStageInfo->Text = L"Stage Info";
			// 
			// RunWorker
			// 
			this->RunWorker->WorkerSupportsCancellation = true;
			this->RunWorker->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &MainForm::scanner_btnRunWorker_DoWork);
			// 
			// background_StartExperiment
			// 
			this->background_StartExperiment->WorkerReportsProgress = true;
			this->background_StartExperiment->WorkerSupportsCancellation = true;
			this->background_StartExperiment->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &MainForm::background_StartExperiment_DoWork);
			// 
			// ExperimentTab
			// 
			this->ExperimentTab->Controls->Add(this->experiment_numSST);
			this->ExperimentTab->Controls->Add(this->label_SingleScanTime);
			this->ExperimentTab->Controls->Add(this->ExperimentSetup_travelTime);
			this->ExperimentTab->Controls->Add(this->ExperimentSetup_NoOfSteps);
			this->ExperimentTab->Controls->Add(this->label_travelTime);
			this->ExperimentTab->Controls->Add(this->label_NoOfScans);
			this->ExperimentTab->Controls->Add(this->experiment_txtActionBox);
			this->ExperimentTab->Controls->Add(this->experiment_txtInfoBox);
			this->ExperimentTab->Controls->Add(this->experiment_nudWaitingTime);
			this->ExperimentTab->Controls->Add(this->labelWaitingTime);
			this->ExperimentTab->Controls->Add(this->labelscansperformed);
			this->ExperimentTab->Controls->Add(this->experiment_progressBarNumberOfScansPerTimePoint);
			this->ExperimentTab->Controls->Add(this->labeltimepointsperformed);
			this->ExperimentTab->Controls->Add(this->experiment_progressBarExperiment);
			this->ExperimentTab->Controls->Add(this->experiment_numNumberOfTimePointsValue);
			this->ExperimentTab->Controls->Add(this->labelNumberOfTimePoints);
			this->ExperimentTab->Controls->Add(this->experiment_btnStopExperiment);
			this->ExperimentTab->Controls->Add(this->experiment_btnStartExperiment);
			this->ExperimentTab->Controls->Add(this->experiment_btnSetupExperiment);
			this->ExperimentTab->Controls->Add(this->Noteslabel);
			this->ExperimentTab->Location = System::Drawing::Point(4, 22);
			this->ExperimentTab->Name = L"ExperimentTab";
			this->ExperimentTab->Size = System::Drawing::Size(717, 484);
			this->ExperimentTab->TabIndex = 5;
			this->ExperimentTab->Text = L"Experiment Setup";
			this->ExperimentTab->UseVisualStyleBackColor = true;
			// 
			// experiment_numSST
			// 
			this->experiment_numSST->DecimalPlaces = 5;
			this->experiment_numSST->Location = System::Drawing::Point(129, 338);
			this->experiment_numSST->Name = L"experiment_numSST";
			this->experiment_numSST->Size = System::Drawing::Size(85, 20);
			this->experiment_numSST->TabIndex = 187;
			this->experiment_numSST->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->experiment_numSST->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 2501, 0, 0, 327680 });
			// 
			// label_SingleScanTime
			// 
			this->label_SingleScanTime->AutoSize = true;
			this->label_SingleScanTime->Location = System::Drawing::Point(11, 345);
			this->label_SingleScanTime->Name = L"label_SingleScanTime";
			this->label_SingleScanTime->Size = System::Drawing::Size(98, 13);
			this->label_SingleScanTime->TabIndex = 186;
			this->label_SingleScanTime->Text = L"Single scan time (s)";
			// 
			// ExperimentSetup_travelTime
			// 
			this->ExperimentSetup_travelTime->DecimalPlaces = 2;
			this->ExperimentSetup_travelTime->Location = System::Drawing::Point(129, 411);
			this->ExperimentSetup_travelTime->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000, 0, 0, 0 });
			this->ExperimentSetup_travelTime->Name = L"ExperimentSetup_travelTime";
			this->ExperimentSetup_travelTime->Size = System::Drawing::Size(85, 20);
			this->ExperimentSetup_travelTime->TabIndex = 185;
			this->ExperimentSetup_travelTime->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->ExperimentSetup_travelTime->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 3000, 0, 0, 131072 });
			// 
			// ExperimentSetup_NoOfSteps
			// 
			this->ExperimentSetup_NoOfSteps->Location = System::Drawing::Point(129, 374);
			this->ExperimentSetup_NoOfSteps->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 8000, 0, 0, 0 });
			this->ExperimentSetup_NoOfSteps->Name = L"ExperimentSetup_NoOfSteps";
			this->ExperimentSetup_NoOfSteps->Size = System::Drawing::Size(85, 20);
			this->ExperimentSetup_NoOfSteps->TabIndex = 184;
			this->ExperimentSetup_NoOfSteps->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->ExperimentSetup_NoOfSteps->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 2173, 0, 0, 0 });
			// 
			// label_travelTime
			// 
			this->label_travelTime->AutoSize = true;
			this->label_travelTime->Location = System::Drawing::Point(11, 418);
			this->label_travelTime->Name = L"label_travelTime";
			this->label_travelTime->Size = System::Drawing::Size(73, 13);
			this->label_travelTime->TabIndex = 183;
			this->label_travelTime->Text = L"Travel time (s)";
			// 
			// label_NoOfScans
			// 
			this->label_NoOfScans->AutoSize = true;
			this->label_NoOfScans->Location = System::Drawing::Point(11, 381);
			this->label_NoOfScans->Name = L"label_NoOfScans";
			this->label_NoOfScans->Size = System::Drawing::Size(64, 13);
			this->label_NoOfScans->TabIndex = 182;
			this->label_NoOfScans->Text = L"No. of steps";
			// 
			// experiment_txtActionBox
			// 
			this->experiment_txtActionBox->Location = System::Drawing::Point(6, 462);
			this->experiment_txtActionBox->Margin = System::Windows::Forms::Padding(2);
			this->experiment_txtActionBox->Name = L"experiment_txtActionBox";
			this->experiment_txtActionBox->ReadOnly = true;
			this->experiment_txtActionBox->Size = System::Drawing::Size(704, 20);
			this->experiment_txtActionBox->TabIndex = 111;
			// 
			// experiment_txtInfoBox
			// 
			this->experiment_txtInfoBox->BackColor = System::Drawing::SystemColors::Info;
			this->experiment_txtInfoBox->Location = System::Drawing::Point(12, 28);
			this->experiment_txtInfoBox->Margin = System::Windows::Forms::Padding(4);
			this->experiment_txtInfoBox->Multiline = true;
			this->experiment_txtInfoBox->Name = L"experiment_txtInfoBox";
			this->experiment_txtInfoBox->ReadOnly = true;
			this->experiment_txtInfoBox->Size = System::Drawing::Size(667, 116);
			this->experiment_txtInfoBox->TabIndex = 94;
			// 
			// experiment_nudWaitingTime
			// 
			this->experiment_nudWaitingTime->Location = System::Drawing::Point(129, 298);
			this->experiment_nudWaitingTime->Margin = System::Windows::Forms::Padding(4);
			this->experiment_nudWaitingTime->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5000, 0, 0, 0 });
			this->experiment_nudWaitingTime->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			this->experiment_nudWaitingTime->Name = L"experiment_nudWaitingTime";
			this->experiment_nudWaitingTime->Size = System::Drawing::Size(85, 20);
			this->experiment_nudWaitingTime->TabIndex = 106;
			this->experiment_nudWaitingTime->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->experiment_nudWaitingTime->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 120, 0, 0, 0 });
			// 
			// labelWaitingTime
			// 
			this->labelWaitingTime->AutoSize = true;
			this->labelWaitingTime->Location = System::Drawing::Point(11, 300);
			this->labelWaitingTime->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->labelWaitingTime->Name = L"labelWaitingTime";
			this->labelWaitingTime->Size = System::Drawing::Size(83, 13);
			this->labelWaitingTime->TabIndex = 105;
			this->labelWaitingTime->Text = L"Waiting Time (s)";
			// 
			// labelscansperformed
			// 
			this->labelscansperformed->AutoSize = true;
			this->labelscansperformed->Location = System::Drawing::Point(515, 216);
			this->labelscansperformed->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->labelscansperformed->Name = L"labelscansperformed";
			this->labelscansperformed->Size = System::Drawing::Size(103, 13);
			this->labelscansperformed->TabIndex = 104;
			this->labelscansperformed->Text = L"Scan / Total Scans ";
			// 
			// experiment_progressBarNumberOfScansPerTimePoint
			// 
			this->experiment_progressBarNumberOfScansPerTimePoint->Location = System::Drawing::Point(336, 219);
			this->experiment_progressBarNumberOfScansPerTimePoint->Margin = System::Windows::Forms::Padding(4);
			this->experiment_progressBarNumberOfScansPerTimePoint->Maximum = 2000;
			this->experiment_progressBarNumberOfScansPerTimePoint->Name = L"experiment_progressBarNumberOfScansPerTimePoint";
			this->experiment_progressBarNumberOfScansPerTimePoint->Size = System::Drawing::Size(171, 14);
			this->experiment_progressBarNumberOfScansPerTimePoint->Step = 1;
			this->experiment_progressBarNumberOfScansPerTimePoint->TabIndex = 103;
			// 
			// labeltimepointsperformed
			// 
			this->labeltimepointsperformed->AutoSize = true;
			this->labeltimepointsperformed->Location = System::Drawing::Point(515, 189);
			this->labeltimepointsperformed->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->labeltimepointsperformed->Name = L"labeltimepointsperformed";
			this->labeltimepointsperformed->Size = System::Drawing::Size(149, 13);
			this->labeltimepointsperformed->TabIndex = 102;
			this->labeltimepointsperformed->Text = L"Time Point / Total Time points";
			// 
			// experiment_progressBarExperiment
			// 
			this->experiment_progressBarExperiment->Location = System::Drawing::Point(336, 183);
			this->experiment_progressBarExperiment->Margin = System::Windows::Forms::Padding(4);
			this->experiment_progressBarExperiment->Maximum = 2000;
			this->experiment_progressBarExperiment->Name = L"experiment_progressBarExperiment";
			this->experiment_progressBarExperiment->Size = System::Drawing::Size(171, 28);
			this->experiment_progressBarExperiment->Step = 1;
			this->experiment_progressBarExperiment->TabIndex = 101;
			// 
			// experiment_numNumberOfTimePointsValue
			// 
			this->experiment_numNumberOfTimePointsValue->Location = System::Drawing::Point(129, 266);
			this->experiment_numNumberOfTimePointsValue->Margin = System::Windows::Forms::Padding(4);
			this->experiment_numNumberOfTimePointsValue->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {
				5000, 0, 0,
					0
			});
			this->experiment_numNumberOfTimePointsValue->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			this->experiment_numNumberOfTimePointsValue->Name = L"experiment_numNumberOfTimePointsValue";
			this->experiment_numNumberOfTimePointsValue->Size = System::Drawing::Size(85, 20);
			this->experiment_numNumberOfTimePointsValue->TabIndex = 100;
			this->experiment_numNumberOfTimePointsValue->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->experiment_numNumberOfTimePointsValue->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000, 0, 0, 0 });
			// 
			// labelNumberOfTimePoints
			// 
			this->labelNumberOfTimePoints->AutoSize = true;
			this->labelNumberOfTimePoints->Location = System::Drawing::Point(11, 268);
			this->labelNumberOfTimePoints->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->labelNumberOfTimePoints->Name = L"labelNumberOfTimePoints";
			this->labelNumberOfTimePoints->Size = System::Drawing::Size(72, 13);
			this->labelNumberOfTimePoints->TabIndex = 99;
			this->labelNumberOfTimePoints->Text = L"# Time Points";
			// 
			// experiment_btnStopExperiment
			// 
			this->experiment_btnStopExperiment->Enabled = false;
			this->experiment_btnStopExperiment->Location = System::Drawing::Point(336, 268);
			this->experiment_btnStopExperiment->Margin = System::Windows::Forms::Padding(4);
			this->experiment_btnStopExperiment->Name = L"experiment_btnStopExperiment";
			this->experiment_btnStopExperiment->Size = System::Drawing::Size(204, 28);
			this->experiment_btnStopExperiment->TabIndex = 98;
			this->experiment_btnStopExperiment->Text = L"Stop Experiment";
			this->experiment_btnStopExperiment->UseVisualStyleBackColor = true;
			this->experiment_btnStopExperiment->Click += gcnew System::EventHandler(this, &MainForm::experiment_btnStopExperiment_Click);
			// 
			// experiment_btnStartExperiment
			// 
			this->experiment_btnStartExperiment->Location = System::Drawing::Point(173, 183);
			this->experiment_btnStartExperiment->Margin = System::Windows::Forms::Padding(4);
			this->experiment_btnStartExperiment->Name = L"experiment_btnStartExperiment";
			this->experiment_btnStartExperiment->Size = System::Drawing::Size(132, 28);
			this->experiment_btnStartExperiment->TabIndex = 97;
			this->experiment_btnStartExperiment->Text = L"Start Experiment";
			this->experiment_btnStartExperiment->UseVisualStyleBackColor = true;
			this->experiment_btnStartExperiment->Click += gcnew System::EventHandler(this, &MainForm::experiment_btnStartExperiment_Click);
			// 
			// experiment_btnSetupExperiment
			// 
			this->experiment_btnSetupExperiment->Location = System::Drawing::Point(7, 183);
			this->experiment_btnSetupExperiment->Margin = System::Windows::Forms::Padding(4);
			this->experiment_btnSetupExperiment->Name = L"experiment_btnSetupExperiment";
			this->experiment_btnSetupExperiment->Size = System::Drawing::Size(159, 28);
			this->experiment_btnSetupExperiment->TabIndex = 96;
			this->experiment_btnSetupExperiment->Text = L"Setup Experiment";
			this->experiment_btnSetupExperiment->UseVisualStyleBackColor = true;
			this->experiment_btnSetupExperiment->Click += gcnew System::EventHandler(this, &MainForm::experiment_btnSetupExperiment_Click);
			// 
			// Noteslabel
			// 
			this->Noteslabel->AutoSize = true;
			this->Noteslabel->Location = System::Drawing::Point(9, 11);
			this->Noteslabel->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->Noteslabel->Name = L"Noteslabel";
			this->Noteslabel->Size = System::Drawing::Size(35, 13);
			this->Noteslabel->TabIndex = 95;
			this->Noteslabel->Text = L"Notes";
			// 
			// StageTab
			// 
			this->StageTab->Controls->Add(this->stage_txtInfoBox);
			this->StageTab->Controls->Add(this->stage_txtZVelocity);
			this->StageTab->Controls->Add(this->stage_txtXPosition);
			this->StageTab->Controls->Add(this->stage_txtYVelocity);
			this->StageTab->Controls->Add(this->stage_txtZPosition);
			this->StageTab->Controls->Add(this->stage_txtXVelocity);
			this->StageTab->Controls->Add(this->stage_txtYPosition);
			this->StageTab->Controls->Add(this->stage_txtActionBox);
			this->StageTab->Controls->Add(this->groupExperiementSetup);
			this->StageTab->Controls->Add(this->stage_btnInitialiseStage);
			this->StageTab->Controls->Add(this->stage_btnLeft);
			this->StageTab->Controls->Add(this->stage_btnSetZVelocity);
			this->StageTab->Controls->Add(this->stage_numXStep);
			this->StageTab->Controls->Add(this->stage_btnSetYVelocity);
			this->StageTab->Controls->Add(this->lblXStepSize);
			this->StageTab->Controls->Add(this->lblZVelocity);
			this->StageTab->Controls->Add(this->stage_numZVelocity);
			this->StageTab->Controls->Add(this->label1);
			this->StageTab->Controls->Add(this->stage_btnDown);
			this->StageTab->Controls->Add(this->lblZPosition);
			this->StageTab->Controls->Add(this->stage_btnFront);
			this->StageTab->Controls->Add(this->lblYVelocity);
			this->StageTab->Controls->Add(this->stage_numYVelocity);
			this->StageTab->Controls->Add(this->stage_numYStep);
			this->StageTab->Controls->Add(this->stage_btnBack);
			this->StageTab->Controls->Add(this->lblZStepSize);
			this->StageTab->Controls->Add(this->lblYStepSize);
			this->StageTab->Controls->Add(this->lblXVelocity);
			this->StageTab->Controls->Add(this->stage_numZStep);
			this->StageTab->Controls->Add(this->stage_numXVelocity);
			this->StageTab->Controls->Add(this->stage_btnSetXVelocity);
			this->StageTab->Controls->Add(this->stage_btnUp);
			this->StageTab->Controls->Add(this->lblYPosition);
			this->StageTab->Controls->Add(this->stage_btnRight);
			this->StageTab->Location = System::Drawing::Point(4, 22);
			this->StageTab->Name = L"StageTab";
			this->StageTab->Padding = System::Windows::Forms::Padding(3);
			this->StageTab->Size = System::Drawing::Size(717, 484);
			this->StageTab->TabIndex = 1;
			this->StageTab->Text = L"Stage";
			this->StageTab->UseVisualStyleBackColor = true;
			// 
			// stage_txtInfoBox
			// 
			this->stage_txtInfoBox->BackColor = System::Drawing::SystemColors::Info;
			this->stage_txtInfoBox->Location = System::Drawing::Point(78, 13);
			this->stage_txtInfoBox->Multiline = true;
			this->stage_txtInfoBox->Name = L"stage_txtInfoBox";
			this->stage_txtInfoBox->Size = System::Drawing::Size(605, 68);
			this->stage_txtInfoBox->TabIndex = 4;
			// 
			// stage_txtZVelocity
			// 
			this->stage_txtZVelocity->Location = System::Drawing::Point(413, 191);
			this->stage_txtZVelocity->Name = L"stage_txtZVelocity";
			this->stage_txtZVelocity->ReadOnly = true;
			this->stage_txtZVelocity->Size = System::Drawing::Size(107, 20);
			this->stage_txtZVelocity->TabIndex = 33;
			// 
			// stage_txtXPosition
			// 
			this->stage_txtXPosition->Location = System::Drawing::Point(6, 113);
			this->stage_txtXPosition->Name = L"stage_txtXPosition";
			this->stage_txtXPosition->ReadOnly = true;
			this->stage_txtXPosition->Size = System::Drawing::Size(100, 20);
			this->stage_txtXPosition->TabIndex = 8;
			// 
			// stage_txtYVelocity
			// 
			this->stage_txtYVelocity->Location = System::Drawing::Point(413, 153);
			this->stage_txtYVelocity->Name = L"stage_txtYVelocity";
			this->stage_txtYVelocity->ReadOnly = true;
			this->stage_txtYVelocity->Size = System::Drawing::Size(107, 20);
			this->stage_txtYVelocity->TabIndex = 23;
			// 
			// stage_txtZPosition
			// 
			this->stage_txtZPosition->Location = System::Drawing::Point(6, 190);
			this->stage_txtZPosition->Name = L"stage_txtZPosition";
			this->stage_txtZPosition->ReadOnly = true;
			this->stage_txtZPosition->Size = System::Drawing::Size(100, 20);
			this->stage_txtZPosition->TabIndex = 28;
			// 
			// stage_txtXVelocity
			// 
			this->stage_txtXVelocity->Location = System::Drawing::Point(413, 114);
			this->stage_txtXVelocity->Name = L"stage_txtXVelocity";
			this->stage_txtXVelocity->ReadOnly = true;
			this->stage_txtXVelocity->Size = System::Drawing::Size(107, 20);
			this->stage_txtXVelocity->TabIndex = 13;
			// 
			// stage_txtYPosition
			// 
			this->stage_txtYPosition->Location = System::Drawing::Point(6, 152);
			this->stage_txtYPosition->Name = L"stage_txtYPosition";
			this->stage_txtYPosition->ReadOnly = true;
			this->stage_txtYPosition->Size = System::Drawing::Size(100, 20);
			this->stage_txtYPosition->TabIndex = 18;
			// 
			// stage_txtActionBox
			// 
			this->stage_txtActionBox->BackColor = System::Drawing::SystemColors::Menu;
			this->stage_txtActionBox->Location = System::Drawing::Point(5, 457);
			this->stage_txtActionBox->Name = L"stage_txtActionBox";
			this->stage_txtActionBox->Size = System::Drawing::Size(712, 20);
			this->stage_txtActionBox->TabIndex = 5;
			// 
			// groupExperiementSetup
			// 
			this->groupExperiementSetup->Controls->Add(this->stage_btnSetY1);
			this->groupExperiementSetup->Controls->Add(this->stage_nudY1Stage);
			this->groupExperiementSetup->Controls->Add(this->stage_nudY2Stage);
			this->groupExperiementSetup->Controls->Add(this->label2);
			this->groupExperiementSetup->Controls->Add(this->stage_zLimit);
			this->groupExperiementSetup->Controls->Add(this->stage_zlimit_text);
			this->groupExperiementSetup->Controls->Add(this->stage_btnSetStageExpParams);
			this->groupExperiementSetup->Controls->Add(this->stage_numObjectiveMagnification);
			this->groupExperiementSetup->Controls->Add(this->stage_numWaveXStep);
			this->groupExperiementSetup->Controls->Add(this->stage_btnSetY2);
			this->groupExperiementSetup->Controls->Add(this->stage_btnSetX4Z4);
			this->groupExperiementSetup->Controls->Add(this->stage_btnSetX3Z3);
			this->groupExperiementSetup->Controls->Add(this->stage_btnSetX2Z2);
			this->groupExperiementSetup->Controls->Add(this->stage_btnSetX1Z1);
			this->groupExperiementSetup->Controls->Add(this->stage_btnSetX0Z0);
			this->groupExperiementSetup->Controls->Add(this->lblY1);
			this->groupExperiementSetup->Controls->Add(this->lblX4Z4);
			this->groupExperiementSetup->Controls->Add(this->lblX3Z3);
			this->groupExperiementSetup->Controls->Add(this->lblX2Z2);
			this->groupExperiementSetup->Controls->Add(this->lblObjective);
			this->groupExperiementSetup->Controls->Add(this->lblX1Z1);
			this->groupExperiementSetup->Controls->Add(this->lblX0Z0);
			this->groupExperiementSetup->Controls->Add(this->lblWaveXStep);
			this->groupExperiementSetup->Controls->Add(this->stage_txtZ4Stage);
			this->groupExperiementSetup->Controls->Add(this->stage_txtX4Stage);
			this->groupExperiementSetup->Controls->Add(this->stage_txtZ3Stage);
			this->groupExperiementSetup->Controls->Add(this->stage_txtX3Stage);
			this->groupExperiementSetup->Controls->Add(this->stage_txtZ2Stage);
			this->groupExperiementSetup->Controls->Add(this->stage_txtX2Stage);
			this->groupExperiementSetup->Controls->Add(this->stage_txtZ1Stage);
			this->groupExperiementSetup->Controls->Add(this->stage_txtX1Stage);
			this->groupExperiementSetup->Controls->Add(this->stage_txtZ0Stage);
			this->groupExperiementSetup->Controls->Add(this->stage_txtX0Stage);
			this->groupExperiementSetup->Location = System::Drawing::Point(6, 217);
			this->groupExperiementSetup->Margin = System::Windows::Forms::Padding(2);
			this->groupExperiementSetup->Name = L"groupExperiementSetup";
			this->groupExperiementSetup->Padding = System::Windows::Forms::Padding(2);
			this->groupExperiementSetup->Size = System::Drawing::Size(1022, 235);
			this->groupExperiementSetup->TabIndex = 36;
			this->groupExperiementSetup->TabStop = false;
			this->groupExperiementSetup->Text = L"Experiemnt Set Up";
			// 
			// stage_btnSetY1
			// 
			this->stage_btnSetY1->Location = System::Drawing::Point(216, 185);
			this->stage_btnSetY1->Name = L"stage_btnSetY1";
			this->stage_btnSetY1->Size = System::Drawing::Size(50, 20);
			this->stage_btnSetY1->TabIndex = 167;
			this->stage_btnSetY1->Text = L"Set Y1";
			this->stage_btnSetY1->UseVisualStyleBackColor = true;
			this->stage_btnSetY1->Click += gcnew System::EventHandler(this, &MainForm::stage_btnSetY1_Click);
			// 
			// stage_nudY1Stage
			// 
			this->stage_nudY1Stage->DecimalPlaces = 6;
			this->stage_nudY1Stage->Location = System::Drawing::Point(63, 186);
			this->stage_nudY1Stage->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5, 0, 0, 0 });
			this->stage_nudY1Stage->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5, 0, 0, System::Int32::MinValue });
			this->stage_nudY1Stage->Name = L"stage_nudY1Stage";
			this->stage_nudY1Stage->Size = System::Drawing::Size(90, 20);
			this->stage_nudY1Stage->TabIndex = 166;
			// 
			// stage_nudY2Stage
			// 
			this->stage_nudY2Stage->DecimalPlaces = 6;
			this->stage_nudY2Stage->Location = System::Drawing::Point(63, 212);
			this->stage_nudY2Stage->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5, 0, 0, 0 });
			this->stage_nudY2Stage->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5, 0, 0, System::Int32::MinValue });
			this->stage_nudY2Stage->Name = L"stage_nudY2Stage";
			this->stage_nudY2Stage->Size = System::Drawing::Size(90, 20);
			this->stage_nudY2Stage->TabIndex = 165;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(10, 214);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(26, 13);
			this->label2->TabIndex = 164;
			this->label2->Text = L"(Y2)";
			// 
			// stage_zLimit
			// 
			this->stage_zLimit->DecimalPlaces = 5;
			this->stage_zLimit->Location = System::Drawing::Point(558, 64);
			this->stage_zLimit->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 25, 0, 0, 0 });
			this->stage_zLimit->Name = L"stage_zLimit";
			this->stage_zLimit->Size = System::Drawing::Size(80, 20);
			this->stage_zLimit->TabIndex = 162;
			this->stage_zLimit->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->stage_zLimit->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 250, 0, 0, 65536 });
			// 
			// stage_zlimit_text
			// 
			this->stage_zlimit_text->AutoSize = true;
			this->stage_zlimit_text->Location = System::Drawing::Point(344, 71);
			this->stage_zlimit_text->Name = L"stage_zlimit_text";
			this->stage_zlimit_text->Size = System::Drawing::Size(37, 13);
			this->stage_zlimit_text->TabIndex = 161;
			this->stage_zlimit_text->Text = L"Z limit ";
			// 
			// stage_btnSetStageExpParams
			// 
			this->stage_btnSetStageExpParams->Enabled = false;
			this->stage_btnSetStageExpParams->Location = System::Drawing::Point(407, 186);
			this->stage_btnSetStageExpParams->Margin = System::Windows::Forms::Padding(4);
			this->stage_btnSetStageExpParams->Name = L"stage_btnSetStageExpParams";
			this->stage_btnSetStageExpParams->Size = System::Drawing::Size(191, 24);
			this->stage_btnSetStageExpParams->TabIndex = 152;
			this->stage_btnSetStageExpParams->Text = L"Set Stage Exp. Parameters";
			this->stage_btnSetStageExpParams->UseVisualStyleBackColor = true;
			this->stage_btnSetStageExpParams->Click += gcnew System::EventHandler(this, &MainForm::stage_btnSetStageExpParams_Click);
			// 
			// stage_numObjectiveMagnification
			// 
			this->stage_numObjectiveMagnification->Location = System::Drawing::Point(558, 134);
			this->stage_numObjectiveMagnification->Name = L"stage_numObjectiveMagnification";
			this->stage_numObjectiveMagnification->Size = System::Drawing::Size(80, 20);
			this->stage_numObjectiveMagnification->TabIndex = 145;
			this->stage_numObjectiveMagnification->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->stage_numObjectiveMagnification->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10, 0, 0, 0 });
			// 
			// stage_numWaveXStep
			// 
			this->stage_numWaveXStep->DecimalPlaces = 5;
			this->stage_numWaveXStep->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 131072 });
			this->stage_numWaveXStep->Location = System::Drawing::Point(558, 101);
			this->stage_numWaveXStep->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5, 0, 0, 0 });
			this->stage_numWaveXStep->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 196608 });
			this->stage_numWaveXStep->Name = L"stage_numWaveXStep";
			this->stage_numWaveXStep->Size = System::Drawing::Size(80, 20);
			this->stage_numWaveXStep->TabIndex = 144;
			this->stage_numWaveXStep->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->stage_numWaveXStep->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 184, 0, 0, 327680 });
			// 
			// stage_btnSetY2
			// 
			this->stage_btnSetY2->Enabled = false;
			this->stage_btnSetY2->Location = System::Drawing::Point(216, 210);
			this->stage_btnSetY2->Name = L"stage_btnSetY2";
			this->stage_btnSetY2->Size = System::Drawing::Size(50, 20);
			this->stage_btnSetY2->TabIndex = 59;
			this->stage_btnSetY2->Text = L"Set Y2";
			this->stage_btnSetY2->UseVisualStyleBackColor = true;
			this->stage_btnSetY2->Click += gcnew System::EventHandler(this, &MainForm::stage_btnSetY2_Click);
			// 
			// stage_btnSetX4Z4
			// 
			this->stage_btnSetX4Z4->Enabled = false;
			this->stage_btnSetX4Z4->Location = System::Drawing::Point(216, 159);
			this->stage_btnSetX4Z4->Name = L"stage_btnSetX4Z4";
			this->stage_btnSetX4Z4->Size = System::Drawing::Size(50, 20);
			this->stage_btnSetX4Z4->TabIndex = 58;
			this->stage_btnSetX4Z4->Text = L"Set";
			this->stage_btnSetX4Z4->UseVisualStyleBackColor = true;
			this->stage_btnSetX4Z4->Click += gcnew System::EventHandler(this, &MainForm::stage_btnSetX4Z4_Click);
			// 
			// stage_btnSetX3Z3
			// 
			this->stage_btnSetX3Z3->Enabled = false;
			this->stage_btnSetX3Z3->Location = System::Drawing::Point(216, 134);
			this->stage_btnSetX3Z3->Name = L"stage_btnSetX3Z3";
			this->stage_btnSetX3Z3->Size = System::Drawing::Size(50, 20);
			this->stage_btnSetX3Z3->TabIndex = 57;
			this->stage_btnSetX3Z3->Text = L"Set";
			this->stage_btnSetX3Z3->UseVisualStyleBackColor = true;
			this->stage_btnSetX3Z3->Click += gcnew System::EventHandler(this, &MainForm::stage_btnSetX3Z3_Click);
			// 
			// stage_btnSetX2Z2
			// 
			this->stage_btnSetX2Z2->Enabled = false;
			this->stage_btnSetX2Z2->Location = System::Drawing::Point(216, 110);
			this->stage_btnSetX2Z2->Name = L"stage_btnSetX2Z2";
			this->stage_btnSetX2Z2->Size = System::Drawing::Size(50, 20);
			this->stage_btnSetX2Z2->TabIndex = 56;
			this->stage_btnSetX2Z2->Text = L"Set";
			this->stage_btnSetX2Z2->UseVisualStyleBackColor = true;
			this->stage_btnSetX2Z2->Click += gcnew System::EventHandler(this, &MainForm::stage_btnSetX2Z2_Click);
			// 
			// stage_btnSetX1Z1
			// 
			this->stage_btnSetX1Z1->Enabled = false;
			this->stage_btnSetX1Z1->Location = System::Drawing::Point(216, 86);
			this->stage_btnSetX1Z1->Name = L"stage_btnSetX1Z1";
			this->stage_btnSetX1Z1->Size = System::Drawing::Size(50, 20);
			this->stage_btnSetX1Z1->TabIndex = 55;
			this->stage_btnSetX1Z1->Text = L"Set";
			this->stage_btnSetX1Z1->UseVisualStyleBackColor = true;
			this->stage_btnSetX1Z1->Click += gcnew System::EventHandler(this, &MainForm::stage_btnSetX1Z1_Click);
			// 
			// stage_btnSetX0Z0
			// 
			this->stage_btnSetX0Z0->Enabled = false;
			this->stage_btnSetX0Z0->Location = System::Drawing::Point(216, 61);
			this->stage_btnSetX0Z0->Name = L"stage_btnSetX0Z0";
			this->stage_btnSetX0Z0->Size = System::Drawing::Size(50, 20);
			this->stage_btnSetX0Z0->TabIndex = 37;
			this->stage_btnSetX0Z0->Text = L"Set";
			this->stage_btnSetX0Z0->UseVisualStyleBackColor = true;
			this->stage_btnSetX0Z0->Click += gcnew System::EventHandler(this, &MainForm::stage_btnSetX0Z0_Click);
			// 
			// lblY1
			// 
			this->lblY1->AutoSize = true;
			this->lblY1->Location = System::Drawing::Point(10, 190);
			this->lblY1->Name = L"lblY1";
			this->lblY1->Size = System::Drawing::Size(26, 13);
			this->lblY1->TabIndex = 53;
			this->lblY1->Text = L"(Y1)";
			// 
			// lblX4Z4
			// 
			this->lblX4Z4->AutoSize = true;
			this->lblX4Z4->Location = System::Drawing::Point(10, 161);
			this->lblX4Z4->Name = L"lblX4Z4";
			this->lblX4Z4->Size = System::Drawing::Size(45, 13);
			this->lblX4Z4->TabIndex = 52;
			this->lblX4Z4->Text = L"(X4, Z4)";
			// 
			// lblX3Z3
			// 
			this->lblX3Z3->AutoSize = true;
			this->lblX3Z3->Location = System::Drawing::Point(10, 137);
			this->lblX3Z3->Name = L"lblX3Z3";
			this->lblX3Z3->Size = System::Drawing::Size(45, 13);
			this->lblX3Z3->TabIndex = 51;
			this->lblX3Z3->Text = L"(X3, Z3)";
			// 
			// lblX2Z2
			// 
			this->lblX2Z2->AutoSize = true;
			this->lblX2Z2->Location = System::Drawing::Point(10, 112);
			this->lblX2Z2->Name = L"lblX2Z2";
			this->lblX2Z2->Size = System::Drawing::Size(45, 13);
			this->lblX2Z2->TabIndex = 50;
			this->lblX2Z2->Text = L"(X2, Z2)";
			// 
			// lblObjective
			// 
			this->lblObjective->AutoSize = true;
			this->lblObjective->Location = System::Drawing::Point(344, 134);
			this->lblObjective->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->lblObjective->Name = L"lblObjective";
			this->lblObjective->Size = System::Drawing::Size(118, 13);
			this->lblObjective->TabIndex = 137;
			this->lblObjective->Text = L"Objective Magnification";
			// 
			// lblX1Z1
			// 
			this->lblX1Z1->AutoSize = true;
			this->lblX1Z1->Location = System::Drawing::Point(10, 88);
			this->lblX1Z1->Name = L"lblX1Z1";
			this->lblX1Z1->Size = System::Drawing::Size(45, 13);
			this->lblX1Z1->TabIndex = 49;
			this->lblX1Z1->Text = L"(X1, Z1)";
			// 
			// lblX0Z0
			// 
			this->lblX0Z0->AutoSize = true;
			this->lblX0Z0->Location = System::Drawing::Point(10, 64);
			this->lblX0Z0->Name = L"lblX0Z0";
			this->lblX0Z0->Size = System::Drawing::Size(45, 13);
			this->lblX0Z0->TabIndex = 37;
			this->lblX0Z0->Text = L"(X0, Z0)";
			// 
			// lblWaveXStep
			// 
			this->lblWaveXStep->AutoSize = true;
			this->lblWaveXStep->Location = System::Drawing::Point(344, 101);
			this->lblWaveXStep->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->lblWaveXStep->Name = L"lblWaveXStep";
			this->lblWaveXStep->Size = System::Drawing::Size(64, 13);
			this->lblWaveXStep->TabIndex = 136;
			this->lblWaveXStep->Text = L"X Step (mm)";
			// 
			// stage_txtZ4Stage
			// 
			this->stage_txtZ4Stage->Location = System::Drawing::Point(139, 159);
			this->stage_txtZ4Stage->Name = L"stage_txtZ4Stage";
			this->stage_txtZ4Stage->ReadOnly = true;
			this->stage_txtZ4Stage->Size = System::Drawing::Size(62, 20);
			this->stage_txtZ4Stage->TabIndex = 46;
			// 
			// stage_txtX4Stage
			// 
			this->stage_txtX4Stage->Location = System::Drawing::Point(66, 159);
			this->stage_txtX4Stage->Name = L"stage_txtX4Stage";
			this->stage_txtX4Stage->ReadOnly = true;
			this->stage_txtX4Stage->Size = System::Drawing::Size(62, 20);
			this->stage_txtX4Stage->TabIndex = 45;
			// 
			// stage_txtZ3Stage
			// 
			this->stage_txtZ3Stage->Location = System::Drawing::Point(139, 134);
			this->stage_txtZ3Stage->Name = L"stage_txtZ3Stage";
			this->stage_txtZ3Stage->ReadOnly = true;
			this->stage_txtZ3Stage->Size = System::Drawing::Size(62, 20);
			this->stage_txtZ3Stage->TabIndex = 44;
			// 
			// stage_txtX3Stage
			// 
			this->stage_txtX3Stage->Location = System::Drawing::Point(66, 134);
			this->stage_txtX3Stage->Name = L"stage_txtX3Stage";
			this->stage_txtX3Stage->ReadOnly = true;
			this->stage_txtX3Stage->Size = System::Drawing::Size(62, 20);
			this->stage_txtX3Stage->TabIndex = 43;
			// 
			// stage_txtZ2Stage
			// 
			this->stage_txtZ2Stage->Location = System::Drawing::Point(139, 110);
			this->stage_txtZ2Stage->Name = L"stage_txtZ2Stage";
			this->stage_txtZ2Stage->ReadOnly = true;
			this->stage_txtZ2Stage->Size = System::Drawing::Size(62, 20);
			this->stage_txtZ2Stage->TabIndex = 42;
			// 
			// stage_txtX2Stage
			// 
			this->stage_txtX2Stage->Location = System::Drawing::Point(66, 110);
			this->stage_txtX2Stage->Name = L"stage_txtX2Stage";
			this->stage_txtX2Stage->ReadOnly = true;
			this->stage_txtX2Stage->Size = System::Drawing::Size(62, 20);
			this->stage_txtX2Stage->TabIndex = 41;
			// 
			// stage_txtZ1Stage
			// 
			this->stage_txtZ1Stage->Location = System::Drawing::Point(139, 86);
			this->stage_txtZ1Stage->Name = L"stage_txtZ1Stage";
			this->stage_txtZ1Stage->ReadOnly = true;
			this->stage_txtZ1Stage->Size = System::Drawing::Size(62, 20);
			this->stage_txtZ1Stage->TabIndex = 40;
			// 
			// stage_txtX1Stage
			// 
			this->stage_txtX1Stage->Location = System::Drawing::Point(66, 86);
			this->stage_txtX1Stage->Name = L"stage_txtX1Stage";
			this->stage_txtX1Stage->ReadOnly = true;
			this->stage_txtX1Stage->Size = System::Drawing::Size(62, 20);
			this->stage_txtX1Stage->TabIndex = 39;
			// 
			// stage_txtZ0Stage
			// 
			this->stage_txtZ0Stage->Location = System::Drawing::Point(139, 61);
			this->stage_txtZ0Stage->Name = L"stage_txtZ0Stage";
			this->stage_txtZ0Stage->ReadOnly = true;
			this->stage_txtZ0Stage->Size = System::Drawing::Size(62, 20);
			this->stage_txtZ0Stage->TabIndex = 38;
			// 
			// stage_txtX0Stage
			// 
			this->stage_txtX0Stage->Location = System::Drawing::Point(66, 61);
			this->stage_txtX0Stage->Name = L"stage_txtX0Stage";
			this->stage_txtX0Stage->ReadOnly = true;
			this->stage_txtX0Stage->Size = System::Drawing::Size(62, 20);
			this->stage_txtX0Stage->TabIndex = 37;
			// 
			// stage_btnInitialiseStage
			// 
			this->stage_btnInitialiseStage->Location = System::Drawing::Point(6, 13);
			this->stage_btnInitialiseStage->Name = L"stage_btnInitialiseStage";
			this->stage_btnInitialiseStage->Size = System::Drawing::Size(66, 68);
			this->stage_btnInitialiseStage->TabIndex = 3;
			this->stage_btnInitialiseStage->Text = L"Initialise";
			this->stage_btnInitialiseStage->UseVisualStyleBackColor = true;
			this->stage_btnInitialiseStage->Click += gcnew System::EventHandler(this, &MainForm::stage_btnInitialiseStage_Click);
			// 
			// stage_btnLeft
			// 
			this->stage_btnLeft->Enabled = false;
			this->stage_btnLeft->Location = System::Drawing::Point(123, 110);
			this->stage_btnLeft->Name = L"stage_btnLeft";
			this->stage_btnLeft->Size = System::Drawing::Size(66, 24);
			this->stage_btnLeft->TabIndex = 1;
			this->stage_btnLeft->Text = L"Left";
			this->stage_btnLeft->UseVisualStyleBackColor = true;
			this->stage_btnLeft->Click += gcnew System::EventHandler(this, &MainForm::stage_btnLeft_Click);
			// 
			// stage_btnSetZVelocity
			// 
			this->stage_btnSetZVelocity->Enabled = false;
			this->stage_btnSetZVelocity->Location = System::Drawing::Point(595, 188);
			this->stage_btnSetZVelocity->Name = L"stage_btnSetZVelocity";
			this->stage_btnSetZVelocity->Size = System::Drawing::Size(66, 24);
			this->stage_btnSetZVelocity->TabIndex = 35;
			this->stage_btnSetZVelocity->Text = L"Set Z Vel";
			this->stage_btnSetZVelocity->UseVisualStyleBackColor = true;
			this->stage_btnSetZVelocity->Click += gcnew System::EventHandler(this, &MainForm::stage_btnSetZVelocity_Click);
			// 
			// stage_numXStep
			// 
			this->stage_numXStep->DecimalPlaces = 6;
			this->stage_numXStep->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 65536 });
			this->stage_numXStep->Location = System::Drawing::Point(195, 114);
			this->stage_numXStep->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10, 0, 0, 0 });
			this->stage_numXStep->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10, 0, 0, System::Int32::MinValue });
			this->stage_numXStep->Name = L"stage_numXStep";
			this->stage_numXStep->Size = System::Drawing::Size(120, 20);
			this->stage_numXStep->TabIndex = 2;
			this->stage_numXStep->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 2, 0, 0, 65536 });
			// 
			// stage_btnSetYVelocity
			// 
			this->stage_btnSetYVelocity->Enabled = false;
			this->stage_btnSetYVelocity->Location = System::Drawing::Point(595, 150);
			this->stage_btnSetYVelocity->Name = L"stage_btnSetYVelocity";
			this->stage_btnSetYVelocity->Size = System::Drawing::Size(66, 24);
			this->stage_btnSetYVelocity->TabIndex = 34;
			this->stage_btnSetYVelocity->Text = L"Set Y Vel";
			this->stage_btnSetYVelocity->UseVisualStyleBackColor = true;
			this->stage_btnSetYVelocity->Click += gcnew System::EventHandler(this, &MainForm::stage_btnSetYVelocity_Click);
			// 
			// lblXStepSize
			// 
			this->lblXStepSize->AutoSize = true;
			this->lblXStepSize->Location = System::Drawing::Point(192, 98);
			this->lblXStepSize->Name = L"lblXStepSize";
			this->lblXStepSize->Size = System::Drawing::Size(109, 13);
			this->lblXStepSize->TabIndex = 7;
			this->lblXStepSize->Text = L"X-Axis Step Size (mm)";
			// 
			// lblZVelocity
			// 
			this->lblZVelocity->AutoSize = true;
			this->lblZVelocity->Location = System::Drawing::Point(410, 176);
			this->lblZVelocity->Name = L"lblZVelocity";
			this->lblZVelocity->Size = System::Drawing::Size(110, 13);
			this->lblZVelocity->TabIndex = 32;
			this->lblZVelocity->Text = L"Z-Axis velocity (mm/s)";
			// 
			// stage_numZVelocity
			// 
			this->stage_numZVelocity->DecimalPlaces = 2;
			this->stage_numZVelocity->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 65536 });
			this->stage_numZVelocity->Location = System::Drawing::Point(524, 192);
			this->stage_numZVelocity->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			this->stage_numZVelocity->Name = L"stage_numZVelocity";
			this->stage_numZVelocity->Size = System::Drawing::Size(64, 20);
			this->stage_numZVelocity->TabIndex = 31;
			this->stage_numZVelocity->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5, 0, 0, 65536 });
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(3, 97);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(79, 13);
			this->label1->TabIndex = 9;
			this->label1->Text = L"X Position (mm)";
			// 
			// stage_btnDown
			// 
			this->stage_btnDown->Enabled = false;
			this->stage_btnDown->Location = System::Drawing::Point(123, 187);
			this->stage_btnDown->Name = L"stage_btnDown";
			this->stage_btnDown->Size = System::Drawing::Size(66, 24);
			this->stage_btnDown->TabIndex = 24;
			this->stage_btnDown->Text = L"Down";
			this->stage_btnDown->UseVisualStyleBackColor = true;
			this->stage_btnDown->Click += gcnew System::EventHandler(this, &MainForm::stage_btnDown_Click);
			// 
			// lblZPosition
			// 
			this->lblZPosition->AutoSize = true;
			this->lblZPosition->Location = System::Drawing::Point(3, 174);
			this->lblZPosition->Name = L"lblZPosition";
			this->lblZPosition->Size = System::Drawing::Size(79, 13);
			this->lblZPosition->TabIndex = 29;
			this->lblZPosition->Text = L"Z Position (mm)";
			// 
			// stage_btnFront
			// 
			this->stage_btnFront->Enabled = false;
			this->stage_btnFront->Location = System::Drawing::Point(321, 150);
			this->stage_btnFront->Name = L"stage_btnFront";
			this->stage_btnFront->Size = System::Drawing::Size(66, 24);
			this->stage_btnFront->TabIndex = 15;
			this->stage_btnFront->Text = L"Front";
			this->stage_btnFront->UseVisualStyleBackColor = true;
			this->stage_btnFront->Click += gcnew System::EventHandler(this, &MainForm::stage_btnFront_Click);
			// 
			// lblYVelocity
			// 
			this->lblYVelocity->AutoSize = true;
			this->lblYVelocity->Location = System::Drawing::Point(410, 137);
			this->lblYVelocity->Name = L"lblYVelocity";
			this->lblYVelocity->Size = System::Drawing::Size(110, 13);
			this->lblYVelocity->TabIndex = 22;
			this->lblYVelocity->Text = L"Y-Axis velocity (mm/s)";
			// 
			// stage_numYVelocity
			// 
			this->stage_numYVelocity->DecimalPlaces = 2;
			this->stage_numYVelocity->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 65536 });
			this->stage_numYVelocity->Location = System::Drawing::Point(524, 154);
			this->stage_numYVelocity->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 250, 0, 0, 0 });
			this->stage_numYVelocity->Name = L"stage_numYVelocity";
			this->stage_numYVelocity->Size = System::Drawing::Size(64, 20);
			this->stage_numYVelocity->TabIndex = 21;
			this->stage_numYVelocity->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 2, 0, 0, 0 });
			// 
			// stage_numYStep
			// 
			this->stage_numYStep->DecimalPlaces = 6;
			this->stage_numYStep->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 65536 });
			this->stage_numYStep->Location = System::Drawing::Point(195, 153);
			this->stage_numYStep->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10, 0, 0, 0 });
			this->stage_numYStep->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10, 0, 0, System::Int32::MinValue });
			this->stage_numYStep->Name = L"stage_numYStep";
			this->stage_numYStep->Size = System::Drawing::Size(120, 20);
			this->stage_numYStep->TabIndex = 16;
			this->stage_numYStep->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 2, 0, 0, 65536 });
			// 
			// stage_btnBack
			// 
			this->stage_btnBack->Enabled = false;
			this->stage_btnBack->Location = System::Drawing::Point(123, 150);
			this->stage_btnBack->Name = L"stage_btnBack";
			this->stage_btnBack->Size = System::Drawing::Size(66, 24);
			this->stage_btnBack->TabIndex = 14;
			this->stage_btnBack->Text = L"Back";
			this->stage_btnBack->UseVisualStyleBackColor = true;
			this->stage_btnBack->Click += gcnew System::EventHandler(this, &MainForm::stage_btnBack_Click);
			// 
			// lblZStepSize
			// 
			this->lblZStepSize->AutoSize = true;
			this->lblZStepSize->Location = System::Drawing::Point(192, 175);
			this->lblZStepSize->Name = L"lblZStepSize";
			this->lblZStepSize->Size = System::Drawing::Size(109, 13);
			this->lblZStepSize->TabIndex = 27;
			this->lblZStepSize->Text = L"Z-Axis Step Size (mm)";
			// 
			// lblYStepSize
			// 
			this->lblYStepSize->AutoSize = true;
			this->lblYStepSize->Location = System::Drawing::Point(192, 137);
			this->lblYStepSize->Name = L"lblYStepSize";
			this->lblYStepSize->Size = System::Drawing::Size(109, 13);
			this->lblYStepSize->TabIndex = 17;
			this->lblYStepSize->Text = L"Y-Axis Step Size (mm)";
			// 
			// lblXVelocity
			// 
			this->lblXVelocity->AutoSize = true;
			this->lblXVelocity->Location = System::Drawing::Point(410, 98);
			this->lblXVelocity->Name = L"lblXVelocity";
			this->lblXVelocity->Size = System::Drawing::Size(110, 13);
			this->lblXVelocity->TabIndex = 12;
			this->lblXVelocity->Text = L"X-Axis velocity (mm/s)";
			// 
			// stage_numZStep
			// 
			this->stage_numZStep->DecimalPlaces = 6;
			this->stage_numZStep->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 65536 });
			this->stage_numZStep->Location = System::Drawing::Point(195, 191);
			this->stage_numZStep->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 25, 0, 0, 0 });
			this->stage_numZStep->Name = L"stage_numZStep";
			this->stage_numZStep->Size = System::Drawing::Size(120, 20);
			this->stage_numZStep->TabIndex = 26;
			this->stage_numZStep->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 65536 });
			// 
			// stage_numXVelocity
			// 
			this->stage_numXVelocity->DecimalPlaces = 2;
			this->stage_numXVelocity->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 65536 });
			this->stage_numXVelocity->Location = System::Drawing::Point(524, 115);
			this->stage_numXVelocity->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 250, 0, 0, 0 });
			this->stage_numXVelocity->Name = L"stage_numXVelocity";
			this->stage_numXVelocity->Size = System::Drawing::Size(64, 20);
			this->stage_numXVelocity->TabIndex = 11;
			this->stage_numXVelocity->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 2, 0, 0, 0 });
			// 
			// stage_btnSetXVelocity
			// 
			this->stage_btnSetXVelocity->Enabled = false;
			this->stage_btnSetXVelocity->Location = System::Drawing::Point(595, 110);
			this->stage_btnSetXVelocity->Name = L"stage_btnSetXVelocity";
			this->stage_btnSetXVelocity->Size = System::Drawing::Size(66, 24);
			this->stage_btnSetXVelocity->TabIndex = 10;
			this->stage_btnSetXVelocity->Text = L"Set X Vel";
			this->stage_btnSetXVelocity->UseVisualStyleBackColor = true;
			this->stage_btnSetXVelocity->Click += gcnew System::EventHandler(this, &MainForm::stage_btnSetXVelocity_Click);
			// 
			// stage_btnUp
			// 
			this->stage_btnUp->Enabled = false;
			this->stage_btnUp->Location = System::Drawing::Point(321, 188);
			this->stage_btnUp->Name = L"stage_btnUp";
			this->stage_btnUp->Size = System::Drawing::Size(66, 24);
			this->stage_btnUp->TabIndex = 25;
			this->stage_btnUp->Text = L"Up";
			this->stage_btnUp->UseVisualStyleBackColor = true;
			this->stage_btnUp->Click += gcnew System::EventHandler(this, &MainForm::stage_btnUp_Click);
			// 
			// lblYPosition
			// 
			this->lblYPosition->AutoSize = true;
			this->lblYPosition->Location = System::Drawing::Point(3, 136);
			this->lblYPosition->Name = L"lblYPosition";
			this->lblYPosition->Size = System::Drawing::Size(79, 13);
			this->lblYPosition->TabIndex = 19;
			this->lblYPosition->Text = L"Y Position (mm)";
			// 
			// stage_btnRight
			// 
			this->stage_btnRight->Enabled = false;
			this->stage_btnRight->Location = System::Drawing::Point(321, 110);
			this->stage_btnRight->Name = L"stage_btnRight";
			this->stage_btnRight->Size = System::Drawing::Size(66, 24);
			this->stage_btnRight->TabIndex = 0;
			this->stage_btnRight->Text = L"Right";
			this->stage_btnRight->UseVisualStyleBackColor = true;
			this->stage_btnRight->Click += gcnew System::EventHandler(this, &MainForm::stage_btnRight_Click);
			// 
			// CameraTab
			// 
			this->CameraTab->Controls->Add(this->txt_camera_infoFile);
			this->CameraTab->Controls->Add(this->camera_ActionBox);
			this->CameraTab->Controls->Add(this->groupBox2);
			this->CameraTab->Controls->Add(this->groupBox1);
			this->CameraTab->Controls->Add(this->GroupCamera1);
			this->CameraTab->Location = System::Drawing::Point(4, 22);
			this->CameraTab->Name = L"CameraTab";
			this->CameraTab->Padding = System::Windows::Forms::Padding(3);
			this->CameraTab->Size = System::Drawing::Size(717, 484);
			this->CameraTab->TabIndex = 0;
			this->CameraTab->Text = L"Camera";
			this->CameraTab->UseVisualStyleBackColor = true;
			// 
			// txt_camera_infoFile
			// 
			this->txt_camera_infoFile->BackColor = System::Drawing::SystemColors::Window;
			this->txt_camera_infoFile->Location = System::Drawing::Point(436, 17);
			this->txt_camera_infoFile->Multiline = true;
			this->txt_camera_infoFile->Name = L"txt_camera_infoFile";
			this->txt_camera_infoFile->Size = System::Drawing::Size(261, 118);
			this->txt_camera_infoFile->TabIndex = 33;
			// 
			// camera_ActionBox
			// 
			this->camera_ActionBox->Location = System::Drawing::Point(6, 457);
			this->camera_ActionBox->Name = L"camera_ActionBox";
			this->camera_ActionBox->ReadOnly = true;
			this->camera_ActionBox->Size = System::Drawing::Size(705, 20);
			this->camera_ActionBox->TabIndex = 3;
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->rdB_HA_ch2);
			this->groupBox2->Controls->Add(this->rdB_HA_ch1);
			this->groupBox2->Controls->Add(this->label5);
			this->groupBox2->Controls->Add(this->nudReferenceHeight);
			this->groupBox2->Controls->Add(this->SetReferenceHighet);
			this->groupBox2->Controls->Add(this->lblReferenceHeight);
			this->groupBox2->Location = System::Drawing::Point(436, 141);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(261, 149);
			this->groupBox2->TabIndex = 31;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Height Adjustment";
			// 
			// rdB_HA_ch2
			// 
			this->rdB_HA_ch2->AutoSize = true;
			this->rdB_HA_ch2->Location = System::Drawing::Point(155, 52);
			this->rdB_HA_ch2->Name = L"rdB_HA_ch2";
			this->rdB_HA_ch2->Size = System::Drawing::Size(73, 17);
			this->rdB_HA_ch2->TabIndex = 2;
			this->rdB_HA_ch2->Text = L"Channel 2";
			this->rdB_HA_ch2->UseVisualStyleBackColor = true;
			// 
			// rdB_HA_ch1
			// 
			this->rdB_HA_ch1->AutoSize = true;
			this->rdB_HA_ch1->Checked = true;
			this->rdB_HA_ch1->Location = System::Drawing::Point(155, 19);
			this->rdB_HA_ch1->Name = L"rdB_HA_ch1";
			this->rdB_HA_ch1->Size = System::Drawing::Size(73, 17);
			this->rdB_HA_ch1->TabIndex = 1;
			this->rdB_HA_ch1->TabStop = true;
			this->rdB_HA_ch1->Text = L"Channel 1";
			this->rdB_HA_ch1->UseVisualStyleBackColor = true;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(16, 33);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(98, 26);
			this->label5->TabIndex = 0;
			this->label5->Text = L"Channel for Height \r\nAdjustment";
			// 
			// nudReferenceHeight
			// 
			this->nudReferenceHeight->Location = System::Drawing::Point(155, 77);
			this->nudReferenceHeight->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 512, 0, 0, 0 });
			this->nudReferenceHeight->Name = L"nudReferenceHeight";
			this->nudReferenceHeight->Size = System::Drawing::Size(60, 20);
			this->nudReferenceHeight->TabIndex = 28;
			this->nudReferenceHeight->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 200, 0, 0, 0 });
			// 
			// SetReferenceHighet
			// 
			this->SetReferenceHighet->Location = System::Drawing::Point(155, 111);
			this->SetReferenceHighet->Name = L"SetReferenceHighet";
			this->SetReferenceHighet->Size = System::Drawing::Size(60, 20);
			this->SetReferenceHighet->TabIndex = 27;
			this->SetReferenceHighet->Text = L"Set";
			this->SetReferenceHighet->UseVisualStyleBackColor = true;
			this->SetReferenceHighet->Click += gcnew System::EventHandler(this, &MainForm::SetReferenceHighet_Click);
			// 
			// lblReferenceHeight
			// 
			this->lblReferenceHeight->AutoSize = true;
			this->lblReferenceHeight->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->lblReferenceHeight->Location = System::Drawing::Point(16, 79);
			this->lblReferenceHeight->Name = L"lblReferenceHeight";
			this->lblReferenceHeight->Size = System::Drawing::Size(91, 13);
			this->lblReferenceHeight->TabIndex = 25;
			this->lblReferenceHeight->Text = L"Reference Height";
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->SetBright_ch2);
			this->groupBox1->Controls->Add(this->nudBrightMax_ch2);
			this->groupBox1->Controls->Add(this->nudBrightMin_ch2);
			this->groupBox1->Controls->Add(this->label4);
			this->groupBox1->Controls->Add(this->label3);
			this->groupBox1->Controls->Add(this->lblBrightMax);
			this->groupBox1->Controls->Add(this->nudBrightMax_ch1);
			this->groupBox1->Controls->Add(this->lblBrightMin);
			this->groupBox1->Controls->Add(this->nudBrightMin_ch1);
			this->groupBox1->Controls->Add(this->SetBright);
			this->groupBox1->Location = System::Drawing::Point(436, 296);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(261, 155);
			this->groupBox1->TabIndex = 30;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Intensity Control";
			// 
			// SetBright_ch2
			// 
			this->SetBright_ch2->Location = System::Drawing::Point(179, 126);
			this->SetBright_ch2->Name = L"SetBright_ch2";
			this->SetBright_ch2->Size = System::Drawing::Size(75, 23);
			this->SetBright_ch2->TabIndex = 29;
			this->SetBright_ch2->Text = L"SET CH2";
			this->SetBright_ch2->UseVisualStyleBackColor = true;
			this->SetBright_ch2->Click += gcnew System::EventHandler(this, &MainForm::SetBright_ch2_Click);
			// 
			// nudBrightMax_ch2
			// 
			this->nudBrightMax_ch2->Location = System::Drawing::Point(179, 61);
			this->nudBrightMax_ch2->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 65535, 0, 0, 0 });
			this->nudBrightMax_ch2->Name = L"nudBrightMax_ch2";
			this->nudBrightMax_ch2->Size = System::Drawing::Size(60, 20);
			this->nudBrightMax_ch2->TabIndex = 28;
			this->nudBrightMax_ch2->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 335, 0, 0, 0 });
			// 
			// nudBrightMin_ch2
			// 
			this->nudBrightMin_ch2->Location = System::Drawing::Point(179, 88);
			this->nudBrightMin_ch2->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 65535, 0, 0, 0 });
			this->nudBrightMin_ch2->Name = L"nudBrightMin_ch2";
			this->nudBrightMin_ch2->Size = System::Drawing::Size(60, 20);
			this->nudBrightMin_ch2->TabIndex = 27;
			this->nudBrightMin_ch2->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 80, 0, 0, 0 });
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(176, 30);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(67, 13);
			this->label4->TabIndex = 26;
			this->label4->Text = L"CHANNEL 2";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(86, 30);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(67, 13);
			this->label3->TabIndex = 25;
			this->label3->Text = L"CHANNEL 1";
			// 
			// lblBrightMax
			// 
			this->lblBrightMax->AutoSize = true;
			this->lblBrightMax->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->lblBrightMax->Location = System::Drawing::Point(13, 63);
			this->lblBrightMax->Name = L"lblBrightMax";
			this->lblBrightMax->Size = System::Drawing::Size(63, 13);
			this->lblBrightMax->TabIndex = 14;
			this->lblBrightMax->Text = L"Bright Level";
			// 
			// nudBrightMax_ch1
			// 
			this->nudBrightMax_ch1->Location = System::Drawing::Point(89, 61);
			this->nudBrightMax_ch1->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 65535, 0, 0, 0 });
			this->nudBrightMax_ch1->Name = L"nudBrightMax_ch1";
			this->nudBrightMax_ch1->Size = System::Drawing::Size(60, 20);
			this->nudBrightMax_ch1->TabIndex = 24;
			this->nudBrightMax_ch1->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 590, 0, 0, 0 });
			// 
			// lblBrightMin
			// 
			this->lblBrightMin->AutoSize = true;
			this->lblBrightMin->Location = System::Drawing::Point(17, 90);
			this->lblBrightMin->Name = L"lblBrightMin";
			this->lblBrightMin->Size = System::Drawing::Size(59, 13);
			this->lblBrightMin->TabIndex = 22;
			this->lblBrightMin->Text = L"Dark Level";
			// 
			// nudBrightMin_ch1
			// 
			this->nudBrightMin_ch1->Location = System::Drawing::Point(89, 88);
			this->nudBrightMin_ch1->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 65535, 0, 0, 0 });
			this->nudBrightMin_ch1->Name = L"nudBrightMin_ch1";
			this->nudBrightMin_ch1->Size = System::Drawing::Size(60, 20);
			this->nudBrightMin_ch1->TabIndex = 23;
			this->nudBrightMin_ch1->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 80, 0, 0, 0 });
			// 
			// SetBright
			// 
			this->SetBright->Location = System::Drawing::Point(89, 130);
			this->SetBright->Name = L"SetBright";
			this->SetBright->Size = System::Drawing::Size(60, 20);
			this->SetBright->TabIndex = 17;
			this->SetBright->Text = L"SET CH1";
			this->SetBright->UseVisualStyleBackColor = true;
			this->SetBright->Click += gcnew System::EventHandler(this, &MainForm::SetBright_Click);
			// 
			// GroupCamera1
			// 
			this->GroupCamera1->Controls->Add(this->Camera_90degreeRotation_checkBox);
			this->GroupCamera1->Controls->Add(this->camera1_numCurrentTMExposure);
			this->GroupCamera1->Controls->Add(this->camera1_numSetTMExposure);
			this->GroupCamera1->Controls->Add(this->camera1_btnSetTMInternalExposure);
			this->GroupCamera1->Controls->Add(this->lblCam1InternalExposure);
			this->GroupCamera1->Controls->Add(this->camera1_comboTMPolarity);
			this->GroupCamera1->Controls->Add(this->camera1_comboTM);
			this->GroupCamera1->Controls->Add(this->lblCam1TMPolarity);
			this->GroupCamera1->Controls->Add(this->lblCam1TriggerMode);
			this->GroupCamera1->Controls->Add(this->groupCam1ImageSize);
			this->GroupCamera1->Controls->Add(this->camera1_btnSetImageDataBits);
			this->GroupCamera1->Controls->Add(this->camera1_comboImageDataBits);
			this->GroupCamera1->Controls->Add(this->lblCam1ImageDataBits);
			this->GroupCamera1->Controls->Add(this->camera1_txtImageDataBits);
			this->GroupCamera1->Controls->Add(this->camera1_btnSetBitMapBits);
			this->GroupCamera1->Controls->Add(this->camera1_comboBitMapBits);
			this->GroupCamera1->Controls->Add(this->lblCam1BitMapBits);
			this->GroupCamera1->Controls->Add(this->camera1_txtBitMapBitsType);
			this->GroupCamera1->Controls->Add(this->camera1_txtInfoBox);
			this->GroupCamera1->Location = System::Drawing::Point(6, 11);
			this->GroupCamera1->Name = L"GroupCamera1";
			this->GroupCamera1->Size = System::Drawing::Size(424, 419);
			this->GroupCamera1->TabIndex = 1;
			this->GroupCamera1->TabStop = false;
			this->GroupCamera1->Text = L"Camera1";
			// 
			// Camera_90degreeRotation_checkBox
			// 
			this->Camera_90degreeRotation_checkBox->AutoSize = true;
			this->Camera_90degreeRotation_checkBox->Location = System::Drawing::Point(282, 230);
			this->Camera_90degreeRotation_checkBox->Name = L"Camera_90degreeRotation_checkBox";
			this->Camera_90degreeRotation_checkBox->Size = System::Drawing::Size(112, 17);
			this->Camera_90degreeRotation_checkBox->TabIndex = 32;
			this->Camera_90degreeRotation_checkBox->Text = L"90-degree rotation";
			this->Camera_90degreeRotation_checkBox->UseVisualStyleBackColor = true;
			this->Camera_90degreeRotation_checkBox->CheckedChanged += gcnew System::EventHandler(this, &MainForm::Camera_90degreeRotation_checkBox_CheckedChanged_1);
			// 
			// camera1_numCurrentTMExposure
			// 
			this->camera1_numCurrentTMExposure->DecimalPlaces = 3;
			this->camera1_numCurrentTMExposure->Location = System::Drawing::Point(147, 355);
			this->camera1_numCurrentTMExposure->Margin = System::Windows::Forms::Padding(2);
			this->camera1_numCurrentTMExposure->Name = L"camera1_numCurrentTMExposure";
			this->camera1_numCurrentTMExposure->ReadOnly = true;
			this->camera1_numCurrentTMExposure->Size = System::Drawing::Size(114, 20);
			this->camera1_numCurrentTMExposure->TabIndex = 31;
			// 
			// camera1_numSetTMExposure
			// 
			this->camera1_numSetTMExposure->DecimalPlaces = 3;
			this->camera1_numSetTMExposure->Location = System::Drawing::Point(268, 355);
			this->camera1_numSetTMExposure->Margin = System::Windows::Forms::Padding(2);
			this->camera1_numSetTMExposure->Name = L"camera1_numSetTMExposure";
			this->camera1_numSetTMExposure->Size = System::Drawing::Size(90, 20);
			this->camera1_numSetTMExposure->TabIndex = 30;
			this->camera1_numSetTMExposure->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100, 0, 0, 0 });
			// 
			// camera1_btnSetTMInternalExposure
			// 
			this->camera1_btnSetTMInternalExposure->Location = System::Drawing::Point(368, 355);
			this->camera1_btnSetTMInternalExposure->Name = L"camera1_btnSetTMInternalExposure";
			this->camera1_btnSetTMInternalExposure->Size = System::Drawing::Size(44, 20);
			this->camera1_btnSetTMInternalExposure->TabIndex = 12;
			this->camera1_btnSetTMInternalExposure->Text = L"Set";
			this->camera1_btnSetTMInternalExposure->UseVisualStyleBackColor = true;
			this->camera1_btnSetTMInternalExposure->Click += gcnew System::EventHandler(this, &MainForm::camera1_SetInternalExposure_Click);
			// 
			// lblCam1InternalExposure
			// 
			this->lblCam1InternalExposure->AutoSize = true;
			this->lblCam1InternalExposure->Location = System::Drawing::Point(7, 358);
			this->lblCam1InternalExposure->Name = L"lblCam1InternalExposure";
			this->lblCam1InternalExposure->Size = System::Drawing::Size(130, 13);
			this->lblCam1InternalExposure->TabIndex = 20;
			this->lblCam1InternalExposure->Text = L"TM Internal Exposure (ms)";
			// 
			// camera1_comboTMPolarity
			// 
			this->camera1_comboTMPolarity->FormattingEnabled = true;
			this->camera1_comboTMPolarity->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"Negative", L"Positive" });
			this->camera1_comboTMPolarity->Location = System::Drawing::Point(334, 300);
			this->camera1_comboTMPolarity->Name = L"camera1_comboTMPolarity";
			this->camera1_comboTMPolarity->Size = System::Drawing::Size(78, 21);
			this->camera1_comboTMPolarity->TabIndex = 19;
			this->camera1_comboTMPolarity->DropDownClosed += gcnew System::EventHandler(this, &MainForm::camera1_comboTMPolarity_DropDownClosed);
			// 
			// camera1_comboTM
			// 
			this->camera1_comboTM->FormattingEnabled = true;
			this->camera1_comboTM->Items->AddRange(gcnew cli::array< System::Object^  >(3) { L"Internal", L"External - Edge", L"External - Level" });
			this->camera1_comboTM->Location = System::Drawing::Point(147, 301);
			this->camera1_comboTM->Name = L"camera1_comboTM";
			this->camera1_comboTM->Size = System::Drawing::Size(115, 21);
			this->camera1_comboTM->TabIndex = 17;
			this->camera1_comboTM->Text = L"Internal trigger";
			this->camera1_comboTM->DropDownClosed += gcnew System::EventHandler(this, &MainForm::camera1_comboTM_DropDownClosed);
			// 
			// lblCam1TMPolarity
			// 
			this->lblCam1TMPolarity->AutoSize = true;
			this->lblCam1TMPolarity->Location = System::Drawing::Point(268, 304);
			this->lblCam1TMPolarity->Name = L"lblCam1TMPolarity";
			this->lblCam1TMPolarity->Size = System::Drawing::Size(60, 13);
			this->lblCam1TMPolarity->TabIndex = 14;
			this->lblCam1TMPolarity->Text = L"TM Polarity";
			// 
			// lblCam1TriggerMode
			// 
			this->lblCam1TriggerMode->AutoSize = true;
			this->lblCam1TriggerMode->Location = System::Drawing::Point(7, 304);
			this->lblCam1TriggerMode->Name = L"lblCam1TriggerMode";
			this->lblCam1TriggerMode->Size = System::Drawing::Size(134, 13);
			this->lblCam1TriggerMode->TabIndex = 10;
			this->lblCam1TriggerMode->Text = L"Camera Trigger Mode (TM)";
			// 
			// groupCam1ImageSize
			// 
			this->groupCam1ImageSize->Controls->Add(this->camera1_btnImageSetSize);
			this->groupCam1ImageSize->Controls->Add(this->camera1_txtImageYOffSet);
			this->groupCam1ImageSize->Controls->Add(this->lblCam1ImageYOffSet);
			this->groupCam1ImageSize->Controls->Add(this->camera1_txtImageYSize);
			this->groupCam1ImageSize->Controls->Add(this->lblCam1ImageYSize);
			this->groupCam1ImageSize->Controls->Add(this->camera1_txtImageXOffSet);
			this->groupCam1ImageSize->Controls->Add(this->lblCam1ImageXOffSet);
			this->groupCam1ImageSize->Controls->Add(this->camera1_txtImageXSize);
			this->groupCam1ImageSize->Controls->Add(this->lblCam1ImageXSize);
			this->groupCam1ImageSize->Location = System::Drawing::Point(10, 193);
			this->groupCam1ImageSize->Name = L"groupCam1ImageSize";
			this->groupCam1ImageSize->Size = System::Drawing::Size(252, 86);
			this->groupCam1ImageSize->TabIndex = 9;
			this->groupCam1ImageSize->TabStop = false;
			this->groupCam1ImageSize->Text = L"Image Size";
			// 
			// camera1_btnImageSetSize
			// 
			this->camera1_btnImageSetSize->Location = System::Drawing::Point(201, 21);
			this->camera1_btnImageSetSize->Name = L"camera1_btnImageSetSize";
			this->camera1_btnImageSetSize->Size = System::Drawing::Size(44, 46);
			this->camera1_btnImageSetSize->TabIndex = 10;
			this->camera1_btnImageSetSize->Text = L"Set";
			this->camera1_btnImageSetSize->UseVisualStyleBackColor = true;
			this->camera1_btnImageSetSize->Click += gcnew System::EventHandler(this, &MainForm::camera1_ImageSetImage_Click);
			// 
			// camera1_txtImageYOffSet
			// 
			this->camera1_txtImageYOffSet->Location = System::Drawing::Point(145, 47);
			this->camera1_txtImageYOffSet->Name = L"camera1_txtImageYOffSet";
			this->camera1_txtImageYOffSet->ReadOnly = true;
			this->camera1_txtImageYOffSet->Size = System::Drawing::Size(50, 20);
			this->camera1_txtImageYOffSet->TabIndex = 10;
			// 
			// lblCam1ImageYOffSet
			// 
			this->lblCam1ImageYOffSet->AutoSize = true;
			this->lblCam1ImageYOffSet->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->lblCam1ImageYOffSet->Location = System::Drawing::Point(85, 48);
			this->lblCam1ImageYOffSet->Name = L"lblCam1ImageYOffSet";
			this->lblCam1ImageYOffSet->Size = System::Drawing::Size(54, 16);
			this->lblCam1ImageYOffSet->TabIndex = 9;
			this->lblCam1ImageYOffSet->Text = L"Y-Offset";
			// 
			// camera1_txtImageYSize
			// 
			this->camera1_txtImageYSize->Location = System::Drawing::Point(29, 47);
			this->camera1_txtImageYSize->Name = L"camera1_txtImageYSize";
			this->camera1_txtImageYSize->ReadOnly = true;
			this->camera1_txtImageYSize->Size = System::Drawing::Size(50, 20);
			this->camera1_txtImageYSize->TabIndex = 7;
			// 
			// lblCam1ImageYSize
			// 
			this->lblCam1ImageYSize->AutoSize = true;
			this->lblCam1ImageYSize->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->lblCam1ImageYSize->Location = System::Drawing::Point(7, 47);
			this->lblCam1ImageYSize->Name = L"lblCam1ImageYSize";
			this->lblCam1ImageYSize->Size = System::Drawing::Size(16, 16);
			this->lblCam1ImageYSize->TabIndex = 6;
			this->lblCam1ImageYSize->Text = L"Y";
			// 
			// camera1_txtImageXOffSet
			// 
			this->camera1_txtImageXOffSet->Location = System::Drawing::Point(145, 21);
			this->camera1_txtImageXOffSet->Name = L"camera1_txtImageXOffSet";
			this->camera1_txtImageXOffSet->ReadOnly = true;
			this->camera1_txtImageXOffSet->Size = System::Drawing::Size(50, 20);
			this->camera1_txtImageXOffSet->TabIndex = 4;
			// 
			// lblCam1ImageXOffSet
			// 
			this->lblCam1ImageXOffSet->AutoSize = true;
			this->lblCam1ImageXOffSet->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->lblCam1ImageXOffSet->Location = System::Drawing::Point(85, 22);
			this->lblCam1ImageXOffSet->Name = L"lblCam1ImageXOffSet";
			this->lblCam1ImageXOffSet->Size = System::Drawing::Size(53, 16);
			this->lblCam1ImageXOffSet->TabIndex = 3;
			this->lblCam1ImageXOffSet->Text = L"X-Offset";
			// 
			// camera1_txtImageXSize
			// 
			this->camera1_txtImageXSize->Location = System::Drawing::Point(29, 21);
			this->camera1_txtImageXSize->Name = L"camera1_txtImageXSize";
			this->camera1_txtImageXSize->ReadOnly = true;
			this->camera1_txtImageXSize->Size = System::Drawing::Size(50, 20);
			this->camera1_txtImageXSize->TabIndex = 1;
			// 
			// lblCam1ImageXSize
			// 
			this->lblCam1ImageXSize->AutoSize = true;
			this->lblCam1ImageXSize->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->lblCam1ImageXSize->Location = System::Drawing::Point(7, 21);
			this->lblCam1ImageXSize->Name = L"lblCam1ImageXSize";
			this->lblCam1ImageXSize->Size = System::Drawing::Size(15, 16);
			this->lblCam1ImageXSize->TabIndex = 0;
			this->lblCam1ImageXSize->Text = L"X";
			// 
			// camera1_btnSetImageDataBits
			// 
			this->camera1_btnSetImageDataBits->Location = System::Drawing::Point(255, 153);
			this->camera1_btnSetImageDataBits->Name = L"camera1_btnSetImageDataBits";
			this->camera1_btnSetImageDataBits->Size = System::Drawing::Size(75, 23);
			this->camera1_btnSetImageDataBits->TabIndex = 8;
			this->camera1_btnSetImageDataBits->Text = L"Set";
			this->camera1_btnSetImageDataBits->UseVisualStyleBackColor = true;
			this->camera1_btnSetImageDataBits->Click += gcnew System::EventHandler(this, &MainForm::camera1_SetImageDataBits_Click);
			// 
			// camera1_comboImageDataBits
			// 
			this->camera1_comboImageDataBits->FormattingEnabled = true;
			this->camera1_comboImageDataBits->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"8-bit", L"16-bit" });
			this->camera1_comboImageDataBits->Location = System::Drawing::Point(128, 155);
			this->camera1_comboImageDataBits->Name = L"camera1_comboImageDataBits";
			this->camera1_comboImageDataBits->Size = System::Drawing::Size(121, 21);
			this->camera1_comboImageDataBits->TabIndex = 7;
			// 
			// lblCam1ImageDataBits
			// 
			this->lblCam1ImageDataBits->AutoSize = true;
			this->lblCam1ImageDataBits->Location = System::Drawing::Point(7, 140);
			this->lblCam1ImageDataBits->Name = L"lblCam1ImageDataBits";
			this->lblCam1ImageDataBits->Size = System::Drawing::Size(82, 13);
			this->lblCam1ImageDataBits->TabIndex = 6;
			this->lblCam1ImageDataBits->Text = L"Image Data Bits";
			// 
			// camera1_txtImageDataBits
			// 
			this->camera1_txtImageDataBits->BackColor = System::Drawing::SystemColors::Control;
			this->camera1_txtImageDataBits->Location = System::Drawing::Point(7, 156);
			this->camera1_txtImageDataBits->Name = L"camera1_txtImageDataBits";
			this->camera1_txtImageDataBits->ReadOnly = true;
			this->camera1_txtImageDataBits->Size = System::Drawing::Size(115, 20);
			this->camera1_txtImageDataBits->TabIndex = 5;
			// 
			// camera1_btnSetBitMapBits
			// 
			this->camera1_btnSetBitMapBits->Location = System::Drawing::Point(255, 114);
			this->camera1_btnSetBitMapBits->Name = L"camera1_btnSetBitMapBits";
			this->camera1_btnSetBitMapBits->Size = System::Drawing::Size(75, 23);
			this->camera1_btnSetBitMapBits->TabIndex = 4;
			this->camera1_btnSetBitMapBits->Text = L"Set";
			this->camera1_btnSetBitMapBits->UseVisualStyleBackColor = true;
			this->camera1_btnSetBitMapBits->Click += gcnew System::EventHandler(this, &MainForm::camera1_SetBitMapBits_Click);
			// 
			// camera1_comboBitMapBits
			// 
			this->camera1_comboBitMapBits->FormattingEnabled = true;
			this->camera1_comboBitMapBits->Items->AddRange(gcnew cli::array< System::Object^  >(1) { L"8-bit" });
			this->camera1_comboBitMapBits->Location = System::Drawing::Point(128, 116);
			this->camera1_comboBitMapBits->Name = L"camera1_comboBitMapBits";
			this->camera1_comboBitMapBits->Size = System::Drawing::Size(121, 21);
			this->camera1_comboBitMapBits->TabIndex = 3;
			// 
			// lblCam1BitMapBits
			// 
			this->lblCam1BitMapBits->AutoSize = true;
			this->lblCam1BitMapBits->Location = System::Drawing::Point(7, 101);
			this->lblCam1BitMapBits->Name = L"lblCam1BitMapBits";
			this->lblCam1BitMapBits->Size = System::Drawing::Size(86, 13);
			this->lblCam1BitMapBits->TabIndex = 2;
			this->lblCam1BitMapBits->Text = L"Bitmap Bits Type";
			// 
			// camera1_txtBitMapBitsType
			// 
			this->camera1_txtBitMapBitsType->BackColor = System::Drawing::SystemColors::Control;
			this->camera1_txtBitMapBitsType->Location = System::Drawing::Point(7, 117);
			this->camera1_txtBitMapBitsType->Name = L"camera1_txtBitMapBitsType";
			this->camera1_txtBitMapBitsType->ReadOnly = true;
			this->camera1_txtBitMapBitsType->Size = System::Drawing::Size(115, 20);
			this->camera1_txtBitMapBitsType->TabIndex = 1;
			// 
			// camera1_txtInfoBox
			// 
			this->camera1_txtInfoBox->BackColor = System::Drawing::SystemColors::Info;
			this->camera1_txtInfoBox->Location = System::Drawing::Point(7, 23);
			this->camera1_txtInfoBox->Multiline = true;
			this->camera1_txtInfoBox->Name = L"camera1_txtInfoBox";
			this->camera1_txtInfoBox->ReadOnly = true;
			this->camera1_txtInfoBox->Size = System::Drawing::Size(397, 69);
			this->camera1_txtInfoBox->TabIndex = 0;
			// 
			// TabControls
			// 
			this->TabControls->Controls->Add(this->ScannerTab);
			this->TabControls->Controls->Add(this->CameraTab);
			this->TabControls->Controls->Add(this->StageTab);
			this->TabControls->Controls->Add(this->ExperimentTab);
			this->TabControls->Location = System::Drawing::Point(0, 28);
			this->TabControls->Name = L"TabControls";
			this->TabControls->SelectedIndex = 0;
			this->TabControls->Size = System::Drawing::Size(725, 510);
			this->TabControls->TabIndex = 1;
			// 
			// ScannerTab
			// 
			this->ScannerTab->Controls->Add(this->scanner_btnSetGain);
			this->ScannerTab->Controls->Add(this->scanner_numGain);
			this->ScannerTab->Controls->Add(this->scanner_numSetJumpDelay);
			this->ScannerTab->Controls->Add(this->lblJumpDelay);
			this->ScannerTab->Controls->Add(this->GroupSingleScanCycleTime);
			this->ScannerTab->Controls->Add(this->GroupScanJob);
			this->ScannerTab->Controls->Add(this->scanner_txtActionBox);
			this->ScannerTab->Controls->Add(this->scanner_txtInfoBox);
			this->ScannerTab->Controls->Add(this->lblMarkSpeed);
			this->ScannerTab->Controls->Add(this->lblJumpSpeed);
			this->ScannerTab->Controls->Add(this->scanner_numSetMarkTime);
			this->ScannerTab->Controls->Add(this->scanner_numSetJumpTime);
			this->ScannerTab->Controls->Add(this->scanner_btnSetSpeed);
			this->ScannerTab->Controls->Add(this->btnInitialiseScanner);
			this->ScannerTab->Location = System::Drawing::Point(4, 22);
			this->ScannerTab->Name = L"ScannerTab";
			this->ScannerTab->Padding = System::Windows::Forms::Padding(3);
			this->ScannerTab->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->ScannerTab->Size = System::Drawing::Size(717, 484);
			this->ScannerTab->TabIndex = 2;
			this->ScannerTab->Text = L"Scanner";
			this->ScannerTab->UseVisualStyleBackColor = true;
			// 
			// scanner_btnSetGain
			// 
			this->scanner_btnSetGain->Location = System::Drawing::Point(280, 13);
			this->scanner_btnSetGain->Name = L"scanner_btnSetGain";
			this->scanner_btnSetGain->Size = System::Drawing::Size(75, 23);
			this->scanner_btnSetGain->TabIndex = 92;
			this->scanner_btnSetGain->Text = L"Set Gain (%)";
			this->scanner_btnSetGain->UseVisualStyleBackColor = true;
			this->scanner_btnSetGain->Click += gcnew System::EventHandler(this, &MainForm::scanner_btnSetGain_Click);
			// 
			// scanner_numGain
			// 
			this->scanner_numGain->Location = System::Drawing::Point(280, 45);
			this->scanner_numGain->Name = L"scanner_numGain";
			this->scanner_numGain->Size = System::Drawing::Size(61, 20);
			this->scanner_numGain->TabIndex = 91;
			this->scanner_numGain->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10, 0, 0, 0 });
			// 
			// scanner_numSetJumpDelay
			// 
			this->scanner_numSetJumpDelay->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10, 0, 0, 0 });
			this->scanner_numSetJumpDelay->Location = System::Drawing::Point(318, 108);
			this->scanner_numSetJumpDelay->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 429496729, 0, 0, 0 });
			this->scanner_numSetJumpDelay->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10, 0, 0, 0 });
			this->scanner_numSetJumpDelay->Name = L"scanner_numSetJumpDelay";
			this->scanner_numSetJumpDelay->Size = System::Drawing::Size(82, 20);
			this->scanner_numSetJumpDelay->TabIndex = 89;
			this->scanner_numSetJumpDelay->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->scanner_numSetJumpDelay->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10, 0, 0, 0 });
			// 
			// lblJumpDelay
			// 
			this->lblJumpDelay->AutoSize = true;
			this->lblJumpDelay->Location = System::Drawing::Point(315, 91);
			this->lblJumpDelay->Name = L"lblJumpDelay";
			this->lblJumpDelay->Size = System::Drawing::Size(85, 13);
			this->lblJumpDelay->TabIndex = 88;
			this->lblJumpDelay->Text = L"Jump Delay (us):";
			// 
			// GroupSingleScanCycleTime
			// 
			this->GroupSingleScanCycleTime->Controls->Add(this->scanner_radioScanJobDualChannel);
			this->GroupSingleScanCycleTime->Controls->Add(this->scanner_radioScanJobSingleChannel);
			this->GroupSingleScanCycleTime->Controls->Add(this->scanner_txtCalculateSST);
			this->GroupSingleScanCycleTime->Controls->Add(this->scanner_btnSSTCalculate);
			this->GroupSingleScanCycleTime->Location = System::Drawing::Point(9, 293);
			this->GroupSingleScanCycleTime->Name = L"GroupSingleScanCycleTime";
			this->GroupSingleScanCycleTime->Size = System::Drawing::Size(459, 100);
			this->GroupSingleScanCycleTime->TabIndex = 87;
			this->GroupSingleScanCycleTime->TabStop = false;
			this->GroupSingleScanCycleTime->Text = L"Single Scan Cycle Time";
			// 
			// scanner_radioScanJobDualChannel
			// 
			this->scanner_radioScanJobDualChannel->AutoSize = true;
			this->scanner_radioScanJobDualChannel->Checked = true;
			this->scanner_radioScanJobDualChannel->Location = System::Drawing::Point(289, 60);
			this->scanner_radioScanJobDualChannel->Name = L"scanner_radioScanJobDualChannel";
			this->scanner_radioScanJobDualChannel->Size = System::Drawing::Size(137, 17);
			this->scanner_radioScanJobDualChannel->TabIndex = 3;
			this->scanner_radioScanJobDualChannel->TabStop = true;
			this->scanner_radioScanJobDualChannel->Text = L"Scanjob_Dual_Channel";
			this->scanner_radioScanJobDualChannel->UseVisualStyleBackColor = true;
			// 
			// scanner_radioScanJobSingleChannel
			// 
			this->scanner_radioScanJobSingleChannel->AutoSize = true;
			this->scanner_radioScanJobSingleChannel->Location = System::Drawing::Point(289, 28);
			this->scanner_radioScanJobSingleChannel->Name = L"scanner_radioScanJobSingleChannel";
			this->scanner_radioScanJobSingleChannel->Size = System::Drawing::Size(144, 17);
			this->scanner_radioScanJobSingleChannel->TabIndex = 2;
			this->scanner_radioScanJobSingleChannel->Text = L"Scanjob_Single_Channel";
			this->scanner_radioScanJobSingleChannel->UseVisualStyleBackColor = true;
			// 
			// scanner_txtCalculateSST
			// 
			this->scanner_txtCalculateSST->Enabled = false;
			this->scanner_txtCalculateSST->Location = System::Drawing::Point(139, 43);
			this->scanner_txtCalculateSST->Name = L"scanner_txtCalculateSST";
			this->scanner_txtCalculateSST->Size = System::Drawing::Size(100, 20);
			this->scanner_txtCalculateSST->TabIndex = 1;
			this->scanner_txtCalculateSST->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// scanner_btnSSTCalculate
			// 
			this->scanner_btnSSTCalculate->Location = System::Drawing::Point(33, 43);
			this->scanner_btnSSTCalculate->Name = L"scanner_btnSSTCalculate";
			this->scanner_btnSSTCalculate->Size = System::Drawing::Size(82, 23);
			this->scanner_btnSSTCalculate->TabIndex = 0;
			this->scanner_btnSSTCalculate->Text = L"calculate (ms)";
			this->scanner_btnSSTCalculate->UseVisualStyleBackColor = true;
			this->scanner_btnSSTCalculate->Click += gcnew System::EventHandler(this, &MainForm::scanner_btnSSTCalculate_Click);
			// 
			// GroupScanJob
			// 
			this->GroupScanJob->Controls->Add(this->scanner_numRepeat);
			this->GroupScanJob->Controls->Add(this->Scanner_Loop_checkBox);
			this->GroupScanJob->Controls->Add(this->labelScannerRepeat);
			this->GroupScanJob->Controls->Add(this->labelScannerY2);
			this->GroupScanJob->Controls->Add(this->labelScannerY1);
			this->GroupScanJob->Controls->Add(this->labelScannerX2);
			this->GroupScanJob->Controls->Add(this->labelScannerX1);
			this->GroupScanJob->Controls->Add(this->scanner_numX2);
			this->GroupScanJob->Controls->Add(this->scanner_numX1);
			this->GroupScanJob->Controls->Add(this->scanner_numY1);
			this->GroupScanJob->Controls->Add(this->scanner_numY2);
			this->GroupScanJob->Controls->Add(this->scanner_btnRun);
			this->GroupScanJob->Controls->Add(this->scanner_btnStop);
			this->GroupScanJob->Location = System::Drawing::Point(9, 140);
			this->GroupScanJob->Name = L"GroupScanJob";
			this->GroupScanJob->Size = System::Drawing::Size(459, 147);
			this->GroupScanJob->TabIndex = 85;
			this->GroupScanJob->TabStop = false;
			this->GroupScanJob->Text = L"Start Job";
			// 
			// scanner_numRepeat
			// 
			this->scanner_numRepeat->Location = System::Drawing::Point(284, 37);
			this->scanner_numRepeat->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5000, 0, 0, 0 });
			this->scanner_numRepeat->Name = L"scanner_numRepeat";
			this->scanner_numRepeat->Size = System::Drawing::Size(48, 20);
			this->scanner_numRepeat->TabIndex = 63;
			this->scanner_numRepeat->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// Scanner_Loop_checkBox
			// 
			this->Scanner_Loop_checkBox->AutoSize = true;
			this->Scanner_Loop_checkBox->Checked = true;
			this->Scanner_Loop_checkBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->Scanner_Loop_checkBox->Location = System::Drawing::Point(284, 75);
			this->Scanner_Loop_checkBox->Name = L"Scanner_Loop_checkBox";
			this->Scanner_Loop_checkBox->Size = System::Drawing::Size(50, 17);
			this->Scanner_Loop_checkBox->TabIndex = 62;
			this->Scanner_Loop_checkBox->Text = L"Loop";
			this->Scanner_Loop_checkBox->UseVisualStyleBackColor = true;
			// 
			// labelScannerRepeat
			// 
			this->labelScannerRepeat->AutoSize = true;
			this->labelScannerRepeat->Location = System::Drawing::Point(281, 21);
			this->labelScannerRepeat->Name = L"labelScannerRepeat";
			this->labelScannerRepeat->Size = System::Drawing::Size(42, 13);
			this->labelScannerRepeat->TabIndex = 61;
			this->labelScannerRepeat->Text = L"Repeat";
			// 
			// labelScannerY2
			// 
			this->labelScannerY2->AutoSize = true;
			this->labelScannerY2->Location = System::Drawing::Point(141, 76);
			this->labelScannerY2->Name = L"labelScannerY2";
			this->labelScannerY2->Size = System::Drawing::Size(20, 13);
			this->labelScannerY2->TabIndex = 3;
			this->labelScannerY2->Text = L"Y2";
			// 
			// labelScannerY1
			// 
			this->labelScannerY1->AutoSize = true;
			this->labelScannerY1->Location = System::Drawing::Point(141, 28);
			this->labelScannerY1->Name = L"labelScannerY1";
			this->labelScannerY1->Size = System::Drawing::Size(20, 13);
			this->labelScannerY1->TabIndex = 2;
			this->labelScannerY1->Text = L"Y1";
			// 
			// labelScannerX2
			// 
			this->labelScannerX2->AutoSize = true;
			this->labelScannerX2->Location = System::Drawing::Point(7, 76);
			this->labelScannerX2->Name = L"labelScannerX2";
			this->labelScannerX2->Size = System::Drawing::Size(20, 13);
			this->labelScannerX2->TabIndex = 1;
			this->labelScannerX2->Text = L"X2";
			// 
			// labelScannerX1
			// 
			this->labelScannerX1->AutoSize = true;
			this->labelScannerX1->Location = System::Drawing::Point(7, 28);
			this->labelScannerX1->Name = L"labelScannerX1";
			this->labelScannerX1->Size = System::Drawing::Size(20, 13);
			this->labelScannerX1->TabIndex = 0;
			this->labelScannerX1->Text = L"X1";
			// 
			// scanner_numX2
			// 
			this->scanner_numX2->Location = System::Drawing::Point(33, 74);
			this->scanner_numX2->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 8388607, 0, 0, 0 });
			this->scanner_numX2->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 8388608, 0, 0, System::Int32::MinValue });
			this->scanner_numX2->Name = L"scanner_numX2";
			this->scanner_numX2->Size = System::Drawing::Size(82, 20);
			this->scanner_numX2->TabIndex = 60;
			this->scanner_numX2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->scanner_numX2->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 4000, 0, 0, System::Int32::MinValue });
			// 
			// scanner_numX1
			// 
			this->scanner_numX1->Location = System::Drawing::Point(33, 26);
			this->scanner_numX1->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 8388607, 0, 0, 0 });
			this->scanner_numX1->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 8388608, 0, 0, System::Int32::MinValue });
			this->scanner_numX1->Name = L"scanner_numX1";
			this->scanner_numX1->Size = System::Drawing::Size(82, 20);
			this->scanner_numX1->TabIndex = 56;
			this->scanner_numX1->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->scanner_numX1->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 4000, 0, 0, 0 });
			// 
			// scanner_numY1
			// 
			this->scanner_numY1->Location = System::Drawing::Point(168, 26);
			this->scanner_numY1->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 8388607, 0, 0, 0 });
			this->scanner_numY1->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 8388608, 0, 0, System::Int32::MinValue });
			this->scanner_numY1->Name = L"scanner_numY1";
			this->scanner_numY1->Size = System::Drawing::Size(82, 20);
			this->scanner_numY1->TabIndex = 55;
			this->scanner_numY1->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->scanner_numY1->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 150, 0, 0, 0 });
			// 
			// scanner_numY2
			// 
			this->scanner_numY2->Location = System::Drawing::Point(168, 74);
			this->scanner_numY2->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 8388607, 0, 0, 0 });
			this->scanner_numY2->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 8388608, 0, 0, System::Int32::MinValue });
			this->scanner_numY2->Name = L"scanner_numY2";
			this->scanner_numY2->Size = System::Drawing::Size(82, 20);
			this->scanner_numY2->TabIndex = 59;
			this->scanner_numY2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->scanner_numY2->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 150, 0, 0, System::Int32::MinValue });
			// 
			// scanner_btnRun
			// 
			this->scanner_btnRun->Enabled = false;
			this->scanner_btnRun->Location = System::Drawing::Point(359, 34);
			this->scanner_btnRun->Name = L"scanner_btnRun";
			this->scanner_btnRun->Size = System::Drawing::Size(70, 23);
			this->scanner_btnRun->TabIndex = 63;
			this->scanner_btnRun->Text = L"Run";
			this->scanner_btnRun->UseVisualStyleBackColor = true;
			this->scanner_btnRun->Click += gcnew System::EventHandler(this, &MainForm::scanner_btnRun_Click);
			// 
			// scanner_btnStop
			// 
			this->scanner_btnStop->BackColor = System::Drawing::Color::Transparent;
			this->scanner_btnStop->Enabled = false;
			this->scanner_btnStop->Location = System::Drawing::Point(359, 71);
			this->scanner_btnStop->Name = L"scanner_btnStop";
			this->scanner_btnStop->Size = System::Drawing::Size(70, 23);
			this->scanner_btnStop->TabIndex = 64;
			this->scanner_btnStop->Text = L"Stop ";
			this->scanner_btnStop->UseVisualStyleBackColor = false;
			this->scanner_btnStop->Click += gcnew System::EventHandler(this, &MainForm::scanner_btnStop_Click);
			// 
			// scanner_txtActionBox
			// 
			this->scanner_txtActionBox->BackColor = System::Drawing::SystemColors::Window;
			this->scanner_txtActionBox->Location = System::Drawing::Point(3, 457);
			this->scanner_txtActionBox->Name = L"scanner_txtActionBox";
			this->scanner_txtActionBox->Size = System::Drawing::Size(708, 20);
			this->scanner_txtActionBox->TabIndex = 40;
			// 
			// scanner_txtInfoBox
			// 
			this->scanner_txtInfoBox->BackColor = System::Drawing::SystemColors::Info;
			this->scanner_txtInfoBox->Location = System::Drawing::Point(84, 14);
			this->scanner_txtInfoBox->Multiline = true;
			this->scanner_txtInfoBox->Name = L"scanner_txtInfoBox";
			this->scanner_txtInfoBox->Size = System::Drawing::Size(164, 68);
			this->scanner_txtInfoBox->TabIndex = 39;
			// 
			// lblMarkSpeed
			// 
			this->lblMarkSpeed->AutoSize = true;
			this->lblMarkSpeed->Location = System::Drawing::Point(217, 91);
			this->lblMarkSpeed->Name = L"lblMarkSpeed";
			this->lblMarkSpeed->Size = System::Drawing::Size(82, 13);
			this->lblMarkSpeed->TabIndex = 51;
			this->lblMarkSpeed->Text = L"Mark Time (ms):";
			// 
			// lblJumpSpeed
			// 
			this->lblJumpSpeed->AutoSize = true;
			this->lblJumpSpeed->Location = System::Drawing::Point(120, 91);
			this->lblJumpSpeed->Name = L"lblJumpSpeed";
			this->lblJumpSpeed->Size = System::Drawing::Size(83, 13);
			this->lblJumpSpeed->TabIndex = 50;
			this->lblJumpSpeed->Text = L"Jump Time (ms):";
			// 
			// scanner_numSetMarkTime
			// 
			this->scanner_numSetMarkTime->DecimalPlaces = 3;
			this->scanner_numSetMarkTime->Location = System::Drawing::Point(220, 108);
			this->scanner_numSetMarkTime->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 167772, 0, 0, 0 });
			this->scanner_numSetMarkTime->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5, 0, 0, 196608 });
			this->scanner_numSetMarkTime->Name = L"scanner_numSetMarkTime";
			this->scanner_numSetMarkTime->Size = System::Drawing::Size(82, 20);
			this->scanner_numSetMarkTime->TabIndex = 49;
			this->scanner_numSetMarkTime->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->scanner_numSetMarkTime->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 22, 0, 0, 0 });
			// 
			// scanner_numSetJumpTime
			// 
			this->scanner_numSetJumpTime->DecimalPlaces = 3;
			this->scanner_numSetJumpTime->Location = System::Drawing::Point(123, 108);
			this->scanner_numSetJumpTime->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 167772, 0, 0, 0 });
			this->scanner_numSetJumpTime->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5, 0, 0, 196608 });
			this->scanner_numSetJumpTime->Name = L"scanner_numSetJumpTime";
			this->scanner_numSetJumpTime->Size = System::Drawing::Size(82, 20);
			this->scanner_numSetJumpTime->TabIndex = 48;
			this->scanner_numSetJumpTime->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->scanner_numSetJumpTime->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 3, 0, 0, 0 });
			// 
			// scanner_btnSetSpeed
			// 
			this->scanner_btnSetSpeed->Enabled = false;
			this->scanner_btnSetSpeed->Location = System::Drawing::Point(6, 105);
			this->scanner_btnSetSpeed->Name = L"scanner_btnSetSpeed";
			this->scanner_btnSetSpeed->Size = System::Drawing::Size(103, 23);
			this->scanner_btnSetSpeed->TabIndex = 47;
			this->scanner_btnSetSpeed->Text = L"Set Time";
			this->scanner_btnSetSpeed->UseVisualStyleBackColor = true;
			this->scanner_btnSetSpeed->Click += gcnew System::EventHandler(this, &MainForm::scanner_btnSetSpeed_Click);
			// 
			// btnInitialiseScanner
			// 
			this->btnInitialiseScanner->Location = System::Drawing::Point(6, 13);
			this->btnInitialiseScanner->Name = L"btnInitialiseScanner";
			this->btnInitialiseScanner->Size = System::Drawing::Size(66, 68);
			this->btnInitialiseScanner->TabIndex = 35;
			this->btnInitialiseScanner->Text = L"Initialise";
			this->btnInitialiseScanner->UseVisualStyleBackColor = true;
			this->btnInitialiseScanner->Click += gcnew System::EventHandler(this, &MainForm::scanner_btnInitialiseScanner_Click);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(725, 541);
			this->Controls->Add(this->TabControls);
			this->Controls->Add(this->stripLiveCameras);
			this->Controls->Add(this->lblStageInfo);
			this->Name = L"MainForm";
			this->Text = L"DSLM";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MainForm::MainForm_FormClosing);
			this->Load += gcnew System::EventHandler(this, &MainForm::MainForm_Load);
			this->stripLiveCameras->ResumeLayout(false);
			this->stripLiveCameras->PerformLayout();
			this->ExperimentTab->ResumeLayout(false);
			this->ExperimentTab->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->experiment_numSST))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ExperimentSetup_travelTime))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ExperimentSetup_NoOfSteps))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->experiment_nudWaitingTime))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->experiment_numNumberOfTimePointsValue))->EndInit();
			this->StageTab->ResumeLayout(false);
			this->StageTab->PerformLayout();
			this->groupExperiementSetup->ResumeLayout(false);
			this->groupExperiementSetup->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->stage_nudY1Stage))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->stage_nudY2Stage))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->stage_zLimit))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->stage_numObjectiveMagnification))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->stage_numWaveXStep))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->stage_numXStep))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->stage_numZVelocity))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->stage_numYVelocity))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->stage_numYStep))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->stage_numZStep))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->stage_numXVelocity))->EndInit();
			this->CameraTab->ResumeLayout(false);
			this->CameraTab->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nudReferenceHeight))->EndInit();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nudBrightMax_ch2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nudBrightMin_ch2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nudBrightMax_ch1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nudBrightMin_ch1))->EndInit();
			this->GroupCamera1->ResumeLayout(false);
			this->GroupCamera1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->camera1_numCurrentTMExposure))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->camera1_numSetTMExposure))->EndInit();
			this->groupCam1ImageSize->ResumeLayout(false);
			this->groupCam1ImageSize->PerformLayout();
			this->TabControls->ResumeLayout(false);
			this->ScannerTab->ResumeLayout(false);
			this->ScannerTab->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->scanner_numGain))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->scanner_numSetJumpDelay))->EndInit();
			this->GroupSingleScanCycleTime->ResumeLayout(false);
			this->GroupSingleScanCycleTime->PerformLayout();
			this->GroupScanJob->ResumeLayout(false);
			this->GroupScanJob->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->scanner_numRepeat))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->scanner_numX2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->scanner_numX1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->scanner_numY1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->scanner_numY2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->scanner_numSetMarkTime))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->scanner_numSetJumpTime))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void MainForm_Shown(
		System::Object^ sender, System::EventArgs^ e)
	{
		Form^ starting = Application::OpenForms["DSLMStartingWindow"];

		if (starting != nullptr)
			starting->Close();
	}
	//When the mains power connected to the stage control boxes is turned off the Z stage has been knows to slam into the bottom of the stage.
	//To avoid this happening when the form is closed the Z stage will go to the lowest point it can so when the mains is turned off it wont drop very far and avoid damage.
	private: System::Void MainForm_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e) {
		char ZAxis[] = "1";

		StageFunctionsOK = StageFunctions::SetAxisVelocity(ControllerBox1ID, ZAxis, 5.0);

		AxisPosition = StageFunctions::GetAxisPosition(ControllerBox1ID, ZAxis);	//Check where the Z stage is currently sitting

		if (AxisPosition < 10)	//If the stage isn't at it's lowest point (-10) then we will want to lower it.
		{
			StageFunctionsOK = StageFunctions::MoveAxisToThisExactPosition(ControllerBox1ID, ZAxis, 9.9); //move the Z stage to the lowest point it can go (-10 may cause damage if the stage moves too quickly and overshoots)
		}
	}

	private: System::Void MainForm_Load(System::Object^ sender, System::EventArgs^ e)
	{
		camera_ActionBox->Text = "Camera is initializing...";

		NrOfCameras = CameraFunctions::CameraInitialization(); //Function checks how many cameras are connected to the computer
		if (NrOfCameras == 0) //If there are no cameras connected to the computer then the code returns an error
		{
			camera_ActionBox->Text = "No Cameras are connected...";
			camera_ActionBox->BackColor = System::Drawing::Color::FromArgb(255, 255, 0, 0);

			camera1_txtBitMapBitsType->Text = "No Camera";
			camera1_txtImageDataBits->Text = "No Camera";

			camera1_txtInfoBox->Text = "No Camera Avaliable";

			camera1_btnLiveImage->Enabled = false;

		}

		if (NrOfCameras == 1) //If there is only one camera connected then the start up commands only need to be run for one camera
		{

			camera_ActionBox->Text = "There is 1 Camera Connected";

			FCAM1 = CameraFunctions::CameraConnectedWithSuccess(0);	//Open Camera and return camera handle (HDCAM variable type)
			CameraFunctions::CAM1 = FCAM1;

			//Camera Information
			CameraFunctions::Camera1Model = CameraFunctions::GetCameraModel(0); //Camera model
			CameraFunctions::Camera1SN = CameraFunctions::GetCameraSN(0); //Camera serial number
			CameraFunctions::Camera1ConnectionType = CameraFunctions::GetCameraConnectionType(0); //Camera connection type (USB usually)

			camera1_txtInfoBox->Text = "Camera Model: " + CameraFunctions::Camera1Model +
				"\r\nCamera Serial Number: " + CameraFunctions::Camera1SN +
				"\r\nCamera Connect Type: " + CameraFunctions::Camera1ConnectionType;

			//Camera Bit Type
			CameraFunctions::Cam1BitmapBits = CameraFunctions::GetBitsType(FCAM1); //get bit data type, 8-bit (default)
			camera1_txtBitMapBitsType->Text = CameraFunctions::Cam1BitmapBits;
			camera1_comboBitMapBits->Text = CameraFunctions::Cam1BitmapBits;

			//Camera Data Type
			CameraFunctionOK = CameraFunctions::SetDataType(FCAM1, 8); //set camera 1 data type to 8-bit (this will effect frame rate)
			CameraFunctions::Cam1DataType = CameraFunctions::GetDataType(FCAM1); //get bit data type, 8-bit (default) and 16-bit are valid options
			camera1_txtImageDataBits->Text = CameraFunctions::Cam1DataType;
			camera1_comboImageDataBits->Text = CameraFunctions::Cam1DataType;

			//Camera Image Size
			CameraFunctionOK = CameraFunctions::SetImageFrame(FCAM1, 2048, 0, 400, 824); //set the frame for the camera to capture to x axis = 2048, x-offset = 0, y = 400, y-offset = 700
			CameraFunctions::Camera1ImagesSizesAndOffsets = CameraFunctions::GetImageFrame(FCAM1); //get the size of the image frame to display in text boxes
			camera1_txtImageXSize->Text = CameraFunctions::Camera1ImagesSizesAndOffsets[0].ToString();
			camera1_txtImageXOffSet->Text = CameraFunctions::Camera1ImagesSizesAndOffsets[1].ToString();
			camera1_txtImageYSize->Text = CameraFunctions::Camera1ImagesSizesAndOffsets[2].ToString();
			camera1_txtImageYOffSet->Text = CameraFunctions::Camera1ImagesSizesAndOffsets[3].ToString();

			//Camera Trigger Mode and polarity
			CameraFunctions::Camera1TriggerMode = CameraFunctions::GetCameraTriggerMode(FCAM1); //get default camera trigger mode (internal - triggered by function)
			CameraFunctions::Camera1TriggerModePolarity = CameraFunctions::GetCameraTriggerModePolarity(FCAM1); //get trigger mode for camera, this is used for edge and level trigger mode. positive high or rising, negative low or falling
			camera1_comboTM->Text = CameraFunctions::Camera1TriggerMode;
			camera1_comboTMPolarity->Text = CameraFunctions::Camera1TriggerModePolarity;

			if (CameraFunctions::Camera1TriggerMode != "Internal")
				camera1_btnSetTMInternalExposure->Enabled = false;

			//Camera Exposure Time
			CameraFunctions::Cam1Exposure = CameraFunctions::GetCameraExposureTime(FCAM1); //get default exposure time
			camera1_numCurrentTMExposure->Value = (int)CameraFunctions::Cam1Exposure*1000;

			//Camera min bright level
			Cam1BrightLevelMin = 0;
			FunctionsForImages::BrightLevelMin = Cam1BrightLevelMin;

			//Camera max bright level
			Cam1BrightLevelMax = 65535;
			FunctionsForImages::BrightLevelMax = Cam1BrightLevelMax;

			//Initialise look up table
			LookUpTable = (unsigned char*)malloc(65536 * sizeof(unsigned char));
			for (int o = 0; o < 65536; o++)
			{
				LookUpTable[o] = static_cast<unsigned char>(o * 255 / 65535);
			}

			FunctionsForImages::LookUT8bit = LookUpTable;		//Passing the 16-bit data type look up table, this only makes the image possible to be displayed

			//SET REFERENCE HEIGHT FOR LIVE IMAGE FORMS
			ImageRefHeightValue = (int)nudReferenceHeight->Value;
			LiveCam1::ImageReferenceHeight = ImageRefHeightValue;

		}

		experiment_txtInfoBox->Text = "TO RUN THE EXPERIMENT PLEASE CHECK THAT:"
			"\r\n1 - Camera 1 Live imaging window(s) should be opened before start the experiment."
			"\r\n2 - In Live imaging window check the Run Experiment box before start running the experiment."
			"\r\n3 - Check that camera(s) is (are) using external trigger the code is written assuming the Scanner triggers the camera.";

		this->txt_camera_infoFile->Text = "Expk0800_ACTN\r\nSample: Chick mGFP\r\nMedium: Albumen\r\nDual Color acquisition:\r\n   488nm - 20 mW \r\n   561nm - 20 mW\r\n\r\n\r\n\r\n\r\n";

	}

#pragma region	Camera Controls
	//These two functions open the live imaging forms
	private: System::Void CameraAcquisition_ProgressChanged(System::Object^ sender,System::ComponentModel::ProgressChangedEventArgs^ e)
	{
		if (e->UserState == nullptr || !ExperimentStarted)
			return;

		int scans = e->ProgressPercentage;
		int acquisition = safe_cast<int>(e->UserState);

		experiment_progressBarNumberOfScansPerTimePoint->Value =System::Math::Min(scans,experiment_progressBarNumberOfScansPerTimePoint->Maximum);

		int completedAcquisitions = acquisition - 1;

		if (scans == LiveCam1::NumberOfScansPerTimePointCam1)
			completedAcquisitions++;

		int positionsPerTimePoint =System::Math::Max(1, LiveCam1::Yloop);

		int completedTimePoints =completedAcquisitions / positionsPerTimePoint;

		experiment_progressBarExperiment->Value =System::Math::Min(completedTimePoints,experiment_progressBarExperiment->Maximum);
	}

	private: System::Void camera1_btnLiveImage_Click(System::Object^ sender,System::EventArgs^ e)
	{
		LiveCam1^ liveform1 = gcnew LiveCam1;

		liveform1->liveCam1_backgroundWorker->ProgressChanged +=gcnew System::ComponentModel::ProgressChangedEventHandler(this,&MainForm::CameraAcquisition_ProgressChanged);

		liveform1->Show(this);
	}

	//The following fuctions allow the user to change the settings for camera 1, camera 2 is futher down
	private: System::Void camera1_SetBitMapBits_Click(System::Object^ sender, System::EventArgs^ e)
	{
		if (camera1_comboBitMapBits->SelectedItem == "8-bit" && camera1_txtBitMapBitsType->Text != "8-bit") //A check is made to see if the bit map being set is already selected
		{
			CameraFunctionOK = CameraFunctions::SetBitsType(FCAM1, 8); //Set the bitmap type to 8 bits, will return true is successful
			if (CameraFunctionOK != FALSE)
			{
				camera1_txtBitMapBitsType->Text = CameraFunctions::GetBitsType(FCAM1); //Get the bitmap type and display in box
				camera_ActionBox->Text = "Camera 1 bit map bits type was changed to 8-bit";
			}
			else
				camera_ActionBox->Text = "Invalid bitmap type seclected"; //If bitmap type is invalid, display error
		}
		CameraFunctions::Cam1BitmapBits = CameraFunctions::GetBitsType(FCAM1); //update camera functions variable to reflect chosen bitmap type
	}
	private: System::Void camera1_SetImageDataBits_Click(System::Object^ sender, System::EventArgs^ e)
	{
		if (camera1_comboImageDataBits->SelectedItem == "8-bit" && camera1_txtImageDataBits->Text != "8-bit") //A check is made to see if the data type being set is already selected
		{
			CameraFunctionOK = CameraFunctions::SetDataType(FCAM1, 8); //set data type to 8-bit, will return true if successful
			if (CameraFunctionOK != FALSE)
			{
				camera1_txtImageDataBits->Text = CameraFunctions::GetDataType(FCAM1); //get data type and dislay in box
				camera_ActionBox->Text = "Camera 1 image data bits was changed to 8-bit";
			}
			else
				camera_ActionBox->Text = "Invalid data type seclected"; //If bitmap type is invalid, display error

		}

		if (camera1_comboImageDataBits->SelectedItem == "16-bit" && camera1_txtImageDataBits->Text != "16-bit") //A check is made to see if the data type being set is already selected
		{
			CameraFunctionOK = CameraFunctions::SetDataType(FCAM1, 16); //set data type to 16-bit, will return true if successful
			if (CameraFunctionOK != FALSE)
			{
				camera1_txtImageDataBits->Text = CameraFunctions::GetDataType(FCAM1); //get data type and dislay in box
				camera_ActionBox->Text = "Camera 1 image data bits was changed to 16-bit";
			}
			else
				camera_ActionBox->Text = "Invalid data type seclected"; //If bitmap type is invalid, display error
		}

		CameraFunctions::Cam1DataType = CameraFunctions::GetDataType(FCAM1); //update camera functions variable to reflect chosen data type
	}
	private: System::Void camera1_ImageSetImage_Click(System::Object^ sender, System::EventArgs^ e)
	{
		ImageSizeForm^ form = gcnew ImageSizeForm();
		form->ShowDialog();

		CameraFunctionOK = CameraFunctions::SetImageFrame(FCAM1, (int)ImageSizeForm::imageXSize, (int)ImageSizeForm::imageXOffSet, (int)ImageSizeForm::imageYSize, (int)ImageSizeForm::imageYOffSet); //set the image size to the values in the numeric boxes returns true if successful
			if (CameraFunctionOK != FALSE)
			{
				CameraFunctions::Camera1ImagesSizesAndOffsets = CameraFunctions::GetImageFrame(FCAM1); //get image size and update image boxes
				camera1_txtImageXSize->Text = CameraFunctions::Camera1ImagesSizesAndOffsets[0].ToString();
				camera1_txtImageXOffSet->Text = CameraFunctions::Camera1ImagesSizesAndOffsets[1].ToString();
				camera1_txtImageYSize->Text = CameraFunctions::Camera1ImagesSizesAndOffsets[2].ToString();
				camera1_txtImageYOffSet->Text = CameraFunctions::Camera1ImagesSizesAndOffsets[3].ToString();
				camera_ActionBox->Text = "Camera 1 frame size values updated.";
				camera_ActionBox->BackColor = System::Drawing::Color::FromArgb(255, 255, 255, 255); //reset actions box to white if there was an error and the box was colours red
			}
			else
			{
				camera_ActionBox->Text = "Camera 1 frame size failed to updated."; //if the camera image size failed to update display an error message and turn the acton box red
				camera_ActionBox->BackColor = System::Drawing::Color::FromArgb(255, 255, 0, 0);
			}

	}

	private: System::Void Camera_90degreeRotation_checkBox_CheckedChanged_1(System::Object^ sender, System::EventArgs^ e) {

		if (Camera_90degreeRotation_checkBox->Checked == true)
		{
			ninety_degree_rotation = true;
			FunctionsForImages::camera_rotated_90_degrees = ninety_degree_rotation;
			LiveCam1::camera_rotated_90_degrees = ninety_degree_rotation;
			camera_ActionBox->Text = "Camera rotated by ninety degrees...";
		}
	}

	private: System::Void camera1_comboTM_DropDownClosed(System::Object^ sender, System::EventArgs^ e) {

		//-----Camera Trigger Mode-----//
		CameraFunctionOK = CameraFunctions::SetCameraTriggerMode(FCAM1, camera1_comboTM->SelectedItem->ToString()); //set the camera trigger mode to the selected item: Internal, External - Edge, External - Level, Software, TDI, TDI Internal, Start, Syncreadout
		if (CameraFunctionOK != FALSE)
		{
			CameraFunctions::Camera1TriggerMode = CameraFunctions::GetCameraTriggerMode(FCAM1); //get camera trigger mode and update camera functions variable
			camera_ActionBox->Text = "Camera 1 Trigger mode updated.";
			camera_ActionBox->BackColor = System::Drawing::Color::FromArgb(255, 255, 255, 255); // change action box back colour to white
		}
		else
		{
			camera1_comboTM->Text = CameraFunctions::Camera1TriggerMode;
			camera_ActionBox->Text = "Error setting camera 1 trigger mode!";
			camera_ActionBox->BackColor = System::Drawing::Color::FromArgb(255, 255, 0, 0); // change action box back colour to red
		}
		//Disable "set internal exposure" button if trigger mode isnt internal
		if (camera1_comboTM->SelectedItem->ToString() != "Internal")
		{
			camera1_btnSetTMInternalExposure->Enabled = false;
		}
		else
		{
			camera1_btnSetTMInternalExposure->Enabled = true;
			CameraFunctions::Cam1Exposure = CameraFunctions::GetCameraExposureTime(FCAM1); //get camera exposure time into camera functions variable
			camera1_numCurrentTMExposure->Value = (int)CameraFunctions::Cam1Exposure; //update current exposure time box
		}
	}
	private: System::Void camera1_comboTMPolarity_DropDownClosed(System::Object^ sender, System::EventArgs^ e) {
		//-----Camera Trigger Mode Polarity-----//
		CameraFunctionOK = CameraFunctions::SetCameraTriggerModePolarity(FCAM1, camera1_comboTMPolarity->SelectedItem->ToString()); //set the camera trigger mode polarity: Positive or Negative
		if (CameraFunctionOK != FALSE)
		{
			CameraFunctions::Camera1TriggerModePolarity = CameraFunctions::GetCameraTriggerModePolarity(FCAM1); //get camera trigger polarity and update camera functions variable
			camera_ActionBox->Text = "Camera 1 Trigger mode polarity updated.";
			camera_ActionBox->BackColor = System::Drawing::Color::FromArgb(255, 255, 255, 255); // change action box back colour to white
		}
		else
		{
			camera1_comboTMPolarity->Text = CameraFunctions::Camera1TriggerModePolarity;
			camera_ActionBox->Text = "Error setting camera 1 trigger mode polarity!";
			camera_ActionBox->BackColor = System::Drawing::Color::FromArgb(255, 255, 0, 0); // change action box back colour to red
		}
	}
	private: System::Void camera1_SetInternalExposure_Click(System::Object^ sender, System::EventArgs^ e)
	{
		CameraFunctionOK = CameraFunctions::SetCameraExposureTime(FCAM1, ((double)camera1_numSetTMExposure->Value/1000)); //set camera exposure time from user input
		if (CameraFunctionOK)
		{
			CameraFunctions::Cam1Exposure = CameraFunctions::GetCameraExposureTime(FCAM1) * 1000; //get camera exposure time into camera functions variable
			camera1_numCurrentTMExposure->Value = (int)CameraFunctions::Cam1Exposure; //update current camera exposure time box
			camera_ActionBox->Text = "Camera 1 exposure time changed to: " + round(CameraFunctions::Cam1Exposure).ToString() + "ms";
		}
		else {
			camera_ActionBox->Text = "Failed to set exposure time for camera 1.";
		}

	}
#pragma endregion

	//-----Require for Live imaging-----//
	private: System::Void SetBright_Click(System::Object^ sender, System::EventArgs^ e) {
		Cam1BrightLevelMin = (int)nudBrightMin_ch1->Value;
		Cam1BrightLevelMax = (int)nudBrightMax_ch1->Value;

		FunctionsForImages::BrightLevelMin = Cam1BrightLevelMin;
		LiveCam1::BrightLevelMin = Cam1BrightLevelMin;

		FunctionsForImages::BrightLevelMax = Cam1BrightLevelMax;
		LiveCam1::BrightLevelMax = Cam1BrightLevelMax;

		camera_ActionBox->Text = "Channel 1 Bright Levels updated, Min: " + Cam1BrightLevelMin.ToString() + " and Max: " + Cam1BrightLevelMax.ToString() + ".";

	}
	private: System::Void SetBright_ch2_Click(System::Object^ sender, System::EventArgs^ e) {

		Cam1BrightLevelMin_ch2 = (int)nudBrightMin_ch2->Value;
		Cam1BrightLevelMax_ch2 = (int)nudBrightMax_ch2->Value;

		FunctionsForImages::BrightLevelMin_ch2 = Cam1BrightLevelMin_ch2;
		LiveCam1::BrightLevelMin_ch2 = Cam1BrightLevelMin_ch2;

		FunctionsForImages::BrightLevelMax_ch2 = Cam1BrightLevelMax_ch2;
		LiveCam1::BrightLevelMax_ch2 = Cam1BrightLevelMax_ch2;

		camera_ActionBox->Text = "Channel 2 Bright Levels updated, Min: " + Cam1BrightLevelMin.ToString() + " and Max: " + Cam1BrightLevelMax.ToString() + ".";
	}

	private: System::Void SetReferenceHighet_Click(System::Object^ sender, System::EventArgs^ e) {
		ImageRefHeightValue = (int)nudReferenceHeight->Value;

		if (ImageRefHeightValue > Convert::ToInt32(camera1_txtImageYSize->Text))
		{
			ImageRefHeightValue = Convert::ToInt32(camera1_txtImageYSize->Text);
		}

		LiveCam1::ImageReferenceHeight = ImageRefHeightValue;

	}

#pragma endregion

#pragma region Stage Controls
	private: System::Void stage_btnInitialiseStage_Click(System::Object^ sender, System::EventArgs^ e)
	{
		stage_txtActionBox->Text = "Initializing Stage...";
		StageFunctionsOK = StageFunctions::StageInitialization(); //Initialise the stages. This function will detect how many stages are connected, set them to use servo mode, set Axis velocity to 5mm/s and auto zero all axes

		if (StageFunctionsOK == FALSE) //if the initialise function fails an error is displayed and the action box turns red
		{
			stage_txtActionBox->Text = "Failed to initialise";
			stage_txtActionBox->BackColor = System::Drawing::Color::FromArgb(255, 255, 0, 0);
			stage_txtInfoBox->Text = StageFunctions::InitializationError;
			return;
		}

		//All button are able to be clicked after stages are initialised correctly
		stage_btnLeft->Enabled = true;
		stage_btnRight->Enabled = true;
		stage_btnFront->Enabled = true;
		stage_btnBack->Enabled = true;
		stage_btnUp->Enabled = true;
		stage_btnDown->Enabled = true;
		stage_btnSetXVelocity->Enabled = true;
		stage_btnSetYVelocity->Enabled = true;
		stage_btnSetZVelocity->Enabled = true;
		stage_btnSetX0Z0->Enabled = true;
		stage_btnSetX1Z1->Enabled = true;
		stage_btnSetX2Z2->Enabled = true;
		stage_btnSetX3Z3->Enabled = true;
		stage_btnSetX4Z4->Enabled = true;
		stage_btnSetY2->Enabled = true;
		stage_btnSetStageExpParams->Enabled = true;
		stage_btnInitialiseStage->Enabled = false;

		//Set all local variable to Stage function variables
		NrOfAxisControllers = StageFunctions::NrOfControllersConnected;
		ControllerBox1 = StageFunctions::Controller1Details;
		ControllerBox2 = StageFunctions::Controller2Details;
		ControllerBox1ID = StageFunctions::ID1;
		ControllerBox2ID = StageFunctions::ID2;

		//Display basic information about controller boxes
		//Controller box 1 has the Z axis in motor 1. Controller box 2 has the X axis in motor 1 and the Y axis in motor 2
		stage_txtInfoBox->Text =
			NrOfAxisControllers.ToString() + " controller boxes connected." +
			"\r\nZ controller: " + ControllerBox1 +
			"\r\nX/Y controller: " + ControllerBox2;

		//Set all axis velocity to 0.2mm/s as this was found to be the best velocity to move the stages with little deviation
		StageFunctionsOK = StageFunctions::SetAxisVelocity(ControllerBox2ID, XAxis, 0.5);
		StageFunctionsOK = StageFunctions::SetAxisVelocity(ControllerBox2ID, YAxis, 0.5);
		StageFunctionsOK = StageFunctions::SetAxisVelocity(ControllerBox1ID, ZAxis, 0.5);

		//Move X,Y stage to 0 so that samples can be placed. z already sent to 0
		StageFunctionsOK = StageFunctions::MoveAxisToThisExactPosition(ControllerBox2ID, XAxis, 0);

		StageFunctionsOK = StageFunctions::MoveAxisToThisExactPosition(ControllerBox2ID, YAxis, 0);

		BOOL stageMoving = StageFunctions::StageIsMoving(ControllerBox2ID, XAxis);

		while (stageMoving)
		{
			stageMoving = StageFunctions::StageIsMoving(ControllerBox2ID, XAxis);
		}

		stageMoving = StageFunctions::StageIsMoving(ControllerBox2ID, YAxis);

		while (stageMoving)
		{
			stageMoving = StageFunctions::StageIsMoving(ControllerBox2ID, YAxis);
		}

		AxisPosition = StageFunctions::GetAxisPosition(ControllerBox2ID, XAxis); //Get X axis position and display in box
		stage_txtXPosition->Text = AxisPosition.ToString();
		XAxisVelocity = StageFunctions::GetAxisVelocity(ControllerBox2ID, XAxis); //Get X axis velocity and display in box
		stage_txtXVelocity->Text = XAxisVelocity.ToString();

		AxisPosition = StageFunctions::GetAxisPosition(ControllerBox2ID, YAxis); //Get Y axis position and display in box
		stage_txtYPosition->Text = AxisPosition.ToString();
		YAxisVelocity = StageFunctions::GetAxisVelocity(ControllerBox2ID, YAxis); //Get Y axis velocity and display in box
		stage_txtYVelocity->Text = YAxisVelocity.ToString();

		AxisPosition = StageFunctions::GetAxisPosition(ControllerBox1ID, ZAxis); //Get Z axis position and display in box
		stage_txtZPosition->Text = AxisPosition.ToString();
		ZAxisVelocity = StageFunctions::GetAxisVelocity(ControllerBox1ID, ZAxis); //Get Z axis velocity and display in box
		stage_txtZVelocity->Text = ZAxisVelocity.ToString();

		if (StageFunctionsOK == FALSE)
		{
			stage_txtActionBox->Text = "Stage positioning failed.";

			stage_txtInfoBox->AppendText(
				"\r\nController 1 error: " + StageFunctions::ErrorMessageString1 +
				"\r\nController 2 error: " + StageFunctions::ErrorMessageString2 +
				"\r\nStage error: " + StageFunctions::ErrorMessageString);
		}
		else
		{
			stage_txtActionBox->Text = "3D stage axes are initialized!";

			stage_txtInfoBox->AppendText(
				"\r\nPosition (mm): X=" + stage_txtXPosition->Text +
				", Y=" + stage_txtYPosition->Text +
				", Z=" + stage_txtZPosition->Text +
				"\r\nVelocity (mm/s): X=" + stage_txtXVelocity->Text +
				", Y=" + stage_txtYVelocity->Text +
				", Z=" + stage_txtZVelocity->Text);
		}

	}
	private: System::Void stage_btnLeft_Click(System::Object^ sender, System::EventArgs^ e)
	{
		StepSize = -(double)stage_numXStep->Value;//Negative for left
		StageFunctionsOK = StageFunctions::MoveAxisThisExactStep(ControllerBox2ID, XAxis, StepSize); //move X axis (Controller box 2, motor 1) relative to it's current position by the amount input by user

		//While loop to allow for stage to move before returning current position.
		StageFunctionsOK = StageFunctions::StageIsMoving(ControllerBox2ID, XAxis);
		while (StageFunctionsOK)
		{
			StageFunctionsOK = StageFunctions::StageIsMoving(ControllerBox2ID, XAxis);
		}

		AxisPosition = StageFunctions::GetAxisPosition(ControllerBox2ID, XAxis); //get position of axis to display in box
		stage_txtXPosition->Text = (AxisPosition).ToString();

		if (StageFunctionsOK == TRUE) //if stage failed to move display error message
		{
			ErrorFromStageFunctions = StageFunctions::ErrorMessageString;
			stage_txtActionBox->Text = "An Error have occurred: " + ErrorFromStageFunctions;
		}
		else
			stage_txtActionBox->Text = "X axis was moved to the left " + StepSize.ToString() + " mm, relative to its previous position.";
	}
	private: System::Void stage_btnRight_Click(System::Object^ sender, System::EventArgs^ e)
	{
		StepSize = (double)stage_numXStep->Value;//Positive for right
		StageFunctionsOK = StageFunctions::MoveAxisThisExactStep(ControllerBox2ID, XAxis, StepSize); //move X axis (Controller box 2, motor 1) relative to it's current position by the amount input by user

		//While loop to allow for stage to move before returning current position.
		StageFunctionsOK = StageFunctions::StageIsMoving(ControllerBox2ID, XAxis);
		while (StageFunctionsOK)
		{
			StageFunctionsOK = StageFunctions::StageIsMoving(ControllerBox2ID, XAxis);
		}

		AxisPosition = StageFunctions::GetAxisPosition(ControllerBox2ID, XAxis); //get position of axis to display in box
		stage_txtXPosition->Text = (AxisPosition).ToString();

		if (StageFunctionsOK == TRUE) //if stage failed to move display error message
		{
			ErrorFromStageFunctions = StageFunctions::ErrorMessageString;
			stage_txtActionBox->Text = "An Error have occurred: " + ErrorFromStageFunctions;
		}
		else
			stage_txtActionBox->Text = "X axis was moved to the Right " + StepSize.ToString() + " mm, relative to its previous position.";
	}
	private: System::Void stage_btnFront_Click(System::Object^ sender, System::EventArgs^ e)
	{
		StepSize = (double)stage_numYStep->Value;//Positive for front
		StageFunctionsOK = StageFunctions::MoveAxisThisExactStep(ControllerBox2ID, YAxis, StepSize); //move Y axis (Controller box 2, motor 2) relative to it's current position by the amount input by user

		//While loop to allow for stage to move before returning current position.
		StageFunctionsOK = StageFunctions::StageIsMoving(ControllerBox2ID, YAxis);
		while (StageFunctionsOK)
		{
			StageFunctionsOK = StageFunctions::StageIsMoving(ControllerBox2ID, YAxis);
		}
		AxisPosition = StageFunctions::GetAxisPosition(ControllerBox2ID, YAxis); //get position of axis to display in box
		stage_txtYPosition->Text = AxisPosition.ToString();

		if (StageFunctionsOK != FALSE) //if stage failed to move display error message
		{
			ErrorFromStageFunctions = StageFunctions::ErrorMessageString;
			stage_txtActionBox->Text = "An Error have occurred: " + ErrorFromStageFunctions;
		}
		else
			stage_txtActionBox->Text = "Y axis was moved to the Front " + StepSize.ToString() + " mm, relative to its previous position.";
	}
	private: System::Void stage_btnBack_Click(System::Object^ sender, System::EventArgs^ e)
	{
		StepSize = -(double)stage_numYStep->Value;	//Negative for back
		StageFunctionsOK = StageFunctions::MoveAxisThisExactStep(ControllerBox2ID, YAxis, StepSize);	//move Y axis (Controller box 2, motor 2) relative to it's current position by the amount input by user
		//While loop to allow for stage to move before returning current position.
		StageFunctionsOK = StageFunctions::StageIsMoving(ControllerBox2ID, YAxis);
		while (StageFunctionsOK)
		{
			StageFunctionsOK = StageFunctions::StageIsMoving(ControllerBox2ID, YAxis);
		}

		AxisPosition = StageFunctions::GetAxisPosition(ControllerBox2ID, YAxis); //get position of axis to display in box
		stage_txtYPosition->Text = AxisPosition.ToString();

		if (StageFunctionsOK != FALSE) //if stage failed to move display error message
		{
			ErrorFromStageFunctions = StageFunctions::ErrorMessageString;
			stage_txtActionBox->Text = "An Error have occurred: " + ErrorFromStageFunctions;
		}
		else
			stage_txtActionBox->Text = "Y axis was moved Backwards " + StepSize.ToString() + " mm, relative to its previous position.";
	}
	private: System::Void stage_btnUp_Click(System::Object^ sender, System::EventArgs^ e)
	{
		StepSize = (double)stage_numZStep->Value;	//Positive for Up
		StageFunctionsOK = StageFunctions::MoveAxisThisExactStep(ControllerBox1ID, ZAxis, StepSize);	//move Z axis (Controller box 1, motor 1) relative to it's current position by the amount input by user

		//While loop to allow for stage to move before returning current position.
		StageFunctionsOK = StageFunctions::StageIsMoving(ControllerBox1ID, ZAxis);
		while (StageFunctionsOK)
		{
			StageFunctionsOK = StageFunctions::StageIsMoving(ControllerBox1ID, ZAxis);
		}

		AxisPosition = StageFunctions::GetAxisPosition(ControllerBox1ID, ZAxis); //get position of axis to display in box
		stage_txtZPosition->Text = AxisPosition.ToString();

		if (StageFunctionsOK != FALSE) //if stage failed to move display error message
		{
			ErrorFromStageFunctions = StageFunctions::ErrorMessageString;
			stage_txtActionBox->Text = "An Error have occurred: " + ErrorFromStageFunctions;
			return;
		}
		else
			stage_txtActionBox->Text = "Z axis was moved Up " + StepSize.ToString() + " mm, relative to its previous position.";
	}
	private: System::Void stage_btnDown_Click(System::Object^ sender, System::EventArgs^ e)
	{
		StepSize = -(double)stage_numZStep->Value;	//negative for Down
		StageFunctionsOK = StageFunctions::MoveAxisThisExactStep(ControllerBox1ID, ZAxis, StepSize);	//move Z axis (Controller box 1, motor 1) relative to it's current position by the amount input by user

		//While loop to allow for stage to move before returning current position.
		StageFunctionsOK = StageFunctions::StageIsMoving(ControllerBox1ID, ZAxis);
		while (StageFunctionsOK)
		{
			StageFunctionsOK = StageFunctions::StageIsMoving(ControllerBox1ID, ZAxis);
		}

		AxisPosition = StageFunctions::GetAxisPosition(ControllerBox1ID, ZAxis); //get position of axis to display in box
		stage_txtZPosition->Text = AxisPosition.ToString();

		if (StageFunctionsOK != FALSE) //if stage failed to move display error message
		{
			ErrorFromStageFunctions = StageFunctions::ErrorMessageString;
			stage_txtActionBox->Text = "An Error have occurred: " + ErrorFromStageFunctions;
		}
		else
			stage_txtActionBox->Text = "Z axis was moved Down " + StepSize.ToString() + " mm, relative to its previous position.";
	}

	//-----THESE WILL NEED TO BE REMOVED AS THE USER WONT BE INPUTTING THE VELOCITY-----//
	private: System::Void stage_btnSetXVelocity_Click(System::Object^ sender, System::EventArgs^ e)
	{
		XAxisVelocity = (double)stage_numXVelocity->Value;
		StageFunctionsOK = StageFunctions::SetAxisVelocity(ControllerBox2ID, XAxis, XAxisVelocity);

		XAxisVelocity = StageFunctions::GetAxisVelocity(ControllerBox2ID, XAxis);
		stage_txtXVelocity->Text = XAxisVelocity.ToString();

		if (StageFunctionsOK == FALSE)
		{
			ErrorFromStageFunctions = StageFunctions::ErrorMessageString;
			stage_txtActionBox->Text = "An Error have occurred: " + ErrorFromStageFunctions;
		}
		else
			stage_txtActionBox->Text = "X velocity was updated to " + XAxisVelocity.ToString() + " mm/s.";
	}
	private: System::Void stage_btnSetYVelocity_Click(System::Object^ sender, System::EventArgs^ e)
	{
		YAxisVelocity = (double)stage_numYVelocity->Value;
		StageFunctionsOK = StageFunctions::SetAxisVelocity(ControllerBox2ID, YAxis, YAxisVelocity);

		YAxisVelocity = StageFunctions::GetAxisVelocity(ControllerBox2ID, YAxis);
		stage_txtYVelocity->Text = YAxisVelocity.ToString();

		if (StageFunctionsOK == FALSE)
		{
			ErrorFromStageFunctions = StageFunctions::ErrorMessageString;
			stage_txtActionBox->Text = "An Error have occurred: " + ErrorFromStageFunctions;
		}
		else
			stage_txtActionBox->Text = "Y velocity was updated to " + YAxisVelocity.ToString() + " mm/s.";
	}
	private: System::Void stage_btnSetZVelocity_Click(System::Object^ sender, System::EventArgs^ e)
	{
		ZAxisVelocity = (double)stage_numZVelocity->Value;
		StageFunctionsOK = StageFunctions::SetAxisVelocity(ControllerBox1ID, ZAxis, ZAxisVelocity);

		ZAxisVelocity = StageFunctions::GetAxisVelocity(ControllerBox1ID, ZAxis);
		stage_txtZVelocity->Text = ZAxisVelocity.ToString();

		if (StageFunctionsOK == FALSE)
		{
			ErrorFromStageFunctions = StageFunctions::ErrorMessageString;
			stage_txtActionBox->Text = "An Error have occurred: " + ErrorFromStageFunctions;
		}
		else
			stage_txtActionBox->Text = "Z velocity was updated to " + ZAxisVelocity.ToString() + " mm/s.";
	}

	//-----THIS IS EXPERIEMNTAL AND WILL BE WORKED ON ONCE THE CODE IS FINISHED-----//
	private: System::Void stage_btnOutputTrigger_Click(System::Object^ sender, System::EventArgs^ e) {

		StageFunctionsOK = StageFunctions::ConfigureTriggerOutput(ControllerBox2ID, 1, 2, 1.0); //Assign trigger output to X axis
		StageFunctionsOK = StageFunctions::ConfigureTriggerOutput(ControllerBox2ID, 1, 3, 6.0); //Set output trigger to in motion

		if (!StageFunctionsOK)
		{
			stage_txtActionBox->Text = "An error occured when setting the output trigger.";
			return;
		}
		stage_txtActionBox->Text = "Stage was set to send external trigger to scanner.";
	}
	private: System::Void stage_btnTriggerOn_Click(System::Object^ sender, System::EventArgs^ e) {
		StageFunctionsOK = StageFunctions::ToggleOutputOnOff(ControllerBox2ID, 1, 1);
		if (StageFunctionsOK)
		{
			stage_txtActionBox->Text = "External trigger activated.";
		}
	}

	//The following functions are to store the X, Z and if wanted the Y position of the stages (Only SetX0Z0 has comments, refer to this function for rest)
	private: System::Void stage_btnSetX0Z0_Click(System::Object^ sender, System::EventArgs^ e)
	{
		XPos = StageFunctions::GetAxisPosition(ControllerBox2ID, XAxis); //Get X axis (Controller box 2, motor 1) position
		XPositions[0] = XPos; //Add position to the 0th position in the array, this will become the experiement starting position
		stage_txtX0Stage->Text = (XPos).ToString();

		ZPos = StageFunctions::GetAxisPosition(ControllerBox1ID, ZAxis); //get the Z axis (Controller box 1, motor 1) position
		ZPositions[0] = ZPos; //Add position to the 0th position in the array, this will become the experiement starting position
		stage_txtZ0Stage->Text = (ZPos).ToString();

		YPos = StageFunctions::GetAxisPosition(ControllerBox2ID, YAxis); //get the Y axis (Controller box 2, motor 2) position
		stage_nudY1Stage->Value = (Decimal)YPos;
		stage_nudY2Stage->Value = (Decimal)YPos;
		YPositions[0] = YPos; //This position is added to the 0th and 1st position to make values the same if not splitting sample into two sections
		YPositions[1] = YPos;

	}
	private: System::Void stage_btnSetX1Z1_Click(System::Object^ sender, System::EventArgs^ e)
	{
		XPos = StageFunctions::GetAxisPosition(ControllerBox2ID, XAxis);
		XPositions[1] = XPos;
		stage_txtX1Stage->Text = (XPos).ToString();

		ZPos = StageFunctions::GetAxisPosition(ControllerBox1ID, ZAxis);
		ZPositions[1] = (ZPos);
		stage_txtZ1Stage->Text = (ZPos).ToString();
	}
	private: System::Void stage_btnSetX2Z2_Click(System::Object^ sender, System::EventArgs^ e)
	{
		XPos = StageFunctions::GetAxisPosition(ControllerBox2ID, XAxis);
		XPositions[2] = XPos;
		stage_txtX2Stage->Text = (XPos).ToString();

		ZPos = StageFunctions::GetAxisPosition(ControllerBox1ID, ZAxis);
		stage_txtZ2Stage->Text = (ZPos).ToString();
		ZPositions[2] = (ZPos);
	}
	private: System::Void stage_btnSetX3Z3_Click(System::Object^ sender, System::EventArgs^ e)
	{
		XPos = StageFunctions::GetAxisPosition(ControllerBox2ID, XAxis);
		XPositions[3] = XPos;
		stage_txtX3Stage->Text = (XPos).ToString();

		ZPos = StageFunctions::GetAxisPosition(ControllerBox1ID, ZAxis);
		stage_txtZ3Stage->Text = (ZPos).ToString();
		ZPositions[3] = (ZPos);

	}
	private: System::Void stage_btnSetX4Z4_Click(System::Object^ sender, System::EventArgs^ e)
	{
		XPos = StageFunctions::GetAxisPosition(ControllerBox2ID, XAxis);
		XPositions[4] = XPos;
		stage_txtX4Stage->Text = (XPos).ToString();

		ZPos = StageFunctions::GetAxisPosition(ControllerBox1ID, ZAxis);
		stage_txtZ4Stage->Text = (ZPos).ToString();
		ZPositions[4] = ZPos;

		//////// experimental parameters///////////////////////////////////////
		double  stepX;
		Decimal  travelTime;

		stepX = (double)stage_numWaveXStep->Value;
		LiveCam1::stepX = stepX*1000;

		int a = abs((int)((XPositions[4] - XPositions[0]) / stepX));
		ExperimentSetup_NoOfSteps->Value = abs((int)((XPositions[4] - XPositions[0]) / stepX));

		travelTime = (Decimal)abs((double)experiment_numSST->Value * (double)ExperimentSetup_NoOfSteps->Value); // + 0.192s the difference between stage time and scan time
		ExperimentSetup_travelTime->Value = travelTime;

	}

	private: System::Void stage_btnSetY1_Click(System::Object^ sender, System::EventArgs^ e) {

		YPos = StageFunctions::GetAxisPosition(ControllerBox2ID, YAxis); //get the Y axis (Controller box 2, motor 2) position
		stage_nudY1Stage->Value = (Decimal)YPos;
		YPositions[0] = YPos; //This position is added to the 1st position in the array
	}

	private: System::Void stage_btnSetY2_Click(System::Object^ sender, System::EventArgs^ e)
	{
		YPos = StageFunctions::GetAxisPosition(ControllerBox2ID, YAxis); //get the Y axis (Controller box 2, motor 2) position
		stage_nudY2Stage->Value = (Decimal)YPos;
		YPositions[1] = YPos; //Add position to the 1st position in the array, this is only used when the sample is scanned in two sections
	}

	private: System::Void stage_btnSetStageExpParams_Click(System::Object^ sender, System::EventArgs^ e) {
		//Exception statement to catch if the user has failed to set all 5 points.
		if (stage_txtX0Stage->Text == "" || stage_txtX1Stage->Text == "" || stage_txtX2Stage->Text == "" || stage_txtX3Stage->Text == "" || stage_txtX4Stage->Text == "")
		{
			stage_txtActionBox->Text = "Not all 5 values for stage positions are set.";
			return;
		}
		else
		{
			XStepSize = (double)stage_numWaveXStep->Value; //Get X step size from user input into variable
			if (XPositions[4] < XPositions[0]) //If the stage is going left to right the step size needs to be inverted
			{
				XStepSize = -XStepSize;
			}

		}

		stage_txtActionBox->Text = "Stage parameters were defined to run an experiment...Now test stage parameters";
	}

#pragma endregion

#pragma region Scanner Code

	private: System::Void scanner_btnInitialiseScanner_Click(System::Object^ sender, System::EventArgs^ e) {

		ScannerFunctionOK = ScanHeadFunctions::ScanheadInitialization(); //Initialises the scanner by loading a correction and program file
		//The scanner Jump and Mark delays to 10 us (microseconds)

		if (ScannerFunctionOK != FALSE) //If the scanner initialises successfully
		{
			scanner_txtActionBox->Text = "Scanner initialized with success!";
			btnInitialiseScanner->Enabled = false;

			ScannerFunctionOK = ScanHeadFunctions::BeamJumpToInitialPosition(100000, InitPosX, InitPosY);

			JumpTime = (double)scanner_numSetJumpTime->Value; //Get jump speed
			MarkTime = (double)scanner_numSetMarkTime->Value; //Get mark speed
			ScannerFunctionOK = ScanHeadFunctions::SetScanSpeed(MarkTime, JumpTime); //Set the scanners mark and scan speed to values from boxes, min 1 max 50,000 (for both)

			//Display the scanner information in the Scanner info box
			scanner_txtInfoBox->Text = "RTC Version: " + ScanHeadFunctions::RTCVersion() + "\r\nHex Version: " + ScanHeadFunctions::HEXVersion() + "\r\nDLL Version: " + ScanHeadFunctions::DLLVersion();

			//Enable all scanner buttons now that the scanner has been initialised successfully

			scanner_btnSetSpeed->Enabled = true;
			scanner_btnRun->Enabled = true;

		}
		else //If scanner fails to initialise display an error message and stop program from continuing
		{
			scanner_txtActionBox->Text = "Scanner was not initialized, please check if scanner is ON and correctly connected...";
			return;
		}

			ScannerFunctionOK = ScanHeadFunctions::ExternalTriggerOn();
			if (!ScannerFunctionOK) {
				scanner_txtActionBox->Text = "Error setting external trigger.";
				return;
			}
			ExternalTrigger = true;
			scanner_txtActionBox->Text = "Scanner set to be triggered by external connection.";
	}

	private: System::Void scanner_btnSetGain_Click(System::Object^ sender, System::EventArgs^ e) {
		gain = (int)scanner_numGain->Value;

		ScannerFunctionOK = ScanHeadFunctions::SetScanGain(gain); //Set the scanners mark and scan speed to values from boxes, min 1 max 50,000 (for both)

		if (ScannerFunctionOK != FALSE) //If successfuly display changed message
		{
			scanner_txtActionBox->Text = "Gain set to: " + gain + " % " ;
			scanner_txtActionBox->BackColor = System::Drawing::Color::FromArgb(255, 255, 255, 255);	//Set actions box to white
		}
		else
		{
			scanner_txtActionBox->Text = "Failed to update scanner gain";
			scanner_txtActionBox->BackColor = System::Drawing::Color::FromArgb(255, 255, 0, 0);	//Set actions box to red
		}
	}

	private: System::Void scanner_btnSetSpeed_Click(System::Object^ sender, System::EventArgs^ e) {
		JumpTime = (double)scanner_numSetJumpTime->Value; //Get user input for jump speed
		MarkTime = (double)scanner_numSetMarkTime->Value;
		ScannerFunctionOK = ScanHeadFunctions::SetScanSpeed(MarkTime, JumpTime); //Set the scanners mark and scan speed to values from boxes, min 1 max 50,000 (for both)

		if (ScannerFunctionOK != FALSE) //If successfuly display changed message
		{
			scanner_txtActionBox->Text = "Mark and Jump Times were changed to: " + MarkTime + " and " + JumpTime + " bits per ms, respectively.";
			scanner_txtActionBox->BackColor = System::Drawing::Color::FromArgb(255, 255, 255, 255);	//Set actions box to white
		}
		else
		{
			scanner_txtActionBox->Text = "Failed to update scanner mark and jump speed";
			scanner_txtActionBox->BackColor = System::Drawing::Color::FromArgb(255, 255, 0, 0);	//Set actions box to red
		}

	}

	private: System::Void scanner_btnSingleScan_Click(System::Object^ sender, System::EventArgs^ e) {
		X1para = (short)scanner_numX1->Value; //Get values of mark and just positions from user input
		X2para = (short)scanner_numX2->Value;
		Y1para = (short)scanner_numY1->Value;
		Y2para = (short)scanner_numY2->Value;

		JumpTime = (double)(scanner_numSetJumpTime->Value) * 1000; //Get values of mark and jump speed from user input; the values should be passed in us
		MarkTime = (double)(scanner_numSetMarkTime->Value) * 1000;
		DelayTime = (double)(scanner_numSetJumpDelay->Value)/10; //delay is set in bits

		double scansPerSecond = 0.0;

		JumpTime = (double)scanner_numSetJumpTime->Value; //Get user input values for mark and jump speed
		MarkTime = (double)scanner_numSetMarkTime->Value;

		//Perform a signle scan using user defined values
		ScannerFunctionOK = ScanHeadFunctions::SingleScanLine(MarkTime, JumpTime, DelayTime, X1para, X2para, Y1para, Y2para);

		if (ScannerFunctionOK != FALSE)
		{
			scanner_txtActionBox->Text = "Single Scan was performed with success.";

		}
		else
			scanner_txtActionBox->Text = "Single Scan encountered an error";

	}

	private: System::Void scanner_btnRun_Click(System::Object^ sender, System::EventArgs^ e) {
		//Begin the background worker that will perform a single scan continuously until cancel is clicked
		RunWorker->RunWorkerAsync();
		scanner_btnStop->Enabled = true;
		scanner_btnRun->Enabled = false;
		scanner_txtActionBox->Text = "Continuous scanning is running.";
	}
	private: System::Void scanner_btnStop_Click(System::Object^ sender, System::EventArgs^ e) {
		//When the cancel button is pressed the background worker will stop running the single scan
		RunWorker->CancelAsync();
		scanner_txtActionBox->Text = "Continuous scanning stopped.";
		scanner_btnStop->Enabled = false;
		scanner_btnRun->Enabled = true;
	}
	private: System::Void scanner_btnRunWorker_DoWork(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e) {
		int n = 0;
		int counter = 0;

		X1para = (short)scanner_numX1->Value; //Get values of mark and just positions from user input
		X2para = (short)scanner_numX2->Value;
		Y1para = (short)scanner_numY1->Value;
		Y2para = (short)scanner_numY2->Value;

		JumpTime = (double)(scanner_numSetJumpTime->Value) * 1000; //Get values of mark and jump speed from user input; the values should be passed in us
		MarkTime = (double)(scanner_numSetMarkTime->Value) * 1000;
		DelayTime = (double)(scanner_numSetJumpDelay->Value) / 10; //delay is set in bits

		if (Scanner_Loop_checkBox->Checked == true)
		{
			while (!RunWorker->CancellationPending)
			{
				if (scanner_radioScanJobSingleChannel->Checked == true) {
					ScannerFunctionOK = ScanHeadFunctions::SingleScanLine(MarkTime, JumpTime, DelayTime, X1para, X2para, Y1para, Y2para);
				}
				else if (scanner_radioScanJobDualChannel->Checked == true) {
					//While cancel button isn't clicked it will continuously perform the same loop taken from mark x,y and jump x,y
					ScannerFunctionOK = ScanHeadFunctions::SingleScanLineDualCh(MarkTime, JumpTime, DelayTime, X1para, X2para, Y1para, Y2para);
				}
			}
		}
		else
		{
				// Define constants for setting DIGITAL OUT1
				const unsigned int DIGITAL_OUT1 = 0x01; // Bit #0
				const unsigned int DIGITAL_OUT2 = 0x02; // Bit #1
				unsigned int Pins = 0; // Initialize Pins to 0 or any other initial value

				set_start_list_1();
				set_scanner_delays(static_cast<UINT>(DelayTime / 10), 0, 0);

				// define scan Job
				if (scanner_radioScanJobSingleChannel->Checked == true) {
					for (int s = 0; s < scanner_numRepeat->Value; s++) {
						// Laser OFF during reposition; Set DIGITAL OUT1 low
						Pins &= ~(DIGITAL_OUT1 | DIGITAL_OUT2);
						set_laser_pin_out_list(Pins);
						timed_jump_abs(Y1para, X1para, JumpTime);

						// Laser ON during marking/exposure ; Set DIGITAL OUT1 high
						Pins = (Pins & ~DIGITAL_OUT2) | DIGITAL_OUT1;
						set_laser_pin_out_list(Pins);
						timed_mark_abs(Y2para, X2para, MarkTime);
					}
				}
				//Calculate scanner cycle time of- mark (channel 1)+mark(channel 2)+delays
				else if (scanner_radioScanJobDualChannel->Checked == true) {
					for (int s = 0; s < scanner_numRepeat->Value; s++) {
						// Jump: both OFF
						Pins &= ~(DIGITAL_OUT1 | DIGITAL_OUT2);
						set_laser_pin_out_list(Pins);
						timed_jump_abs(Y1para, X1para, JumpTime);

						// Mark: 488 ON, 561 OFF
						Pins = (Pins & ~DIGITAL_OUT2) | DIGITAL_OUT1;
						set_laser_pin_out_list(Pins);
						timed_mark_abs(Y2para, X2para, MarkTime);

						// Jump: both OFF
						Pins &= ~(DIGITAL_OUT1 | DIGITAL_OUT2);
						set_laser_pin_out_list(Pins);
						timed_jump_abs(Y2para, X2para, JumpTime);

						// Mark: 561 ON, 488 OFF
						Pins = (Pins & ~DIGITAL_OUT1) | DIGITAL_OUT2;
						set_laser_pin_out_list(Pins);
						timed_mark_abs(Y1para, X1para, MarkTime);
					}
				}
				set_end_of_list();

				execute_list_1();				// start marking

				Pins &= ~(DIGITAL_OUT1 | DIGITAL_OUT2);
				set_laser_pin_out_list(Pins);
		}

	}
	private: System::Void scanner_btnSSTCalculate_Click(System::Object^ sender, System::EventArgs^ e) {

		double sst;

		//calculate scanner cycle time of mark+jump for single channel (in ms)
		if (scanner_radioScanJobSingleChannel->Checked == true) {
			sst = (double)scanner_numSetJumpTime->Value + (double)scanner_numSetMarkTime->Value + (double)scanner_numSetJumpDelay->Value/1000;

			LiveCam1::dualChannelAcquisition = false;

			nudBrightMax_ch2->Enabled = false;
			nudBrightMin_ch2->Enabled = false;
			SetBright_ch2->Enabled = false;
			rdB_HA_ch2->Enabled = false;

		}
		//Calculate scanner cycle time of- mark (channel 1)+mark(channel 2)+delays
		if (scanner_radioScanJobDualChannel->Checked == true) {
			sst = 2*((double)scanner_numSetJumpTime->Value + (double)scanner_numSetMarkTime->Value + (double)scanner_numSetJumpDelay->Value/1000);

			LiveCam1::dualChannelAcquisition = true;

			nudBrightMax_ch2->Enabled = true;
			nudBrightMin_ch2->Enabled = true;
			SetBright_ch2->Enabled = true;
			rdB_HA_ch2->Enabled = true;

		}
		scanner_txtCalculateSST->Text = sst.ToString();

		double sst_seconds = sst / 1000.0;
		experiment_numSST->Value = (Decimal)sst_seconds;

	}
#pragma endregion

#pragma region Experiment Code

	private: System::Void btn_measureVelocity_Click(System::Object^ sender, System::EventArgs^ e) {

		double sst;
		int stepNumbers;
		sst = (double)((sqrt((double)(X1para - X2para) * (X1para - X2para) + (double)(Y1para - Y2para) * (Y1para - Y2para))) / MarkTime) + ((sqrt((double)(X1para - X2para) * (X1para - X2para) + (double)(Y1para - Y2para) * (Y1para - Y2para))) / JumpTime);
		stepNumbers = (int)ExperimentSetup_NoOfSteps->Value;

	}
	private: System::Void experiment_btnSetupExperiment_Click(System::Object ^ sender, System::EventArgs ^ e) {

		NumberOfTimePoints = (int)experiment_numNumberOfTimePointsValue->Value; //Get number of time point calculated in experiment setup
		NumberOfScansPerTimePoint = (int)ExperimentSetup_NoOfSteps->Value;

		experiment_progressBarExperiment->Value = 0; //Set the progress bar to 0 incase experiment has been run before
		experiment_progressBarExperiment->Maximum = NumberOfTimePoints; //Maximum value of the progress bar
		experiment_progressBarNumberOfScansPerTimePoint->Value = 0; //Timepoint progressbar minimum set to 0
		experiment_progressBarNumberOfScansPerTimePoint->Maximum = NumberOfScansPerTimePoint; //Maximum value of the timepoint progress bar

		WaitingTimeBetweenTimePoints = ((int)experiment_nudWaitingTime->Value) * 1000;

		//Check if two Y positions have been entered
		if (YPositions[0] != YPositions[1]) {
			Yloop = 2;
		}
		else {
			Yloop = 1;
		}

		if (rdB_HA_ch1->Checked == true)
			LiveCam1::HA_channel = 1;
		else if (rdB_HA_ch2->Checked == true)
			LiveCam1::HA_channel = 2;

		LiveCam1::Yloop = (int)Yloop;
		LiveCam1::NumberOfScansPerTimePointCam1 = (int)ExperimentSetup_NoOfSteps->Value;
		LiveCam1::NumberOfTimePointsCam1 = NumberOfTimePoints * Yloop;
		LiveCam1::WaitingTimeExperiment = WaitingTimeBetweenTimePoints;
		LiveCam1::magnification = (int)stage_numObjectiveMagnification->Value;
		LiveCam1::sst = (double)experiment_numSST->Value;
		LiveCam1::travelTime = (double)ExperimentSetup_travelTime->Value;

		//Scanner folder will be the first that is not empty, from Form2/Form3, or info will be saved on Disk D:
		if (LiveCam1::Folder != "")
			Folder = LiveCam1::Folder;
		else
			Folder = "D:";

		experiment_txtActionBox->Text = "Experiment was set up successfully";
	}

	private: System::Void experiment_btnStartExperiment_Click(System::Object ^ sender, System::EventArgs ^ e) {

		experiment_txtActionBox->BackColor = System::Drawing::Color::FromArgb(255, 255, 255, 255);	//Reset actions box to white

		experiment_btnStopExperiment->Enabled = true;
		experiment_btnStartExperiment->Enabled = false;
		experiment_btnSetupExperiment->Enabled = false;


		ExperimentStarted = true;
		LiveCam1::ExperimentStartedCam1 = ExperimentStarted;

		//Starting experiment with scanner controlling the cameras with hardware signal

			experiment_txtActionBox->Text = "Experiment running.";

			///////start info file here and continue writing in LiveCam1

			// Get the text from the text box
			String^ textBoxContent = this->txt_camera_infoFile->Text;

			StreamWriter^ infoWriter;
			FileInfo^ infoFile = gcnew FileInfo(Folder + "\\_info.txt");
			FileStream^ infoSave = infoFile->Open(FileMode::Append, FileAccess::Write, FileShare::Read);
			infoWriter = gcnew StreamWriter(infoSave);
			infoWriter->WriteLine(" ");
			infoWriter->WriteLine("\t\t {0} --------- Information of the experiment---------", DateTime::Now.ToString("HH:mm:ss.fff"));

			// Write the text box content to the file
			infoWriter->WriteLine(textBoxContent);

			infoWriter->Close();

			///////////////////////// write time file /////////////////////////////

			StreamWriter^ timeWriter;
			FileInfo^ stageFile = gcnew FileInfo(LiveCam1::Folder + "\\time.txt");
			FileStream^ stageSave = stageFile->Open(FileMode::Append, FileAccess::Write);
			timeWriter = gcnew StreamWriter(stageSave);

			timeWriter->WriteLine("\t\t\t------------New Scan------- {0} ---------", DateTime::Now.ToString("HH:mm:ss.fff"));
			timeWriter->WriteLine("DSLM Control Software");

			timeWriter->WriteLine("Stage parameters: ");
			timeWriter->WriteLine("	Y1: {0}\t\tY2: {1}", stage_nudY1Stage->Value, stage_nudY2Stage->Value);
			timeWriter->WriteLine("	Step X: {0}", stage_numWaveXStep->Value);
			timeWriter->WriteLine("	Number of steps: {0}", ExperimentSetup_NoOfSteps->Value);
			timeWriter->WriteLine("	Stage Velocity for X, Y and Z axis: {0}\t{1}\t{2} ", stage_numXVelocity->Value, stage_numYVelocity->Value, stage_numZVelocity->Value);

			timeWriter->WriteLine("	X1: {0}", System::Convert::ToDouble(stage_txtX0Stage->Text));
			timeWriter->WriteLine("	X2: {0}", System::Convert::ToDouble(stage_txtX1Stage->Text));
			timeWriter->WriteLine("	X3: {0}", System::Convert::ToDouble(stage_txtX2Stage->Text));
			timeWriter->WriteLine("	X4: {0}", System::Convert::ToDouble(stage_txtX3Stage->Text));
			timeWriter->WriteLine("	X5: {0}", System::Convert::ToDouble(stage_txtX4Stage->Text));

			timeWriter->WriteLine("	Z1: {0}", System::Convert::ToDouble(stage_txtZ0Stage->Text));
			timeWriter->WriteLine("	Z2: {0}", System::Convert::ToDouble(stage_txtZ1Stage->Text));
			timeWriter->WriteLine("	Z3: {0}", System::Convert::ToDouble(stage_txtZ2Stage->Text));
			timeWriter->WriteLine("	Z4: {0}", System::Convert::ToDouble(stage_txtZ3Stage->Text));
			timeWriter->WriteLine("	Z5: {0}", System::Convert::ToDouble(stage_txtZ4Stage->Text));
			timeWriter->WriteLine("	Z limit: {0}", stage_zLimit->Value);

			timeWriter->WriteLine("Scanner parameters: ");
			timeWriter->WriteLine("	Scanner jump time: {0}", scanner_numSetJumpTime->Value);
			timeWriter->WriteLine("	Scanner mark time: {0}", scanner_numSetMarkTime->Value);

			timeWriter->WriteLine("Wait between time points: {0}", experiment_nudWaitingTime->Value);
			timeWriter->WriteLine("Stage Travel time: {0}", ExperimentSetup_travelTime->Value);
			timeWriter->WriteLine("Number of time points: {0}", experiment_numNumberOfTimePointsValue->Value);
			timeWriter->WriteLine("Magnification: {0}", stage_numObjectiveMagnification->Value);

			timeWriter->Close();
			stageSave->Close();
			////////////////////////////////////////////////////////////////////////

			background_StartExperiment->RunWorkerAsync();

	}

	private: System::Void experiment_btnStopExperiment_Click(System::Object ^ sender, System::EventArgs ^ e) {

		if (background_StartExperiment->IsBusy)
		{
			background_StartExperiment->CancelAsync();
		}

		ExperimentStarted = false;
		LiveCam1::ExperimentStartedCam1 = ExperimentStarted;

		experiment_btnStopExperiment->Enabled = false;
		experiment_btnStartExperiment->Enabled = true;
		experiment_btnSetupExperiment->Enabled = true;

	}

	private: System::Void background_StartExperiment_DoWork(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e) {
		//Local variable, need to be put at beginning of each background worker
		SYSTEMTIME scanstarttime, scanstoptime;
		SYSTEMTIME systime, systime1, systimeTemp;
		scanningcont = true;
		numberofscans = 0;
		int timePoint;			//Number of timepoints
		numberofscans = 1;	//Number of scan per time point
		TimeForStepPlusScan = gcnew array <int>(NumberOfScansPerTimePoint);	//Array with all the times for step + scan
		
		int NoOfSteps = (int)ExperimentSetup_NoOfSteps->Value;
		int numberOfReducedPoints = 200; // Desired number of reduced points

		// Define constants for setting DIGITAL OUT1 to trigger Laser
		const unsigned int DIGITAL_OUT1 = 0x01; // Bit #0
		const unsigned int DIGITAL_OUT2 = 0x02; // Bit #1
		unsigned int Pins = 0; // Initiali   ze Pins to 0 or any other initial value

		////////////// calculate X velocity ////////////////////
		Decimal travelTime = ExperimentSetup_travelTime->Value;
		double XStepVel = abs((XPositions[4] - XPositions[0]) / (double)travelTime);
																	
		// for height adjustment
		double zPositionPixelsum; // temporary variable
		double zPositionPixelaverage, ZNew; // temporary variable
		array<double>^ zPositionPixels = gcnew array<double>(NoOfSteps); // container for height adjustment from acquisition software
		array<String^>^ pos = gcnew array<String^>(NoOfSteps); // container for height adjustment from acquisition software
		double zAxisLimit = (double) stage_zLimit->Value;

		double stepX = (double)stage_numWaveXStep->Value;
		int Mag = (int)stage_numObjectiveMagnification->Value;
		double physicalPixelSize = 0.0065; // 0.0065 mm = 6.5 um
		static double sqrt2 = 1.4142135623730950488016887242097;
		const double zPixelToAxis = (physicalPixelSize / sqrt2) / Mag;
		int startpos, endpos; // temporary variable

		array<double>^ XPositionsForAcq = gcnew array<double>(NoOfSteps); // container for stage position values (X-coordinate)
		array<int>^ positionBoundaries = gcnew array<int>(n_XPositions); // container ind
		array<double>^ DistanceX = gcnew array<double>(n_XPositions);
		array<double>^ DistanceZ = gcnew array<double>(n_XPositions);
		array<double>^ VelocityForZ = gcnew array<double>(n_XPositions);

		//get scan parameter from user interface
		X1para = (short)scanner_numX1->Value; //Get values of mark and just positions from user input
		X2para = (short)scanner_numX2->Value;
		Y1para = (short)scanner_numY1->Value;
		Y2para = (short)scanner_numY2->Value;

		WaitScan = 1000 * ((int)experiment_nudWaitingTime->Value);

		JumpTime = (double)(scanner_numSetJumpTime->Value) * 1000; //Get values of mark and jump speed from user input
		MarkTime = (double)(scanner_numSetMarkTime->Value) * 1000;
		DelayTime = (double)scanner_numSetMarkTime->Value;

		StreamWriter^ timeWriter;
		FileInfo^ stageFile = gcnew FileInfo(LiveCam1::Folder + "\\time.txt");
		FileStream^ stageSave = stageFile->Open(FileMode::Append, FileAccess::Write, FileShare::Read);
		timeWriter = gcnew StreamWriter(stageSave);

		timeWriter->WriteLine(" ");
		timeWriter->WriteLine(" ");
		timeWriter->WriteLine(" ");
		timeWriter->WriteLine(" ");
		timeWriter->WriteLine(" ");
		timeWriter->WriteLine(" ");
		timeWriter->WriteLine(" \t\t\t\t CONTINUOUS SCANNING ");
		timeWriter->WriteLine(" ");

		for (int o = 1; o <= 4; o++)
		{
			DistanceX[o] = XPositions[o] - XPositions[o - 1];
		}

		timeWriter->WriteLine("Total distance covered is {0} mm.", DistanceX[1] + DistanceX[2] + DistanceX[3] + DistanceX[4]);

		timeWriter->Flush();

	////////////////////////////////////////////////////////////////////////

	set_start_list(1);
	set_scanner_delays(static_cast<UINT>(DelayTime / 10), 0, 0);

		// define scan Job + trigger LASER CellX
		if (scanner_radioScanJobSingleChannel->Checked == true) {

			for (int stp = 0; stp < NoOfSteps; stp++)
			{
				// Jump: Laser OFF (OFF = HIGH)
				Pins |= DIGITAL_OUT1;                 // OFF = HIGH
				Pins &= ~DIGITAL_OUT2;                // keep other line low (optional)
				set_laser_pin_out_list(Pins);
				timed_jump_abs(Y1para, X1para, JumpTime);

				// Mark: Laser ON (ON = LOW pulse)
				Pins &= ~DIGITAL_OUT1;                // ON = LOW
				set_laser_pin_out_list(Pins);
				timed_mark_abs(Y2para, X2para, MarkTime);

			}
			timeWriter->WriteLine("The job parameters for Single channel mode: ");
			timeWriter->WriteLine("\t\t\tset_start_list(1)");
			timeWriter->WriteLine("\t\t\tset_scanner_delays({0} , 0, 0 )", (DelayTime / 10));
			timeWriter->WriteLine("\t\t\ttimed_jump_abs({0}, {1}, {2})", Y1para, X1para, JumpTime);
			timeWriter->WriteLine("\t\t\ttimed_mark_abs({0}, {1}, {2})", Y2para, X2para, MarkTime);
			timeWriter->WriteLine("\t\t\tset_end_of_list());");
			timeWriter->Flush();

		}
		//Calculate scanner cycle time of- mark (channel 1)+mark(channel 2)+delays
		//Calculate scanner cycle time of- mark (channel 1)+mark(channel 2)+delays
		else if (scanner_radioScanJobDualChannel->Checked == true) {
			for (int stp = 0; stp < NoOfSteps; stp++)
			{
				// Jump: both OFF (OFF = HIGH)
				Pins |= (DIGITAL_OUT1 | DIGITAL_OUT2);     // both HIGH = OFF
				set_laser_pin_out_list(Pins);
				timed_jump_abs(Y1para, X1para, JumpTime);

				// Mark: 488 ON (LOW), 561 OFF (HIGH)
				Pins |= DIGITAL_OUT2;                      // 561 OFF = HIGH
				Pins &= ~DIGITAL_OUT1;                     // 488 ON = LOW
				set_laser_pin_out_list(Pins);
				timed_mark_abs(Y2para, X2para, MarkTime);

				// Jump: both OFF (HIGH)
				Pins |= (DIGITAL_OUT1 | DIGITAL_OUT2);
				set_laser_pin_out_list(Pins);
				timed_jump_abs(Y2para, X2para, JumpTime);

				// Mark: 561 ON (LOW), 488 OFF (HIGH)
				Pins |= DIGITAL_OUT1;                      // 488 OFF = HIGH
				Pins &= ~DIGITAL_OUT2;                     // 561 ON = LOW
				set_laser_pin_out_list(Pins);
				timed_mark_abs(Y1para, X1para, MarkTime);

			}

			timeWriter->WriteLine("The job parameters for Dual channel mode: ");
			timeWriter->WriteLine("set_start_list(1)");
			timeWriter->WriteLine("\t\t\tset_scanner_delays({0} , 0, 0 )", (DelayTime / 10));
			timeWriter->WriteLine("\t\t\ttimed_jump_abs({0}, {1}, {2})", Y1para, X1para, JumpTime);
			timeWriter->WriteLine("\t\t\ttimed_mark_abs({0}, {1}, {2})", Y2para, X2para, MarkTime);
			timeWriter->WriteLine("\t\t\ttimed_jump_abs({0}, {1}, {2})", Y2para, X2para, JumpTime);
			timeWriter->WriteLine("\t\t\ttimed_mark_abs({0}, {1}, {2})", Y1para, X1para, MarkTime);
			timeWriter->WriteLine("set_end_of_list());");
			timeWriter->Flush();

		}

		// End safe: lasers OFF
		Pins &= ~(DIGITAL_OUT1 | DIGITAL_OUT2);
		set_laser_pin_out_list(Pins);

		set_end_of_list();

		/////////////////////////// start acquiring TP////////////////////////////

		GetSystemTime(&systime);
		long CurrentTime = static_cast<long>(systime.wMilliseconds) + static_cast<long>(systime.wSecond) * 1000 + static_cast<long>(systime.wMinute) * 1000 * 60 +static_cast<long>(systime.wHour) * 1000 * 60 * 60;
		long PreviousTime = CurrentTime - WaitScan;
		long PreviousDay = static_cast<long>(systime.wDay);

		timePoint = 1;

		while (timePoint <= NumberOfTimePoints && ExperimentStarted == true)
		{

			// Wait until WaitScan [ms] time has passed between iterations
			GetSystemTime(&systime);
			CurrentTime = static_cast<long>(systime.wMilliseconds) +static_cast<long>(systime.wSecond) * 1000 + static_cast<long>(systime.wMinute) * 1000 * 60 + static_cast<long>(systime.wHour) * 1000 * 60 * 60;
			if (PreviousDay != static_cast<long>(systime.wDay))
				PreviousTime -= 1000 * 60 * 60 * 24;

			while ((PreviousTime + WaitScan > CurrentTime) && ExperimentStarted == true) {
				Sleep(100);
				GetSystemTime(&systime);
				CurrentTime = static_cast<long>(systime.wMilliseconds) +static_cast<long>(systime.wSecond) * 1000 +static_cast<long>(systime.wMinute) * 1000 * 60 +static_cast<long>(systime.wHour) * 1000 * 60 * 60;
				if (PreviousDay != static_cast<long>(systime.wDay)) {
					PreviousTime -= 1000 * 60 * 60 * 24;
					PreviousDay = static_cast<long>(systime.wDay);
				}
			}

			GetSystemTime(&systime);
			PreviousDay = (long)systime.wDay;
			PreviousTime = ((long)systime.wMilliseconds) + ((long)systime.wSecond) * 1000 + ((long)systime.wMinute) * 1000 * 60 + ((long)systime.wHour) * 1000 * 60 * 60;

			timeWriter->WriteLine("//////////////////");
			timeWriter->Write("Time Point: {0}", timePoint);
			timeWriter->Write(" \t\t {0} \r\n", DateTime::Now.ToString("HH:mm:ss.fff"));
			timeWriter->WriteLine(" ");
			timeWriter->Flush();

			XPositionsForAcq[0] = XPositions[0];
			positionBoundaries[0] = 0;
			DistanceX[0] = 0;
			DistanceZ[0] = 0;
			VelocityForZ[0] = 0;
			int currentBoundaryIndex = 1;

			for (int j = 1; j <= (n_XPositions - 1); j++)
			{
				DistanceX[j] = abs(XPositions[j] - XPositions[j - 1]);
				DistanceZ[j] = abs(ZPositions[j] - ZPositions[j - 1]);
				// avoid zero velocity
				if (abs(DistanceZ[j]) < 0.001) {
					ZPositions[j] -= 0.001;
					DistanceZ[j] = ZPositions[j] - ZPositions[j - 1];
				}

				// check that the velocity is positive
				VelocityForZ[j] = abs(((DistanceZ[j] / DistanceX[j]) * XStepVel)); // [mm/s]
			}

			for (int i = 1; i < NoOfSteps - 1; i++)
			{
				XPositionsForAcq[i] = XPositions[0] - i * stepX;
				if (XPositionsForAcq[i] < XPositions[currentBoundaryIndex])
				{
					positionBoundaries[currentBoundaryIndex] = i - 1;
					currentBoundaryIndex++;
				}
				}

			positionBoundaries[n_XPositions - 1] = NoOfSteps - 2;

			GetSystemTime(&scanstarttime);

			for (int yIndex = 0; yIndex < Yloop; yIndex++)
			{
				StageFunctionsOK = StageFunctions::ToggleOutputOnOff(ControllerBox2ID, 1, 0); //Turn the external trigger off

				LiveCam1::yIndex = (int)yIndex;

				//Bring axes to their initial positions
				StageFunctionsOK = StageFunctions::SetAxisVelocity(ControllerBox2ID, XAxis, 1);
				StageFunctionsOK = StageFunctions::SetAxisVelocity(ControllerBox2ID, YAxis, 1);
				StageFunctionsOK = StageFunctions::SetAxisVelocity(ControllerBox1ID, ZAxis, 0.5);

				double MaxZ = ZPositions[0]; //[mm]
				for (int PointIndex = 1; PointIndex < n_XPositions; PointIndex++)
					if (ZPositions[PointIndex] < MaxZ)
						MaxZ = ZPositions[PointIndex];

				StageFunctionsOK = StageFunctions::MoveAxisToThisExactPosition(ControllerBox1ID, ZAxis, MaxZ);
				StageFunctionsOK = StageFunctions::StageIsMoving(ControllerBox1ID, ZAxis);
				while (StageFunctionsOK)
				{
					StageFunctionsOK = StageFunctions::StageIsMoving(ControllerBox1ID, ZAxis);
				}

				StageFunctionsOK = StageFunctions::MoveAxisToThisExactPosition(ControllerBox2ID, YAxis, YPositions[yIndex]);

				StageFunctionsOK = StageFunctions::MoveAxisToThisExactPosition(ControllerBox2ID, XAxis, XPositions[0]);

				StageFunctionsOK = StageFunctions::MoveAxisToThisExactPosition(ControllerBox1ID, ZAxis, ZPositions[0]);

				StageFunctionsOK = StageFunctions::StageIsMoving(ControllerBox2ID, YAxis);
				while (StageFunctionsOK != FALSE)
				{
					StageFunctionsOK = StageFunctions::StageIsMoving(ControllerBox2ID, YAxis);
				}

				StageFunctionsOK = StageFunctions::StageIsMoving(ControllerBox2ID, XAxis);
				while (StageFunctionsOK != FALSE)
				{
					StageFunctionsOK = StageFunctions::StageIsMoving(ControllerBox2ID, XAxis);
				}

				StageFunctionsOK = StageFunctions::StageIsMoving(ControllerBox1ID, ZAxis);
				while (StageFunctionsOK)
				{
					StageFunctionsOK = StageFunctions::StageIsMoving(ControllerBox1ID, ZAxis);
				}

				// set experimental parameters for the stage
				StageFunctionsOK = StageFunctions::SetAxisVelocity(ControllerBox2ID, XAxis, XStepVel);
				StageFunctionsOK = StageFunctions::SetAxisVelocity(ControllerBox1ID, ZAxis, VelocityForZ[1]);

				/////////////////// set output trigget conditions ///////////////////////////

				StageFunctionsOK = StageFunctions::ConfigureTriggerOutput(ControllerBox2ID, 1, 2, 1.0); //Assign trigger output to X axis
				StageFunctionsOK = StageFunctions::ConfigureTriggerOutput(ControllerBox2ID, 1, 3, 6.0); //Set output trigger to in motion
				StageFunctionsOK = StageFunctions::ConfigureTriggerOutput(ControllerBox2ID, 1, 7, 0.0); // Set polarity to active HIGH

				StageFunctionsOK = StageFunctions::ConfigureTriggerInput(ControllerBox1ID, 1, 3, 2);

				/////////////// start trigger and mvoing X, Z axis///////////////////////////

				StageFunctionsOK = StageFunctions::ToggleOutputOnOff(ControllerBox2ID, 1, 1); //Turn the external trigger on.
				StageFunctionsOK = StageFunctions::ToggleInputOnOff(ControllerBox1ID, 1, 1); //Turn the input trigger on.

				//Check if stage is ready for starting the timepoint, only when this is TRUE the code proceeds
				StageFunctionsOK = StageFunctions::ControllerIsReadyForNewCommand(ControllerBox1ID);
				while (StageFunctionsOK != TRUE)
				{
					StageFunctionsOK = StageFunctions::ControllerIsReadyForNewCommand(ControllerBox1ID);
				}
				StageFunctionsOK = StageFunctions::ControllerIsReadyForNewCommand(ControllerBox2ID);
				while (StageFunctionsOK != TRUE)
				{
					StageFunctionsOK = StageFunctions::ControllerIsReadyForNewCommand(ControllerBox2ID);
				}

				LiveCam1::ScannerIsRunning = true;

				timeWriter->WriteLine("\tX\t\t\tY\t\t\tZ\t\t\ttime\t\t\txVel\t\t\tzVel");
				GetSystemTime(&systime);
				timeWriter->WriteLine("\t{0:0.0000000000000000}\t{1:0.0000000000000000}\t{2:0.0000000000000000}\t{3}\t{4:0.0000000000000000}\t{5:0.0000000000000000}", StageFunctions::GetAxisPosition(ControllerBox2ID, XAxis), StageFunctions::GetAxisPosition(ControllerBox2ID, YAxis), StageFunctions::GetAxisPosition(ControllerBox1ID, ZAxis), DateTime::Now.ToString("HH:mm:ss.fff"), XStepVel, VelocityForZ[1]);

				//execute scan Job
				execute_list(1);				// start marking; scanJob

				StageFunctionsOK = StageFunctions::MoveAxisToThisExactPosition(ControllerBox2ID, XAxis, XPositions[n_XPositions - 1]);

				StageFunctionsOK = StageFunctions::MoveAxisToThisExactPosition(ControllerBox1ID, ZAxis, ZPositions[1]);

				for (int j = 2; j <= (n_XPositions - 1); j++) {
					//wait till z is finish
					StageFunctionsOK = StageFunctions::StageIsMoving(ControllerBox1ID, ZAxis);
					while (StageFunctionsOK)
					{
						StageFunctionsOK = StageFunctions::StageIsMoving(ControllerBox1ID, ZAxis);
					}
					timeWriter->WriteLine("\t{0:0.0000000000000000}\t{1:0.0000000000000000}\t{2:0.0000000000000000}\t{3}\t{4:0.0000000000000000}\t{5:0.0000000000000000}", StageFunctions::GetAxisPosition(ControllerBox2ID, XAxis), StageFunctions::GetAxisPosition(ControllerBox2ID, YAxis), StageFunctions::GetAxisPosition(ControllerBox1ID, ZAxis), DateTime::Now.ToString("HH:mm:ss.fff"), XStepVel, VelocityForZ[j]);

					//set new z velocity (one based index)
					StageFunctionsOK = StageFunctions::SetAxisVelocity(ControllerBox1ID, ZAxis, VelocityForZ[j]);

					// start moving Z axis (zero based index)
					StageFunctionsOK = StageFunctions::MoveAxisToThisExactPosition(ControllerBox1ID, ZAxis, ZPositions[j]);
				}

				StageFunctionsOK = StageFunctions::StageIsMoving(ControllerBox2ID, XAxis);
				while (StageFunctionsOK != FALSE)
				{
					StageFunctionsOK = StageFunctions::StageIsMoving(ControllerBox2ID, XAxis);
				}

				//Check if stage is moving, only when this is FALSE the code proceeds
				StageFunctionsOK = StageFunctions::StageIsMoving(ControllerBox1ID, ZAxis);
				while (StageFunctionsOK)
				{
					StageFunctionsOK = StageFunctions::StageIsMoving(ControllerBox1ID, ZAxis);
				}
				
				LiveCam1::ScannerIsRunning = false;

				GetSystemTime(&systimeTemp);
				timeWriter->WriteLine("\t{0:0.0000000000000000}\t{1:0.0000000000000000}\t{2:0.0000000000000000}\t{3}\t{4:0.0000000000000000}\t{5:0.0000000000000000}", StageFunctions::GetAxisPosition(ControllerBox2ID, XAxis), StageFunctions::GetAxisPosition(ControllerBox2ID, YAxis), StageFunctions::GetAxisPosition(ControllerBox1ID, ZAxis), DateTime::Now.ToString("HH:mm:ss.fff"), safe_cast<System::Object^>(0),safe_cast<System::Object^>(0));
				//velocities are written as zero. This is needed for the MATLAB stage plot script.

				GetSystemTime(&systime1);
				timeWriter->WriteLine("\tTime passed: {0} ms", ((systime1.wHour - systime.wHour) * 60 * 60 * 1000 + (systime1.wMinute - systime.wMinute) * 60 * 1000 + (systime1.wSecond - systime.wSecond) * 1000 + (systime1.wMilliseconds - systime.wMilliseconds)));
				timeWriter->WriteLine("");
				timeWriter->Flush();

				StageFunctionsOK = StageFunctions::ToggleOutputOnOff(ControllerBox2ID, 1, 0); //Turn the external trigger off (avoids retriggering). //check whether this is necessary
				StageFunctionsOK = StageFunctions::ToggleInputOnOff(ControllerBox1ID, 1, 0); //Turn the input trigger off.

				if (background_StartExperiment->CancellationPending)
				{
					GetSystemTime(&scanstoptime);
					ExperimentStarted = false;
					LiveCam1::ExperimentStartedCam1 = ExperimentStarted;

					delete[] TimeForStepPlusScan;

					e->Cancel = true;
					break;
				}
				else
				{
					GetSystemTime(&scanstoptime);

					timePoint++;
					numberofscans = 1;

					GetSystemTime(&scanstarttime);	//In case the experiment is cancelled here, this time will go to info file
					if (NrOfCameras == 1)
					{
						while (LiveCam1::AcquireNextTimePoint == false)
						{
							if (background_StartExperiment->CancellationPending)
							{
								GetSystemTime(&scanstoptime);
								ExperimentStarted = false;

								delete[] TimeForStepPlusScan;

								e->Cancel = true;
								break;
							}
						}
					}
				}

				/////////////////////////////////////////   height adjustment calculation   //////////////////////////////////////////////////////////

				timeWriter->WriteLine("\t{0}\tWaiting to open height adjustment file...", DateTime::Now.ToString("HH:mm:ss.fff"));

				if (yIndex == (Yloop - 1))// the height adjustment compensation will happen after all Y coordinates are covered
				{

					int smoothingPoints = 24;  // Number of points to consider on each side of the current point
					int iterations = 10;      // Number of smoothing passes

					for (int i = 0; i < NoOfSteps; i++)
					{
						zPositionPixels[i] = LiveCam1::ImageHeightAdjustmentPixelsDifference[i]; // same pixel value is repeated
					}

					timeWriter->WriteLine("\t{0}\tPosition file read.", DateTime::Now.ToString("HH:mm:ss.fff"));

					for (int ZIndex = 0; ZIndex < n_XPositions; ZIndex++) { // n_XPositions = number points used for interpolation
						zPositionPixelsum = 0;
						zPositionPixelaverage = 0;

						if (ZIndex > 0) { startpos = (positionBoundaries[ZIndex - 1] + positionBoundaries[ZIndex]) / 2; }
						else { startpos = 0; }

						if (ZIndex < (n_XPositions - 1)) { endpos = (positionBoundaries[ZIndex] + positionBoundaries[ZIndex + 1]) / 2; } // n_XPositions = number points used for interpolation
						else { endpos = positionBoundaries[n_XPositions - 1]; } // n_XPositions = number points used for interpolation

						for (int counter = startpos; counter <= endpos; counter++) {
							zPositionPixelsum += zPositionPixels[counter];
						}

						zPositionPixelaverage = (int)(zPositionPixelsum / (endpos - startpos + 1));
						ZNew = ZPositions[ZIndex] - zPositionPixelaverage * zPixelToAxis;
						if (abs(ZNew) < abs(zAxisLimit)) {
							ZPositions[ZIndex] = ZNew;
						}
						else {
							timeWriter->WriteLine("\tThe Height Adjustment for Z{0} point it is out of boundaries, the new Z{1} value is {2} mm", ZIndex + 1, ZIndex + 1, ZNew);
						}
					}
					GetSystemTime(&systime);
					timeWriter->WriteLine("\t{0}\tNew heights evaluated.", DateTime::Now.ToString("HH:mm:ss.fff"));

					timeWriter->Flush();
				}
				/////////////////////////////////////////////////////// end of Height Adjustment calculations //////////////////////////////////

			Sleep(1000);

			} // end of y loop

		}

		timeWriter->Close();
		stageSave->Close();
	}

	private:

#pragma endregion

private: System::Void txtTestBox_TextChanged(System::Object^ sender, System::EventArgs^ e) {
}
};
}

