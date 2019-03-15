================
CameraControl
================

This program is a prototype
It's purpose to be able to connect to and control Multiple GEV Vision Devices
It works by finding GEV devices by MAC addresses and using the Pleora SDK to control and monitor them

Current Functionality:
=====================
Find a single Device by HardCoded MAC Address
Disconnect from a single Device

Future Functionality:
====================
Connect to and control mutliple devices
Stream RAW Data from Devices
Write that RAW Data for transport to safe server
Automate the Data Collection Process with Timers
Be Able to Power Cycle Cameras remotely
Data Stream doesn't just write a picture; it opens a stream and lets you decide what to save
--Find a way to store data while opening up data stream
--Don't want to store one image; want to sotre a sum of images
	--Once a minute we sotre for or five images and add them together
	--Without having to write four files and add them


Decisions/Choices to be Made:
===========================
Frequency of Data collection
Whether Data Collection will be always on or started on the ground

Notes to Self/To do:
====================
Read up on PvSystemEventSink
Read up on instantiating own classes
Figure out if this will be necessary (hunch says yes)
