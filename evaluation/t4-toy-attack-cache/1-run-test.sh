#!/usr/bin/env bash
ROOT="$(realpath "$(dirname "$0")")"

MSTP_ROOT_DIR="$(realpath "${ROOT}/../../m-step")"

COPILOT_DIR="$(realpath "${MSTP_ROOT_DIR}/copilot")"
GRAPH_SCRIPT_DIR="$(realpath "${ROOT}/../scripts")"
MSTP_DIR="$(realpath "${MSTP_ROOT_DIR}/m-step")"
NS_DIR="$(realpath "${ROOT}/../../toy-attack-cache-src")"

LOG_DIR=${ROOT}/logs
RAW_TRACE=${LOG_DIR}/0_raw_trace.txt
OUTPUTS=${ROOT}/outputs

TEST_NAME="C04-toy_attack_cache"
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

#-------------------------------------------------------------------------------
# Clean up if requested
#-------------------------------------------------------------------------------
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
if [ "$VISUALIZE_ONLY" = true ]; then
    python3 ${GRAPH_SCRIPT_DIR}/gen_matrix.py \
            -p "${MATRIX_FILE}"           \
            -o "${OUTPUTS}/"

    xdg-open ${OUTPUTS}/${TEST_NAME}_matrix.png
        exit 0
fi

#-------------------------------------------------------------------------------
# Configure target
#-------------------------------------------------------------------------------
${NS_DIR}/0-config.sh

#-------------------------------------------------------------------------------
# Build target & Deploy
#-------------------------------------------------------------------------------
${NS_DIR}/1-compile.sh
${NS_DIR}/2-deploy.sh

#-------------------------------------------------------------------------------
# Log results
#-------------------------------------------------------------------------------
sleep 2
rm ${RAW_TRACE} | true
${NS_DIR}/3-monitor.sh -o ${RAW_TRACE}

#-------------------------------------------------------------------------------
# Filter results
#-------------------------------------------------------------------------------
# MATRIX_FILE_IF=${LOG_DIR}/${TEST_NAME}_v1.txt

# grep --no-group-separator -A 16 -E 'TRACE IF' ${RAW_TRACE} |\
# grep -v -E 'TRACE' > ${PATTERN}

# # Append "orange else" header to matrix file (same line)
# cat > ${MATRIX_FILE_IF} << 'EOF'
# Color_type: Integer #Integer or Probability
# Gradient: orange
# # Min_number_of_colors: 2
# # Y: 
# # X: 

# EOF
# # Append results to matrix file (same line)
# cat ${PATTERN} >> ${MATRIX_FILE_IF}

# python3 ${GRAPH_SCRIPT_DIR}/gen_matrix.py \
#         -p "${MATRIX_FILE_IF}"           \
#         -o "${OUTPUTS}/"
# #-------------------------------------------------------------------------------
# # Draw graphs
# #-------------------------------------------------------------------------------
# python3 ${GRAPH_SCRIPT_DIR}/gen_matrix.py \
#         -p "${MATRIX_FILE_ELSE}"           \
#         -o "${OUTPUTS}/"

#  xdg-open ${OUTPUTS}/${TEST_NAME}_v1.png


#-------------------------------------------------------------------------------
# Filter results
#-------------------------------------------------------------------------------
MATRIX_FILE=${LOG_DIR}/${TEST_NAME}_v2.txt

grep --no-group-separator -A 1 -E 'TRACE ELSE' ${RAW_TRACE} |\
grep -v -E 'TRACE' > ${PATTERN}

# Append "orange if" header to matrix file (same line)
echo -n 'orange if ' > ${MATRIX_FILE}

# Append results to matrix file (same line)
cat ${PATTERN} >> ${MATRIX_FILE}

grep --no-group-separator -A 1 -E 'TRACE IF' ${RAW_TRACE} |\
grep -v -E 'TRACE' > ${PATTERN}

# Append "orange else" header to matrix file (same line)
echo -n 'orange else ' >> ${MATRIX_FILE}

# Append results to matrix file (same line)
cat ${PATTERN} >> ${MATRIX_FILE}
rm ${PATTERN} | true

python3 ${GRAPH_SCRIPT_DIR}/gen_template_matrix.py    \
        -p "${MATRIX_FILE}"                         \
        -o "${OUTPUTS}/"

# xdg-open ${OUTPUTS}/${TEST_NAME}_v2.png