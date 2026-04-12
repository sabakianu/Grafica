set table "sg3.pgf-plot.table"; set format "%.5f"
set format "%.7e";; set samples 25, 25; set dummy x,y; set isosamples 25, 25; splot [x=-1.024:1.024] [y=-1.024:1.024] x * x + y * y;
