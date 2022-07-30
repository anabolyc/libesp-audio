/*
  AudioOutputMixer
  Simple mixer which can combine multiple inputs to a single output stream

  Copyright (C) 2017  Earle F. Philhower, III

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _AUDIOOUTPUTMIXER_H
#define _AUDIOOUTPUTMIXER_H

#include "AudioOutput.h"

class AudioOutputMixer;

// The output stub exported by the mixer for use by the generator
class AudioOutputMixerStub : public AudioOutput
{
public:
  AudioOutputMixerStub(AudioOutputMixer *sink, int id);
  virtual ~AudioOutputMixerStub() override;
  virtual bool SetRate(int hz) override;
  virtual bool SetBitsPerSample(int bits) override;
  virtual bool SetChannels(int channels) override;
  void SetChannels(bool left, bool right);
  virtual bool begin() override;
  virtual bool ConsumeSample(int16_t sample[2]) override;
  virtual bool stop() override;

protected:
  AudioOutputMixer *parent;
  bool left = true, right = true;
  int id;
};

// Single mixer object per output
class AudioOutputMixer : public AudioOutput
{
public:
  AudioOutputMixer(int samples);
  virtual ~AudioOutputMixer() override;
  virtual bool SetRate(int hz) override;
  virtual bool SetBitsPerSample(int bits) override;
  virtual bool SetChannels(int channels) override;
  virtual bool begin() override;
  virtual bool ConsumeSample(int16_t sample[2]) override;
  virtual bool stop() override;
  virtual bool loop() override; // Send all existing samples we can to I2S

  AudioOutputMixerStub *NewInput(); // Get a new stub to pass to a generator
  int AddOutput(AudioOutput *sink);
  void RemoveOutput(int id);

  // Stub called functions
  friend class AudioOutputMixerStub;

private:
  void RemoveInput(int id);
  bool SetRate(int hz, int id);
  bool SetBitsPerSample(int bits, int id);
  bool SetChannels(int channels, int id);
  bool begin(int id);
  bool ConsumeSample(int16_t sample[2], int id);
  bool stop(int id);

protected:
  enum
  {
    maxStubs = 8
  };
  enum
  {
    maxSinks = 8
  };
  bool stubAllocated[maxStubs];
  bool stubRunning[maxStubs];
  AudioOutput *sinks[maxSinks];
  bool sinkStarted[maxSinks];

  int16_t buffSize;
  int32_t *leftAccum;
  int32_t *rightAccum;

  int16_t writePtr[maxStubs]; // Array of pointers for allocated stubs
  int16_t readPtr;
};

#endif
