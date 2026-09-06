#!/bin/bash
docker run -dit --security-opt seccomp=unconfined \
       --name apeshit \
       -v $HOME:/host pradyun2/598ape /bin/bash

echo "Go apeshit"
