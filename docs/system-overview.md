# System overview — Prototype of Standardized Thermal/Humidity Ocular Device

The prototype follows a simple sense–decide–actuate loop:

1. The DHT11 supplies air temperature and relative humidity readings.
2. The ESP32-S3 checks the session switch and sensor validity.
3. The controller reports status over Serial and drives the buzzer.
4. Separate GPIO signals can command the heater and atomizer power stages.
5. High-current loads are supplied from an external bench supply, not from an ESP32 pin.

The enclosure was assembled quickly around a goggle-like frame and a breadboard. It is useful as a demonstration of system integration, but it is not a finished wearable or a validated treatment chamber.
