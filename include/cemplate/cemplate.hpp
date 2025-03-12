#pragma once

#include <algorithm>
#include <cstdint>
#include <functional>
#include <iostream>
#include <stack>
#include <string>
#include <unordered_set>
#include <variant>

namespace cemplate
{

template<typename T>
using transform_f = std::function<std::string(const T&)>;

struct to_string
{
  template<typename T>
  std::string operator()(const T& val)
  {
    return static_cast<std::string>(val);
  }
};

template<class InputIt, class UnaryFunc = to_string>
std::string join(InputIt first,
                 InputIt last,
                 const std::string& delim,
                 UnaryFunc func = to_string())
{
  std::string res;

  if (first == last) {
    return res;
  }

  res += func(*first);
  std::for_each(
      ++first, last, [&](const auto& val) { res += delim + func(val); });

  return res;
}

template<typename T, class UnaryFunc = to_string>
std::string join(std::initializer_list<T> list,
                 const std::string& delim,
                 UnaryFunc func = to_string())
{
  return join(std::begin(list), std::end(list), delim, func);
}

struct param_t
{
  param_t(const std::string& type, const std::string& name)
      : m_value(type + " " + name)
  {
  }

  operator std::string() const { return m_value; }  // NOLINT

  const auto& value() const { return m_value; }

private:
  std::string m_value;
};

class InitlistElem;

class InitlistNode
{
public:
  InitlistNode(std::initializer_list<InitlistElem> list);

  template<typename InputItr, typename UnaryFunc>
  InitlistNode(InputItr first, InputItr last, UnaryFunc func);

  std::string format(uint64_t lvl) const;

  operator std::string() const;  // NOLINT

private:
  std::vector<InitlistElem> m_values;  // NOLINT
};

class InitlistElem
{
public:
  InitlistElem(std::string value)  // NOLINT
      : m_value(std::move(value))
  {
  }

  InitlistElem(std::string_view value)  // NOLINT
      : m_value(std::in_place_type<std::string>, value)
  {
  }

  InitlistElem(const char* value)  // NOLINT
      : m_value(value)
  {
  }

  InitlistElem(std::initializer_list<InitlistElem> list)  // NOLINT
      : m_value(std::in_place_type<InitlistNode>, list)
  {
  }

  InitlistElem(InitlistNode list)  // NOLINT
      : m_value(std::move(list))
  {
  }

  const auto& value() const { return m_value; }

private:
  std::variant<std::string, InitlistNode> m_value;
};

static auto indent(std::size_t lvl)
{
  return std::string(lvl * 2, ' ');
}

std::string string(const std::string& value);

class Program
{
public:
  explicit Program(std::ostream& ost)
      : m_ost(ost)
  {
  }

  using s_t = const std::string&;
  using l_t = const std::vector<std::string>&;
  using p_t = const std::vector<param_t>&;
  using i_t = const InitlistNode&;

  // NOLINTBEGIN
  Program& line_empty();
  Program& line_value(s_t value);
  Program& value(s_t value);

  Program& string(s_t value);
  Program& pragma(s_t value);
  Program& include(s_t header);
  Program& includeL(s_t header);

  Program& comment(s_t value);
  Program& multilineComment(l_t values);

  Program& call(s_t func, l_t args);

  Program& statement(s_t content);
  Program& ret(s_t value);

  Program& declaration(s_t type, s_t name, s_t value);
  Program& declaration(s_t type, s_t name, i_t value);

  Program& require(s_t value);
  Program& template_decl(l_t params);
  Program& template_def(s_t var, l_t params);

  Program& function_decl(s_t name, s_t ret, l_t params = {});

  Program& function_open(s_t name, s_t ret, l_t params = {});
  Program& function_close(s_t name);

  Program& namespace_open(s_t name);
  Program& namespace_close(s_t name);

  // NOLINTEND

  Program& function_open(s_t name, s_t ret, p_t params)
  {
    return function_open(
        name,
        ret,
        std::vector<std::string>(std::begin(params), std::end(params)));
  }

  Program& function_decl(s_t name, s_t ret, p_t params)
  {
    return function_decl(
        name,
        ret,
        std::vector<std::string>(std::begin(params), std::end(params)));
  }

private:
  std::string indent() const { return ::cemplate::indent(m_indent); }

  std::size_t m_indent = 0;

  // function
  std::string m_function_last;

  // namespace
  std::unordered_set<std::string> m_namespace_seen;
  std::stack<std::string> m_namespace_stack;

  // ost
  std::ostream& m_ost;  // NOLINT
};

inline InitlistNode::InitlistNode(std::initializer_list<InitlistElem> list)
    : m_values(list)
{
}

template<typename InputItr, typename UnaryFunc>
InitlistNode::InitlistNode(InputItr first, InputItr last, UnaryFunc func)
{
  m_values.reserve(last - first);
  std::for_each(
      first, last, [&](const auto& val) { m_values.emplace_back(func(val)); });
}

}  // namespace cemplate
