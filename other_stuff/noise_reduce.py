# -*- coding: utf-8 -*-
"""
Created on Wed Oct  7 19:37:52 2020

@author: ankan
"""

from noisereduce import reduce_noise
from scipy.io.wavfile import read, write
from numpy import transpose, array, max

class NoiseReduceWrap:
    ##
    # Constructor, opens and reads .wav file.
    # Also scales the sound data to float[1,0].
    def __init__(self, filename):
        self._rate, self._data = read(filename)
        self._data = self._data/max(self._data)
    
    ##
    # Reduces noise from 2 channel data.
    # @param noise_sample_start The starting time(seconds) of noise sample in the data
    # @param noise_sample_end The ending time(seconds) of noise sample in the data
    def reduce_noise_2chnl(self, noise_sample_start, noise_sample_end):
        noise_start = int(noise_sample_start * self._rate)
        noise_end = int(noise_sample_end * self._rate)
        return transpose(array([reduce_noise(self._data[:,0], self._data[noise_start:noise_end,0]), reduce_noise(self._data[:,1], self._data[noise_start:noise_end,1])]))
    
    ##
    # Reduces noise from 2 channel data and writes a .wav file from it.
    # Uses directly the reduce_noise_2chnl above and writes output to .wav.
    def reduce_noise_2chnl_n_write(self, filename, noise_sample_start, noise_sample_end):
        write(filename, self._rate, self.reduce_noise_2chnl(noise_sample_start, noise_sample_end))

if __name__ == "__main__":
    filename = input("Give .wav filename to read\n")
    nrw = NoiseReduceWrap(filename)
    noise_start = float(input("From what time(seconds) a part with only noise starts? "))
    noise_end = float(input("On what time(seconds) the part with only noise ends? "))
    filename = input("Give .wav filename to write\n")
    nrw.reduce_noise_2chnl_n_write(filename, noise_start, noise_end)
