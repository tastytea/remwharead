#!/bin/bash
args=""
start=0

# Read everything betweeen " and the next " into args.
while IFS= read -r -n1 c; do
    if [[ ${start} -eq 0 ]]; then
        [[ "${c}" == '"' ]] && start=1
        continue
    fi
    if [[ "$c" != '"' ]] ; then
        args="${args}$c"
        continue
    fi
    break;
done

remwharead ${args}

echo -en '\x06\x00\x00\x00''"test"'
