#include <string>
#include <fstream>
#include <sstream>
#include <streambuf>

#include "3rdparty/nlohmann/json.h"
#include "appc/schema/image.h"
#include "appc/schema/container.h"


using Json = nlohmann::json;

int dumpAIM(const Json& json);
int dumpCRM(const Json& json);


int main(int args, char** argv) {
  if (args < 2) {
    std::cerr << "Usage: " << argv[0] << " <image or container manifest to parse>" << std::endl;
    return EXIT_FAILURE;
  }

  std::ifstream f(argv[1]);
  std::stringstream buffer {};
  buffer << f.rdbuf();
  std::string json_str { buffer.str() };

  Json manifest = Json::parse(json_str);

  if (manifest["acKind"] == "ContainerRuntimeManifest") {
    return dumpCRM(manifest);
  }
  else if (manifest["acKind"] == "ImageManifest") {
    return dumpAIM(manifest);
  }
  else {
    std::cerr << "Unknown manifest type." << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int dumpAIM(const Json& json)
{
  auto manifest_try = appc::schema::ImageManifest::from_json(json);
  if (!manifest_try) {
    std::cerr << "Could not parse manifest: " << std::endl;
    std::cerr << manifest_try.failure_reason() << std::endl;
    return EXIT_FAILURE;
  }

  appc::schema::ImageManifest manifest = *manifest_try;

  std::cout << "Kind: " << manifest.ac_kind.value << std::endl;
  std::cout << "Version: " << manifest.ac_version.value << std::endl;
  std::cout << "Image Name: " << manifest.image_name.value << std::endl;
  std::cout << "Labels:" << std::endl;
  for (auto& label : manifest.labels.array) {
    std::cout << "  " << label.name << " -> " << label.value << std::endl;
  }
  std::cout << "App:" << std::endl;
    std::cout << "  Exec:" << std::endl;
    for (auto& arg : manifest.app.exec.array) {
      std::cout << "    " << arg.value << std::endl;
    }
    std::cout << "  User: " << manifest.app.user.value << std::endl;
    std::cout << "  Group: " << manifest.app.group.value << std::endl;
    std::cout << "  Event Handlers:" << std::endl;
    for (auto& handler : manifest.app.event_handlers.array) {
      std::cout << "    " << handler.name.value << std::endl;
      for (auto& arg : handler.exec.array) {
      std::cout << "      " << arg.value << std::endl;
      }
    }
    std::cout << "  Isolators:" << std::endl;
    for (auto& isolator : manifest.app.isolators.array) {
      std::cout << "    " << isolator.name << " -> " << isolator.value << std::endl;
    }
    std::cout << "  Mount Points:" << std::endl;
    for (auto& mount : manifest.app.mount_points.array) {
      std::cout << "    " << mount.name << std::endl;
      std::cout << "      Path: " << mount.path << std::endl;
      std::cout << "      Read Only: " << mount.readOnly << std::endl;
    }
  std::cout << "Dependencies:" << std::endl;
  for (auto& dep : manifest.dependencies.array) {
    std::cout << "  " << dep.app_name.value << std::endl;
    std::cout << "    Image ID: " << dep.image_id.value << std::endl;
    std::cout << "    Labels:" << std::endl;
    for (auto& label : dep.labels.array) {
      std::cout << "      " << label.name << " -> " << label.value << std::endl;
    }
  }
  std::cout << "Path Whitelist:" << std::endl;
  for (auto& path : manifest.path_whitelist.array) {
    std::cout << "  " << path.value << std::endl;
  }
  std::cout << "Annotations:" << std::endl;
  for (auto& annotation : manifest.annotations.array) {
    std::cout << "  " << annotation.name << " -> " << annotation.value << std::endl;
  }

  return EXIT_SUCCESS;
}

int dumpCRM(const Json& json)
{
  auto manifest_try = appc::schema::ContainerRuntimeManifest::from_json(json);
  if (!manifest_try) {
    std::cerr << "Could not parse manifest: " << std::endl;
    std::cerr << manifest_try.failure_reason() << std::endl;
    return EXIT_FAILURE;
  }

  appc::schema::ContainerRuntimeManifest manifest = *manifest_try;

  auto valid = manifest.validate();

  if (!valid) {
    std::cerr << "Manifest is invalid: " << valid.message << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "Kind: " << manifest.ac_kind.value << std::endl;
  std::cout << "Version: " << manifest.ac_version.value << std::endl;
  std::cout << "UUID: " << manifest.uuid.value << std::endl;
  std::cout << "Apps:" << std::endl;
  for (auto& app : manifest.app_refs.array) {
    std::cout << "    ImageID: " << app.image_id.value << std::endl;
    if (app.app_name) {
      std::cout << "    App: " << app.app_name->value << std::endl;
    }
    std::cout << "    Isolators:" << std::endl;
    if (app.isolators) {
      auto& isolators = *app.isolators;
      for (auto& isolator : isolators.array) {
        std::cout << "      " << isolator.name << " -> " << isolator.value << std::endl;
      }
    }
    std::cout << "    Annotations:" << std::endl;
    if (app.annotations) {
      auto& annotations = *app.annotations;
      for (auto& annotation : annotations.array) {
        std::cout << "      " << annotation.name << " -> " << annotation.value << std::endl;
      }
    }
  }
  if (manifest.volumes) {
    auto volumes = *manifest.volumes;
    std::cout << "Volumes:" << std::endl;
    for (auto& volume : volumes.array) {
      std::cout << "  Kind: " << volume.kind << std::endl;
      std::cout << "  Source: " << volume.source << std::endl;
      std::cout << "  ReadOnly: " << volume.readOnly << std::endl;
      std::cout << "  Fulfills: " << std::endl;
      for (auto& target : volume.fulfills) {
        std::cout << "    " << target << std::endl;
      }
    }
  }
  else {
    std::cout << "No Volumes" << std::endl;
  }
  if (manifest.isolators) {
    auto isolators = *manifest.isolators;
    std::cout << "Isolators:" << std::endl;
    for (auto& isolator : isolators.array) {
      std::cout << "  " << isolator.name << " -> " << isolator.value << std::endl;
    }
  }
  else {
    std::cout << "No Isolators" << std::endl;
  }
  if (manifest.annotations) {
    auto annotations = *manifest.annotations;
    std::cout << "Annotations:" << std::endl;
    for (auto& annotation : annotations.array) {
      std::cout << "  " << annotation.name << " -> " << annotation.value << std::endl;
    }
  }
  else {
    std::cout << "No Annotations" << std::endl;
  }
  return EXIT_SUCCESS;
}
