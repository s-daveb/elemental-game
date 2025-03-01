#!/bin/sh

# Get the directory where the script is located
SCRIPT_DIR="$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)"

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

echo "Checking test coverage for headers in $MODULES_DIR..."
echo "====================================================="

NO_TESTS=""
WITH_TESTS=""

# Find all .hpp files in Modules/elemental/
while IFS= read -r header; do
    filename=$(basename "$header")
    base="${filename%.hpp}"

    # Check if a corresponding test file exists
    test_file="$TESTS_DIR/$base.test.cpp"

    if [ -f "$test_file" ]; then
        WITH_TESTS="${WITH_TESTS}✅ ${filename} → ${test_file}
"
    else
        NO_TESTS="${NO_TESTS}❌ ${filename} → No corresponding test found
"
    fi
done <<EOF
$(find "$MODULES_DIR" -type f -name "*.hpp")
EOF

# Print files without tests first
echo "Files without unit tests:"
echo "-------------------------"
# Print the NO_TESTS variable, ensuring the newlines are preserved
printf "%s" "$NO_TESTS"

echo ""
echo "Files with unit tests:"
echo "----------------------"
# Print the WITH_TESTS variable, ensuring the newlines are preserved
printf "%s" "$WITH_TESTS"

