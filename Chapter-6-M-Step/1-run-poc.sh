#!/usr/bin/env bash
ROOT="$(realpath "$(dirname "$0")")"

MSTP_DIR="$(realpath "${ROOT}/../m-step-thesis-exp")"
MSTP_POC_DIR="$(realpath "${MSTP_DIR}/evaluation/t12-mstp-keypad-poc/1-run-test.sh")"
MSTP_TESTS_DIR="$(realpath "${MSTP_DIR}/m-step-thesis-exp/evaluation/1-run-all-tests.sh")"

RAW_TRACE="${ROOT}/logs/trace.log"

# Static Configuration Values
CLEAN=false

#-------------------------------------------------------------------------------
# Parse arguments
#-------------------------------------------------------------------------------
set -e

while [[ $# -gt 0 ]]; do
    case "$1" in
        -c|--clean)
            CLEAN=true
            shift
            ;;
        -h|--help)
            echo "Usage: $0 [-c|--clean]"
            exit 0
            ;;
        *)
            echo "Unknown argument: $1"
            echo "Usage: $0 [-c|--clean]"
            exit 1
            ;;
    esac
done

#-------------------------------------------------------------------------------
# Clean up if requested
#-------------------------------------------------------------------------------
if [ "$CLEAN" = true ]; then
    echo "Cleaning logs directory..."
    # ${BUSTED_POC_DIR}/0_setup_code.sh -c
    echo "Clean completed."
    exit 0
fi

