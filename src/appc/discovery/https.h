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

#include <fstream>
#include <mutex>
#include <cstdio>
#include <curl/curl.h>

#include "appc/discovery/types.h"
#include "appc/util/status.h"


namespace appc {
namespace discovery {
namespace https {


std::once_flag curl_initialized;


struct WriteHandle {
  const char* filename;
  FILE* fileptr;
};


static size_t writer(void* buffer, size_t size, size_t nmemb, void* stream) {
  WriteHandle* handle = static_cast<WriteHandle*>(stream);
  if (handle && !handle->fileptr) {
    handle->fileptr = fopen(handle->filename, "wb");
  }
  if (!handle->fileptr) return -1;
  return fwrite(buffer, size, nmemb, handle->fileptr);
}


Status get(const URI& remote_uri, const Path& write_filename) {
  std::call_once(curl_initialized, []() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
  });
  // curl_global_cleanup(), when?

  std::unique_ptr<CURL, decltype(&curl_easy_cleanup)> curl{curl_easy_init(), curl_easy_cleanup};

  if (!curl) return Error("Could not initialize curl.");

  // FIXME Temporary
  curl_easy_setopt(curl.get(), CURLOPT_VERBOSE, 1L);
  curl_easy_setopt(curl.get(), CURLOPT_URL, remote_uri.c_str());
  curl_easy_setopt(curl.get(), CURLOPT_FAILONERROR, 1L);

  char error_buffer[CURL_ERROR_SIZE];
  curl_easy_setopt(curl.get(), CURLOPT_ERRORBUFFER, &error_buffer);

// FIXME plumb this through, cleanup.
#ifndef __APPLE__
  const char* pinned_pub_key = getenv("APPC_PINNED_KEY");
  if (pinned_pub_key != NULL) {
    std::cerr << "Pinning certificate to " << pinned_pub_key << std::endl;
    if (curl_easy_setopt(curl.get(), CURLOPT_PINNEDPUBLICKEY, pinned_pub_key) != CURLE_OK) {
      return Error("Could not pin certificate to APPC_PINNED_KEY");
    }
    curl_easy_setopt(curl.get(), CURLOPT_SSL_VERIFYPEER, 0L);
  }
#endif

  WriteHandle handle{write_filename.c_str(), NULL};

  curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, writer);
  curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &handle);

  CURLcode result = curl_easy_perform(curl.get());

  if (handle.fileptr) fclose(handle.fileptr);

  if(result != CURLE_OK) return Error(error_buffer);

  return Success();
}


} // namespace https
} // namespace discovery
} // namespace simple
