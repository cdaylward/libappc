# libappc

## Overview

A C++ library for working with [App Containers](https://github.com/appc/spec). The goal of the
libary is to be a flexible toolkit: manifest parsing and creation, pluggable discovery, image
creation / extraction / caching, thin-provisioned file systems, etc...not a particular
[ACE](https://github.com/appc/spec/blob/master/SPEC.md#app-container-executor) implementation. In
other words, the goal is that you can use it to make one. Some or all pieces may be used, just use
the headers you require.

## Getting Started

Requires functional std::regex (If using gcc, >= 4.9)

1. Bootstrap it (download and build dependencies): `./bootstrap.sh`
2. Run the tests: `./test.sh`
3. Build the examples: `./build.sh`

## Examples

Example code is located in src/examples. Run `build.sh` and the examples will be built in
bin/examples, e.g.

```
$ ./build.sh
[ 34%] Built target _get_gtest
[ 69%] Built target _get_libarchive
[ 73%] Built target gtest
[ 78%] Built target gtest_main
[ 82%] Built target discover_image
[ 86%] Built target generate_complete_crm
[ 91%] Built target generate_minimal_crm
[ 95%] Built target parse
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

$ ./bin/examples/discovery/discover_image
Resolved: example.com/worker -> file:///tmp/images/example.com/worker-0.0.1-linux-x86_64.aci
Fetch failed: file:///tmp/images/example.com/worker-0.0.1-linux-x86_64.aci No such file or directory
Resolved: example.com/worker -> https://example.com/worker-0.0.1-linux-x86_64.aci
The requested URL returned error: 404 Not Found
Fetch failed: Failed to fetch https://example.com/worker-0.0.1-linux-x86_64.aci
Failed to retrieve image for example.com/worker
```

## Status

Early, early (like really early) development. Most pieces are incomplete and require tests, build
scripts need work, use of syntax likely inconsistent, etc.

Exists in some form:
- Schema parsing and validation
- Discovery (Simple, Local, no Meta)

## Contributing

See [CONTRIBUTING.md](https://github.com/cdaylward/libappc/blob/master/CONTRIBUTING.md)

## License

libappc is licensed under the Apache License, Version 2.0.

Included 3rd party libraries/headers (subject to change):

* JSON parsing is performed by Niels Lohmann's library which is licensed under the MIT license.
* Image extraction is performed by libarchive which is licensed under the BSD 2-Clause license.
