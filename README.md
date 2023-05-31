![SARA](img/banner.png)

<h3 align="center">
Stazione per l'Analisi di Raffiche Avverse
<br>
<br>

[//]: # (TODO: aggiungere contributori)

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

