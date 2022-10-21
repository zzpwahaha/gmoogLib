#pragma once

#include "stdafx.h"

#include <iostream>

#include <thread>
#include <chrono>

#include "BoostAsyncSerial.h"
#include <windows.h>

#include "MessagePrinter.h"
#include "MessageSender.h"

//#define DLLEXPORT extern "C" __declspec(dllexport)

class gigaMoog {

public:

	int test();
	void zeroAll();
	void endMessage();
	void setDAC(int DAC, int channels, float *freqs, float *amps, float *phases);

	gigaMoog();
	virtual ~gigaMoog(void);

private:
	BoostAsyncSerial fpga;
};

extern "C" {
	__declspec(dllexport) gigaMoog* gm_new(void) { return new gigaMoog(); }
	__declspec(dllexport) int gm_test(gigaMoog* gm) { return gm->test(); }
	__declspec(dllexport) void gm_zeroAll(gigaMoog* gm) { gm->zeroAll(); }
	__declspec(dllexport) void gm_setDAC(gigaMoog* gm, int DAC, int channels, float freqs[], float amps[], float phases[]) { gm->setDAC(DAC, channels, freqs, amps, phases); }
	__declspec(dllexport) void gm_endMessage(gigaMoog* gm) { gm->endMessage(); }
}