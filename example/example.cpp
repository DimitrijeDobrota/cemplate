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
  std::cout << func("void", "test", {});
  std::cout << func_close();
  std::cout << func("void", "test", {{"int", "val1"}, {"std::string", "val2"}});
  std::cout << func_close();
  std::cout << nspace_close("cemplate");

  return 0;
}
