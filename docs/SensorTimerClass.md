# The SensorTimer class

## Purpose

The SensorTimer class makes it easier to handle multiple sensors inside the program. Sensors could be for example PMS5003, CCS811, BME280. These sensors have different needs. The CCS811 for example needs 30 Minutes before delivering reliable readings. The PMS5003 on the other hand needs to start its fan and laser about 30 seconds before taking a reading.

To keep track of all the different states inside the main loop can be complicated. This class takes this problem away from the programmer.

## Usage

1. For each sensor a timer element has to be created:

```c++
SensorTimer ccstimer;
```

2. Inside the setup function, determine if the sensor is available and set up the time:

```c++
  if (!ccs.begin())
  {
    ccstimer.setAvailable(false);       // Sensor is not available
  }
  else
  {
    ccstimer.setAvailable(true);        // Sensor is available
    ccstimer.setRepeatTime(180);        // Report readings every 3 Minutes
    ccstimer.setWarmupTime(0);          // Time needed from warm up to reading
    ccstimer.setColdstartTime(1800);    // Wait 30 mins before first reading
    ccstimer.skipWaiting();             // Skip the repeat time (makes no sense here)
  }
```

3. Inside main loop: Start wake up/warm up the sensor when its time:

```c++
 if (ccstimer.isTimetoWakeup())
  {
    ccstimer.wakeUp();                      // Tell timer that sensor has been waked up
    digitalWrite(g_CCS811_wake_pin, LOW);   // Wake up sensor
  }
```

4. Determine if a reading from the sensor can be attempted:

```c++
if (ccstimer.isAvailable() && ccstimer.isReady())
  {
    if (!ccs.readData())
    {
      CO2 = ccs.geteCO2();
      ccstimer.readingsTaken();              // Tell timer that readings are available
      ccstimer.startover();                  // Start timer over
      digitalWrite(g_CCS811_wake_pin, HIGH); // Sending Sensor to sleep...
    }
  }
```

5. Determine if readings are waiting to be reported:

```c++
if (ccstimer.readingsWaiting())
  {
    // reporting to MQTT example
    if (mqttClient.publish(topic.c_str(), String(CO2)))
    {
    }

    ccstimer.readingsReported();               // Tell timer that the readings have been reported
  }
```

## Sequence Diagram

[![](https://mermaid.ink/img/eyJjb2RlIjoic2VxdWVuY2VEaWFncmFtXG4gIHBhcnRpY2lwYW50IGwgYXMgbG9vcFxuICBwYXJ0aWNpcGFudCBzIGFzIFNlbnNvclRpbWVyXG4gIG9wdFxuICBOb3RlIG92ZXIgbCxzOiBzZXR0aW5nIHBhcmFtZXRlcnNcbiAgbC0-PnM6IHNldEF2YWlsYWJsZSh0cnVlKVxuICBsLT4-czogc2V0UmVwZWF0VGltZSgxODApXG4gIGwtPj5zOiBzZXRXYXJtdXBUaW1lKDMwKVxuICBsLT4-czogc2V0Q29sZHN0YXJ0VGltZSgyNDApXG4gIGVuZFxuICByZWN0IHJnYigyNDAsIDI0MCwgMjU1KVxuICBsb29wXG4gICAgbC0-PnM6IChib29sKSBpc0F2YWlsYWJsZT9cbiAgICBzLT4-bDogdHJ1ZVxuICAgIGwtPj5zOiAoYm9vbCkgaXNUaW1lVG9XYWtldXA_XG4gICAgcy0-Pmw6IHRydWVcbiAgICBOb3RlIG92ZXIgbCxzOiBtYWluIHByb2dyYW0gd2FrZXMgc2Vuc29yXG4gICAgbC0-PnM6IChib29sKSBpc1JlYWR5P1xuICAgIHMtPj5sOiBmYWxzZVxuICAgIE5vdGUgb3ZlciBsLHM6IHRyeWluZyBhZ2FpblxuICAgIGwtPj5zOiAoYm9vbCkgaXNSZWFkeT9cbiAgICBzLT4-bDogdHJ1ZVxuICAgIE5vdGUgb3ZlciBsLHM6IG1haW4gcHJvZ3JhbSB0YWtlcyByZWFkaW5nc1xuICAgIGwtPj5zOiByZWFkaW5nc1Rha2VuXG4gICAgbC0-PnM6IHN0YXJ0b3ZlclxuICAgIGwtPj5zOiAoYm9vbCkgcmVhZGluZ3NXYWl0aW5nP1xuICAgIHMtPj5sOiB0cnVlXG4gICAgTm90ZSBvdmVyIGwsczogbWFpbiBwcm9ncmFtIHJlcG9ydHMgcmVhZGluZ3NcbiAgICBsLT4-czogcmVhZGluZ3NUYWtlblxuICAgIGwtPj5zOiBzdGFydG92ZXJcbiAgICBlbmRcbiAgZW5kIiwibWVybWFpZCI6eyJ0aGVtZSI6ImRlZmF1bHQifSwidXBkYXRlRWRpdG9yIjpmYWxzZX0)](https://mermaid-js.github.io/mermaid-live-editor/#/edit/eyJjb2RlIjoic2VxdWVuY2VEaWFncmFtXG4gIHBhcnRpY2lwYW50IGwgYXMgbG9vcFxuICBwYXJ0aWNpcGFudCBzIGFzIFNlbnNvclRpbWVyXG4gIG9wdFxuICBOb3RlIG92ZXIgbCxzOiBzZXR0aW5nIHBhcmFtZXRlcnNcbiAgbC0-PnM6IHNldEF2YWlsYWJsZSh0cnVlKVxuICBsLT4-czogc2V0UmVwZWF0VGltZSgxODApXG4gIGwtPj5zOiBzZXRXYXJtdXBUaW1lKDMwKVxuICBsLT4-czogc2V0Q29sZHN0YXJ0VGltZSgyNDApXG4gIGVuZFxuICByZWN0IHJnYigyNDAsIDI0MCwgMjU1KVxuICBsb29wXG4gICAgbC0-PnM6IChib29sKSBpc0F2YWlsYWJsZT9cbiAgICBzLT4-bDogdHJ1ZVxuICAgIGwtPj5zOiAoYm9vbCkgaXNUaW1lVG9XYWtldXA_XG4gICAgcy0-Pmw6IHRydWVcbiAgICBOb3RlIG92ZXIgbCxzOiBtYWluIHByb2dyYW0gd2FrZXMgc2Vuc29yXG4gICAgbC0-PnM6IChib29sKSBpc1JlYWR5P1xuICAgIHMtPj5sOiBmYWxzZVxuICAgIE5vdGUgb3ZlciBsLHM6IHRyeWluZyBhZ2FpblxuICAgIGwtPj5zOiAoYm9vbCkgaXNSZWFkeT9cbiAgICBzLT4-bDogdHJ1ZVxuICAgIE5vdGUgb3ZlciBsLHM6IG1haW4gcHJvZ3JhbSB0YWtlcyByZWFkaW5nc1xuICAgIGwtPj5zOiByZWFkaW5nc1Rha2VuXG4gICAgbC0-PnM6IHN0YXJ0b3ZlclxuICAgIGwtPj5zOiAoYm9vbCkgcmVhZGluZ3NXYWl0aW5nP1xuICAgIHMtPj5sOiB0cnVlXG4gICAgTm90ZSBvdmVyIGwsczogbWFpbiBwcm9ncmFtIHJlcG9ydHMgcmVhZGluZ3NcbiAgICBsLT4-czogcmVhZGluZ3NUYWtlblxuICAgIGwtPj5zOiBzdGFydG92ZXJcbiAgICBlbmRcbiAgZW5kIiwibWVybWFpZCI6eyJ0aGVtZSI6ImRlZmF1bHQifSwidXBkYXRlRWRpdG9yIjpmYWxzZX0)
