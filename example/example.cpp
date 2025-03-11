#include <iostream>

#include "cemplate/cemplate.hpp"

int main()
{
  using namespace std::string_literals;  // NOLINT
  using namespace cemplate;  // NOLINT

  Namespace nspace;
  Function func;

  // clang-format off
  // NOLINTBEGIN
   program(std::cout, {
          Pragma("once"),
          empty(),
          Include("format"),
          Include("iostream"),
          Include("string"),
          empty(),
          nspace.open("cemplate"),

          Comment("some test function"),
          func.open("test", "int"),
              Return("3"),
          func.close("test"),

          Comment("another test function"),
          func.open("test", "void", {{{"int"s, "val1"s}}}),
          func.close("test"),

          MultilineComment({"", "yet, another test function", "this time with multiple params"}),
          func.open( "test", "void", {{"int"s, "val1"s}, {"std::string"s, "val2"s}}),
          func.close("test"),

          FunctionD("decl", "void"),
          empty(),

          "static const test_class test = ",
          Initlist({"val11", "val12", {"val21", "val22"}, "val13"}),

          nspace.close("cemplate")
  });
  // NOLINTEND
  // clang-format on

  return 0;
}
