#!/bin/sh

SCRIPT_DIR="$(cd "$(dirname "$0")"; pwd)";
BASE_DIR=$(realpath "$SCRIPT_DIR/..")

BUILD_DIRS="vim-debug build release debug"

die() {
	if [ -n $1 ]; then
		echo $1
		echo
	fi
	echo "An error has ocurred. Aborting"
	return 1
}

[[ "$(realpath .)" == "$BASE_DIR" ]] || die "Please run from project root."


mybuilddir="."
for dir in $BUILD_DIRS; do
	[ -d "${dir}" ] && mybuilddir="${dir}"; break;
done

cmd="nice -20 run-clang-tidy -fix -format -j 4 -config-file=${BASE_DIR}/.clang-tidy -p ${mybuilddir} -use-color 1"
echo $cmd
exec $cmd



