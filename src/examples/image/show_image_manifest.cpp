#include <string>
#include <fstream>
#include <sstream>
#include <streambuf>

#include "3rdparty/nlohmann/json.h"

#include "appc/image/image.h"


using namespace appc::image;
using Json = nlohmann::json;

int main(int args, char** argv) {
  if (args < 2) {
    std::cerr << "Usage: " << argv[0] << " <App Container Image>" << std::endl;
    return EXIT_FAILURE;
  }

  const std::string filename{argv[1]};

  Image image{filename};

  auto manifest = image.manifest();
  if (!manifest) {
    std::cerr << "Could not get manifest: " << manifest.failure_reason() << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << from_result(manifest) << std::endl;
  // Json json;
  // try {
  //   json = Json::parse(from_result(manifest));
  // } catch (const std::invalid_argument& err) {
  //   std::cerr << err.what() << std::endl;
  //   return EXIT_FAILURE;
  // }
  // std::cout << json.dump(4) << std::endl;

  return EXIT_SUCCESS;
}
