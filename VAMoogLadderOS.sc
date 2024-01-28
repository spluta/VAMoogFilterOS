VAMoogLadderOS : UGen {
	*ar { |sig, freq=1000, q_val=1, oversample=1, mul = 1, add = 0|
		if(sig.rate!='audio'){sig = K2A.ar(sig)};
		if(freq.rate!='audio'){freq = K2A.ar(freq)};
		if(q_val.rate!='audio'){q_val = K2A.ar(q_val)};
		^this.multiNew('audio', sig, freq, q_val, oversample).madd(mul, add);
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}
