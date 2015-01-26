#pragma once

#include <iostream>
#include <cstdlib>
#include <archive.h>
#include <archive_entry.h>

#include "appc/util/status.h"
#include "appc/util/try.h"


namespace appc {
namespace image {


using FileList = std::vector<std::string>;

const std::string manifest_filename{"manifest"};
const std::string rootfs_filename{"rootfs"};


// Work in progress, experimental, no API here yet.


class Image {
  public:
    const std::string filename;

    explicit Image(const std::string& filename)
    : filename(filename) {}

    Try<FileList> file_list() {
      std::unique_ptr<struct archive, decltype(&archive_read_free)> archive{
          archive_read_new(), archive_read_free};
      // TODO restrict this to ACI spec
      archive_read_support_filter_all(archive.get());
      archive_read_support_format_all(archive.get());
      int ret_val = archive_read_open_filename(archive.get(), filename.c_str(), 10240);
      if (ret_val != ARCHIVE_OK) {
        return Failure<FileList>(archive_error_string(archive.get()));
      }

      FileList file_list{};
      for (struct archive_entry* entry;
           archive_read_next_header(archive.get(), &entry) == ARCHIVE_OK;) {
        file_list.emplace_back(archive_entry_pathname(entry));
        archive_read_data_skip(archive.get());
      }

      return Result(file_list);
    }

    Status validate_structure() {
      std::unique_ptr<struct archive, decltype(&archive_read_free)> archive{
          archive_read_new(), archive_read_free};
      archive_read_support_filter_all(archive.get());
      archive_read_support_format_all(archive.get());
      int ret_val = archive_read_open_filename(archive.get(), filename.c_str(), 10240);
      if (ret_val != ARCHIVE_OK) {
        return Invalid(archive_error_string(archive.get()));
      }

      for (struct archive_entry* entry;
           archive_read_next_header(archive.get(), &entry) == ARCHIVE_OK;) {
        const std::string raw_path{ archive_entry_pathname(entry) };
        const mode_t entry_mode = archive_entry_filetype(entry);
        std::string path;
        if (raw_path.length() > 2 && raw_path.compare(0, 2, "./") == 0) {
          path = raw_path.substr(2);
        }
        else {
          path = raw_path;
        }
        // TODO fixup
        if (path == manifest_filename) {
          if (!(entry_mode & AE_IFREG)) {
            return Invalid("manifest is not a regular file");
          }
        }
        else if (path == rootfs_filename) {
          if (!(entry_mode & AE_IFDIR)) {
            return Invalid("rootfs is not a directory");
          }
        }
        else if (path.length() <= rootfs_filename.length() ||
                 path.compare(0, rootfs_filename.length(), rootfs_filename) != 0) {
          return Invalid(path + " is not under rootfs.");
        }
        // TODO check for foul beasts like ..

        archive_read_data_skip(archive.get());
      }

      return Valid();
    }
};


} // namespace image
} // namespace appc
