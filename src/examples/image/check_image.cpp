#include <string>
#include <fstream>
#include <sstream>
#include <streambuf>

#include "3rdparty/nlohmann/json.h"

#include "appc/image/image.h"


using namespace appc::image;


int main(int args, char** argv) {
  if (args < 2) {
    std::cerr << "Usage: " << argv[0] << " <App Container Image>" << std::endl;
    return EXIT_FAILURE;
  }

  const std::string filename{argv[1]};

  Image image{filename};

  auto valid_structure = image.validate_structure();
  if (!valid_structure) {
    std::cerr << filename << " not a valid ACI: " << valid_structure.message << std::endl;
    return EXIT_FAILURE;
  }

  std::cerr << "ACI is valid." << std::endl;

  return EXIT_SUCCESS;
}
