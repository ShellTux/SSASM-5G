#!/bin/sh
set -e

usage() {
	echo "Usage: $(basename "$0") [options] n"
	echo Options:
	echo "  -h, --help                   Print this usage message"
	exit 1
}

for arg
do
	[ "$arg" = -h ] && usage
	[ "$arg" = --help ] && usage
done

[ "$#" -ne 1 ] && usage

n="$1"

for _ in $(seq 1 "$n")
do
	plafond_inicial="$(seq 100 1000 | sort --random-sort | head -1)"
	max_authorization_requests="$(seq 1 100 | sort --random-sort | head -1)"
	video_interval="$(seq 1 100 | sort --random-sort | head -1)"
	music_interval="$(seq 1 100 | sort --random-sort | head -1)"
	social_interval="$(seq 1 100 | sort --random-sort | head -1)"
	reserved_data="$(seq 1 100 | sort --random-sort | head -1)"

	(
	set -xe
	./mobile_user \
		"$plafond_inicial" \
		"$max_authorization_requests" \
		"$video_interval" \
		"$music_interval" \
		"$social_interval" \
		"$reserved_data"
	)
done

./backoffice_user
