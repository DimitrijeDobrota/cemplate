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

inline void program(std::ostream& ost, std::initializer_list<std::string> args)
{
  std::for_each(
      std::begin(args), std::end(args), [&](const auto& val) { ost << val; });
}

inline const char* empty()
{
  return "\n";
}

// clang-format off
// NOLINTBEGIN
std::string String(const std::string& value);
std::string Pragma(const std::string& value);
std::string Include(const std::string& header);
std::string IncludeL(const std::string& header);

std::string Comment(const std::string& value);
std::string MultilineComment(const std::vector<std::string>& values);
inline std::string MultilineComment(const std::string& value) { return MultilineComment(std::vector({value})); }

std::string Call(const std::string& func, const std::vector<std::string>& args);
inline std::string Call(const std::string& func, const std::string& arg) { return Call(func, std::vector({arg})); }

std::string Statement(const std::string& content);
std::string Return(const std::string& value);

std::string Declaration(const std::string& type, const std::string& name, const std::string& value);

std::string Requires(const std::string& value);
std::string Template(const std::vector<std::string>& params);
std::string TemplateD(const std::string& var, const std::vector<std::string>& params);
inline std::string Template(const std::string& param) { return Template(std::vector({param})); }
inline std::string TemplateD(const std::string& var, const std::string& param) { return TemplateD(var, std::vector({param})); }

struct param_t
{
param_t(std::string type, std::string name)
    : m_value(std::move(type) + " " + std::move(name))
{
}

operator std::string() const { return m_value; }  // NOLINT

const auto& value() const { return m_value; }

private:
std::string m_value;
};

std::string FunctionD(const std::string& name, const std::string& ret, const std::vector<std::string>& params = {});
inline std::string FunctionD(const std::string& name, const std::string& ret, const std::vector<param_t>& params) { return FunctionD(name, ret, std::vector<std::string>(std::begin(params), std::end(params))); }

// NOLINTEND
// clang-format on

class InitlistElem;

class Initlist
{
public:
  Initlist(std::initializer_list<InitlistElem> list);

  template<typename InputItr, typename UnaryFunc>
  Initlist(InputItr first, InputItr last, UnaryFunc func);

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
      : m_value(std::in_place_type<Initlist>, list)
  {
  }

  InitlistElem(Initlist list)  // NOLINT
      : m_value(std::move(list))
  {
  }

  const auto& value() const { return m_value; }

private:
  std::variant<std::string, Initlist> m_value;
};

inline Initlist::Initlist(std::initializer_list<InitlistElem> list)
    : m_values(list)
{
}

template<typename InputItr, typename UnaryFunc>
Initlist::Initlist(InputItr first, InputItr last, UnaryFunc func)
{
  m_values.reserve(last - first);
  std::for_each(
      first, last, [&](const auto& val) { m_values.emplace_back(func(val)); });
}

class Function
{
public:
  std::string open(const std::string& name,
                   const std::string& ret,
                   const std::vector<std::string>& params = {});

  std::string open(const std::string& name,
                   const std::string& ret,
                   const std::vector<param_t>& params)
  {
    return open(name,
                ret,
                std::vector<std::string>(std::begin(params), std::end(params)));
  }

  std::string close(const std::string& name);

private:
  std::string m_last;
};

class Namespace
{
public:
  std::string open(const std::string& name);
  std::string close(const std::string& name);

private:
  std::unordered_set<std::string> m_seen;
  std::stack<std::string> m_stk;
};

}  // namespace cemplate
