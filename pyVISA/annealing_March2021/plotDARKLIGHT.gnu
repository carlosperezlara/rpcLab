#set logscale xy
set logscale y
set border 3; set tics nomirror
set xrange [28:*]

set grid ytics mytics
set mytics 10
set grid xtics mxtics
set mxtics 5
set grid

plot "outputfiles/n20ish/5e13-2/data1_LED.csv" using 3:6, \
     "outputfiles/n20ish/5e13-2/data2_LED.csv" using 3:6, \
     "outputfiles/n20ish/5e13-2/data1_DARK.csv" using 3:6, \
     "outputfiles/n20ish/5e13-2/data2_DARK.csv" using 3:6, \
     "outputfiles/n20ish/5e13-1/data1_LED.csv" using 3:6, \
     "outputfiles/n20ish/5e13-1/data2_LED.csv" using 3:6, \
     "outputfiles/n20ish/5e13-1/data1_DARK.csv" using 3:6, \
     "outputfiles/n20ish/5e13-1/data2_DARK.csv" using 3:6, \
     "outputfiles/n20ish/2e14-1/data1_LED.csv" using 3:6, \
     "outputfiles/n20ish/2e14-1/data2_LED.csv" using 3:6, \
     "outputfiles/n20ish/2e14-1/data1_DARK.csv" using 3:6, \
     "outputfiles/n20ish/2e14-1/data2_DARK.csv" using 3:6, \
     "outputfiles/n20ish/2e14-2/data1_LED.csv" using 3:6, \
     "outputfiles/n20ish/2e14-2/data2_LED.csv" using 3:6, \
     "outputfiles/n20ish/2e14-2/data1_DARK.csv" using 3:6, \
     "outputfiles/n20ish/2e14-2/data2_DARK.csv" using 3:6
