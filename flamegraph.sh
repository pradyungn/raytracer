#!/bin/bash
FGPATH=../FlameGraph

perf record -e cycles:u -g -- ./main.exe -i inputs/pianoroom.ray --ppm -o output/pianoroom.ppm -H 500 -W 500
perf script > perfs.out
cat perfs.out | ./$FGPATH/stackcollapse-perf.pl | ./$FGPATH/flamegraph.pl --colors=js > profile.svg
