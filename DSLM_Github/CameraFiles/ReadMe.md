# Hamamatsu camera dependencies

## Files to obtain separately

Obtain a compatible Hamamatsu DCAM SDK from the authorized support channel for your camera.

Place the SDK headers and x64 import library directly in CameraFiles. Place the matching x64 dcamapi.dll in the repository root.

Install the appropriate Hamamatsu camera drivers using the manufacturer's installation procedure.

## Compatibility

The application uses legacy DCAM calls, including dcam_init. Use an SDK version that supports these calls.

## Distribution

Hamamatsu SDK files and runtime components are external dependencies excluded from this public source distribution.

Obtain and use them under their applicable license.