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
	void _zeroAll(MessageSender& ms);
	void endMessage();
	void _endMessage(MessageSender& ms);
	void setDAC(int DAC, int channels, double *freqs, double *amps, double *phases);
	void _setDAC(MessageSender& ms, int DAC, int channels, double* freqs, double* amps, double* phases);

	void zeroAndSetDAC(int DAC, int channels, double* freqs, double* amps, double* phases);
	void zeroAndSetTwoDACs(int DAC0, int channels0, double* freqs0, double* amps0, double* phases0, 
		int DAC1, int channels1, double* freqs1, double* amps1, double* phases1);

	void send(MessageSender& ms);

	gigaMoog();
	virtual ~gigaMoog(void);

private:
	BoostAsyncSerial fpga;
};

extern "C" {
	__declspec(dllexport) gigaMoog* gm_new(void) { return new gigaMoog(); }
	__declspec(dllexport) int gm_test(gigaMoog* gm) { return gm->test(); }
	__declspec(dllexport) void gm_zeroAll(gigaMoog* gm) { gm->zeroAll(); }
	__declspec(dllexport) void gm_setDAC(gigaMoog* gm, int DAC, int channels, double freqs[], double amps[], double phases[]) {
		gm->setDAC(DAC, channels, freqs, amps, phases); }
	__declspec(dllexport) void gm_endMessage(gigaMoog* gm) { gm->endMessage(); }
	__declspec(dllexport) void gm_zeroAndSetDAC(gigaMoog* gm, int DAC, int channels, double freqs[], double amps[], double phases[]) {
		gm->zeroAndSetDAC(DAC, channels, freqs, amps, phases); }
	__declspec(dllexport) void gm_zeroAndSetTwoDACs(gigaMoog* gm, int DAC0, int channels0, double freqs0[], double amps0[], double phases0[],
		int DAC1, int channels1, double freqs1[], double amps1[], double phases1[]) { 
		gm->zeroAndSetTwoDACs(DAC0, channels0, freqs0, amps0, phases0, DAC1, channels1, freqs1, amps1, phases1); }
}