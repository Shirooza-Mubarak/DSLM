#pragma once

namespace MyDSLM {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for ImageSizeForm
	/// </summary>
	public ref class ImageSizeForm : public System::Windows::Forms::Form
	{
	public:
		ImageSizeForm(void)
		{
			InitializeComponent();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ImageSizeForm()
		{
			if (components)
			{
				delete components;
			}
		}

	public:
		//-----Variable to be passed to main form-----//
		static int imageXSize = 2048;
		static int imageXOffSet = 0;
		static int imageYSize = 400;
		static int imageYOffSet = 824;

	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::TrackBar^ image_trackBarYOffSet;

	private: System::Windows::Forms::TrackBar^ image_trackBarXOffSet;

	private: System::Windows::Forms::TextBox^ image_CameraPosition;
	private: System::Windows::Forms::TextBox^ image_Reference;
	private: System::Windows::Forms::TrackBar^ image_trackBarY;

	private: System::Windows::Forms::TrackBar^ image_trackBarX;
	private: System::Windows::Forms::Button^ image_btnOK;

	private: System::Windows::Forms::NumericUpDown^ image_numXSize;

	private: System::Windows::Forms::NumericUpDown^ image_numXOffset;
	private: System::Windows::Forms::NumericUpDown^ image_numYSize;
	private: System::Windows::Forms::NumericUpDown^ image_numYOffset;
	private: System::Windows::Forms::TextBox^ image_centerline;
	private: System::Windows::Forms::TextBox^ image_txtActionBox;

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
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->image_trackBarYOffSet = (gcnew System::Windows::Forms::TrackBar());
			this->image_trackBarXOffSet = (gcnew System::Windows::Forms::TrackBar());
			this->image_CameraPosition = (gcnew System::Windows::Forms::TextBox());
			this->image_Reference = (gcnew System::Windows::Forms::TextBox());
			this->image_trackBarY = (gcnew System::Windows::Forms::TrackBar());
			this->image_trackBarX = (gcnew System::Windows::Forms::TrackBar());
			this->image_btnOK = (gcnew System::Windows::Forms::Button());
			this->image_numXSize = (gcnew System::Windows::Forms::NumericUpDown());
			this->image_numXOffset = (gcnew System::Windows::Forms::NumericUpDown());
			this->image_numYSize = (gcnew System::Windows::Forms::NumericUpDown());
			this->image_numYOffset = (gcnew System::Windows::Forms::NumericUpDown());
			this->image_centerline = (gcnew System::Windows::Forms::TextBox());
			this->image_txtActionBox = (gcnew System::Windows::Forms::TextBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->image_trackBarYOffSet))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->image_trackBarXOffSet))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->image_trackBarY))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->image_trackBarX))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->image_numXSize))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->image_numXOffset))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->image_numYSize))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->image_numYOffset))->BeginInit();
			this->SuspendLayout();
			// label4
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(615, 92);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(45, 13);
			this->label4->TabIndex = 19;
			this->label4->Text = L"Y Offset";
			// label3
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(625, 43);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(35, 13);
			this->label3->TabIndex = 18;
			this->label3->Text = L"Y size";
			// label2
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(12, 575);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(45, 13);
			this->label2->TabIndex = 17;
			this->label2->Text = L"X Offset";
			// label1
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 531);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(35, 13);
			this->label1->TabIndex = 16;
			this->label1->Text = L"X size";
			// image_trackBarYOffSet
			this->image_trackBarYOffSet->Location = System::Drawing::Point(581, 16);
			this->image_trackBarYOffSet->Maximum = 512;
			this->image_trackBarYOffSet->Minimum = 12;
			this->image_trackBarYOffSet->Name = L"image_trackBarYOffSet";
			this->image_trackBarYOffSet->Orientation = System::Windows::Forms::Orientation::Vertical;
			this->image_trackBarYOffSet->Size = System::Drawing::Size(45, 512);
			this->image_trackBarYOffSet->TabIndex = 15;
			this->image_trackBarYOffSet->TickStyle = System::Windows::Forms::TickStyle::None;
			this->image_trackBarYOffSet->Value = 16;
			this->image_trackBarYOffSet->ValueChanged += gcnew System::EventHandler(this, &ImageSizeForm::trackBar_ValueChanged);
			// image_trackBarXOffSet
			this->image_trackBarXOffSet->Location = System::Drawing::Point(12, 592);
			this->image_trackBarXOffSet->Maximum = 512;
			this->image_trackBarXOffSet->Minimum = 12;
			this->image_trackBarXOffSet->Name = L"image_trackBarXOffSet";
			this->image_trackBarXOffSet->Size = System::Drawing::Size(512, 45);
			this->image_trackBarXOffSet->TabIndex = 14;
			this->image_trackBarXOffSet->TickStyle = System::Windows::Forms::TickStyle::None;
			this->image_trackBarXOffSet->Value = 12;
			this->image_trackBarXOffSet->ValueChanged += gcnew System::EventHandler(this, &ImageSizeForm::trackBar_ValueChanged);
			// image_CameraPosition
			this->image_CameraPosition->BackColor = System::Drawing::Color::Lime;
			this->image_CameraPosition->Location = System::Drawing::Point(12, 12);
			this->image_CameraPosition->MaxLength = 530;
			this->image_CameraPosition->Multiline = true;
			this->image_CameraPosition->Name = L"image_CameraPosition";
			this->image_CameraPosition->ReadOnly = true;
			this->image_CameraPosition->Size = System::Drawing::Size(512, 64);
			this->image_CameraPosition->TabIndex = 13;
			// image_Reference
			this->image_Reference->BackColor = System::Drawing::SystemColors::ActiveCaption;
			this->image_Reference->Location = System::Drawing::Point(12, 12);
			this->image_Reference->Multiline = true;
			this->image_Reference->Name = L"image_Reference";
			this->image_Reference->ReadOnly = true;
			this->image_Reference->Size = System::Drawing::Size(512, 512);
			this->image_Reference->TabIndex = 12;
			// image_trackBarY
			this->image_trackBarY->Location = System::Drawing::Point(530, 16);
			this->image_trackBarY->Maximum = 512;
			this->image_trackBarY->Name = L"image_trackBarY";
			this->image_trackBarY->Orientation = System::Windows::Forms::Orientation::Vertical;
			this->image_trackBarY->Size = System::Drawing::Size(45, 512);
			this->image_trackBarY->TabIndex = 11;
			this->image_trackBarY->TickStyle = System::Windows::Forms::TickStyle::None;
			this->image_trackBarY->ValueChanged += gcnew System::EventHandler(this, &ImageSizeForm::trackBar_ValueChanged);
			// image_trackBarX
			this->image_trackBarX->Location = System::Drawing::Point(12, 547);
			this->image_trackBarX->Maximum = 512;
			this->image_trackBarX->Name = L"image_trackBarX";
			this->image_trackBarX->Size = System::Drawing::Size(512, 45);
			this->image_trackBarX->TabIndex = 10;
			this->image_trackBarX->TickStyle = System::Windows::Forms::TickStyle::None;
			this->image_trackBarX->ValueChanged += gcnew System::EventHandler(this, &ImageSizeForm::trackBar_ValueChanged);
			// image_btnOK
			this->image_btnOK->Location = System::Drawing::Point(530, 547);
			this->image_btnOK->Name = L"image_btnOK";
			this->image_btnOK->Size = System::Drawing::Size(75, 63);
			this->image_btnOK->TabIndex = 20;
			this->image_btnOK->Text = L"OK";
			this->image_btnOK->UseVisualStyleBackColor = true;
			this->image_btnOK->Click += gcnew System::EventHandler(this, &ImageSizeForm::image_btnOK_Click);
			// image_numXSize
			this->image_numXSize->Location = System::Drawing::Point(53, 529);
			this->image_numXSize->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 2048, 0, 0, 0 });
			this->image_numXSize->Name = L"image_numXSize";
			this->image_numXSize->Size = System::Drawing::Size(120, 20);
			this->image_numXSize->TabIndex = 21;
			this->image_numXSize->ValueChanged += gcnew System::EventHandler(this, &ImageSizeForm::numeric_ValueChanged);
			// image_numXOffset
			this->image_numXOffset->Location = System::Drawing::Point(60, 573);
			this->image_numXOffset->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 2048, 0, 0, 0 });
			this->image_numXOffset->Name = L"image_numXOffset";
			this->image_numXOffset->Size = System::Drawing::Size(120, 20);
			this->image_numXOffset->TabIndex = 22;
			this->image_numXOffset->ValueChanged += gcnew System::EventHandler(this, &ImageSizeForm::numeric_ValueChanged);
			// image_numYSize
			this->image_numYSize->Location = System::Drawing::Point(666, 41);
			this->image_numYSize->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 2048, 0, 0, 0 });
			this->image_numYSize->Name = L"image_numYSize";
			this->image_numYSize->Size = System::Drawing::Size(120, 20);
			this->image_numYSize->TabIndex = 23;
			this->image_numYSize->ValueChanged += gcnew System::EventHandler(this, &ImageSizeForm::numeric_ValueChanged);
			// image_numYOffset
			this->image_numYOffset->Location = System::Drawing::Point(666, 90);
			this->image_numYOffset->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 2048, 0, 0, 0 });
			this->image_numYOffset->Name = L"image_numYOffset";
			this->image_numYOffset->Size = System::Drawing::Size(120, 20);
			this->image_numYOffset->TabIndex = 24;
			this->image_numYOffset->ValueChanged += gcnew System::EventHandler(this, &ImageSizeForm::numeric_ValueChanged);
			// image_centerline
			this->image_centerline->BackColor = System::Drawing::Color::Red;
			this->image_centerline->Location = System::Drawing::Point(12, 268);
			this->image_centerline->Multiline = true;
			this->image_centerline->Name = L"image_centerline";
			this->image_centerline->ReadOnly = true;
			this->image_centerline->Size = System::Drawing::Size(512, 1);
			this->image_centerline->TabIndex = 25;
			// image_txtActionBox
			this->image_txtActionBox->Location = System::Drawing::Point(12, 636);
			this->image_txtActionBox->Name = L"image_txtActionBox";
			this->image_txtActionBox->Size = System::Drawing::Size(780, 20);
			this->image_txtActionBox->TabIndex = 26;
			// ImageSizeForm
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(804, 668);
			this->Controls->Add(this->image_txtActionBox);
			this->Controls->Add(this->image_centerline);
			this->Controls->Add(this->image_numYOffset);
			this->Controls->Add(this->image_numYSize);
			this->Controls->Add(this->image_numXOffset);
			this->Controls->Add(this->image_numXSize);
			this->Controls->Add(this->image_btnOK);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->image_trackBarYOffSet);
			this->Controls->Add(this->image_trackBarXOffSet);
			this->Controls->Add(this->image_CameraPosition);
			this->Controls->Add(this->image_Reference);
			this->Controls->Add(this->image_trackBarY);
			this->Controls->Add(this->image_trackBarX);
			this->Name = L"ImageSizeForm";
			this->Text = L"ImageSizeForm";
			this->Load += gcnew System::EventHandler(this, &ImageSizeForm::ImageSizeForm_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->image_trackBarYOffSet))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->image_trackBarXOffSet))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->image_trackBarY))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->image_trackBarX))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->image_numXSize))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->image_numXOffset))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->image_numYSize))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->image_numYOffset))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

private: System::Void trackBar_ValueChanged(System::Object^ sender, System::EventArgs^ e) {
	image_trackBarXOffSet->Maximum = (512 + 12) - image_trackBarX->Value;
	image_trackBarYOffSet->Maximum = (512 + 12) - image_trackBarY->Value;

	image_numXSize->Value = image_trackBarX->Value * 2048 / image_trackBarX->Maximum; //Gets ratio of bar size to real image size
	image_numXOffset->Value = (image_trackBarXOffSet->Value - 12) * 2048 / image_trackBarX->Maximum;

	image_numYSize->Value = image_trackBarY->Value * 2048 / image_trackBarY->Maximum;
	image_numYOffset->Value = (image_trackBarYOffSet->Value - 12) * 2048 / image_trackBarY->Maximum;

	image_CameraPosition->Location = System::Drawing::Point(image_trackBarXOffSet->Value, image_trackBarYOffSet->Value);
	image_CameraPosition->Size = System::Drawing::Size(image_trackBarX->Value, image_trackBarY->Value);
}
private: System::Void numeric_ValueChanged(System::Object^ sender, System::EventArgs^ e) {
	image_numXOffset->Maximum = 2048 - image_numXSize->Value;
	image_numYOffset->Maximum = 2048 - image_numYSize->Value;

	image_trackBarX->Value = System::Convert::ToInt16((double)image_numXSize->Value * 512 / 2048);
	image_trackBarXOffSet->Value = System::Convert::ToInt16((double)image_numXOffset->Value * 512 / 2048 + 12);

	image_trackBarY->Value = System::Convert::ToInt16((double)image_numYSize->Value * 512 / 2048);
	image_trackBarYOffSet->Value = System::Convert::ToInt16((double)image_numYOffset->Value * 512 / 2048 + 12);
}

private: System::Void image_btnOK_Click(System::Object^ sender, System::EventArgs^ e) {
	image_txtActionBox->BackColor = System::Drawing::Color::FromArgb(255, 255, 255, 255);	//Reset actions box to white

	//Get image X, Y, XOffset and YOffset into integer variables
	imageXSize = (int)image_numXSize->Value;
	imageXOffSet = (int)image_numXOffset->Value;
	imageYSize = (int)image_numYSize->Value;
	imageYOffSet = (int)image_numYOffset->Value;

	if (imageXSize % 128 == 0 && imageXOffSet % 4 == 0 && imageYSize % 4 == 0 && imageYOffSet % 4 == 0) //check that all sizes are a multiple of 4, if not the camera function will fail
	{
		this->Close(); //If they are a multiple of 4 close the form and continue image size set function
	}
	else  //if not display an error message and change the action box to red
	{
		image_txtActionBox->Text = "Make sure the X image size is a multiple of 128 and the X offset, Y size and Y offset are multiples of 4";
		image_txtActionBox->BackColor = System::Drawing::Color::FromArgb(255, 255, 0, 0);	//change colour of text box to red
	}
}

private: System::Void ImageSizeForm_Load(System::Object^ sender, System::EventArgs^ e) {
	//Allows previous user input to be viewed when set image size form is re opened
	image_trackBarX->Value = imageXSize / 4; //track bar is value divided by 4 as the reference image size is 4 times small than actualy size
	image_trackBarXOffSet->Value = imageXOffSet / 4 + 12; //track bar value has 12 added to is as the offset from left side of form is 12

	image_trackBarY->Value = imageYSize / 4; //track bar is value divided by 4 as the reference image size is 4 times small than actualy size
	image_trackBarYOffSet->Value = imageYOffSet / 4 + 12; //track bar value has 12 added to is as the offset from left side of form is 12

	//Following lines redraw the image text box in the location and size of pervious input values
	this->image_CameraPosition->Size = System::Drawing::Size(System::Convert::ToInt16(imageXSize * 512 / 2048), System::Convert::ToInt16(imageYSize * 512 / 2048));
	this->image_CameraPosition->Location = System::Drawing::Point(System::Convert::ToInt16(imageXOffSet * 512 / 2048 + 12), System::Convert::ToInt16(imageYOffSet * 512 / 2048 + 12));
	}
};
}

