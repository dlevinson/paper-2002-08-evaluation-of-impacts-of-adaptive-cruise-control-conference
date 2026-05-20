#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")" && pwd)"
mkdir -p "$ROOT/build"
CXX="${CXX:-clang++}"
CXXFLAGS="${CXXFLAGS:--std=gnu++98 -Wall -Wextra}"
$CXX $CXXFLAGS "$ROOT/cth-and-gipps/road_pipeline_prototype.cpp" -o "$ROOT/build/road_pipeline_prototype"
$CXX $CXXFLAGS "$ROOT/cth-and-gipps/road_acc_cth.cpp" -o "$ROOT/build/road_acc_cth"
$CXX $CXXFLAGS "$ROOT/cth-and-gipps/road_manual_gipps.cpp" -o "$ROOT/build/road_manual_gipps"
$CXX $CXXFLAGS "$ROOT/mixed-cth/road_mixed_cth.cpp" -o "$ROOT/build/road_mixed_cth"
$CXX $CXXFLAGS "$ROOT/mixed-cth/road_mixed_cth_penetration_sweep.cpp" -o "$ROOT/build/road_mixed_cth_penetration_sweep"
