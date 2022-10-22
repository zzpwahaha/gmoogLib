#pragma once

#include "stdafx.h"
#include "lib.h"

gigaMoog::gigaMoog() : fpga("COM5", 115200) {
}

gigaMoog::~gigaMoog(void) {
}

int gigaMoog::test() {
	MessageSender ms;
	{
		Message m = Message::make().destination(MessageDestination::KA007)
			.DAC(MessageDAC::DAC0).channel(0)
			.setting(MessageSetting::LOADFREQUENCY)
			.frequencyMHz(100).amplitudePercent(100).phaseDegrees(0.0);
		ms.enqueue(m);

		Message m2 = Message::make().destination(MessageDestination::KA007)
			.DAC(MessageDAC::DAC0).channel(1)
			.setting(MessageSetting::LOADFREQUENCY)
			.frequencyMHz(101).amplitudePercent(100).phaseDegrees(0.0);
		ms.enqueue(m2);
	}

	for (int channel = 2; channel < 64; channel++) {
		Message m = Message::make().destination(MessageDestination::KA007)
			.DAC(MessageDAC::DAC0).channel(channel)
			.setting(MessageSetting::LOADFREQUENCY)
			.frequencyMHz(0).amplitudePercent(0).phaseDegrees(0.0);;
		ms.enqueue(m);
	}

	for (int channel = 0; channel < 64; channel++) {
		Message m = Message::make().destination(MessageDestination::KA007)
			.DAC(MessageDAC::DAC1).channel(channel)
			.setting(MessageSetting::LOADFREQUENCY)
			.frequencyMHz(0).amplitudePercent(0).phaseDegrees(0.0);;
		ms.enqueue(m);
	}

	for (int channel = 0; channel < 64; channel++) {
		Message m = Message::make().destination(MessageDestination::KA007)
			.DAC(MessageDAC::DAC2).channel(channel)
			.setting(MessageSetting::LOADFREQUENCY)
			.frequencyMHz(0).amplitudePercent(0).phaseDegrees(0.0);;
		ms.enqueue(m);
	}

	for (int channel = 0; channel < 64; channel++) {
		Message m = Message::make().destination(MessageDestination::KA007)
			.DAC(MessageDAC::DAC3).channel(channel)
			.setting(MessageSetting::LOADFREQUENCY)
			.frequencyMHz(0).amplitudePercent(0).phaseDegrees(0.0);;
		ms.enqueue(m);
	}

	{
		Message m = Message::make().destination(MessageDestination::KA007)
			.setting(MessageSetting::TERMINATE_SEQ);
		ms.enqueue(m);
	}

	ms.getQueueElementCount();

	//BoostAsyncSerial fpga("COM5", 115200);

	MessagePrinter rec;

	fpga.setReadCallback(boost::bind(&MessagePrinter::callback, rec, _1));

	fpga.write(ms.getMessageBytes());


	std::cout << "exiting..." << std::endl;

	return 0;
}

void gigaMoog::zeroAll() {
	MessageSender ms;

	_zeroAll(ms);
	
	send(ms);
}

// Zero all channels
void gigaMoog::_zeroAll(MessageSender& ms)
{
	for (int channel = 0; channel < 64; channel++) {
		Message m = Message::make().destination(MessageDestination::KA007)
			.DAC(MessageDAC::DAC0).channel(channel)
			.setting(MessageSetting::LOADFREQUENCY)
			.frequencyMHz(0).amplitudePercent(0).phaseDegrees(0.0);;
		ms.enqueue(m);
	}

	for (int channel = 0; channel < 64; channel++) {
		Message m = Message::make().destination(MessageDestination::KA007)
			.DAC(MessageDAC::DAC1).channel(channel)
			.setting(MessageSetting::LOADFREQUENCY)
			.frequencyMHz(0).amplitudePercent(0).phaseDegrees(0.0);;
		ms.enqueue(m);
	}

	for (int channel = 0; channel < 64; channel++) {
		Message m = Message::make().destination(MessageDestination::KA007)
			.DAC(MessageDAC::DAC2).channel(channel)
			.setting(MessageSetting::LOADFREQUENCY)
			.frequencyMHz(0).amplitudePercent(0).phaseDegrees(0.0);;
		ms.enqueue(m);
	}

	for (int channel = 0; channel < 64; channel++) {
		Message m = Message::make().destination(MessageDestination::KA007)
			.DAC(MessageDAC::DAC3).channel(channel)
			.setting(MessageSetting::LOADFREQUENCY)
			.frequencyMHz(0).amplitudePercent(0).phaseDegrees(0.0);;
		ms.enqueue(m);
	}
}

void gigaMoog::setDAC(int DAC, int channels, float *freqs, float *amps, float *phases) {
	MessageSender ms;

	_setDAC(ms, DAC, channels, freqs, amps, phases);

	send(ms);

	bool exit = false;

	std::cout << "exiting..." << std::endl;

}

void gigaMoog::_setDAC(MessageSender& ms, int DAC, int channels, float* freqs, float* amps, float* phases)
{
	MessageDAC dacset;
	if (DAC == 0) {
		dacset = MessageDAC::DAC0;
	}
	else if (DAC == 1) {
		dacset = MessageDAC::DAC1;
	}
	else if (DAC == 2) {
		dacset = MessageDAC::DAC2;
	}
	else if (DAC == 3) {
		dacset = MessageDAC::DAC3;
	}
	else {
		throw std::runtime_error("ERROR: unrecognized moog DAC selection");
	}

	for (unsigned chan = 0; chan < channels; chan++) {
		Message m = Message::make().destination(MessageDestination::KA007)
			.DAC(dacset).channel(chan)
			.setting(MessageSetting::LOADFREQUENCY)
			.frequencyMHz(freqs[chan]).amplitudePercent(amps[chan]).phaseDegrees(phases[chan]);;
		ms.enqueue(m);
		std::cout << "DAC: " << DAC << ", Channel: " << chan << ", Freq: " << freqs[chan] << ", Amp: " << amps[chan] << ", Phase(deg): " << phases[chan] << std::endl;
	}
}

void gigaMoog::zeroAndSetDAC(int DAC, int channels, float* freqs, float* amps, float* phases)
{
	MessageSender ms;;
	_zeroAll(ms);
	_setDAC(ms, DAC, channels, freqs, amps, phases);
	_endMessage(ms);

	send(ms);
}

void gigaMoog::zeroAndSetTwoDACs(int DAC0, int channels0, float* freqs0, float* amps0, float* phases0, int DAC1, int channels1, float* freqs1, float* amps1, float* phases1)
{
	MessageSender ms;;
	_zeroAll(ms);
	_setDAC(ms, DAC0, channels0, freqs0, amps0, phases0);
	_setDAC(ms, DAC1, channels1, freqs1, amps1, phases1);
	_endMessage(ms);

	send(ms);
}

void gigaMoog::send(MessageSender& ms)
{
	ms.getQueueElementCount();
	//BoostAsyncSerial fpga("COM6", 115200);
	MessagePrinter rec;
	//fpga.setReadCallback(boost::bind(&MessagePrinter::callback, rec, _1));
	fpga.write(ms.getMessageBytes());
}

void gigaMoog::endMessage() {
	MessageSender ms;

	_endMessage(ms);

	send(ms);
}

void gigaMoog::_endMessage(MessageSender& ms)
{
	Message m = Message::make().destination(MessageDestination::KA007)
		.setting(MessageSetting::TERMINATE_SEQ);
	ms.enqueue(m);
}
