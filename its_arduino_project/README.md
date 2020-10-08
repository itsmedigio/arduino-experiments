
# Progetto Arduino per ITS 

Qua potete scaricare il mio progetto per ITS, una semplice lampada comandata tramite **NodeRED**.
La lampada si collega al Wi-Fi (bisogna impostare le vostre credenziali nel file .ino) e comunica con NodeRED attraverso il broker MQTT mosca.
 
Riceve dalla Dashboard diversi comandi:

 - 4 colori predefiniti (rosso, verde, blu e bianco tramite bottone ON)
 - Un qualsiasi colore attraverso un color picker
 - Comando per spegnere la lampada, aumentare/diminuire la luminosità della stessa

Inoltre la Dashboard riceve dalla lampada la temperatura e l'umidità corrente della stanza in cui è messa attraverso un sensore DHT11 collegato in questo esempio nel pin D2.

Alla lampada è inoltre collegato un display  OLED SSD1306 in cui vengono visualizzati i medesimi dati.

**Cosa ho utilizzato?**
Utilizzo le librerie Adafruit 1306 per il display e sempre da Adafruit quelle di Neopixel, per controllare il ring RGB che nel mio caso ha 20 led. 
Utilizzo poi le librerie per il sensore DHT11 della Grove.
NodeRED versione 1.1.3

**Alcune immagini della lampada**

La dashboard di NodeRED
![La dashboard di NodeRED](https://github.com/itsmedigio/arduino-experiments/blob/master/its_arduino_project/img/lamp_gui.png)

L'Arduino e i suoi moduli
![L'Arduino](https://github.com/itsmedigio/arduino-experiments/blob/master/its_arduino_project/img/the_arduino.jpg)

La lampada (inserita in una bottiglia di plastica, e come base un vecchio pezzo di clarinetto)
![The lamp](https://github.com/itsmedigio/arduino-experiments/blob/master/its_arduino_project/img/the_lamp.jpg)
