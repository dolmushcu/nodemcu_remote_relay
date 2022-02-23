# Control a relay over the internet with [nodemcu](https://en.wikipedia.org/wiki/NodeMCU)

This is a hoby project that emerged as a result of some needs. This repo includes the ino file as well as some other required files which can be uploaded into the nodemcu by the arduino ide.

Basically what this code does is it connects to a webserver at regular intervals that responds with a json data and it gets the json, decode it and do stuff accordingly.

It also includes a very naive implementation of wifimanager library that can be found [here](https://github.com/tzapu/WiFiManager).

(This code is made for the esp8266 model)
