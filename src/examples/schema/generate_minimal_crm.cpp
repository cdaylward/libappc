#include "3rdparty/nlohmann/json.h"

#include "appc/schema/container.h"

#include "appc/util/option.h"


using Json = nlohmann::json;
using namespace appc::schema;


int main(int args, char** argv) {

  const auto minimal_manifest = ContainerRuntimeManifest(
    AcVersion("0.2.0"),
    AcKind("ContainerRuntimeManifest"),
    UUID("0F426158-97EE-49F8-B4A3-792ECDA926FB"),
    AppRefs({
      AppRef(
        ImageID("sha512-cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e"))
    }));

  const Json minimal_json = ContainerRuntimeManifest::to_json(minimal_manifest);
  std::cout << minimal_json.dump(4) << std::endl;

  return EXIT_SUCCESS;
}

