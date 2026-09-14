# Wiring and connections

These are the intended controller assignments used in the sketch. Verify every connection against the actual module revision before powering a load.

| Function | ESP32-S3 GPIO |
| --- | ---: |
| DHT11 data | 17 |
| Buzzer | 4 |
| Session switch (active low) | 15 |
| Heater driver control | 18 |
| Atomizer driver control | 19 |

The DHT11 uses 3.3 V logic and a shared ground. The MOSFET module receives a control signal from the ESP32, while the Peltier or atomizer must be powered by a correctly rated external supply. Do not infer the high-current terminal polarity from a photograph alone: the module labels in the supplied photo show `SIG`, `VCC`, `GND`, `VIN`, `GND`, and `V+`, but the exact terminal behavior depends on the board revision.

The IRF520 is not an ideal high-current 3.3 V logic-level stage. A future revision should use a properly rated logic-level MOSFET or a dedicated driver and should include fusing, current limiting, heatsinking, and an independent thermal cutoff.
