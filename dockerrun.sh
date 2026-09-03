#!/bin/bash
docker run -it --security-opt seccomp=unconfined -v $HOME:/host pradyun2/598ape /bin/bash
