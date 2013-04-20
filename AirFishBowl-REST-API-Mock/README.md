## AirFishBowl REST API Mock System
Allows testing of interfaces for the system by emulating the firmware running on the AirFish

    Author: David Kirwan - https://github.com/davidkirwan/
    Licence: See LICENCE in directory root

## Usage
This system is written in Ruby, and while it is possible to run on a Windows machine, it will
be easier to use on Linux or Mac.

Dependencies are managed using the Ruby _bundler_ gem, if this gem is not installed first install
using the _gem install bundler_ command. Once installed bundler can be used to download the other
required dependencies by the system with the following command in the directory root:

    _bundle install_ 

To launch the system using foreman at localhost on port 80:

    _sudo foreman start -p 80_

## Accessing
Once running, the system is availible at http://localhost and will behave just like the firmware
running on the AirFishBowl fish. This can be used to aid in testing of interfaces such as the
iOS application, Kinect or Javascript Alive tasks.
