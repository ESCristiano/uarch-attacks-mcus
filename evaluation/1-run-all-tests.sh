#!/usr/bin/env bash
ROOT="$(realpath "$(dirname "$0")")"

# T1_DIR="$(realpath "${ROOT}/t1-cch-cache")"
T2_DIR="$(realpath "${ROOT}/t2-cch-nemesis")"
T4_DIR="$(realpath "${ROOT}/t4-toy-attack-cache")"
T5_DIR="$(realpath "${ROOT}/t5-toy-attack-nemesis")"
T6_DIR="$(realpath "${ROOT}/t6-toy-attack-busted")"
RESULTS_DIR="${ROOT}/results"

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
    # ${T1_DIR}/1-run-test.sh -c
    ${T2_DIR}/1-run-test.sh -c
    ${T4_DIR}/1-run-test.sh -c
    ${T5_DIR}/1-run-test.sh -c
    ${T6_DIR}/1-run-test.sh -c
    rm -rf "${RESULTS_DIR}" || true
    echo "Clean completed."
    exit 0
fi

mkdir -p "${RESULTS_DIR}"

# Run tests with optional timing
# ${T1_DIR}/1-run-test.sh

sleep 1 # Sleep for let the board reset and avoid potential issues with the next test
${T2_DIR}/1-run-test.sh
mkdir -p "${RESULTS_DIR}/t2-cch-nemesis" || true
cp "${T2_DIR}/outputs/C04-cch_0x0f_matrix.png" "${RESULTS_DIR}/t2-cch-nemesis/"
cp "${T2_DIR}/outputs/C04-cch_0x55_matrix.png" "${RESULTS_DIR}/t2-cch-nemesis/"
cp "${T2_DIR}/outputs/C04-cch_0xf0_matrix.png" "${RESULTS_DIR}/t2-cch-nemesis/"
cp "${T2_DIR}/outputs/C04-cch_0xff_matrix.png" "${RESULTS_DIR}/t2-cch-nemesis/"
cp "${T2_DIR}/outputs/irq_latency_mov_ldr.png" "${RESULTS_DIR}/t2-cch-nemesis/"

sleep 1 # Sleep for let the board reset and avoid potential issues with the next test
${T4_DIR}/1-run-test.sh
mkdir -p "${RESULTS_DIR}/t4-toy-attack-cache" || true
cp "${T4_DIR}/outputs/C04-toy_attack_cache_v2_matrix.png" "${RESULTS_DIR}/t4-toy-attack-cache/"

sleep 1 # Sleep for let the board reset and avoid potential issues with the next test
${T5_DIR}/1-run-test.sh
mkdir -p "${RESULTS_DIR}/t5-toy-attack-nemesis" || true
cp "${T5_DIR}/outputs/C04-toy_attack_nemesis_bare_matrix.png" "${RESULTS_DIR}/t5-toy-attack-nemesis/"

sleep 1 # Sleep for let the board reset and avoid potential issues with the next test
${T6_DIR}/1-run-test.sh
mkdir -p "${RESULTS_DIR}/t6-toy-attack-busted" || true
cp "${T6_DIR}/outputs/C04-toy_attack_busted_v2_matrix.png" "${RESULTS_DIR}/t6-toy-attack-busted/"
