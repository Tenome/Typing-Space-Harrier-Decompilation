#!/bin/sh
# usage: run_batch.sh <minutes> <workers-per-job> <parallel-jobs> addr...   (run from anywhere; needs VC6_DIR and orig/tsh.exe)
MIN=$1; W=$2; J=$3; shift 3
cd "$(dirname "$0")" || exit 1
mkdir -p ../../build/M
printf '%s\n' "$@" | xargs -P "$J" -I{} sh -c "python perm.py {} --minutes $MIN --workers $W > ../../build/M/out_{}.txt 2>&1"
