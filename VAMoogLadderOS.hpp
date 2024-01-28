
//#pragma once

#include "SC_PlugIn.hpp"
#include "VariableOversampling.hpp"

namespace VAMoogLadderOS {

class VAMoogLadderOS : public SCUnit {
public:
  VAMoogLadderOS();

  // Destructor
  ~VAMoogLadderOS();

  VariableOversampling<> oversample[3];

  float process(float sig, float freq, float q_val);
  float process_os(float sig, float freq, float q_val);

private:
  // Calc function
  void next_a(int nSamples);

  enum InputParams { Sig, Freq, Q, OverSample, NumInputParams };
  enum Outputs { Out1, NumOutputParams };

  float m_last_1{0.f};
  float m_last_2{0.f};
  float m_last_3{0.f};
  float m_last_4{0.f};
  float m_inv_sqrt2{1.f / std::sqrt(2.f)};
  float m_step_val{1.f/44100.f};
  float m_sample_rate{44100.f};

  float* osBuffers[3];

  int m_oversampling_index{0};
  int m_oversampling_ratio{1};
  float m_nyquist{44100.f/2.f};

};

} // namespace VAMoogLadderOS

