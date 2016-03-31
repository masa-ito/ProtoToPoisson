
set size 0.5, 0.5
set output "airCooledCylinder_10points.eps"
pause 1

set terminal postscript eps enhanced color "Arial,"

set xlabel "x [m]"
set ylabel "temperature [degree]"
set xtics 0.2
set ytics 25
plot "airCooledCylinder_10points.log" u 1:2 with point linewidth 3 title "calc.", "airCooledCylinder_10points.log" u 1:3 with line linewidth 3 title "exact"

set output

set size 1.0, 1.0
set output "airCooledCylinder_10points.jpg"
pause 1

set terminal jpeg font "Arial,25" 

replot

set output

