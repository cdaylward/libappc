# libappc

[![Build Status](https://travis-ci.org/cdaylward/libappc.svg?branch=master)](https://travis-ci.org/cdaylward/libappc)

## Overview

A C++ library for working with [App Containers](https://github.com/appc/spec). The goal of the
libary is to be a flexible toolkit: manifest parsing and creation, pluggable discovery, image
creation / extraction / caching, thin-provisioned file systems, etc...not a particular
[ACE](https://github.com/appc/spec/blob/master/SPEC.md#app-container-executor) implementation. In
other words, the goal is that you can use it to make one. Some or all pieces may be used, just use
the headers you require.

## Getting Started

Requires libarchive, libcurl, and functional std::regex (If using gcc, >= 4.9)

1. Bootstrap it (download and build dependencies): `./bootstrap.sh`
2. Run the tests: `./test.sh`
3. Build the examples: `./build.sh`

## Status

Early, early (like really early) development. Many pieces are incomplete and require tests, build
scripts need work, use of syntax likely inconsistent, etc.

Components that exist in some form:
- Schema parsing and validation
- Discovery (Simple, Local, no Meta)
- ACI inspection and extraction

## Examples

Example code is located in src/examples. Run `build.sh` and the examples will be built in
bin/examples, e.g.

```
$ ./build.sh
--- 8< ---
[100%] Built target validate

$ ./bin/examples/schema/generate_complete_crm > /tmp/container.json

$ ./bin/examples/schema/validate /tmp/container.json || echo invalid

$ ./bin/examples/schema/parse /tmp/container.json
Kind: ContainerRuntimeManifest
Version: 0.2.0
UUID: 0F426158-97EE-49F8-B4A3-792ECDA926FB
--- 8< ---

$ ./bin/examples/schema/validate image_bad.json
Invalid Manifest: "imageID must be <hash name>-<hex representation> format"

$ ./bin/examples/discovery/discover_image nosecone.net/example/test 
Resolved: nosecone.net/example/test -> file:///tmp/images/nosecone.net/example/test-1.0.0-linux-amd64.aci
Fetch failed: file:///tmp/images/nosecone.net/example/test-1.0.0-linux-amd64.aci No such file or directory
Resolved: nosecone.net/example/test -> https://nosecone.net/example/test-1.0.0-linux-amd64.aci
Fetched: https://nosecone.net/example/test-1.0.0-linux-amd64.aci
Stored: file:///tmp/images/nosecone.net/example/test-1.0.0-linux-amd64.aci
```

## Contributing

See [CONTRIBUTING.md](https://github.com/cdaylward/libappc/blob/master/CONTRIBUTING.md)

## License

libappc is licensed under the Apache License, Version 2.0.

Included 3rd party libraries/headers (subject to change):

* JSON parsing is performed by Niels Lohmann's library which is licensed under the MIT license.
* Image retrieval is performed by libcurl which is licensed under the MIT license.
* Image extraction is performed by libarchive which is licensed under the BSD 2-Clause license.
