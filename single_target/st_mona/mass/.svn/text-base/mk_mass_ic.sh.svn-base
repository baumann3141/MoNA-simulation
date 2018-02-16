#!/bin/bash

gawk '
BEGIN {
print "static const char* massTab[] = {"
}

END {
  print "\"\""         # just so we don not worry about the last comma 
  print "};"

  print "// first index (see mass.mas03)"
  print "static const unsigned int massIdxFirst = ", istart, ";"
  print "// last index is given by size"
  print "static const unsigned int massIdxLast  = ", NR - 1, ";"
}

{
gsub("\"","\\\"")      # replace quotes 
gsub("#",".")          # replace pound sign with decimal point (extrapolated values)
print "\"" $0 "\","    # print the line surrounded by quotes and followed by a comma
}

$6 == "n" {istart = NR - 1} # first line with valid data has an "n" in the 6 column (neutron)
'

