#!/bin/sh

trap 'debugprint "Exiting script"; exit' INT

if [ -z "$FSWATCH_ENV_LOADED" ]; then
	source ./scripts/fswatch-build.env
fi
if [ -z "$FSWATCH_ENV_LOADED" ]; then
	debugprint "Please run from project root containing fwatch-build.env" 1>&2
	exit 255
fi

unset FSWATCH_ENV_LOADED
# Check the current niceness level
current_niceness=$(ps -o nice= -p $$)

if [ $current_niceness -lt ${FSWATCH_DESIRED_NICENESS}  ]; then
	debugprint "Changing niceness to ${FSWATCH_DESIRED_NICENESS}"
	exec nice -${FSWATCH_DESIRED_NICENESS} -- $0
else
	debugprint "Script Initialized"
fi

reset
debugprint "Scanning source directories for file changes..."
while true; do
    # Watch for changes in .hpp and .cpp files in the current directory
    changed_file=$(fswatch -1 --event=Updated ${FSWATCH_SOURCE_DIRS} 2>/dev/null)

    # If there are changes, wait for a moment and then run the command
    if [ -n "$changed_file" ]; then
	debugprint  "File change detected: ${changed_file}"
	debugprint -n  "Regenerating docs..."

        nice -20 -- cmake --build ${FSWATCH_BUILD_DIR} -t ctest -j 2
	debugprint "done!"
    fi

done

