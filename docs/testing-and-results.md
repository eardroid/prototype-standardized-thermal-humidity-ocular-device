# Testing notes

This file is deliberately a record template rather than a set of invented results.

| Check | Observation | Status |
| --- | --- | --- |
| ESP32 startup and Serial output | Controller boot message and status output | Observed during bench work |
| DHT11 temperature and humidity readout | Values reported over Serial | Observed during bench work |
| Session switch and buzzer | Manual status control | Observed during bench work |
| Peltier thermal characterization | Requires controlled PSU run, heatsinking, and measurement | Not recorded here |
| Atomizer mist output | First available atomizer was defective | Not demonstrated |
| Independent hardware cutoff | Not fitted in this prototype | Not implemented |

For a repeatable next test, record supply voltage, current limit, ambient conditions, sensor position, load duration, measured surface temperature, and whether condensation appeared. These bench tests must remain separate from any claim of clinical safety or effectiveness.
