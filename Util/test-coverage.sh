#!/bin/bash

# Get the directory where the script is located
SCRIPT_DIR="$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)"

# Read blocklist file into an array, one filename per element
EXEMPT_FILES=()
while IFS= read -r line; do
    EXEMPT_FILES+=("$line")
done < "${SCRIPT_DIR}/test-coverage.blocklist"

# Move one directory up
cd "$SCRIPT_DIR/.."

MODULES_DIR="Modules/elemental"
TESTS_DIR="Tests"

# Check if directories exist
if [ ! -d "$MODULES_DIR" ]; then
    echo "Could not find $MODULES_DIR"
    exit 1
fi

if [ ! -d "$TESTS_DIR" ]; then
    echo "Could not find $TESTS_DIR"
    exit 1
fi

echo "Checking test coverage for headers in $MODULES_DIR..." | tee  ~/.test-cover.sh.lastrun.log
echo "=====================================================" | tee -a ~/.test-cover.sh.lastrun.log
echo
echo "Exempted Modules (non-testable, etc)"
echo "-------------------------------------"
for exempt in "${EXEMPT_FILES[@]}"; do
    echo "☑️ $exempt -> Skipped..."
done
echo

NO_TESTS=""
WITH_TESTS=""

TESTED_MODULES=0
TOTAL_MODULES=0

# Find all .hpp files in Modules/elemental/ (excluding .impl.hpp files)
while IFS= read -r header; do
    filename=$(basename "$header")
    base="${filename%.hpp}"

    # Check if the header is exempt
    exempted=false
    for exempt in "${EXEMPT_FILES[@]}"; do
        if [ "$exempt" = "$filename" ]; then
            exempted=true
            break
        fi
    done

    if $exempted; then
        echo "$filename is exempt." >> ~/.test-cover.sh.lastrun.log
        continue
    fi

    # Increase total modules count (non-exempt only)
    TOTAL_MODULES=$((TOTAL_MODULES + 1))

    # Check if a corresponding test file exists
    test_file="$(find "$TESTS_DIR" -iname "$base.test.cpp")"
    if [ -f "$test_file" ]; then
        WITH_TESTS="${WITH_TESTS}✅  ${filename} → ${test_file}\n"
        TESTED_MODULES=$((TESTED_MODULES + 1))
    else
        echo "$filename is not exempt." >> ~/.test-cover.sh.lastrun.log
        NO_TESTS="${NO_TESTS}❌  ${filename} → No corresponding test found!\n"
    fi

done <<EOF
$(find "$MODULES_DIR" -type f -name "*.hpp" -and -not -name "*.impl.hpp")
EOF

# Print files without tests first
echo "Files without unit tests:"
echo "-------------------------"
printf "%b" "$NO_TESTS"

echo ""
echo "Files with unit tests:"
echo "----------------------"
printf "%b" "$WITH_TESTS"

# Calculate and print coverage percentage
if [ "$TOTAL_MODULES" -gt 0 ]; then
    # Using awk for floating point calculation
    coverage=$(awk "BEGIN {printf \"%.2f\", ($TESTED_MODULES/$TOTAL_MODULES)*100}")
    echo ""
    echo "Summary:"
    echo "--------"
    echo "Tested Modules: $TESTED_MODULES"
    echo "Total Modules (non-exempt): $TOTAL_MODULES"
    echo "Code Coverage: $coverage%"
else
    echo "No non-exempt modules found."a
fi
