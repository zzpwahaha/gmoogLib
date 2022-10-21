from ctypes import *
import pythoncom
import numpy as np
import time 

gmoogLib = cdll.LoadLibrary("./x64/Release/gmoogLib.dll")

class GM_python(object):
    def __init__(self):
        gmoogLib.gm_new.argtypes = []
        gmoogLib.gm_new.restype = c_void_p
        
        gmoogLib.gm_test.argtypes = [c_void_p]
        gmoogLib.gm_test.restype = c_int
        
        gmoogLib.gm_zeroAll.argtypes = [c_void_p]
        gmoogLib.gm_zeroAll.restype = c_void_p
        
        gmoogLib.gm_endMessage.argtypes = [c_void_p]
        gmoogLib.gm_endMessage.restype = c_void_p
        
        gmoogLib.gm_setDAC.argtypes = [c_void_p, c_int, c_int, POINTER(c_float), POINTER(c_float), POINTER(c_float)]
        gmoogLib.gm_setDAC.restype = c_void_p
        
        self.obj = gmoogLib.gm_new()
    
    def test(self):
        return gmoogLib.gm_test(self.obj)
    def zeroAll(self):
        gmoogLib.gm_zeroAll(self.obj)
    def endMessage(self):
        gmoogLib.gm_endMessage(self.obj)
    def setDAC(self, dac, channels, freqs, amps, phases):
        gmoogLib.gm_setDAC(self.obj, dac, channels, freqs, amps, phases)

gmoog = GM_python()



numtones0 = 4
numtones1 = 4


bgoff = (165, 165)

DACoffset = 0
amp_guessD0 = 80
amp_guessD1 = 80

lincorr0 = 0
lincorr1 = 0

freqcenterD0 = 82
spacingD0 = 0.5
freqcenterD1 = 82
spacingD1 = 3

tone_inds0 = np.arange(numtones0)
tone_inds1 = np.arange(numtones1)

freqsD0 = np.arange(freqcenterD0-spacingD0*(numtones0-1)/2, freqcenterD0+spacingD0*numtones0/2, spacingD0)
freqsD1 = np.arange(freqcenterD1-spacingD1*(numtones1-1)/2, freqcenterD1+spacingD1*numtones1/2, spacingD1)

print(freqsD0)

phases0 = np.pi*((tone_inds0+1)**2)/(numtones0)
phases1 = np.pi*((tone_inds1+1)**2)/(numtones1)

phase_degs0 = np.round(phases0*180/np.pi%360, 1)
phase_degs1 = np.round(phases1*180/np.pi%360, 1)

ampscale = 1

init_ampsD0 = np.ones(numtones0)*amp_guessD0 + (np.arange(numtones0)-numtones0/2)*lincorr0
init_ampsD1 = np.ones(numtones1)*amp_guessD1 + (np.arange(numtones1)-numtones1/2)*lincorr1

ampD0 = init_ampsD0
ampD1 = init_ampsD1

seq0 = c_float * numtones0
seq1 = c_float * numtones1

gmoog.zeroAll()
time.sleep(0.5)
gmoog.setDAC(0+DACoffset, len(freqsD0), seq0(*freqsD0), seq0(*ampD0), seq0(*phase_degs0))
time.sleep(0.5)
gmoog.setDAC(1+DACoffset, len(freqsD1), seq1(*freqsD1), seq1(*ampD1), seq1(*phase_degs1))
time.sleep(0.5)
gmoog.endMessage()
time.sleep(0.5)

for ind in tone_inds0:
    print('set', "DAC"+str(0+DACoffset), ind, ampD0[ind], freqsD0[ind], phase_degs0[ind])

print()

for ind in tone_inds1:
    print('set', "DAC"+str(1+DACoffset), ind, ampD1[ind], freqsD1[ind], phase_degs1[ind])