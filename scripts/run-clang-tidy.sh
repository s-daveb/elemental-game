#!/bin/sh

SCRIPT_DIR="$(cd "$(dirname "$0")"; pwd)";
BASE_DIR=$(realpath "$SCRIPT_DIR/..")

BUILD_DIRS="vim-debug build release debug"
commit_changes_flags=""
additional_flags=""

die() {
	if [ -n $1 ]; then
		echo $1
		echo
	fi
	echo "An error has ocurred. Aborting"
	return 1
}

echo_exec() {
	echo "$@"
	$@
}

[[ "$(realpath .)" == "$BASE_DIR" ]] || die "Please run from project root."

# Parse command-line arguments
while [ "$#" -gt "0" ]; do
	case "$1" in
		-fix)
			commit_changes_flags="-fix -format"
			;;
		-quiet)
			additional_flags="${additional_flags} -quiet"
			;;
		-use-color)
			[ -n "$2" ] && [ $2 -eq 1 ] && shift
			additional_flags="${additional_flags} -use-color 1"
			;;
		*)
			additional_flags="${additional_flags} $1"
			;;
	esac
	shift
done

mybuilddir="."
for dir in $BUILD_DIRS; do
	[ -d "${dir}" ] && mybuilddir="${dir}"; break;
done

cmd="nice -20 run-clang-tidy ${commit_changes_flags} -config-file=${BASE_DIR}/.clang-tidy -p ${mybuilddir} -j 4 ${additional_flags}"

echo_exec $cmd



