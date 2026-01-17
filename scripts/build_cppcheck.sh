#!/usr/bin/bash

CPPCHECK_VER=2.18.3
CMAKE_BUILD_DIR=build
CMAKE_OPTS=()

while getopts ":b:o:v:" opt; do
  case "${opt}" in
    b) CMAKE_BUILD_DIR="${OPTARG}"
    ;;
    o) IFS=" " read -r -a CMAKE_OPTS <<< "${OPTARG}"
    ;;
    v) CPPCHECK_VER="${OPTARG}"
    ;;
    *) printf "Unknown option %s; available options: \n\
        -b (build dir)\n\
        -o (cmake opts)\n\
        -v (cppcheck version)\n" "${opt}"
      exit 1
    ;;
  esac
done

wget "https://github.com/danmar/cppcheck/archive/${CPPCHECK_VER}.zip"
unzip -q "${CPPCHECK_VER}.zip"
rm "${CPPCHECK_VER}.zip"
mv "cppcheck-${CPPCHECK_VER}" cppcheck

cd cppcheck || { echo "Eroare cd"; exit 1; }
cmake -S . -B "${CMAKE_BUILD_DIR}" -DEXTERNALS_AS_SYSTEM=ON "${CMAKE_OPTS[@]}"
cmake --build "${CMAKE_BUILD_DIR}" -j6
