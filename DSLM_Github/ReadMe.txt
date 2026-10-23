# DSLM microscope control software

C++/CLI Windows Forms software for controlling a DSLM microscope with a Hamamatsu camera, SCANLAB RTC5 scanner, and PI stages.

## Requirements

- Windows x64.
- Visual Studio with MSVC v142 and C++/CLI support.
- Windows 10 SDK and .NET Framework 4.0 targeting prerequisites.
- Compatible manufacturer SDKs and hardware drivers.

See the README in each component folder for the required external files.

## Build

Open MyDSLM.sln in Visual Studio, select Release and x64,
and build the solution.

The executable is generated at: "x64/Release/MyDSLM.exe"

Keep this folder structure because the application uses it to locate manufacturer dependencies.

## Run

Method 1: 
Launch MyDSLM.exe from "x64/Release/MyDSLM.exe"

Method 2:
Launch RunMyDSLM.bat from the repository root after building.

It sets the DLL search paths and starts the application.

Manufacturer dependencies must remain installed locally even though they are excluded from this repository.

## Distribution

The application code was written by the repository author.
Manufacturer dependencies are supplied separately and remain subject to their applicable licenses.