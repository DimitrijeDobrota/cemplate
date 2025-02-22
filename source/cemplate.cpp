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
  return std::format("namespace {}\n{{\n\n", name);
}

std::string nspace_close(const std::string& name)
{
  return std::format("\n}} // namespace {}\n\n", name);
}

std::string ret(const std::string& val)
{
  return std::format("return {};\n", val);
}

std::string func(const std::string& ret,
                 const std::string& name,
                 std::vector<param_t> params,
                 bool decl)
{
  static const auto format = [](const param_t& param)
  { return param.name.empty() ? param.type : param.type + ' ' + param.name; };

  std::string res;

  res += ret + ' ';
  res += name + '(';
  if (!params.empty()) {
    res += format(params[0]);
    for (std::size_t i = 1; i < params.size(); i++) {
      res += ", " + format(params[i]);
    }
  }
  res += ')';
  res += decl ? ";\n" : " {\n";

  return res;
}

std::string func_close()
{
  return "}\n\n";
}

}  // namespace cemplate
