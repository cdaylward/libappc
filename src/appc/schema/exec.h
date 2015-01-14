
#pragma once

#include "appc/schema/common.h"


namespace appc {
namespace schema {


struct ExecArg : StringType<ExecArg> {
  explicit ExecArg(const std::string& arg)
  : StringType<ExecArg>(arg) {}

  Status validate() const {
    // TODO(cdaylward)
    return Valid();
  }
};

struct Exec : ArrayType<Exec, ExecArg> {
  explicit Exec(const std::vector<ExecArg>& args)
  : ArrayType<Exec, ExecArg>(args) {}

  Status validate() const {
    // TODO(cdaylward)
    return Valid();
  }
};


} // namespace schema
} // namespace appc
