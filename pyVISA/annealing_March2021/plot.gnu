#set logscale xy
set logscale y
set border 3; set tics nomirror
set xrange [0:*]   
#set xrange [38:*]


plot "outputdata.csv" using 3:6, \
     "outputdata.csv" using 3:6, \
     "outputdata.csv" using 3:6, \
     "outputdata.csv" using 3:6, \
     "outputdata.csv" using 3:6, \
     "outputdata.csv" using 3:6
