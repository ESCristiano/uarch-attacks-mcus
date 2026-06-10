#!/usr/bin/env bash
ROOT="$(realpath "$(dirname "$0")")"

# T1_DIR="$(realpath "${ROOT}/t1-cch-cache")"
# T2_DIR="$(realpath "${ROOT}/t2-int-lat")"
# T3_DIR="$(realpath "${ROOT}/t3-cch-int")"
# T4_DIR="$(realpath "${ROOT}/t4-cch-busted")"
T5_DIR="$(realpath "${ROOT}/t5-toy-attack-cache")"
T6_DIR="$(realpath "${ROOT}/t6-toy-attack-nemesis")"
T7_DIR="$(realpath "${ROOT}/t7-toy-attack-busted")"
RESULTS_DIR="${ROOT}/results"

# Static Configuration Values
CLEAN=false
TIMING=false
TIMING_LOG="${ROOT}/log_test_timing.txt"

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
    # ${T1_DIR}/1-run-test.sh -c
    # ${T2_DIR}/1-run-test.sh -c
    # ${T3_DIR}/1-run-test.sh -c
    # ${T4_DIR}/1-run-test.sh -c
    ${T5_DIR}/1-run-test.sh -c
    ${T6_DIR}/1-run-test.sh -c
    ${T7_DIR}/1-run-test.sh -c
    rm -rf "${TIMING_LOG}" || true
    rm -rf "${RESULTS_DIR}" || true
    echo "Clean completed."
    exit 0
fi

mkdir -p "${RESULTS_DIR}"

# Initialize timing log if enabled
if [ "$TIMING" = true ]; then
    rm -rf "${TIMING_LOG}" || true
    echo "Test Execution Timing - $(date)" > "${TIMING_LOG}"
    echo "======================================" >> "${TIMING_LOG}"
fi

# Run tests with optional timing
# ${T1_DIR}/1-run-test.sh

# sleep 1 # Sleep for let the board reset and avoid potential issues with the next test
# ${T2_DIR}/1-run-test.sh

# sleep 1 # Sleep for let the board reset and avoid potential issues with the next test
# ${T3_DIR}/1-run-test.sh

# sleep 1 # Sleep for let the board reset and avoid potential issues with the next test

# ${T4_DIR}/1-run-test.sh

sleep 1 # Sleep for let the board reset and avoid potential issues with the next test

${T5_DIR}/1-run-test.sh
cp "${T5_DIR}/outputs/C04-toy_attack_cache_v2_matrix.png" "${RESULTS_DIR}/"

sleep 1 # Sleep for let the board reset and avoid potential issues with the next test

${T6_DIR}/1-run-test.sh
cp "${T6_DIR}/outputs/C04-toy_attack_nemesis_bare_matrix.png" "${RESULTS_DIR}/"

sleep 1 # Sleep for let the board reset and avoid potential issues with the next test

${T7_DIR}/1-run-test.sh
cp "${T7_DIR}/outputs/C04-toy_attack_busted_v2_matrix.png" "${RESULTS_DIR}/"


if [ "$TIMING" = true ]; then
    echo "" >> "${TIMING_LOG}"
    echo "Log saved to: ${TIMING_LOG}"
    cat "${TIMING_LOG}"
fi