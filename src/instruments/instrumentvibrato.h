#ifndef INSTRUMENT_VIBRATO
#define INSTRUMENT_VIBRATO

#include <vector>
#include <string>
#include "instrument.h"
#include "envelope_adsr.h"

namespace upc {
  class InstrumentVibrato: public upc::Instrument {
    EnvelopeADSR adsr;
    float a = 0, b = 0;
    float fc, fm;
	  float A, N;
    std::vector<float> tbl;
  public:
    InstrumentVibrato(const std::string &param = "");
    void command(long cmd, long note, long velocity=1);
    const std::vector<float> & synthesize();
    bool is_active() const {return bActive;}
  };
}

#endif
