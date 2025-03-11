#include <iostream>

#include "cemplate/cemplate.hpp"

int main()
{
  using namespace std::string_literals;  // NOLINT
  using namespace cemplate;  // NOLINT

  // clang-format off
  // NOLINTBEGIN
   program(std::cout, {
          Pragma("once"),
          empty(),
          Include("format"),
          Include("iostream"),
          Include("string"),
          empty(),
          Namespace("cemplate"),

          Comment("some test function"),
          Function("test", "int"),
              Return("3"),
          Function("test"),

          Comment("another test function"),
          Function("test", "void", {{{"int"s, "val1"s}}}),
          Function("test"),

          MultilineComment({"", "yet, another test function", "this time with multiple params"}),
          Function( "test", "void", {{"int"s, "val1"s}, {"std::string"s, "val2"s}}),
          Function("test"),

          FunctionD("decl", "void"),
          empty(),

          "static const test_class test = ",
          Initlist({"val11", "val12", {"val21", "val22"}, "val13"}),

          Namespace("cemplate")
  });
  // NOLINTEND
  // clang-format on

  return 0;
}
