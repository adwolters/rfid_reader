# RFID Reader Project

This project consists of using a Wiegand RFID Panel to activate and deactivate my alarm in home-assistant. What it does is, it will read from the panel and sent the pin over mqtt to home-assistant. There it will be evaluated and if correct it will deactivate (or activate) the alarm. 

# The hardware
- Wiegand 26/34 RFID reader
- Wemos D1 mini

# Wiring schema
To be added

# Arduino libraries used
- PubSubClient (mqtt)
- Wiegand
- ESP8266 Wifi

# TODO
- The code is still sent plain text, obviously this is not secure. So next up will be to encrypt this code
- Only tested with the keypad entry, not actual RFID tags. They are still on their way from China. 
