![SARA](img/banner.png)

<h3 align="center">
Stazione per l'Analisi di Raffiche Avverse
<br>
<br>

<!-- ALL-CONTRIBUTORS-BADGE:START - Do not remove or modify this section -->
[![All Contributors](https://img.shields.io/badge/all_contributors-2-orange.svg?style=flat-square)](#contributors-)
<!-- ALL-CONTRIBUTORS-BADGE:END -->

</h3>


[//]: # (TODO: vecchio readme, capire cosa potrebbe essere utile)


<!---

#### Components

-  
-  ANEMOMETRO
-  AS5048A - MAGNETOMETRO
-  Waveshare ePaper - Schermino per stampa a video imformazioni
-  ESP32 Development Board - Scheda principale
-  GPS
-  MICRO SD
-  BME280 - Sensore temperatura pressione umidità
-  ADS1115 - Convertitore analogico-digitale(ADC) e amplificatore programmabile(PGA)
-  RTC - Real Time Clock
-  Compass - Non sappiamo cosa sia

##### Come usare la stazione
	0) Calibrare la bandierina in modo da trovare lo 0° (usa la seriale e muovi la bandierina usando la punta come ago) e segnare la posizione
	1) Posizionare la punta della bandierina sullo 0 (quello è il polo N del magnete)
	2) Creare un Hotspot Wifi con SSD: "toolbox", password: "Toolbox.Torino" (in caso non funzioni controllate il codice) 
	3) avviare la stazione che si connetterà automaticamente all'hotspot
	4) i dati sono pubblicati sul server MQTT e visibili anche da Grafana

-->

#### Sensors

- Anemometer Adafruit: wind speed sensor
- BME280: temperature, humidity pressure sensor
- Magnetic Encoder AS5600: wind direction sensor
- GPS Neo6-M: current position tracker

#### Output
- E-paper Waveshare 1'54" : paper-like screen to visualize data
- Serial port: transmit data using serial communication
- SD: data logger

#### Controller
- ESP 32 WROOM DEVKIT V2
 
#### Setutp
1) Position the Weather station in the desired location
2) Open the 3 leg stand and adjust the height
3) Modify the connection parameters in the *platformio.ini* file and flash the controller
4) Make sure the switch is set to OFF
5) Connect a 12V Lipo Battery to the board
6) Using a multimeter, calibrate the STEPDOWN to 5V output
7) Create a 2.4Gb/s wifi hotspot with the previously specified credentials
8) Turn ON the switch
9) The LED on the ESP32 controller will start blinking when it is connected, if not reset the system and retry

### How it works

#### Publishing to server
The Weather Station publishes data on a remote MQTT server. Listening to the same server is a process
running on the Team VPS hosted at (*server.policumbent.it*) that collects data from the server and saves it locally.
The process should also redirect the incoming data to the Alice clients connected, through the WebSocket protocol.

#### Connection to the app
The data sent to the Mqtt server can also be visualized through any mqtt visualizer and it is also
the app designed specifically for this purpose

#### Backup SD
The data is not directly saved on the SD card at every cycle, but it is buffered in the SPIFFS memory 
and then copied on the SD every 10s. This guarantees protection over unwanted interruptions.


## Contributors ✨

Thanks goes to these wonderful people
<!-- ALL-CONTRIBUTORS-LIST:START - Do not remove or modify this section -->
<!-- prettier-ignore-start -->
<!-- markdowns-disable -->
<table>

<td align="center"><a href="https://github.com/AlbertoEusebio"><img src="https://avatars.githubusercontent.com/u/72319445?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Alberto Eusebio</b></sub></a><br /><a href="https://github.com/policumbent/bob/commits?author=AlbertoEusebio" title="Code"></a></td>
<td align="center"><a href="https://github.com/CornagliaRiccardo"><img src="https://avatars.githubusercontent.com/u/81438517?v=4?s=100" width="100px;" alt=""/><br /><sub><b>CornagliaRiccardo</b></sub></a><br /><a href="https://github.com/policumbent/bob/commits?author=CornagliaRiccardo" title="Code"></a></td>
</table>