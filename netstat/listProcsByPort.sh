#!/usr/bin/env bash

# stderr: noises
netstat -nepal 2>/dev/null | grep -i 22

# only tcp
netstat -nepal -t 2>/dev/null | grep -i 22

netstat -lnp 2>/dev/null
# will list the pid and process name next to each
# listening port. This will work under Linux,
# but not all others (like AIX.)
# Add -t if you want TCP only.