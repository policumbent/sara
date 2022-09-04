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

- Anemometer Adafruit: wind speed
- BME: temperature, humidity pressure
- Magnetic Encoder: wind direction
- RTC: timestamp
- GPS

#### Output
- E-paper: visualize data in real time
- Serial: transmit data using serial communication
- SD: save data

#### Board
- ESP 32 WROOM

