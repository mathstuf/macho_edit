#include "command.h"

#include <iostream>
#include <memory>

class MakeFat : public Command {
public:
  ~MakeFat() override = default;

  Result Apply(MachO& macho) override {
    if (macho.is_fat) {
      return Result::IgnorableFailure;
    }

    macho.make_fat();
    return Result::Success;
  }
};

class MakeThin : public Command {
public:
  MakeThin(std::string arch) : archName(std::move(arch)) {}
  ~MakeThin() override = default;

  Result Apply(MachO& macho) override {
    uint32_t idx;
    for (idx = 0; idx < macho.archs.size(); ++idx) {
      if (macho.archs[idx].name() == archName) {
        break;
      }
    }
    if (idx == macho.archs.size()) {
      std::cerr << "Architecture " << archName << " not found in " << macho.path << std::endl;
      return Result::Failure;
    }

    if (!macho.is_fat) {
      return Result::IgnorableFailure;
    }

    macho.make_thin(idx);
    return Result::Success;
  }
private:
  std::string archName;
};

class RemoveArch : public Command {
public:
  RemoveArch(std::string arch) : archName(std::move(arch)) {}
  ~RemoveArch() override = default;

  Result Apply(MachO& macho) override {
    uint32_t idx;
    for (idx = 0; idx < macho.archs.size(); ++idx) {
      if (macho.archs[idx].name() == archName) {
        break;
      }
    }
    if (idx == macho.archs.size()) {
      std::cerr << "Architecture " << archName << " not found in " << macho.path << std::endl;
      return Result::Failure;
    }

    macho.remove_arch(idx);
    return Result::Success;
  }
private:
  std::string archName;
};

// TODO: ExtractArch
// TODO: InsertArch

class ListLoadCommands : public Command {
public:
  ListLoadCommands(std::string arch) : archName(std::move(arch)) {}
  ~ListLoadCommands() override = default;

  Result Apply(MachO& macho) override {
    bool const isAll = archName == "all";
    for (auto const& arch : macho.archs) {
      if (isAll || arch.name() == archName) {
        arch.print_load_commands();
      }
    }

    return Result::Success;
  }
private:
  std::string archName;
};

// TODO: Remove load command
// TODO: Insert load command
// TODO: Move load command

class RemoveCodeSignature : public Command {
public:
  RemoveCodeSignature(std::string arch) : archName(std::move(arch)) {}
  ~RemoveCodeSignature() override = default;

  Result Apply(MachO& macho) override {
    bool const isAll = archName == "all";
    for (uint32_t idx = 0; idx < macho.archs.size(); ++idx) {
      if (isAll || macho.archs[idx].name() == archName) {
        macho.remove_codesignature(idx);
      }
    }

    return Result::Success;
  }
private:
  std::string archName;
};

Command::~Command() = default;

std::unique_ptr<Command> Command::Create(std::string const& name)
{
  if (name == "make-fat") {
    return std::make_unique<MakeFat>();
  }
  static std::string const make_thin = "make-thin-";
  if (name.starts_with(make_thin)) {
    return std::make_unique<MakeThin>(name.substr(make_thin.size()));
  }
  static std::string const remove_arch = "remove-arch-";
  if (name.starts_with(remove_arch)) {
    return std::make_unique<RemoveArch>(name.substr(remove_arch.size()));
  }
  static std::string const list_load_commands_all = "list-load-commands";
  static std::string const list_load_commands = "list-load-commands-";
  if (name.starts_with(list_load_commands_all)) {
    return std::make_unique<ListLoadCommands>("all");
  }
  if (name.starts_with(list_load_commands)) {
    return std::make_unique<ListLoadCommands>(name.substr(list_load_commands.size()));
  }
  static std::string const remove_code_signatures_all = "remove-code-signatures";
  static std::string const remove_code_signatures = "remove-code-signatures-";
  if (name.starts_with(remove_code_signatures_all)) {
    return std::make_unique<RemoveCodeSignature>("all");
  }
  if (name.starts_with(remove_code_signatures)) {
    return std::make_unique<RemoveCodeSignature>(name.substr(remove_code_signatures.size()));
  }

  return std::unique_ptr<Command>();
}
