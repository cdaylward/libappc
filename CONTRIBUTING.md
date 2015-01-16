# How to Contribute

## What You Need to Know

libappc is licensed under the [Apache License, Version 2.0](https://github.com/cdaylward/libappc/blob/master/LICENSE). By contributing to libappc, you are agreeing to to the [Developer Certificate of Origin 1.1](https://github.com/cdaylward/libappc/blob/master/DCO.txt)

## Getting Started

* Fork the repository on GitHub.
* Run ```./bootstrap.sh && ./test.sh && ./build.sh```
* Experiment wildly.
* File bugs, submit pull requests for patches (many smaller commits are prefered to fix-all-the-things forklift patches).

## Commit Messages

Please use the following format. Messages should be in present tense.

```
<subsystem>: <synopsis of modification>
<BLANK LINE>
<rationale for this change, additional details,
 this may be omitted if change is trivial>
<BLANK LINE>
<list of issues fixed, if any>
```

e.g.

```
examples: Fix exit status of parser when validation fails.

Exit non-zero when validation of manifests fails.

Fixes #22
```
