#!/bin/bash
# sourced from https://nodejs.org/learn/diagnostics/flame-graphs

FGPATH=../FlameGraph

case $1 in
    "pianoroom")
        perf record -e cycles:u -g -- \
         ./main.exe -i inputs/pianoroom.ray --ppm -o output/pianoroom.ppm -H 500 -W 500
    ;;

    "globe")
        perf record -e cycles:u -g -- \
         ./main.exe -i inputs/globe.ray --ppm  -a inputs/globe.animate --movie -F 24
    ;;

    "elephant")
        perf record -e cycles:u -g -- \
         ./main.exe -i inputs/elephant.ray --ppm  -a inputs/elephant.animate -F 24 -W 100 -H 100
    ;;

    *)
        echo "fahhhhh"
        exit 1
        ;;
esac

perf script > perfs.out
cat perfs.out | ./$FGPATH/stackcollapse-perf.pl | ./$FGPATH/flamegraph.pl --colors=js > $1.svg
