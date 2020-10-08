# -*- coding: utf-8 -*-
"""
Created on Wed Oct  7 19:37:52 2020

@author: ankan
"""

import noisereduce as nr
from scipy.io import wavfile
import numpy as np

rate, data = wavfile.read('9w5d.wav')
data = data/np.max(data)
reduced = nr.reduce_noise(data[:,0], data[9000:11000,0])
wavfile.write('test.wav', rate, reduced)