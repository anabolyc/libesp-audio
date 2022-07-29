#include <Arduino.h>
#include "AudioGeneratorAAC.h"
#include "AudioOutputI2S.h"
#include "AudioFileSourcePROGMEM.h"
#include "sampleaac.h"

AudioFileSourcePROGMEM *in;
AudioGeneratorAAC *aac;
AudioOutputI2S *out;

void setup()
{
  Serial.begin(SERIAL_BAUD);

  audioLogger = &Serial;
  in = new AudioFileSourcePROGMEM(sampleaac, sizeof(sampleaac));
  aac = new AudioGeneratorAAC();
  out = new AudioOutputI2S();

  aac->begin(in, out);
}


void loop()
{
  if (aac->isRunning()) {
    aac->loop();
  } else {
    Serial.printf("AAC done\n");
    delay(1000);
  }
}

