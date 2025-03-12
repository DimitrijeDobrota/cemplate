#include <iostream>

#include "cemplate/cemplate.hpp"

int main()
{
  using namespace std::string_literals;  // NOLINT
  using namespace cemplate;  // NOLINT

  Program program(std::cout);

  // clang-format off
  program.pragma("once")
      .line_empty()
      .include("format")
      .include("iostream")
      .include("string")
      .line_empty()
      .namespace_open("cemplate")

      .comment("some test function")
      .function_open("test", "int")
        .ret("3")
      .function_close("test")

      .comment("another test function")
      .function_open("test", "void", {{{"int"s, "val1"s}}})
      .function_close("test")

      .multilineComment({"", "yet, another test function", "this time with multiple params"})
      .function_open("test", "void", {{"int"s, "val1"s}, {"std::string"s, "val2"s}})
      .function_close("test")

      .function_decl("decl", "void")
      .line_empty()

      .declaration("static const test_class", "test", {"val11", "val12", {"val21", "val22"}, "val13"})

      .namespace_close("cemplate");
  // clang-format on

  return 0;
}
