// HTTPS+HTML meta tags to resolve an app name to a downloadable URL. For example,
// if the ACE is looking for example.com/reduce-worker it will request:
//
// https://example.com/reduce-worker?ac-discovery=1
// Then inspect the HTML returned for meta tags that have the following format:
//
// <meta name="ac-discovery" content="prefix-match url-tmpl">
// <meta name="ac-discovery-pubkeys" content="prefix-match url">
// ac-discovery should contain a URL template that can be rendered to retrieve the
// ACI or signature
// ac-discovery-pubkeys should contain a URL that provides a set of public keys
// that can be used to verify the signature of the ACI
// Some examples for different schemes and URLs:
//
// <meta name="ac-discovery" content="example.com https://storage.example.com/{os}/{arch}/{name}-{version}.{ext}?torrent">
// <meta name="ac-discovery" content="example.com hdfs://storage.example.com/{name}-{version}-{os}-{arch}.{ext}">
// <meta name="ac-discovery-pubkeys" content="example.com https://example.com/pubkeys.gpg">
//
// The algorithm first ensures that the prefix of the AC Name matches the
// prefix-match and then if there is a match it will request the equivalent of:
//
// where appname, version, os, and arch are set to their respective values for the
// application, and ext is either aci or sig for retrieving an app container image
// or signature respectively.
//
// In our example above this would be:
//
// sig: https://storage.example.com/linux/amd64/reduce-worker-1.0.0.sig
// aci: https://storage.example.com/linux/amd64/reduce-worker-1.0.0.aci
// keys: https://example.com/pubkeys.gpg
// This mechanism is only used for discovery of contents URLs.
//
// If the first attempt at fetching the discovery URL returns a status code other
// than 200 OK, 3xx, or does not contain any ac-discovery meta tags then the next
// higher path in the name should be tried. For example if the user has
// example.com/project/subproject and we first try example.com/project/subproject
// but don't find a meta tag then try example.com/project then try example.com.
//
// All HTTP redirects should be followed when the discovery URL returns a 3xx status code.

#pragma once


