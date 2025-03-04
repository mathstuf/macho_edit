#include "command.h"
#include "macho.h"

#include <memory>
#include <string>
#include <vector>

#include <cstdlib>

int main(int argc, char* argv[]) {
  std::vector<std::unique_ptr<Command>> commands;

  for (int i = 1; i < argc; ++i) {
    std::string const arg = argv[i];
    if (arg == "--") {
      commands.clear();
    } else if (arg.starts_with("--")) {
      auto cmd = Command::Create(arg.substr(2));
      if (!cmd) {
        return EXIT_FAILURE;
      }
      commands.emplace_back(std::move(cmd));
    } else {
      MachO macho(arg.c_str());
      for (auto& cmd : commands) {
        auto res = cmd->Apply(macho);
        if (res == Command::Result::Failure) {
          return EXIT_FAILURE;
        }
      }
    }
  }

  return EXIT_SUCCESS;
}
