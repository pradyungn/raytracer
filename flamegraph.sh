#!/bin/bash
# sourced from https://nodejs.org/learn/diagnostics/flame-graphs

FGPATH=../FlameGraph

COMMAND=""
case $1 in
    "pianoroom")
         COMMAND="./main.exe -i inputs/pianoroom.ray --ppm -o output/pianoroom.ppm -H 500 -W 500"
    ;;

    "globe")
         COMMAND="./main.exe -i inputs/globe.ray --ppm  -a inputs/globe.animate --movie -F 24"
    ;;

    "elephant")
         COMMAND="./main.exe -i inputs/elephant.ray --ppm  -a inputs/elephant.animate -F 24"
    ;;

    *)
        echo "fahhhhh"
        exit 1
        ;;
esac

if [ "$#" -eq 2 ]; then
    $COMMAND
else
    perf record -e cycles:u -g -- $COMMAND
    perf script > perfs.out
    cat perfs.out | ./$FGPATH/stackcollapse-perf.pl | ./$FGPATH/flamegraph.pl --colors=js > $1.svg
fi
