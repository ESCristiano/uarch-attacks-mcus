#!/usr/bin/env bash
ROOT="$(realpath "$(dirname "$0")")"

BUSTED_DIR="$(realpath "${ROOT}/BUSted")"
BUSTED_POC_DIR="$(realpath "${BUSTED_DIR}/PoC-TF-M/TF_M_dma_side_channel")"
COPILOT_DIR="$(realpath "${ROOT}/../Chapter-4-First-Evidence/m-step/copilot/copilot.sh")"

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
    ${BUSTED_POC_DIR}/0_setup_code.sh -c
    echo "Clean completed."
    exit 0
fi

# # Configure the build system
${BUSTED_POC_DIR}/0_setup_code.sh

# # Compile the Secure and Non-Secure images
${BUSTED_POC_DIR}/1_compile.sh

# # Deploy to the target board
bash ${BUSTED_DIR}/PoC-TF-M/Build_STM32L552_level2/regression.sh
sleep 1
${BUSTED_POC_DIR}/2_deploy.sh

# Run the PoC and see the terminal output
sleep 1
rm ${RAW_TRACE} | true
${COPILOT_DIR} -m "${RAW_TRACE}"
