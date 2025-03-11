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

std::string String(const std::string& value)
{
  return std::format(R"("{}")", value);
}

std::string Pragma(const std::string& value)
{
  return std::format("#pragma {}\n", value);
}

std::string Include(const std::string& header)
{
  return std::format("#include <{}>\n", header);
}

std::string IncludeL(const std::string& header)
{
  return std::format("#include \"{}\"\n", header);
}

std::string Namespace(const std::string& name)
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

std::string Comment(const std::string& value)
{
  return std::format("{}// {}\n", indent(), value);
}

std::string MultilineComment(const std::vector<std::string>& values)
{
  return std::format(
      "{}/* {}\n*/\n",
      indent(),
      join(std::begin(values), std::end(values), "\n" + indent() + "   "));
}

std::string Call(const std::string& func, const std::vector<std::string>& args)
{
  return std::format(
      "{}({})", func, join(std::begin(args), std::end(args), ", "));
}

std::string Statement(const std::string& content)
{
  return std::format("{}{};\n", indent(), content);
}

std::string Return(const std::string& value)
{
  return std::format("{}return {};\n", indent(), value);
}

std::string Declaration(const std::string& type,
                        const std::string& name,
                        const std::string& value)
{
  return std::format("{}{} {} = {};\n", indent(), type, name, value);
}

std::string Requires(const std::string& value)
{
  return std::format("{}requires {}\n", indent(indent_lvl + 1), value);
}

std::string Template(const std::vector<std::string>& params)
{
  return std::format("{}template <{}>\n",
                     indent(),
                     join(std::begin(params), std::end(params), ", "));
}

std::string TemplateD(const std::string& var,
                      const std::vector<std::string>& params)
{
  return std::format(
      "{}<{}>", var, join(std::begin(params), std::end(params), ", "));
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

  for (const auto& elem : m_values) {
    std::visit([&](const auto& val) { res += eval(val, lvl + 1); },
               elem.value());
  }

  return res;
}

Initlist::operator std::string() const
{
  return std::format("{{\n{}{}}}", format(indent_lvl + 1), indent());
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
  return std::format("{} {}({})\n{{\n",
                     ret(),
                     name(),
                     join(std::begin(params()), std::end(params()), ", "));
}

FunctionD::operator std::string() const
{
  return std::format("{} {}({});\n",
                     ret(),
                     name(),
                     join(std::begin(params()), std::end(params()), ", "));
}

}  // namespace cemplate
