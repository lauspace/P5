#ifndef INSTRUMENT_SENO
#define INSTRUMENT_SENO

#include <vector>
#include <string>
#include "instrument.h"
#include "envelope_adsr.h"

namespace upc {
  class InstrumentDumb: public upc::Instrument {
    EnvelopeADSR adsr;
    unsigned int index, prev, back;
	  float f0, theta, nota, step;
    float A;
    std::vector<float> tbl;
  public:
    seno(const std::string &param = "");
    void command(long cmd, long note, long velocity=1);
    const std::vector<float> & synthesize();
    bool is_active() const {return bActive;}
  };
}

#endif