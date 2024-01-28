// VAMoogLadderOS.cpp
// c++ code and improvements by Sam Pluta 2024
// based on the faust implementation by Dario Sanfilippo
// which is based on Lorenzo Della Cioppa's correction to Pirkle's implementation of Zavalishin 2012 (VAFilter Design)
// MIT-style STK-4.3 license

#include "VAMoogLadderOS.hpp"
#include "SC_PlugIn.hpp"
#include "SC_PlugIn.h"

static InterfaceTable *ft;

namespace VAMoogLadderOS
{
  VAMoogLadderOS::VAMoogLadderOS()
  {
    m_sample_rate = (float) sampleRate();

    m_oversampling_index = sc_clip((int)in0(OverSample), 0, 4);
    for (int i = 0; i < 3; ++i) {
      oversample[i].reset(m_sample_rate);
      oversample[i].setOversamplingIndex(m_oversampling_index);
      osBuffers[i] = oversample[i].getOSBuffer();
    }

    m_oversampling_ratio = oversample[0].getOversamplingRatio();

    m_nyquist = m_sample_rate * m_oversampling_ratio/2.f;

    m_step_val = 1.f / m_nyquist;

    mCalcFunc = make_calc_function<VAMoogLadderOS, &VAMoogLadderOS::next_a>();
    next_a(1);
  }
  VAMoogLadderOS::~VAMoogLadderOS() {}

  float VAMoogLadderOS::process(float sig, float freq, float q_val)
  {
    float cf = sc_clip(freq, 0.f, m_nyquist*0.6f);

    //float q_clipped = sc_clip(q_val, 0.f, 1.f);

    //k is the feedback coefficient of the entire circuit
    float k = 4.f * q_val;

    float omegaWarp = tanf(pi*cf*m_step_val);
    float g = omegaWarp/(1.f+omegaWarp);

    float g4 = g*g*g*g;
    float s4 = g*g*g*(m_last_1*(1-g))+g*g*(m_last_2*(1-g))+g*(m_last_3*(1-g))+(m_last_4*(1-g));
    
    //internally clips the feedback signal to prevent the filter from blowing up
    //s4 = sc_clip(s4, -2.f, 2.f);
    if(s4 > 1.f)
      s4 = tanh(s4-1.f)+1.f;
    else if(s4 < -2.f)
      s4 = tanh(s4+1.f)-1.f;

    //input is the incoming signal minus the feedback from the last stage, which is all going through a custom distortion function
    float input = (sig-k*s4)/(1.f+k*g4);

    float v1 = g*(input-m_last_1);
    float lp1 = m_last_1+v1;

    float v2 = g*(lp1-m_last_2);
    float lp2 = m_last_2+v2;

    float v3 = g*(lp2-m_last_3);
    float lp3 = m_last_3+v3;

    float v4 = g*(lp3-m_last_4);
    float lp4 = m_last_4+v4;

    m_last_1 = lp1;
    m_last_2 = lp2;
    m_last_3 = lp3;
    m_last_4 = lp4;

    return lp4;
  }

  float VAMoogLadderOS::process_os(float sig, float freq, float q_val)
  {
    oversample[0].upsample(sig);
    oversample[1].upsample(freq);
    oversample[2].upsample(q_val);

    float out = 0.f;

    for (int k = 0; k < m_oversampling_ratio; k++){
      osBuffers[0][k] = process(osBuffers[0][k], osBuffers[1][k], osBuffers[2][k]);
    }
    if (m_oversampling_index != 0)
      out = oversample[0].downsample();
    else
      out = osBuffers[0][0];
    // if ( std::fpclassify( out ) == FP_SUBNORMAL )
    //   out = 0.f;
    if ( isnormal( out ) == false)
      out = 0.f;
    return out;
  }

  void VAMoogLadderOS::next_a(int nSamples)
  {
    const float *sig = in(Sig);
    const float *freq = in(Freq);
    const float *q_val = in(Q);
    float *outbuf = out(Out1);

    for (int i = 0; i < nSamples; ++i)
    {
      outbuf[i] = process_os(sig[i], freq[i], q_val[i]);
    }
  }
}

PluginLoad(VAMoogLadderOS)
{
  ft = inTable;
  registerUnit<VAMoogLadderOS::VAMoogLadderOS>(ft, "VAMoogLadderOS", false);

}
