#!/usr/bin/env bash
ROOT="$(realpath "$(dirname "$0")")"

MSTP_ROOT_DIR="$(realpath "${ROOT}/../../m-step")"
COPILOT_DIR="$(realpath "${MSTP_ROOT_DIR}/copilot")"
COPILOT="${COPILOT_DIR}/copilot.sh"

# Static Configuration Values
RAW_TRACE="./logs/log.txt"

#-------------------------------------------------------------------------------
# Parse arguments
#-------------------------------------------------------------------------------
set -e

while [[ $# -gt 0 ]]; do
    case "$1" in
        -o|--output)
            RAW_TRACE="$2"
            if [[ -z "$RAW_TRACE" ]]; then
                echo "Error: -o|--output requires an output file path."
                exit 1
            fi
            shift 2
            ;;
        -h|--help)
            echo "Usage: $0 [-o|--output <log_file>]"
            exit 0
            ;;
        *)
            echo "Unknown argument: $1"
            echo "Usage: $0 [-o|--output <log_file>]"
            exit 1
            ;;
    esac
done

#-------------------------------------------------------------------------------
# Log results
#-------------------------------------------------------------------------------
${COPILOT} -m "${RAW_TRACE}"

