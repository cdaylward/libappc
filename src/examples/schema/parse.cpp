#include <string>
#include <fstream>
#include <sstream>
#include <streambuf>

#include "3rdparty/nlohmann/json.h"
#include "appc/schema/image.h"
#include "appc/schema/container.h"


using Json = nlohmann::json;
using namespace appc::schema;

static Status dump_CRM(const ContainerRuntimeManifest& container);
static Status dump_IM(const ImageManifest& image);


// This has no real utility outside of illustrating how to parse and access a manifest.
int main(int args, char** argv) {
  if (args < 2) {
    std::cerr << "Usage: " << argv[0] << " <image or container manifest to parse>" << std::endl;
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

  // In practice, you would not need to switch based on the kind as done here, you would know
  // the manifest type a priori.
  const std::string kind = json[std::string{"acKind"}];

  Status valid = [&kind, &json]() {
    if (kind == "ContainerRuntimeManifest") {
      auto manifest_try = ContainerRuntimeManifest::from_json(json);
      return manifest_try ? dump_CRM(from_result(manifest_try)) : Invalid(manifest_try.failure_reason());
    }
    else if (kind == "ImageManifest") {
      auto manifest_try = ImageManifest::from_json(json);
      return manifest_try ? dump_IM(from_result(manifest_try)) : Invalid(manifest_try.failure_reason());
    }
    return Invalid("Unknown manifest kind: " + kind);
  }();

  if (!valid) {
    std::cerr << "Invalid Manifest: " << valid.message << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}


static Status dump_CRM(const ContainerRuntimeManifest& container)
{
  auto valid = container.validate();
  if (!valid) return valid;

  std::cout << "Kind: " << container.ac_kind.value << std::endl;
  std::cout << "Version: " << container.ac_version.value << std::endl;
  std::cout << "UUID: " << container.uuid.value << std::endl;
  std::cout << "Apps:" << std::endl;
  for (auto& app : container.app_refs) {
    std::cout << "    ImageID: " << app.image_id.value << std::endl;
    if (app.app_name) {
      std::cout << "      App: " << app.app_name->value << std::endl;
    }
    std::cout << "      Isolators:" << std::endl;
    if (app.isolators) {
      auto& isolators = from_some(app.isolators);
      for (auto& isolator : isolators) {
        std::cout << "        " << isolator.name << " -> " << isolator.value << std::endl;
      }
    }
    std::cout << "      Annotations:" << std::endl;
    if (app.annotations) {
      auto& annotations = from_some(app.annotations);
      for (auto& annotation : annotations) {
        std::cout << "        " << annotation.name << " -> " << annotation.value << std::endl;
      }
    }
  }
  if (container.volumes) {
    auto volumes = from_some(container.volumes);
    std::cout << "Volumes:" << std::endl;
    for (auto& volume : volumes) {
      std::cout << "  Kind: " << volume.kind.value << std::endl;
      std::cout << "  Fulfills: " << std::endl;
      for (auto& target : volume.fulfills) {
        std::cout << "    " << target.value << std::endl;
      }
    }
  }
  else {
    std::cout << "No Volumes" << std::endl;
  }
  if (container.isolators) {
    auto isolators = from_some(container.isolators);
    std::cout << "Isolators:" << std::endl;
    for (auto& isolator : isolators) {
      std::cout << "  " << isolator.name << " -> " << isolator.value << std::endl;
    }
  }
  else {
    std::cout << "No Isolators" << std::endl;
  }
  if (container.annotations) {
    auto annotations = from_some(container.annotations);
    std::cout << "Annotations:" << std::endl;
    for (auto& annotation : annotations) {
      std::cout << "  " << annotation.name << " -> " << annotation.value << std::endl;
    }
  }
  else {
    std::cout << "No Annotations" << std::endl;
  }

  return Valid();
}


static Status dump_IM(const ImageManifest& image)
{
  auto valid = image.validate();
  if (!valid) return valid;

  std::cout << "Kind: " << image.ac_kind.value << std::endl;
  std::cout << "Version: " << image.ac_version.value << std::endl;
  std::cout << "Name: " << image.name.value << std::endl;
  if (image.labels) {
    auto labels = from_some(image.labels);
    std::cout << "Labels:" << std::endl;
    for (auto& label : labels) {
      std::cout << "  " << label.name << " -> " << label.value << std::endl;
    }
  }
  if (image.app) {
    auto app = from_some(image.app);
    std::cout << "App:" << std::endl;
    std::cout << "  Exec:" << std::endl;
    for (auto& arg : app.exec) {
      std::cout << "    " << arg.value << std::endl;
    }
    std::cout << "  User: " << app.user.value << std::endl;
    std::cout << "  Group: " << app.group.value << std::endl;
    if (app.event_handlers) {
      auto event_handlers = from_some(app.event_handlers);
      std::cout << "  Event Handlers:" << std::endl;
      for (auto& handler : event_handlers) {
        std::cout << "    " << handler.name.value << std::endl;
        for (auto& arg : handler.exec) {
        std::cout << "      " << arg.value << std::endl;
        }
      }
    }
    if (app.ports) {
      auto ports = from_some(app.ports);
      std::cout << "  Ports:" << std::endl;
      for (auto& port : ports) {
        std::cout << "    Name: " << port.name.value << std::endl;
        std::cout << "    Number: " << port.port.value << std::endl;
        std::cout << "    Protocol: " << port.protocol.value << std::endl;
      }
    }
    if (app.isolators) {
      auto isolators = from_some(app.isolators);
      std::cout << "  Isolators:" << std::endl;
      for (auto& isolator : isolators) {
        std::cout << "    " << isolator.name << " -> " << isolator.value << std::endl;
      }
    }
    if (app.mount_points) {
      auto mount_points = from_some(app.mount_points);
      std::cout << "  Mount Points:" << std::endl;
      for (auto& mount : mount_points) {
        std::cout << "    " << mount.name.value << std::endl;
        std::cout << "      Path: " << mount.path.value << std::endl;
        std::cout << "      Read Only: " << mount.read_only << std::endl;
      }
    }
  }
  if (image.dependencies) {
    auto dependencies = from_some(image.dependencies);
    std::cout << "Dependencies:" << std::endl;
    for (auto& dep : dependencies) {
      std::cout << "  " << dep.app_name.value << std::endl;
      if (dep.image_id) {
        auto image_id = from_some(dep.image_id);
        std::cout << "    Image ID: " << image_id.value << std::endl;
      }
      if (dep.labels) {
        auto labels = from_some(dep.labels);
        std::cout << "    Labels:" << std::endl;
        for (auto& label : labels) {
          std::cout << "      " << label.name << " -> " << label.value << std::endl;
        }
      }
    }
  }
  if (image.path_whitelist) {
    auto path_whitelist = from_some(image.path_whitelist);
    std::cout << "Path Whitelist:" << std::endl;
    for (auto& path : path_whitelist) {
      std::cout << "  " << path.value << std::endl;
    }
  }
  if (image.annotations) {
    auto annotations = from_some(image.annotations);
    std::cout << "Annotations:" << std::endl;
    for (auto& annotation : annotations) {
      std::cout << "  " << annotation.name << " -> " << annotation.value << std::endl;
    }
  }

  return Valid();
}
