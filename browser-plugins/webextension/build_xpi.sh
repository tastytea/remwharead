#!/bin/sh

zip -r -FS ../remwharead.xpi --exclude 'native-wrapper/*' --exclude build_xpi.sh -- *
