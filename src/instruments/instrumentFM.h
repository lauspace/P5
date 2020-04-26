#ifndef INSTRUMENT_FM
#define INSTRUMENT_FM

#include <vector>
#include <string>
#include "instrument.h"
#include "envelope_adsr.h"

namespace upc {
  class InstrumentFM: public upc::Instrument {
    EnvelopeADSR adsr;
    float N1, N2, I;
    float fc, fm;
    int N;
    unsigned int index;
    float a, b;
	  float A;
    std::vector<float> tbl;
  public:
    InstrumentFM(const std::string &param = "");
    void command(long cmd, long note, long velocity=1);
    const std::vector<float> & synthesize();
    bool is_active() const {return bActive;}
  };
}

#endif
