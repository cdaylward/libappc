#include <string>
#include <fstream>
#include <sstream>
#include <streambuf>

#include "3rdparty/nlohmann/json.h"

#include "appc/image/image.h"


using Json = nlohmann::json;
using namespace appc::image;

// Rank hacking
int main(int args, char** argv) {

  const std::string filename{argv[1]};

  Image image{filename};

  auto valid_structure = image.validate_structure();
  if (!valid_structure) {
    std::cerr << filename << " not a valid ACI: " << valid_structure.message << std::endl;
    return EXIT_FAILURE;
  }

  std::cerr << "ACI is valid." << std::endl;

  auto file_list_try = image.file_list();

  if (!file_list_try) {
    std::cerr << file_list_try.failure_reason() << std::endl;
    return EXIT_FAILURE;
  }

  std::cerr << "File list:" << std::endl;
  for (const auto& filename : *file_list_try) {
    std::cout << filename << std::endl;
  }

  std::cerr << "Manifest:" << std::endl;
  std::cerr << from_result(image.manifest()) << std::endl;

  return EXIT_SUCCESS;
}
