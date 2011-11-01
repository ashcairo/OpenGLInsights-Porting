Description
===========
Samples provided for the Porting Mobile Apps to WebGL chapter of OpenGL Insights.

In each folder you'll find a webgl and mobile folder.
The webgl folder requires to be run from a web server, in order to serve textures.
In the mobile folder, you'll also find iOS, Qt and Android ports.


External components used
========================
2c (Apache 2.0)
https://github.com/orangelabsuk/2c
https://github.com/orangelabsuk/2cWebGL
Cross platform 3D application framework used to develop this app.
*Modified - stripped down to relate to chapter material.

libzip (libzip)
http://nih.at/libzip/
Used for Android port to open up packaged resources.
Found in Dev/Android/Source/jni/libzip

LearningWebGL.com
http://learningwebgl.com/blog/?p=370
WebGL renderer based off tutorial 5.

glMatrix.js (New BSD)
https://github.com/toji/gl-matrix
High performance matrix and vector operations for WebGL
Found in js/external folder.
*Modified - added some operations.