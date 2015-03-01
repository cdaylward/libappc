# Schema

A strongly-typed representation of the App Container schema. Each entity in the schema has functions
to convert to and from JSON as well as a validate() function that checks the values against the
constraints outlined in the App Container specification. Parsing the JSON schema is exception-free
(JSON library exceptions are trapped). Neither parsing nor validation fail-fast in an attempt to
collate all errors (still immature).

# Specification Compliance

Currently ~ 0.2.0. This is very fuzzy at the moment but this will converge as the specification
matures.

## Top-Level Entities

- Image Manifest: ```appc/schema/image.h```
- Container Runtime Manifest ```appc/schema/container.h```

## TODO

- [ ] spec-compliant validate() for all types
- [ ] breadcrumbs in error messages ("container/volumes: error text")
