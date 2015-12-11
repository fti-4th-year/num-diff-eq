#!/bin/sh

./run
gnuplot -p -e "set logscale y; set format y '10^{%L}'; plot 'euler.txt' w l, 'runge-kutta.txt' w l, 'euler-mod.txt' w l, 1 + x"
#gnuplot -p -e "plot 'euler.txt', 'runge-kutta.txt', 'euler-mod.txt', 1 + x"
#gnuplot -p -e "set logscale y; set format y '10^{%L}'; plot 'euler.txt' u 1:3 w l, 'runge-kutta.txt' u 1:3 w l, 'euler-mod.txt' u 1:3 w l;"
#gnuplot -p -e "set logscale y 2; set format y '2^{%L}'; plot 'euler.txt', 'runge-kutta.txt', 'euler-mod.txt';"
