import numpy as np
from scipy import signal


# Define the filter parameters
Wn = (20)/(20/2)  # in hz filter
fs = 20  # Sample rate in hz
N = 3
b, a = signal.butter(N, Wn, btype='lowpass', fs=fs)

print(b, a)
