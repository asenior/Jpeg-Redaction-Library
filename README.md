JPEG Redaction Library
======================

This is an open source C++ library that can parse EXIF format files- that is files in the standard EXIF format for representing JPEG-encoded data. 
The library can parse JPEG and EXIF contents and is intended to be used to rewrite the files in a modified form.
The library has been developed for the guardianproject/[SecureSmartCam](https://github.com/guardianproject/SecureSmartCam) project.
The main distinguishing feature of the library is the ability to erase regions of an image (for privacy protection) in the compressed domain.

The basic library is now functionally complete, though with rough edges.
* It can read & parse EXIF/JPEG files
* Write JPEG files
* Parse EXIF tags
* Parse IPTC tags
* Simple operations on EXIF tags
* Redact (wipe) rectangular regions in JPEG images (and thumbnails).
* Reverse image redactions.

In the future it is intended that the library will support the following:
* Extensive API around editing EXIF & IPTC data.
* Extension to MPEG4 video.

Known problems:
* Not extensively tested. (Range of test images, redaction region corner cases)
* Doesn't preserve maker notes.
* Redaction regions are stored as strips, so overlapping redaction rectangles cannot be independently encrypted.
* Limited redaction methods (currently just grey rectangle, or horizontal strips of constant colour.
 
The main library is in /lib/. Unit tests and sample code can be found in /test/
A simple program to redact an image is in /bin/

Please let me know if you are interested in using the library, and if you have any feature requests.

