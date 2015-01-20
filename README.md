# libappc

## Overview

A C++ library for doing things with [App Containers](https://github.com/appc/spec). The goal of the libary is to be a flexible toolkit: manifest parsing and creation, pluggable discovery, image creation/extraction/caching, thin-provisioned file systems, etc. Configuration and data interchange will almost always be JSON. Some or all pieces may be used, just use the headers you require.

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

Example code is located in src/examples. build.sh will build them and place them in bin/examples, e.g.

```
$ ./bin/examples/validate_manifest container.json || echo fail
$ ./bin/examples/validate_manifest container_bad.json || echo fail
Container runtime manifest is invalid: "0.1 is not a valid semver 2.0 version string.", "UUID must be a rfc4122-formatted string."
fail
$
```


## Status

Early, early (like really early) development. Most pieces are incomplete and require tests, build scripts need work, use of syntax likely inconsistent, etc.

## Contributing

See [CONTRIBUTING.md](https://github.com/cdaylward/libappc/blob/master/CONTRIBUTING.md)

## License

libappc is licensed under the Apache License, Version 2.0.

Included 3rd party libraries/headers (subject to change):

* JSON parsing is performed by Niels Lohmann's library which is licensed under the MIT license.
* Image extraction is performed by libarchive which is licensed under the BSD 2-Clause license.
