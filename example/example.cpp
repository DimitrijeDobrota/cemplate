#include <iostream>

#include "cemplate/cemplate.hpp"

int main()
{
  using namespace cemplate;  // NOLINT

  std::cout << pragma_once();

  std::cout << include("format");
  std::cout << include("iostream");
  std::cout << include("string");
  std::cout << '\n';

  std::cout << nspace("cemplate");

  std::cout << func("test", "int", {});
  std::cout << ret("3");
  std::cout << func("test");

  std::cout << func("test", "void", {{"int", "val1"}, {"std::string", "val2"}});
  std::cout << func("test");

  std::cout << func_decl("decl", "void", {});

  std::cout << nspace("cemplate");

  return 0;
}
