#include <format>
#include <functional>
#include <iostream>
#include <stack>
#include <unordered_set>

#include "cemplate/cemplate.hpp"

namespace
{

static std::uint64_t indent_lvl = 0;  // NOLINT

auto indent(std::uint64_t lvl = indent_lvl)
{
  return std::string(lvl * 2, ' ');
}

void warning(const std::string& message, const std::string& addition)  // NOLINT
{
  std::cerr << "Warning: " << message;
  if (!addition.empty()) {
    std::cerr << " - " + addition;
  }
  std::cerr << '\n' << std::flush;
}

template<typename T>
std::string accumulate(const std::vector<T>& values,
                       const std::function<std::string(const T&)>& format,
                       const std::string& delim)
{
  std::string res;

  if (!values.empty()) {
    res += format(values[0]);
    for (std::size_t i = 1; i < values.size(); i++) {
      res += delim + format(values[i]);
    }
  }

  return res;
}

}  // namespace

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
  return std::format("{}return {};\n", indent(), val);
}

std::string string(const std::string& string)
{
  return std::format(R"("{}")", string);
}

std::string decl(const std::string& type, const std::string& name)
{
  return std::format("{}{} {} = ", indent(), type, name);
}

std::string eval(const std::string& val, std::uint64_t lvl);  // NOLINT
std::string eval(const initlist& list, std::uint64_t lvl);  // NOLINT

std::string initlist::format(uint64_t lvl) const
{
  std::string res;

  for (const auto& node : values) {
    std::visit([&](const auto& value) { res += eval(value, lvl + 1); },
               node.value());
  }

  return res;
}

std::string eval(const initlist& list, std::uint64_t lvl)
{
  return std::format(
      "{}{{\n{}{}}},\n", indent(lvl), list.format(lvl + 1), indent(lvl));
}

std::string eval(const std::string& val, std::uint64_t lvl)
{
  return std::format("{}{},\n", indent(lvl), val);
}

std::ostream& operator<<(std::ostream& ost, const initlist& list)
{
  return ost << std::format(
             "{{\n{}{}}};\n", list.format(indent_lvl + 1), indent());
}

std::string func_helper(const std::string& name,  // NOLINT
                        const std::string& ret,
                        const std::vector<param_t>& params)
{
  static const auto format = [](const param_t& param)
  { return param.name.empty() ? param.type : param.type + ' ' + param.name; };

  return std::format(
      "{} {}({})", ret, name, accumulate<param_t>(params, format, ", "));
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
    indent_lvl++;
    return func_helper(name, ret, params) + " {\n";
  }

  if (last != name) {
    warning("function is not closed", last);
  }

  last.clear();
  indent_lvl--;
  return "}\n\n";
}

std::string func_decl(const std::string& name,
                      const std::string& ret,
                      const std::vector<param_t>& params)
{
  return func_helper(name, ret, params) + ";\n";
}

}  // namespace cemplate
