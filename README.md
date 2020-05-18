## This is a final project in NYU course Real-time Embedded System
### Name: "Embedded Sentry"
### Objectives: 
* Use the data collected from an ADXL345 accelerometer to record a hand movement sequence as a password to "unlock" a resource, e.g. a door.
* Save the recorded sequence on the microcontroller(we use SAMD21_Xplained), using a “Record Key” feature.
* User then must replicate the password within sufficient tolerances to unlock the resource. 
* A successful unlock is indicated by a visual indication(door opening).

We use Arduino IDE with SAMD21 package to run the code. Put the two files in C:\Program Files (x86)\Arduino\libraries directory, then build a new Arduino project. 
