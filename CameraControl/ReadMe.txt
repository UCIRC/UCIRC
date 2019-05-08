================
CameraControl
================

This program is a prototype
It's purpose to be able to connect to and control Multiple GEV Vision Devices
It works by finding GEV devices by MAC/IP addresses and using the Pleora SDK to control and monitor them

Current Functionality:
=====================
Try and find a device via a configuration stored in a pvxml
If that fails it intitiates a backup protocol
	It then stores that new configuration in a pvxml
Connect to that Device
Open up a stream
Create a pipeline
Acquire Images

REQUIRED TESTING AND FIXES
====================
Need to fix the macro bug (low priority, makes for ugly code)
Test to see if the program restores a device if there is none connected
Test to see if storing a Configuration successfully overwrites previous configurations

Future Functionality:
====================
Connect to and control mutliple devices
Automate the Data Collection Process with Timers
Be Able to Power Cycle Cameras remotely
Be able to acquire and Sum 4 images
Needs to do so using Raw Data
Those Images need to be saved under a timestamp
--Once a minute we sotre for or five images and add them together
--Without having to write four files and add them


Decisions/Choices to be Made:
===========================
Frequency of Data collection
Whether Data Collection will be always on or started on the ground

Notes to Self/To do:
====================
Read up on PvSystemEventSink

