## AirFishBowl
The AirFishBowl is a remote controllable / autonomous helium air blimp in the shape of a fish. It is powered using an OpenPicus 
Flyport microcontroller and has a beautifully designed interface which is served from a Raspberry Pi. The system has a simple 
HTTP REST API for which we have multiple implemented controllers such as the Javascript autonomous Alive system, the Kinect 
controller, directly from the HTTP interface and even iOS app. The system is a collaborative project between multiple 
undergraduate degrees in the schools of Engineering and Science at the _Waterford Institute of Technology, Ireland_.

- __Lecturer:__ Jason Berry
- __Project Blog:__ [link](http://www.airfishbowl.com "Project Blog")
- __Project Wiki:__ [link](https://github.com/ElectronicsWIT/AirFishBowl/wiki "Project Wiki")


![AirFishBowl](http://electronicswit.github.io/AirFishBowl/airfish-architecture.png)

### Contents
The AirFishBowl project repository contains the following directories:

    .
    ├── AirFishBowl-iOS 
    ├── AirFishBowl-REST-API-Mock
    ├── AirFishBowl
    ├── Branding
    ├── Kinect
    └── RPi_WebServer



- __AirFishBowl-iOS__ Coming soon (TM)
- __AirFishBowl-REST-API-Mock__ Contains a mock for the AirFishBowl REST API which can be used to test interfaces for the system
- __AirFishBowl__ Contains the OpenPicus Project for the AirFishBowl Fish
- __Branding__ Contains the Branding assets used by the AirFishBowl project
- __Kinect__ Contains the C# Microsoft Kinect application for interacting with the AirFishBowl REST API
- __RPi_Webserver__ Contains the Django WebApp which can be run on the Raspberry Pi and acts as a front end for the AirFishBowl system
