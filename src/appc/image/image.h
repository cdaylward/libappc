// Copyright 2015 Charles D. Aylward
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// A (possibly updated) copy of of this software is available at
// https://github.com/cdaylward/libappc

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


static int copy_data(struct archive* in, struct archive* out) {
  const void* buff;
  size_t size;
  off_t offset;

  for (;;) {
    int r = archive_read_data_block(in, &buff, &size, &offset);
    if (r == ARCHIVE_EOF) return ARCHIVE_OK;
    if (r < ARCHIVE_OK) return r;
    r = archive_write_data_block(out, buff, size, offset);
    if (r < ARCHIVE_OK) return r;
  }
}


static Try<std::string> read_data_into_string(struct archive* in) {
  const void* buff;
  size_t size;
  off_t offset;

  std::string result{};
  for (;;) {
    int r = archive_read_data_block(in, &buff, &size, &offset);
    if (r == ARCHIVE_EOF) break;
    if (r < ARCHIVE_OK) return Failure<std::string>(archive_error_string(in));
    result.append(static_cast<const char*>(buff), size);
  }
  return Result(result);
}


class Image {
private:
  static std::string trim_dot_slash(const std::string& path) {
    return path.length() > 2 && path.compare(0, 2, "./") == 0 ? path.substr(2) : path;
  }

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
    {
      struct archive_entry* entry;
      while (archive_read_next_header(archive.get(), &entry) == ARCHIVE_OK) {
        file_list.emplace_back(archive_entry_pathname(entry));
        archive_read_data_skip(archive.get());
      }
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

    unsigned int manifest_count = 0;
    {
      struct archive_entry* entry;
      while (archive_read_next_header(archive.get(), &entry) == ARCHIVE_OK) {
        const std::string raw_path{ archive_entry_pathname(entry) };
        const std::string path = trim_dot_slash(raw_path);
        const mode_t entry_mode = archive_entry_filetype(entry);
        // TODO fixup
        if (path == manifest_filename) {
          manifest_count++;
          if (manifest_count > 1) {
            return Invalid("Multiple manifest dentries present.");
          }
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
    }

    return Valid();
  }

  Try<std::string> manifest() {
    std::unique_ptr<struct archive, decltype(&archive_read_free)> archive{
        archive_read_new(), archive_read_free};
    archive_read_support_filter_all(archive.get());
    archive_read_support_format_all(archive.get());
    int ret_val = archive_read_open_filename(archive.get(), filename.c_str(), 10240);
    if (ret_val != ARCHIVE_OK) {
      return Failure<std::string>(archive_error_string(archive.get()));
    }

    {
      struct archive_entry* entry;
      while (archive_read_next_header(archive.get(), &entry) == ARCHIVE_OK) {
        const std::string raw_path{ archive_entry_pathname(entry) };
        const mode_t entry_mode = archive_entry_filetype(entry);
        std::string path = raw_path.length() > 2 && raw_path.compare(0, 2, "./") == 0 ?
                             raw_path.substr(2) : raw_path;
        if (path == manifest_filename) {
          if (!(entry_mode & AE_IFREG)) {
            return Failure<std::string>("manifest is not a regular file");
          }
          return read_data_into_string(archive.get());
        }
        archive_read_data_skip(archive.get());
      }
    }

    return Failure<std::string>("Archive did not contain a manifest");
  }
};


} // namespace image
} // namespace appc
