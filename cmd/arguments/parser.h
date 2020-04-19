#include <string>

namespace cmd {

class CommandLineArgumentInterface {
public:
  CommandLineArgumentInterface(int argc, char **argv);
  std::string GetPathToLoadProjectFrom() { return load_path_; }
  std::string GetPathToSaveProjectTo() { return save_path_; }
  bool ParsingSuccessfull() { return !error_; };
  virtual ~CommandLineArgumentInterface() = default;

private:
  std::string load_path_{};
  std::string save_path_{};
  bool error_{true};
};

} // namespace cmd