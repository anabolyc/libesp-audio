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

#include "AudioOutputVAMeter.h"

AudioOutputVAMeter::AudioOutputVAMeter(uint8_t _bitdepth) {
  bitdepth = _bitdepth;
  samples = 1 << bitdepth;
  window = new int16_t[samples * 2];
  memset(window, 0, sizeof(window));
}

bool AudioOutputVAMeter::begin()
{

  return true;
}

bool AudioOutputVAMeter::ConsumeSample(int16_t sample[2])
{
  uint32_t ix = 0;
  for (uint8_t i=0; i < channels; i++) {
    ix = i * samples + sample_counter;
    vasum[i] -= window[ix];
    window[ix] = abs(sample[i]);
    vasum[i] += window[ix];
    va[i] = vasum[i] >> bitdepth;
  }

  for (uint8_t i = channels; i < 2; i++) {
    vasum[i] = vasum[i - 1];
    va[i] = vasum[i] >> bitdepth;
  }

  if (++sample_counter >= samples) {
    sample_counter = 0;
    // Serial.printf("%d %d\n", vasum[0], vasum[1]);
  }

  return true;
}


bool AudioOutputVAMeter::stop()
{
  // memset(window, 0, sizeof(window));
  // memset(vasum, 0, sizeof(window));
  // memset(va, 0, sizeof(window));
  return true;
}

AudioOutputVAMeter::~AudioOutputVAMeter() {
  delete window;
}