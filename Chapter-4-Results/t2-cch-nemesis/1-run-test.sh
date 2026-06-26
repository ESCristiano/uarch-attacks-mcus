#!/usr/bin/env bash
ROOT="$(realpath "$(dirname "$0")")"

MSTP_ROOT_DIR="$(realpath "${ROOT}/../../m-step-thesis-exp")"

COPILOT_DIR="$(realpath "${MSTP_ROOT_DIR}/copilot")"
GRAPH_SCRIPT_DIR="$(realpath "${ROOT}/../scripts")"
MSTP_DIR="$(realpath "${MSTP_ROOT_DIR}/m-step")"
NS_DIR="$(realpath "${MSTP_ROOT_DIR}/ns-world-bare")"

LOG_DIR=${ROOT}/logs
RAW_TRACE=${LOG_DIR}/0_raw_trace.txt
OUTPUTS=${ROOT}/outputs

TEST_NAME="C04-cch_nemesis"
MATRIX_FILE=${LOG_DIR}/${TEST_NAME}.txt
PATTERN=${LOG_DIR}/${TEST_NAME}_aux.txt

# Static Configuration Values
CLEAN=false
VISUALIZE_ONLY=false

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
        -vi|--visualize)
            VISUALIZE_ONLY=true
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

# -------------------------------------------------------------------------------
# Clean up if requested
# -------------------------------------------------------------------------------
if [ "$CLEAN" = true ]; then
    echo "Cleaning logs directory..."
    rm -r ${LOG_DIR}/* || true
    rm -r ${OUTPUTS}/* || true
    ${NS_DIR}/0-config.sh -c
    echo "Clean completed."
    exit 0
fi

#-------------------------------------------------------------------------------
# Draw graphs only
#-------------------------------------------------------------------------------
# if [ "$VISUALIZE_ONLY" = true ]; then
#     python3 ${GRAPH_SCRIPT_DIR}/gen_matrix.py \
#             -p "${MATRIX_FILE}"           \
#             -o "${OUTPUTS}/"

#     xdg-open ${OUTPUTS}/${TEST_NAME}_matrix.png
#         exit 0
# fi

#-------------------------------------------------------------------------------
# Configure target
#-------------------------------------------------------------------------------
${NS_DIR}/0-config.sh

#-------------------------------------------------------------------------------
# Build target & Deploy
#-------------------------------------------------------------------------------
TEST_CONFIG="${MSTP_DIR}/mstp-eval/inc/test_eval_config.h"
TEST_CONFIG_S="${MSTP_DIR}/mstp-victims/s/inc/test_eval_config.h"

# Enable TEST 
for config in "${TEST_CONFIG}" "${TEST_CONFIG_S}"; do
    rm -rf "${config}" || true
    cat > "${config}" << 'EOF'
#define TEST_CCH_NEMESIS_ENABLE

EOF
done

${NS_DIR}/1-compile.sh --test-mode
${NS_DIR}/2-deploy.sh

#-------------------------------------------------------------------------------
# Log results
#-------------------------------------------------------------------------------
rm ${RAW_TRACE} | true
${NS_DIR}/3-monitor.sh -o ${RAW_TRACE}

#-------------------------------------------------------------------------------
# Extract traces
#-------------------------------------------------------------------------------
python3 ${ROOT}/extract_traces.py 

#-------------------------------------------------------------------------------
# Covert-Channel Nemesis: Draw matrices
#-------------------------------------------------------------------------------
MATRIX_FILE=${LOG_DIR}/C04-cch_0x0f.txt

python3 ${GRAPH_SCRIPT_DIR}/gen_matrix.py \
        -p "${MATRIX_FILE}"           \
        -o "${OUTPUTS}/"

MATRIX_FILE=${LOG_DIR}/C04-cch_0x55.txt

python3 ${GRAPH_SCRIPT_DIR}/gen_matrix.py \
        -p "${MATRIX_FILE}"           \
        -o "${OUTPUTS}/"

MATRIX_FILE=${LOG_DIR}/C04-cch_0xf0.txt

python3 ${GRAPH_SCRIPT_DIR}/gen_matrix.py \
        -p "${MATRIX_FILE}"           \
        -o "${OUTPUTS}/"

MATRIX_FILE=${LOG_DIR}/C04-cch_0xff.txt

python3 ${GRAPH_SCRIPT_DIR}/gen_matrix.py \
        -p "${MATRIX_FILE}"           \
        -o "${OUTPUTS}/"

#-------------------------------------------------------------------------------
# Nemesis Interrupt Lat: Draw Trace
#-------------------------------------------------------------------------------
python3 ${ROOT}/script-int-lat/create_graph_int_lats.py