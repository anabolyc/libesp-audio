#include <Arduino.h>

#if defined(ARDUINO_ARCH_RP2040)
#define WIFI_OFF
class __x
{
public:
  __x(){};
  void mode(){};
};
__x WiFi;
#elif defined(ESP32)
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

#include "AudioFileSourcePROGMEM.h"
#include "AudioGeneratorWAV.h"
#include "AudioOutputI2S.h"
#include "AudioOutputNull.h"
#include "AudioOutputVAMeter.h"
#include "AudioOutputMixer.h"

// VIOLA sample taken from https://ccrma.stanford.edu/~jos/pasp/Sound_Examples.html
#include "viola.h"

AudioGeneratorWAV *wav[2];
AudioFileSourcePROGMEM *file[2];
AudioOutputI2S *out;
AudioOutputNull *nullout;
AudioOutputVAMeter *serialout;
AudioOutputMixer *mixer;
AudioOutputMixerStub *stub[2];
int serialoutid = -1;

void setup()
{
  WiFi.mode(WIFI_OFF);
  Serial.begin(SERIAL_BAUD);
  delay(1000);
  Serial.printf("WAV start\n");

  audioLogger = &Serial;
  file[0] = new AudioFileSourcePROGMEM(viola, sizeof(viola));
  
  mixer = new AudioOutputMixer(32);
  out = new AudioOutputI2S();
  Serial.printf("Added I2S output: %d\n", mixer->AddOutput(out));

  nullout = new AudioOutputNull();
  Serial.printf("Added NULL output: %d\n", mixer->AddOutput(nullout));
  
  serialout = new AudioOutputVAMeter();
  serialoutid = mixer->AddOutput(serialout);
  Serial.printf("Added I2S output: %d\n", serialoutid);

  stub[0] = mixer->NewInput();
  stub[0]->SetGain(0.1);
  // stub[0]->SetChannels(true, false);
  wav[0] = new AudioGeneratorWAV();
  wav[0]->begin(file[0], stub[0]);
  // Begin wav[1] later in loop()
  Serial.printf("starting 1\n");
}

void loop()
{
  static uint32_t start = 0;
  static bool go = false;

  if (!start)
    start = millis();

  if (wav[0]->isRunning())
  {
    if (!wav[0]->loop())
    {
      wav[0]->stop();
      stub[0]->stop();
      Serial.printf("stopping 1\n");
    }
  }

  if (millis() - start > 3000)
  {
    if (!go)
    {
      //Serial.printf("Removing output %d\n", serialoutid);
      //mixer->RemoveOutput(serialoutid);

      Serial.printf("starting 2\n");
      stub[1] = mixer->NewInput();
      stub[1]->SetGain(0.1);
      // stub[1]->SetChannels(false, true);
      wav[1] = new AudioGeneratorWAV();
      file[1] = new AudioFileSourcePROGMEM(viola, sizeof(viola));
      wav[1]->begin(file[1], stub[1]);
      go = true;
    }

    if (wav[1]->isRunning())
    {
      if (!wav[1]->loop())
      {
        wav[1]->stop();
        stub[1]->stop();
        Serial.printf("stopping 2\n");
      }
    }
  }
}
