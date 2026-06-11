#!/usr/bin/env bash
ROOT="$(realpath "$(dirname "$0")")"

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
# Clean Build Directories
#-------------------------------------------------------------------------------
if [[ "$CLEAN" == "true" ]]; then
    echo "Cleaning NS and TF-M building directories."
    
    rm -rf ${ROOT}/build  \
           ${ROOT}/src/NonSecure/build/ \
           ${ROOT}/src/Secure/build/  || true
else
#-------------------------------------------------------------------------------
# Config NS
#-------------------------------------------------------------------------------
    # NS
    cmake -G "Eclipse CDT4 - Unix Makefiles" ${ROOT}/src -B ${ROOT}/build/
fi