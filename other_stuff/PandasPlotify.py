#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Mar 29 15:54:20 2021

@author: buildbuntu
"""

from pandas import read_csv

df = read_csv("data.csv")
df.plot()