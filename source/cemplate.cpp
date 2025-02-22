#include <format>
#include <iostream>
#include <stack>
#include <unordered_set>

#include "cemplate/cemplate.hpp"

namespace
{

void warning(const std::string& message, const std::string& addition)  // NOLINT
{
  std::cerr << "Warning: " << message;
  if (!addition.empty()) {
    std::cerr << " - " + addition;
  }
  std::cerr << '\n' << std::flush;
}

}  // namespace

namespace cemplate
{

static std::uint32_t indent = 0;  // NOLINT

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
  static std::unordered_set<std::string> seen;
  static std::stack<std::string> stk;

  if (stk.empty() || stk.top() != name) {
    if (seen.contains(name)) {
      warning("nesting namespaces of the same name", name);
    }

    seen.insert(name);
    stk.push(name);

    return std::format("namespace {}\n{{\n\n", name);
  }

  seen.erase(name);
  stk.pop();
  return std::format("\n}} // namespace {}\n\n", name);
}

std::string ret(const std::string& val)
{
  return std::format("return {};\n", val);
}

std::string func_helper(const std::string& name,
                        const std::string& ret,
                        const std::vector<param_t>& params)
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

  return res;
}

std::string func(const std::string& name,
                 const std::string& ret,
                 const std::vector<param_t>& params)
{
  static std::string last;

  if (last.empty()) {
    if (ret.empty()) {
      warning("function should have a return type", name);
    }

    last = name;
    return func_helper(name, ret, params) + ") {\n";
  }

  if (last != name) {
    warning("function is not closed", last);
  }

  last.clear();
  return "}\n\n";
}

std::string func_decl(const std::string& name,
                      const std::string& ret,
                      const std::vector<param_t>& params)
{
  return func_helper(name, ret, params) + ");\n";
}

}  // namespace cemplate
