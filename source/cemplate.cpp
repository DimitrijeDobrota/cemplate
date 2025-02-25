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

Pragma::operator std::string() const
{
  return std::format("#pragma {}\n", m_value);
}

Include::operator std::string() const
{
  return std::format("#include <{}>\n", m_header);
}

IncludeL::operator std::string() const
{
  return std::format("#include \"{}\"\n", m_header);
}

Namespace::operator std::string() const
{
  static std::unordered_set<std::string> seen;
  static std::stack<std::string> stk;

  if (stk.empty() || stk.top() != m_name) {
    if (seen.contains(m_name)) {
      warning("nesting namespaces of the same name", m_name);
    }

    seen.insert(m_name);
    stk.push(m_name);

    return std::format("namespace {}\n{{\n\n", m_name);
  }

  seen.erase(m_name);
  stk.pop();
  return std::format("\n}} // namespace {}\n\n", m_name);
}

Return::operator std::string() const
{
  return std::format("{}return {};\n", indent(), m_value);
}

String::operator std::string() const
{
  return std::format(R"("{}")", m_value);
}

Declaration::operator std::string() const
{
  return std::format("{}{} {} = {};\n", indent(), m_type, m_name, m_value);
}

Call::operator std::string() const
{
  return std::format("{}({})", func(), join(args(), ", "));
}

Statement::operator std::string() const
{
  return std::format("{}{};\n", indent(), m_content);
}

std::string Initlist::format(uint64_t lvl) const
{
  const auto eval = []<typename T>(const T& val, std::uint64_t llvl)
  {
    if constexpr (std::is_same_v<T, std::string>) {
      return std::format("{}{},\n", indent(llvl), val);
    } else if (std::is_same_v<T, Initlist>) {
      return std::format(
          "{}{{\n{}{}}},\n", indent(llvl), val.format(llvl + 1), indent(llvl));
    } else {
      return std::string();
    }
  };

  std::string res;

  for (const auto& elem : values) {
    std::visit([&](const auto& val) { res += eval(val, lvl + 1); },
               elem.value());
  }

  return res;
}

Initlist::operator std::string() const
{
  return std::format("{{\n{}{}}};\n", format(indent_lvl + 1), indent());
}

Function::operator std::string() const
{
  static std::string last;

  if (!last.empty()) {
    if (last != name()) {
      warning("function is not closed", last);
    }

    last.clear();
    indent_lvl--;
    return "}\n\n";
  }

  if (ret().empty()) {
    warning("function should have a return type", name());
  }

  last = name();
  indent_lvl++;
  return std::format("{} {}({})\n{{\n", ret(), name(), join(params(), ", "));
}

FunctionD::operator std::string() const
{
  return std::format("{} {}({});\n", ret(), name(), join(params(), ", "));
}

Template::operator std::string() const
{
  return std::format("{}template <{}>\n", indent(), join(m_params, ", "));
}

TemplateD::operator std::string() const
{
  return std::format("{}<{}>", m_var, join(m_params, ", "));
}

Requires::operator std::string() const
{
  return std::format("{}requires {}\n", indent(indent_lvl + 1), m_value);
}

}  // namespace cemplate
