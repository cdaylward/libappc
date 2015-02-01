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

  const auto file_list = image.file_list();
  if (!file_list) {
    std::cerr << file_list.failure_reason() << std::endl;
    return EXIT_FAILURE;
  }

  for (const auto& filename : *file_list) {
    std::cout << filename << std::endl;
  }

  return EXIT_SUCCESS;
}
