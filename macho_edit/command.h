#pragma once

#include "macho.h"

#include <memory>

class Command {
public:
  virtual ~Command();

  enum class Result {
    Success,
    Failure,
    IgnorableFailure,
  };

  virtual Result Apply(MachO& macho) = 0;

  static std::unique_ptr<Command> Create(std::string const& name);
};
