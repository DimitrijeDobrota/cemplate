#include <format>

#include "cemplate/cemplate.hpp"

namespace cemplate
{

std::string pragma_once()
{
  return "#pragma once\n\n";
}

std::string include(const std::string& header, bool local)
{
  return local ? std::format("#include \"{}\"\n", header)
               : std::format("#include <{}>\n", header);
}

std::string nspace(const std::string& name)
{
  return std::format("namespace {}\n{{\n\n\n", name);
}

std::string nspace_close(const std::string& name)
{
  return std::format("\n}} // namespace {}\n\n", name);
}

std::string func(const std::string& ret,
                 const std::string& name,
                 std::vector<param_t> params)
{
  std::string res;

  res += ret + ' ';
  res += name + '(';
  if (!params.empty()) {
    res += params[0].type + ' ' + params[0].name;
    for (std::size_t i = 1; i < params.size(); i++) {
      res += ", " + params[i].type + ' ' + params[i].name;
    }
  }
  res += ") {\n";

  return res;
}

std::string func_close()
{
  return "}\n\n";
}

}  // namespace cemplate
