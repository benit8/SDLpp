#!/bin/sh
set -e

dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
buildDir="$dir/.build"
if [ ! -d "$buildDir" ]; then
	mkdir "$buildDir"
fi

clear ; clear
cd "$buildDir"

cmake .. -G "Unix Makefiles"
cmake --build . -- -j $(nproc)

cd "$dir"