/*
  AudioOutputVAMeter
  Writes a mostly correct WAV file to the serial port
  
  Copyright (C) 2022  Andriy Malyshenko

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

#ifndef _AUDIOOUTPUTSERIALWAV_H
#define _AUDIOOUTPUTSERIALWAV_H

#include "AudioOutput.h"

class AudioOutputVAMeter : public AudioOutput
{
  public:
    AudioOutputVAMeter(uint8_t bitdepth = 10);
    ~AudioOutputVAMeter();

    virtual bool begin() override;
    virtual bool ConsumeSample(int16_t sample[2]) override;
    virtual bool stop() override;

  protected:
    int32_t vasum[2] = {0, 0};
    int16_t va[2] = {0, 0};

  private:
    uint8_t bitdepth;
    uint32_t samples;
    uint32_t sample_counter = 0;
    int16_t* window;
};

#endif

