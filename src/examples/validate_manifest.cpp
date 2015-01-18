#include <string>
#include <fstream>
#include <sstream>
#include <streambuf>

#include "3rdparty/nlohmann/json.h"
#include "appc/schema/image.h"
#include "appc/schema/container.h"


using Json = nlohmann::json;

int validate_im(const Json& json);
int validate_crm(const Json& json);


int main(int args, char** argv) {
  if (args < 2) {
    std::cerr << "Usage: " << argv[0] << " <image or container manifest to parse>" << std::endl;
    return EXIT_FAILURE;
  }

  std::ifstream f(argv[1]);
  std::stringstream buffer {};
  buffer << f.rdbuf();
  std::string json_str { buffer.str() };

  const Json manifest = Json::parse(json_str);
  const std::string kind = manifest[std::string{"acKind"}];

  if (kind == "ContainerRuntimeManifest") {
    return validate_crm(manifest);
  }
  else if (kind == "ImageManifest") {
    return validate_im(manifest);
  }
  else {
    std::cerr << "Unknown manifest type: " << kind << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int validate_im(const Json& json)
{
  auto manifest_try = appc::schema::ImageManifest::from_json(json);
  if (!manifest_try) {
    std::cerr << "Could not parse manifest: " << std::endl;
    std::cerr << manifest_try.failure_reason() << std::endl;
    return EXIT_FAILURE;
  }

  auto valid = manifest_try->validate();

  if (!valid) {
    std::cerr << "Image manifest is invalid: " << valid.message << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}


int validate_crm(const Json& json)
{
  auto manifest_try = appc::schema::ContainerRuntimeManifest::from_json(json);
  if (!manifest_try) {
    std::cerr << "Could not parse manifest: " << std::endl;
    std::cerr << manifest_try.failure_reason() << std::endl;
    return EXIT_FAILURE;
  }

  auto valid = manifest_try->validate();

  if (!valid) {
    std::cerr << "Container runtime manifest is invalid: " << valid.message << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

