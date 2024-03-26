#####################################################
###                                               ###
### BU CAS CS454/654 Embedded Systems Development ###
###                                               ###
### Author: Renato Mancuso (BU)                   ###
###                                               ###
### File: FFT Generation/plotting demo            ###
###                                               ###
#####################################################

from scipy.fftpack import fft, fftshift, fftfreq
import numpy as np
import matplotlib.pyplot as plt

## Where to save generated figures
savepath="/scratch/team-7a/Lab4/python/output";

## Generic function to compute the FFT.
## @s: sampled signal for which to compute the FFT
## @fc: frequency of sampling in Hz
## returns: xf and yf ready for plotting

def getFFT(s, fc):
    # Sampling period
    Tc = 1/fc
    # Number of samples
    N = len(s)

    # Compute y-axis values 
    yf = fftshift(fft(s)/(N/2))

    # Shift and normalize x-axis values
    xf = fftshift(fftfreq(N, Tc))

    return xf, yf


def plotSignal(x, y, title):
    plt.title(title)
    plt.xlabel("time (s)")
    plt.ylabel("$s(t)$ value")
    plt.plot(x, y)
    plt.grid()
    plt.show()
    

def plotFFT(xf, yf, extra_label=""):
    plt.title("Fast Fuorier Transform (FFT) " + extra_label)
    plt.xlabel("frenqeucy (Hz)")
    plt.ylabel("magnitude of harmonic")
    plt.plot(xf, np.abs(yf))
    plt.grid()
    plt.show()
    
def generateSquareWave(freq, amp, sampling, duration):
	Tc = 1/freq
	x = np.linspace(0, duration, duration * sampling, endpoint=False)
	square_wave = amp/2 * np.sign(np.sin(2 * np.pi * Tc * x)) + amp/2
	return x, square_wave

# PART A
sampling_freq = 10000
sampling_duration = 10
x, y = generateSquareWave(1, 5, sampling_freq, sampling_duration)
plotSignal(x, y, "Square Wave")

xf, yf = getFFT(y, sampling_freq)
plotFFT(xf, yf, "Square Wave FFT")

