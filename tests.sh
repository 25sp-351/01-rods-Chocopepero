#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'


EXEC="./main"


if [ ! -f "$EXEC" ]; then
    echo -e "${RED}Error: $EXEC not found. Run 'make' first.${NC}"
    exit 1
fi

# Using "\n" caused problems with comparison, so use literal line breaks instead.
tests=(
    # Normal Case (No Remainder)
   "1" "1,5" "1 @ 1 = 5
Remainder: 0
Value: 5"
    # No Applicable Pieces
    "5" "10,50" "Remainder: 5
Value: 0"
    # Duplicate Pieces
    "10" "2,4
2,4
2,4" "5 @ 2 = 20
Remainder: 0
Value: 20"
    # Value Based Check
    "8" "4,4
6,5" "2 @ 4 = 8
Remainder: 0
Value: 8"

    "6" "3,10" "2 @ 3 = 20
Remainder: 0
Value: 20"

    "10" "1,3
2,1
3,2"
"10 @ 1 = 30
Remainder: 0
Value: 30"
)

total_tests=${#tests[@]}
total_tests=$((total_tests / 3))
passed=0

echo "Running $total_tests tests..."

for ((i=0; i<${#tests[@]}; i+=3)); do
    rod_length="${tests[i]}"
    input="${tests[i+1]}"
    expected="${tests[i+2]}"

    output=$(echo -e "$input" | $EXEC "$rod_length" | tr -d '\r')

    if diff <(echo "$output") <(echo "$expected") >/dev/null; then
        echo -e "${GREEN}Test $((i/3+1))/$total_tests PASSED${NC} (Rod Length: $rod_length)"
        ((passed++))
    else
        echo -e "${RED}Test $((i/3+1))/$total_tests FAILED${NC} (Rod Length: $rod_length)"
        echo -e "Expected:\n$expected"
        echo -e "Got:\n$output"
    fi
done

echo -e "\n$passed/$total_tests tests passed."


if [ "$passed" -ne "$total_tests" ]; then
    exit 1
fi
