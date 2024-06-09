#include <cstdio>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <vector>

class Item {
public:
  std::string name;
  std::string path;
  virtual std::string displayName() const = 0;
  virtual void select() const = 0;
  Item(const std::string &name, const std::string &path)
      : name(name), path(path) {}
  virtual ~Item() = default;
};

class File : public Item {
public:
  int size;

  std::string displayName() const override { return name; };
  void select() const override{};
  File(const std::string &name, int size) : Item(name, ""), size(size) {}
};

class Directory : public Item {
public:
  std::vector<std::shared_ptr<Item>> children;
  Directory(const std::string &name, const std::string &path)
      : Item(name, path) {}
  void addChild(std::shared_ptr<Item> child) { children.push_back(child); }
  void display(int cursor, bool renaming, std::string rename_text) {
    int i = 0;
    for (const auto &child : children) {
      std::string childName = child->displayName();
      if (i == cursor) {
        std::cout << "\033[7m" +
                         (renaming ? "*" + rename_text : child->displayName()) +
                         "\033[0m"
                  << std::endl;
      } else {
        std::cout << child->displayName() << std::endl;
      }
      i++;
    }
  }
  std::string getName(int cursor) { return children[cursor]->name; }
  std::string displayName() const override { return name + "/"; }

  void rename(int cursor, std::string new_name) {
    std::filesystem::rename(children[cursor]->name, new_name);
    children[cursor]->name = new_name;
  }
  void delete_item(int cursor) {
    std::filesystem::remove(children[cursor]->name);
    children.erase(children.begin() + cursor);
  }
  void select(int cursor) {
    std::filesystem::remove(children[cursor]->name);
    children.erase(children.begin() + cursor);
  }

  void select() const override {}
};

void clear() { printf("\033[H\033[J"); }

std::string back(std::string path) {
  int last_slash = path.find_last_of('/');
  std::string back = path;
  if (last_slash != std::string::npos) {
    back = back.substr(0, last_slash);
  }
  return back;
}

void loadDirectory(std::shared_ptr<Directory> directory, std::string path) {
  directory->addChild(std::make_shared<Directory>("..", back(directory->path)));
  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    if (entry.is_directory()) {
      std::shared_ptr<Directory> dir = std::make_shared<Directory>(
          entry.path().filename().string(), entry.path().string());
      directory->addChild(dir);
    } else {
      std::shared_ptr<File> file = std::make_shared<File>(
          entry.path().filename().string(), entry.file_size());
      directory->addChild(file);
    }
  }
}

int main(int argc, char *argv[]) {
  struct termios orig_termios, new_termios;
  tcgetattr(STDIN_FILENO, &orig_termios);
  new_termios = orig_termios;
  new_termios.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

  char *starting_directory = argv[1];

  int cursor = 0;

  std::filesystem::path current_dir = std::filesystem::current_path();
  std::shared_ptr<Directory> directory = std::make_shared<Directory>(
      current_dir.filename().string(), current_dir.string());

  directory->addChild(std::make_shared<Directory>("..", back(directory->path)));
  for (const auto &entry : std::filesystem::directory_iterator(current_dir)) {
    if (entry.is_directory()) {
      std::shared_ptr<Directory> dir = std::make_shared<Directory>(
          entry.path().filename().string(), entry.path().string());
      directory->addChild(dir);
    } else {
      std::shared_ptr<File> file = std::make_shared<File>(
          entry.path().filename().string(), entry.file_size());
      directory->addChild(file);
    }
  }

  bool renaming = false;
  std::string rename_text;
  std::string previous_name;

  clear();
  directory->display(cursor, renaming, rename_text);

  while (1) {
    char c = getchar();
    if (renaming) {
      if (c == 10) {
        directory->rename(cursor, rename_text);
        renaming = false;
      } else if (c == 27) {
        renaming = false;
      } else if (c == 127) {
        rename_text.pop_back();
      } else {
        rename_text.push_back(c);
      }
    } else {
      if (c == 'q') {
        return 0;
      }
      if (c == 'j' && cursor < directory->children.size() - 1) {
        cursor += 1;
      }
      if (c == 'k' && cursor > 0) {
        cursor -= 1;
      }
      if (c == 'r') {
        renaming = true;
        rename_text = directory->getName(cursor);
        previous_name = directory->getName(cursor);
      }
      if (c == 'd') {
        directory->delete_item(cursor);
      }
      if (c == 'a') {
        directory->delete_item(cursor);
      }
      if (c == 10) {
        if (directory->children[cursor]->path != "") {
          std::cout << current_dir.string() << std::endl;
          directory =
              std::make_shared<Directory>(directory->children[cursor]->name,
                                          directory->children[cursor]->path);
          loadDirectory(directory, directory->path);
        }
        cursor = 0;
      }
    }
    clear();
    directory->display(cursor, renaming, rename_text);
  }

  return 0;
}
