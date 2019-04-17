================
CameraControl
================

This program is a prototype
It's purpose to be able to connect to and control Multiple GEV Vision Devices
It works by finding GEV devices by MAC/IP addresses and using the Pleora SDK to control and monitor them

Current Functionality:
=====================

Find a single Device by Variable MAC or IP Address
Connect to that Device
Open up a stream
Create a pipeline
Acquire Images

Future Functionality:
====================
Connect to and control mutliple devices
Automate the Data Collection Process with Timers
Be Able to Power Cycle Cameras remotely
Be able to acquire and Sum 4 images
--Once a minute we sotre for or five images and add them together
--Without having to write four files and add them


Decisions/Choices to be Made:
===========================
Frequency of Data collection
Whether Data Collection will be always on or started on the ground

Notes to Self/To do:
====================
Read up on PvSystemEventSink
