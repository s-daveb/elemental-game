#!/bin/sh

SCRIPT_DIR="$(cd "$(dirname "$0")" || exit; pwd)"
BASE_DIR=$(realpath "$SCRIPT_DIR/..")

usage() {
	cat <<EOF
Usage: $(basename "$0") [OPTIONS] <command> [args...]

Runs a command in every CMake subdirectory found under the current directory.

OPTIONS:
  -h, --help	Show this help message and exit

ARGUMENTS:
  command	   The command to execute in each CMake directory
  args		  Optional arguments passed to the command

EXAMPLES:
  $(basename "$0") ls -la
  $(basename "$0") cmake --build .
  $(basename "$0") git status
EOF
}

die() {
	if [ -n "$1" ]; then
		echo "$1" >&2
		echo >&2
	fi
	echo "An error has occurred. Aborting." >&2
	exit 1
}

# Parameter checking
case "$1" in
	-h|--help)
		usage
		exit 0
		;;
	"")
		usage >&2
		echo >&2
		die "Error: no command specified."
		;;
esac

CMAKE_DIRS="$(find . -type d -iname 'CMake' -not -path './.git/*')"

if [ -z "$CMAKE_DIRS" ]; then
	die "No CMake directories found."
fi

for dir in $CMAKE_DIRS; do
	echo >&2
	echo "-=- $dir  -=-" >&2   
	echo >&2
	cd "$dir" || die "Could not cd to $dir"
	"$@"
	cd "$BASE_DIR" || die "Could not return to base dir"
done

# vim: set ts=4 sw=4 noet: 
