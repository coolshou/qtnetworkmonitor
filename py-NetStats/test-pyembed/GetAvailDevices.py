#!/bin/env python

"""
This file returns the description of the available network devices.
"""

import os

def findAvailDevices():
	fo = os.popen('ifconfig')

	Devices = []

	for line in fo:
		result = line.find('Link encap')
		if result != -1:
			Devices.append( line[0:result].strip() )

	return Devices


if (__name__ == '__main__'):

	print "Available devices:"
	print findAvailDevices()
