#!/bin/csh -f
# execute IPOPT

# parameters:
#    $1 : scratch directory
#    $2 : working directory
#    $3 : "$1", parameter file
#    $4 : control file name
#    $5 : system directory

${5}gmsip_nx.exe $4
