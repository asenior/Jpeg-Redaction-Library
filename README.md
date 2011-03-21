JPEG Redaction Library
======================

This is a C++ library that can parse EXIF format files- that is files in the standard EXIF format for representing JPEG-encoded data. 
The library can parse JPEG and EXIF contents and is intended to be used to rewrite the files in a modified form.
The library has been developed for the guardianproject/SecureSmartCam project.
The main distinguishing feature of the library is the ability to erase regions of an image (for privacy protection) in the compressed domain.

The library is not yet fully functional, but can read & parse EXIF/JPEG files
and write simple JPEG files, and is able to wipe regions of the image. 

In the future it is intended that the library will support an extensive API around editing EXIF data.


The main library is in /lib/. Unit tests and sample code can be found in /test/

Please let me know if you are interested in using the library, and if you have any feature requests.

