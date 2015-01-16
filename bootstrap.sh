#!/bin/bash

set -e

(cd 3rdparty; cmake . && make)
cmake .

