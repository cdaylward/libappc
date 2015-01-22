#include <string>
#include <fstream>
#include <sstream>
#include <streambuf>

#include "3rdparty/nlohmann/json.h"

#include "appc/schema/image.h"
#include "appc/schema/container.h"


using Json = nlohmann::json;
using namespace appc::schema;


int main(int args, char** argv) {
  if (args < 2) {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " <image or container runtime manifest to validate>" << std::endl;
    return EXIT_FAILURE;
  }

  std::ifstream f(argv[1]);
  if (!f) {
    std::cerr << "Could not open " << argv[1] << std::endl;
    return EXIT_FAILURE;
  }

  std::stringstream buffer {};
  buffer << f.rdbuf();
  std::string json_str { buffer.str() };

  Json json;
  try {
    // invalid_argument is thrown in JSON is malformed.
    json = Json::parse(json_str);
  } catch (const std::invalid_argument& err) {
    std::cerr << err.what() << std::endl;
    return EXIT_FAILURE;
  }

  const std::string kind = json[std::string{"acKind"}];

  Status valid = [&kind, &json]() {
    if (kind == "ContainerRuntimeManifest") {
      auto manifest = ContainerRuntimeManifest::from_json(json);
      return manifest ? manifest->validate() : Invalid(manifest.failure_reason());
    }
    else if (kind == "ImageManifest") {
      auto manifest = ImageManifest::from_json(json);
      return manifest ? manifest->validate() : Invalid(manifest.failure_reason());
    }
    return Invalid("Unknown manifest kind: " + kind);
  }();

  if (!valid) {
    std::cerr << "Invalid Manifest: " << valid.message << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
