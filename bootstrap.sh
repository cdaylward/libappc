#!/usr/bin/env bash

set -e

(cd 3rdparty; cmake -Wno-dev . && make)
cmake .

