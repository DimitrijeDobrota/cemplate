#include <iostream>

#include "cemplate/cemplate.hpp"

int main()
{
  using namespace std::string_literals;  // NOLINT
  using namespace cemplate;  // NOLINT

  std::cout << Pragma("once") << '\n';

  std::cout << Include("format");
  std::cout << Include("iostream");
  std::cout << Include("string");
  std::cout << '\n';

  std::cout << Namespace("cemplate");

  std::cout << Function("test", "int");
  std::cout << Return("3");
  std::cout << Function("test");

  std::cout << Function("test", "void", {{{"int"s, "val1"s}}});
  std::cout << Function("test");

  std::cout << Function(
      "test", "void", {{"int"s, "val1"s}, {"std::string"s, "val2"s}});
  std::cout << Function("test");

  std::cout << FunctionD("decl", "void");
  std::cout << '\n';

  std::cout << "static const test_class test = ";
  std::cout << Initlist({"val11", "val12", {"val21", "val22"}, "val13"});

  std::cout << Namespace("cemplate");

  return 0;
}
