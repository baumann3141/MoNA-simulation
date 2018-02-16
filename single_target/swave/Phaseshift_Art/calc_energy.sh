#!/bin/bash
# To calculate the energy from a list of scattering lengths

# Inputs
file="pot_scat.dat" # File that contains scattering length
col=2 # Column of data that contains scattering length
m=939.565 # MeV - Mass of scattered particle
bound=0 # 1 for only unbound states (negative scattering), 0 for all.

# Constants
hc=197.3 # MeV fm

# Script
# Essentially use E=(hc)^2/(2*m/c^2*a^2)
if (($bound==1))
then
	awk -v col=$col -v m=$m -v hc=$hc '{if ($col<0) {print $0" "hc*hc/(2*m*$col*$col)}}' $file
else
	awk -v col=$col -v m=$m -v hc=$hc '{print $0" "hc*hc/(2*m*$col*$col)}' $file
fi
