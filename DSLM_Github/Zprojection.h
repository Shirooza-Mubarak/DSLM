#pragma once

namespace MyDSLM {

    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;
    using namespace System::Drawing::Imaging;

    public ref class Zprojection : public System::Windows::Forms::Form
    {
    public:
        
        Zprojection(int width, int height)
        {
            InitializeComponent();
            this->ClientSize = System::Drawing::Size(width, height);
            this->BackColor = Color::Black; // Set background to black for better contrast
        }

        void UpdateZprojectionImage(Bitmap^ zProjectionBitmap)
        {
            if (zProjectionBitmap == nullptr || this->IsDisposed || this->Disposing)
                return;

            if (this->InvokeRequired)
            {
                this->Invoke(
                    gcnew Action<Bitmap^>(
                        this, &Zprojection::UpdateZprojectionImage),
                    gcnew cli::array<System::Object^>(1) { zProjectionBitmap });
                return;
            }

            Bitmap^ nextImage =
                safe_cast<Bitmap^>(zProjectionBitmap->Clone());

            System::Drawing::Image^ previousImage =
                pictureBoxZProjection->Image;

            pictureBoxZProjection->Image = nextImage;

            if (previousImage != nullptr)
                delete previousImage;

            pictureBoxZProjection->Refresh();
        }

    protected:
        ~Zprojection()
        {
            if (components)
            {
                delete components;
            }
        }

    private:
        Bitmap^ ConvertToTrueGrayscale(Bitmap^ original)
        {
            Bitmap^ newBitmap = gcnew Bitmap(original->Width, original->Height, Imaging::PixelFormat::Format24bppRgb);
            Graphics^ g = Graphics::FromImage(newBitmap);

            // Create the grayscale ColorMatrix
            array<float>^ matrixElements = gcnew array<float>(5 * 5) {
                0.299f, 0.299f, 0.299f, 0, 0,
                    0.587f, 0.587f, 0.587f, 0, 0,
                    0.114f, 0.114f, 0.114f, 0, 0,
                    0, 0, 0, 1, 0,
                    0, 0, 0, 0, 1
            };

            // Create the ColorMatrix using the array
            Imaging::ColorMatrix^ colorMatrix = gcnew Imaging::ColorMatrix();
            colorMatrix->Matrix00 = matrixElements[0];
            colorMatrix->Matrix01 = matrixElements[1];
            colorMatrix->Matrix02 = matrixElements[2];
            colorMatrix->Matrix10 = matrixElements[5];
            colorMatrix->Matrix11 = matrixElements[6];
            colorMatrix->Matrix12 = matrixElements[7];
            colorMatrix->Matrix20 = matrixElements[10];
            colorMatrix->Matrix21 = matrixElements[11];
            colorMatrix->Matrix22 = matrixElements[12];

            // Set up the ImageAttributes
            Imaging::ImageAttributes^ attributes = gcnew Imaging::ImageAttributes();
            attributes->SetColorMatrix(colorMatrix);

            // Draw the image with the color matrix applied
            g->DrawImage(original,
                System::Drawing::Rectangle(0, 0, original->Width, original->Height),
                0, 0, original->Width, original->Height,
                GraphicsUnit::Pixel, attributes);

            delete g;
            return newBitmap;
        }

    private:
        System::Windows::Forms::PictureBox^ pictureBoxZProjection;
        System::ComponentModel::Container^ components;

        void InitializeComponent(void)
        {
            this->pictureBoxZProjection = (gcnew System::Windows::Forms::PictureBox());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxZProjection))->BeginInit();
            this->SuspendLayout();
            // pictureBoxZProjection
            this->pictureBoxZProjection->Dock = System::Windows::Forms::DockStyle::Fill;
            this->pictureBoxZProjection->Location = System::Drawing::Point(0, 0);
            this->pictureBoxZProjection->Name = L"pictureBoxZProjection";
            this->pictureBoxZProjection->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
            this->pictureBoxZProjection->TabIndex = 0;
            this->pictureBoxZProjection->TabStop = false;
            // Zprojection
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->Controls->Add(this->pictureBoxZProjection);
            this->Name = L"Zprojection";
            this->Text = L"Z-Projection";
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBoxZProjection))->EndInit();
            this->ResumeLayout(false);
        }
    };
}
