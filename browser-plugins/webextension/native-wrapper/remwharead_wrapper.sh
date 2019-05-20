#!/bin/sh

read args
args="$(echo ${args} | sed -E 's/^.+\"(.+)\"$/\1/')" # Remove length and quotes.
remwharead ${args}
