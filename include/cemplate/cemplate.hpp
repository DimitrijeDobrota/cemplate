#pragma once

#include <algorithm>
#include <cstdint>
#include <functional>
#include <iostream>
#include <string>
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

template<typename... Args>
void program(std::ostream& ost, const Args&... args)
{
  ((ost << to_string()(args)), ...);
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
std::string Namespace(const std::string& name);

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
inline std::string Template(const std::string& param) { return Template(std::vector({param})); }
std::string TemplateD(const std::string& var, const std::vector<std::string>& params);
inline std::string TemplateD(const std::string& var, const std::string& param) { return TemplateD(var, std::vector({param})); }
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

  explicit Function(std::string name)
      : m_name(std::move(name))
  {
  }

  Function(std::string name,
           std::string ret,
           const std::vector<param_t>& params)
      : m_name(std::move(name))
      , m_ret(std::move(ret))
      , m_params(params.begin(), params.end())
  {
  }

  Function(std::string name,
           std::string ret,
           std::vector<std::string> params = {})
      : m_name(std::move(name))
      , m_ret(std::move(ret))
      , m_params(std::move(params))
  {
  }
  const auto& name() const { return m_name; }
  const auto& ret() const { return m_ret; }
  const auto& params() const { return m_params; }

  operator std::string() const;  // NOLINT

private:
  std::string m_name;
  std::string m_ret;
  std::vector<std::string> m_params;
};

class FunctionD : public Function
{
public:
  FunctionD(std::string name,
            std::string ret,
            const std::vector<param_t>& params)
      : Function(std::move(name), std::move(ret), params)
  {
  }

  FunctionD(std::string name,
            std::string ret,
            std::vector<std::string> params = {})
      : Function(std::move(name), std::move(ret), std::move(params))
  {
  }

  operator std::string() const;  // NOLINT
};

}  // namespace cemplate
