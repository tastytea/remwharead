#!/bin/bash

if [[ -f Doxyfile ]]; then
    mkdir -p doc
    (doxygen -s -g - && cat Doxyfile && echo -n "PROJECT_NUMBER = " &&
         grep -Eo '[0-9]+.[0-9]+.[0-9]+$' CMakeLists.txt) | doxygen -
fi
