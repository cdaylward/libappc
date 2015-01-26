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

  const std::string base_path{"/tmp/containers/2A2D327D-D3D1-417B-9E3A-177378CF1315"};

  const auto extracted = image.extract_rootfs_to(base_path);
  if (!extracted) {
    std::cerr << "Failed to write rootfs: " << extracted.message << std::endl;
  }

  std::cerr << "Extracted rootfs to: " << base_path << std::endl;

  return EXIT_SUCCESS;
}
