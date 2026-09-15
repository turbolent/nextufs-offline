#!/bin/sh
set -eu

NEXTUFS=${NEXTUFS:-./nextufs}
CORRUPT=${CORRUPT:-./tests/fsck/tools/corrupt_raw_case}
WORK="${1:-.scratch}/fsck-images"
mkdir -p "$WORK"

expect_status()
{
	expected=$1
	shift
	status=0
	"$@" > "$WORK/check.log" 2>&1 || status=$?
	if test "$status" -ne "$expected"; then
		cat "$WORK/check.log" >&2
		echo "expected status $expected, got $status: $*" >&2
		exit 1
	fi
}

for layout in raw labeled; do
	image="$WORK/$layout.img"
	if test "$layout" = raw; then
		"$NEXTUFS" mkimg --raw --force-overwrite "$image" 32M >/dev/null
	else
		"$NEXTUFS" mkimg --force-overwrite "$image" 32M >/dev/null
	fi
	"$NEXTUFS" mkfile "$image" /alpha alpha >/dev/null
	# Exercise indirect block pointers as well as the inode/superblock layout.
	"$NEXTUFS" mkfile "$image" /big x >/dev/null
	"$NEXTUFS" mkfile --truncate "$image" /big 120000 >/dev/null
	cp "$image" "$WORK/before.img"
	expect_status 0 "$NEXTUFS" fsck -n "$image"
	grep -F 'Phase 5' "$WORK/check.log" >/dev/null
	cmp "$image" "$WORK/before.img"
	"$CORRUPT" bad-block-count "$image"
	cp "$image" "$WORK/before.img"
	expect_status 8 "$NEXTUFS" fsck -n "$image"
	grep -F 'INCORRECT BLOCK COUNT' "$WORK/check.log" >/dev/null
	cmp "$image" "$WORK/before.img"
	expect_status 0 "$NEXTUFS" fsck -y "$image"
	grep -F 'FILE SYSTEM WAS MODIFIED' "$WORK/check.log" >/dev/null
	expect_status 0 "$NEXTUFS" fsck -n "$image"
	test "$("$NEXTUFS" browse --raw "$image" /alpha)" = alpha
done

# Setup failures must not be reported as successful checks.
printf 'not a filesystem\n' > "$WORK/invalid.img"
expect_status 8 "$NEXTUFS" fsck -n "$WORK/invalid.img"
expect_status 8 "$NEXTUFS" fsck -n "$WORK/raw.img/missing"

case "${FSCK_IMAGE_ONLY:-$(uname -s)}" in
	Linux) ;;
	*)
		expect_status 2 "$NEXTUFS" fsck
		grep -F 'requires an image file' "$WORK/check.log" >/dev/null
		expect_status 8 "$NEXTUFS" fsck -y "$WORK"
		grep -F 'supports only image files' "$WORK/check.log" >/dev/null
		expect_status 8 "$NEXTUFS" fsck -y /dev/null
		grep -F 'supports only image files' "$WORK/check.log" >/dev/null
		;;
esac

echo 'fsck image regressions passed'
