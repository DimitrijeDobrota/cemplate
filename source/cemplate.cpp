#include <format>
#include <iostream>

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

std::string string(const std::string& value)
{
  return std::format(R"("{}")", value);
}

Program& Program::line_empty()
{
  m_ost << "\n";
  return *this;
}

Program& Program::line_value(s_t value)
{
  m_ost << indent() << value;
  return *this;
}

Program& Program::value(s_t value)
{
  m_ost << value;
  return *this;
}

Program& Program::string(s_t value)
{
  m_ost << ::cemplate::string(value);
  return *this;
}

Program& Program::pragma(s_t value)
{
  m_ost << std::format("#pragma {}\n", value);
  return *this;
}

Program& Program::include(s_t header)
{
  m_ost << std::format("#include <{}>\n", header);
  return *this;
}

Program& Program::includeL(s_t header)
{
  m_ost << std::format("#include \"{}\"\n", header);
  return *this;
}

Program& Program::comment(s_t value)
{
  m_ost << std::format("{}// {}\n", indent(), value);
  return *this;
}

Program& Program::multilineComment(l_t values)
{
  m_ost << std::format(
      "{}/* {}\n*/\n",
      indent(),
      join(std::begin(values), std::end(values), "\n" + indent() + "   "));
  return *this;
}

Program& Program::call(s_t func, l_t args)
{
  m_ost << std::format(
      "{}({})", func, join(std::begin(args), std::end(args), ", "));
  return *this;
}

Program& Program::statement(s_t content)
{
  m_ost << std::format("{}{};\n", indent(), content);
  return *this;
}

Program& Program::ret(s_t value)
{
  m_ost << std::format("{}return {};\n", indent(), value);
  return *this;
}

Program& Program::declaration(s_t type, s_t name, s_t value)
{
  m_ost << std::format("{}{} {} = {};\n", indent(), type, name, value);
  return *this;
}

Program& Program::declaration(s_t type, s_t name, i_t value)
{
  m_ost << std::format("{}{} {} = {{\n{}{}}};\n",
                       indent(),
                       type,
                       name,
                       value.format(m_indent + 1),
                       indent());
  return *this;
}

Program& Program::require(s_t value)
{
  m_ost << std::format(
      "{}requires {}\n", ::cemplate::indent(m_indent + 1), value);
  return *this;
}

Program& Program::template_decl(l_t params)
{
  m_ost << std::format("{}template <{}>\n",
                       indent(),
                       join(std::begin(params), std::end(params), ", "));
  return *this;
}

Program& Program::template_def(s_t var, l_t params)
{
  m_ost << std::format(
      "{}<{}>", var, join(std::begin(params), std::end(params), ", "));
  return *this;
}

Program& Program::function_decl(s_t name, s_t ret, l_t params)
{
  m_ost << std::format("{} {}({});\n",
                       ret,
                       name,
                       join(std::begin(params), std::end(params), ", "));
  return *this;
}

std::string InitlistNode::format(uint64_t lvl) const
{
  const auto eval = []<typename T>(const T& val, std::uint64_t llvl)
  {
    if constexpr (std::is_same_v<T, std::string>) {
      return std::format("{}{},\n", indent(llvl), val);
    } else {
      return std::format(
          "{}{{\n{}{}}},\n", indent(llvl), val.format(llvl + 1), indent(llvl));
    }
  };

  std::string res;

  for (const auto& elem : m_values) {
    std::visit([&](const auto& val) { res += eval(val, lvl + 1); },
               elem.value());
  }

  return res;
}

Program& Program::function_open(s_t name, s_t ret, l_t params)
{
  if (!m_function_last.empty()) {
    warning("opening, but function is not closed", m_function_last);
  }

  m_function_last = name;
  m_indent++;
  m_ost << std::format("{} {}({})\n{{\n",
                       ret,
                       name,
                       join(std::begin(params), std::end(params), ", "));
  return *this;
}

Program& Program::function_close(s_t name)
{
  if (m_function_last != name) {
    warning("closing, but function is not closed", m_function_last);
  }

  m_function_last.clear();
  m_indent--;
  m_ost << "}\n\n";
  return *this;
}

Program& Program::namespace_open(s_t name)
{
  if (m_namespace_seen.contains(name)) {
    warning("nesting namespaces of the same name", name);
  }

  m_namespace_seen.insert(name);
  m_namespace_stack.push(name);

  m_ost << std::format("namespace {}\n{{\n\n", name);
  return *this;
}

Program& Program::namespace_close(s_t name)
{
  if (m_namespace_stack.empty() || m_namespace_stack.top() != name) {
    warning("closing unproper namespace", name);
  }

  m_namespace_seen.erase(name);
  m_namespace_stack.pop();
  m_ost << std::format("\n}} // namespace {}\n\n", name);
  return *this;
}

}  // namespace cemplate
