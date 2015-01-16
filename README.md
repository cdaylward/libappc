# libappc

## Overview

A C++ library for doing things with [App Containers](https://github.com/appc/spec). The goal of the libary is to be a flexible toolkit: manifest parsing and creation, pluggable discovery, image creation/extraction/caching, thin-provisioned file systems, etc. Configuration and data interchange will almost always be JSON. Some or all pieces may be used in varying implementations.

## Getting Started

Bootstrap it (download and buld dependencies):

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

```
$ ./bin/examples/parse .../image.json
Kind: ImageManifest
Version: 0.1.1
Image Name: example.com/reduce-worker
Labels:
  version -> 1.0.0
  arch -> amd64
  os -> linux
--- 8< ---
```

Additional examples located in src/examples

## Status

Early, early development.

## Contributing

See [CONTRIBUTING.md](https://github.com/cdaylward/libappc/blob/master/CONTRIBUTING.md)

## License

libappc is licensed under the Apache License, Version 2.0.

Included 3rd party libraries/headers (subject to change):

* JSON parsing is performed by Niels Lohmann's library which is licensed under the MIT license.
* Image extraction is performed by libarchive which is licensed under the BSD 2-Clause license.
