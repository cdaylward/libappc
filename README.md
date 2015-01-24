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

Bootstrap it (download and build dependencies):

```
./bootstrap.sh
```

Run the tests:

```
./test.sh
```

Build the examples:

```
./build.sh
```

## Examples

Example code is located in src/examples. Run `build.sh` and the examples will be built in
bin/examples, e.g.

```
$ ./build.sh
[ 36%] Built target _get_gtest
[ 72%] Built target _get_libarchive
[ 77%] Built target gtest
[ 81%] Built target gtest_main
[ 86%] Built target generate_complete_crm
[ 90%] Built target generate_minimal_crm
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
```


## Status

Early, early (like really early) development. Most pieces are incomplete and require tests, build
scripts need work, use of syntax likely inconsistent, etc.

## Contributing

See [CONTRIBUTING.md](https://github.com/cdaylward/libappc/blob/master/CONTRIBUTING.md)

## License

libappc is licensed under the Apache License, Version 2.0.

Included 3rd party libraries/headers (subject to change):

* JSON parsing is performed by Niels Lohmann's library which is licensed under the MIT license.
* Image extraction is performed by libarchive which is licensed under the BSD 2-Clause license.
