#include <iostream>
#include <math.h>
#include "instrument_vibrato.h"
#include "keyvalue.h"

#include <stdlib.h>
#include "stdio.h"

using namespace upc;
using namespace std;

InstrumentVibrato::InstrumentVibrato(const std::string &param)
  : adsr(SamplingRate, param) {
  bActive = false;
  x.resize(BSIZE);

  /*
    You can use the class keyvalue to parse "param" and configure your instrument.
    Take a Look at keyvalue.h
  */
  KeyValue kv(param);
  int N;

  if (!kv.to_int("fm", fm))
    fm = 10;
  if (!kv.to_int("I",I))
    I = 0.5;

}


void InstrumentVibrato::command(long cmd, long note, long vel) {
  if (cmd == 9) {		//'Key' pressed: attack begins
    bActive = true;
    adsr.start();
    fc = pow(2, ((note-69.0)/12.0)) * 440.0/SamplingRate;
	  A = vel / 127.0;
  }
  else if (cmd == 8) {	//'Key' released: sustain ends, release begins
    adsr.stop();
  }
  else if (cmd == 0) {	//Sound extinguished without waiting for release to end
    adsr.end();
  }
}


const vector<float> & InstrumentVibrato::synthesize() {
  if (not adsr.active()) {
    x.assign(x.size(), 0);
    bActive = false;
    return x;
  }
  else if (not bActive)
    return x;

  for (unsigned int i=0; i<x.size(); ++i) {
    x[i] = A*sin(a + I*sin(b));
    a = a + 2*M_PI*fc;
    b = b + 2*M_PI*N*fc;

    while (a >= M_PI){
      a = a - 2*M_PI;
    }
    while (b >= M_PI){
      b = b - 2*M_PI;
    }
  }
  adsr(x); //apply envelope to x and update internal status of ADSR

  return x;
}
