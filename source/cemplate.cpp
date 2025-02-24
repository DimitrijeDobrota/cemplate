#include <format>
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

call::operator std::string() const
{
  return std::format("{}({})", func(), args());
}

std::ostream& operator<<(std::ostream& ost, const call& rhs)
{
  return ost << static_cast<std::string>(rhs);
}

call_s::operator std::string() const
{
  return std::format("{}{}({});\n", indent(), func(), args());
}

std::ostream& operator<<(std::ostream& ost, const call_s& rhs)
{
  return ost << static_cast<std::string>(rhs);
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

std::ostream& operator<<(std::ostream& ost, const initlist& rhs)
{
  return ost << std::format(
             "{{\n{}{}}};\n", rhs.format(indent_lvl + 1), indent());
}

std::string func_helper(const std::string& name,  // NOLINT
                        const std::string& ret,
                        const std::vector<std::string>& params)
{
  return std::format("{} {}({})", ret, name, accumulate(params, ", "));
}

std::ostream& operator<<(std::ostream& ost, const func& rhs)
{
  static std::string last;

  if (last.empty()) {
    if (rhs.ret().empty()) {
      warning("function should have a return type", rhs.name());
    }

    last = rhs.name();
    indent_lvl++;
    return ost << func_helper(rhs.name(), rhs.ret(), rhs.params()) + "\n{\n";
  }

  if (last != rhs.name()) {
    warning("function is not closed", last);
  }

  last.clear();
  indent_lvl--;
  return ost << "}\n\n";
}

std::ostream& operator<<(std::ostream& ost, const func_decl& rhs)
{
  return ost << func_helper(rhs.name(), rhs.ret(), rhs.params()) + ";\n";
}

tmplate::operator std::string() const
{
  return std::format("{}template <{}>\n", indent(), accumulate(m_params, ","));
}

std::ostream& operator<<(std::ostream& ost, const tmplate& rhs)
{
  return ost << static_cast<std::string>(rhs);
}

tmplate_spec::operator std::string() const
{
  return std::format("{}<{}>", m_var, m_param);
}

std::ostream& operator<<(std::ostream& ost, const tmplate_spec& rhs)
{
  return ost << static_cast<std::string>(rhs);
}

rquires::operator std::string() const
{
  return std::format("{}requires {}\n", indent(indent_lvl + 1), m_value);
}

std::ostream& operator<<(std::ostream& ost, const rquires& rhs)
{
  return ost << static_cast<std::string>(rhs);
}

}  // namespace cemplate
