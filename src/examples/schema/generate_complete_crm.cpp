#include "3rdparty/nlohmann/json.h"

#include "appc/schema/container.h"

#include "appc/util/option.h"


using Json = nlohmann::json;
using namespace appc::schema;


int main(int args, char** argv) {

  const auto complete_manifest = ContainerRuntimeManifest(
    AcVersion("0.2.0"),
    AcKind("ContainerRuntimeManifest"),
    UUID("0F426158-97EE-49F8-B4A3-792ECDA926FB"),
    AppRefs({
      AppRef(
        ImageID("sha512-cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e"),
        Some(AppName("work-worker")),
        Some(Isolators({
          Isolator("networkIO/readBandwidth", "eth0 100M"),
          Isolator("networkIO/writeBandwidth", "eth0 100M")
        })),
        Some(Annotations({
          Annotation("foo", "baz"),
          Annotation("fizz", "buzz")
        })))
    }),
    Some(Volumes({
      Volume(
        VolumeKind("host"),
        MountPointNames({
          MountPointName("homes"),
        }),
        Some(VolumeSource("/home")),
        Some(ReadOnly(false))),
      Volume(
        VolumeKind("empty"),
        MountPointNames({
          MountPointName("data"),
        }))
    })),
    Some(Isolators({
      Isolator("cpu/mask", "0-2"),
      Isolator("memory/limit", "2G")
    })),
    Some(Annotations({
      Annotation("created", "2015-01-21T20:20:20.0"),
      Annotation("documentation", "https://github.com/cdaylward/libappc"),
      Annotation("homepage", "https://github.com/cdaylward/libappc"),
    })));

  const Json complete_json = ContainerRuntimeManifest::to_json(complete_manifest);
  std::cout << complete_json.dump(4) << std::endl;

  return EXIT_SUCCESS;
}

