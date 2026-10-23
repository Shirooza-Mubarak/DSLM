#include "stdafx.h"
#include"MainForm.h"
#include"LiveCam1.h"
#include"ImageSizeForm.h"

using namespace MyDSLM;

[STAThreadAttribute]
void main()
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(true);

	Form^ starting = gcnew Form();
	starting->Name = "DSLMStartingWindow";
	starting->Text = "DSLM";
	starting->ClientSize = System::Drawing::Size(300, 80);
	starting->StartPosition = FormStartPosition::CenterScreen;
	starting->FormBorderStyle = FormBorderStyle::FixedDialog;
	starting->ControlBox = false;
	starting->UseWaitCursor = true;

	Label^ message = gcnew Label();
	message->Text = "Starting DSLM... Please wait.";
	message->Dock = DockStyle::Fill;
	message->TextAlign =
		System::Drawing::ContentAlignment::MiddleCenter;

	starting->Controls->Add(message);
	starting->Show();
	starting->Refresh();

	MainForm^ mainWindow = gcnew MainForm();
	Application::Run(mainWindow);
}
