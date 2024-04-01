#!/bin/sh
set -e

if [ "$(id --user)" -lt 1000 ]
then
	printf '%s\n' 'This script is meant to remove non-root ipcs'
	exit 1
fi

set -xe

ipcrm --all
