#!/bin/sh
# Replace directory in remwharead.json

sed 's|/usr/lib/mozilla/native-messaging-hosts|'"${1}"'|' remwharead.json > "${2}"

