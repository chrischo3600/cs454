import numpy as np
from scipy import signal


# Define the filter parameters
Wn = (2)/(20/2)  # in hz filter
N = 3
b, a = signal.butter(N, Wn, btype='lowpass', analog=False)

print(b, a)
