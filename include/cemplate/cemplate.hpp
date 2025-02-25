#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <variant>

namespace cemplate
{

template<typename T>
using transform_f = std::function<std::string(const T&)>;

template<typename T>
std::string to_string(const T& val)
{
  return val;
}

template<typename T>
std::string join(
    const std::vector<T>& values,
    const std::string& delim,
    const transform_f<std::type_identity_t<T>>& transform = to_string<T>)
{
  std::string res;

  if (!values.empty()) {
    res += transform(values[0]);
    for (std::size_t i = 1; i < values.size(); i++) {
      res += delim + transform(values[i]);
    }
  }

  return res;
}

class InitlistElem;

class Initlist
{
public:
  std::string format(uint64_t lvl) const;

  operator std::string() const;  // NOLINT
  friend std::ostream& operator<<(std::ostream& ost, const Initlist& rhs);

  template<typename... Args>
  void emplace_back(Args... args)
  {
    values.emplace_back(std::forward<Args>(args)...);
  }

  std::vector<InitlistElem> values;  // NOLINT
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
  friend std::ostream& operator<<(std::ostream& ost, const Function& rhs);

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
  friend std::ostream& operator<<(std::ostream& ost, const FunctionD& rhs);
};

class Call
{
public:
  Call(std::string func, std::vector<std::string> args)
      : m_func(std::move(func))
      , m_args(std::move(args))
  {
  }

  Call(std::string func, std::string arg)
      : m_func(std::move(func))
      , m_args({std::move(arg)})
  {
  }

  operator std::string() const;  // NOLINT
  friend std::ostream& operator<<(std::ostream& ost, const Call& rhs);

protected:
  const auto& func() const { return m_func; }
  const auto& args() const { return m_args; }

private:
  std::string m_func;
  std::vector<std::string> m_args;
};

class Statement
{
public:
  explicit Statement(std::string content)
      : m_content(std::move(content))
  {
  }

  operator std::string() const;  // NOLINT
  friend std::ostream& operator<<(std::ostream& ost, const Statement& rhs);

private:
  std::string m_content;
};

class Template
{
public:
  explicit Template(std::vector<std::string> params)
      : m_params(std::move(params))
  {
  }

  explicit Template(std::string param)
      : m_params({std::move(param)})
  {
  }

  operator std::string() const;  // NOLINT
  friend std::ostream& operator<<(std::ostream& ost, const Template& rhs);

private:
  std::vector<std::string> m_params;
};

class TemplateD
{
public:
  TemplateD(std::string var, std::vector<std::string> params)
      : m_var(std::move(var))
      , m_params(std::move(params))
  {
  }

  TemplateD(std::string var, std::string param)
      : m_var(std::move(var))
      , m_params({std::move(param)})
  {
  }

  operator std::string() const;  // NOLINT
  friend std::ostream& operator<<(std::ostream& ost, const TemplateD& rhs);

private:
  std::string m_var;
  std::vector<std::string> m_params;
};

class Requires
{
public:
  explicit Requires(std::string value)
      : m_value(std::move(value))
  {
  }

  operator std::string() const;  // NOLINT
  friend std::ostream& operator<<(std::ostream& ost, const Requires& rhs);

private:
  std::string m_value;
};

class Pragma
{
public:
  explicit Pragma(std::string value)
      : m_value(std::move(value))
  {
  }

  operator std::string() const;  // NOLINT
  friend std::ostream& operator<<(std::ostream& ost, const Pragma& rhs);

private:
  std::string m_value;
};

class Include
{
public:
  explicit Include(std::string header)
      : m_header(std::move(header))
  {
  }

  operator std::string() const;  // NOLINT
  friend std::ostream& operator<<(std::ostream& ost, const Include& rhs);

private:
  std::string m_header;
};

class IncludeL
{
public:
  explicit IncludeL(std::string header)
      : m_header(std::move(header))
  {
  }

  operator std::string() const;  // NOLINT
  friend std::ostream& operator<<(std::ostream& ost, const IncludeL& rhs);

private:
  std::string m_header;
};

class Namespace
{
public:
  explicit Namespace(std::string name)
      : m_name(std::move(name))
  {
  }

  operator std::string() const;  // NOLINT
  friend std::ostream& operator<<(std::ostream& ost, const Namespace& rhs);

private:
  std::string m_name;
};

class Return
{
public:
  explicit Return(std::string value)
      : m_value(std::move(value))
  {
  }

  operator std::string() const;  // NOLINT
  friend std::ostream& operator<<(std::ostream& ost, const Return& rhs);

private:
  std::string m_value;
};

class Declaration
{
public:
  Declaration(std::string type, std::string name, std::string value)
      : m_type(std::move(type))
      , m_name(std::move(name))
      , m_value(std::move(value))
  {
  }

  operator std::string() const;  // NOLINT
  friend std::ostream& operator<<(std::ostream& ost, const Declaration& rhs);

private:
  std::string m_type;
  std::string m_name;
  std::string m_value;
};

class String
{
public:
  explicit String(std::string value)
      : m_value(std::move(value))
  {
  }

  operator std::string() const;  // NOLINT
  friend std::ostream& operator<<(std::ostream& ost, const String& rhs);

private:
  std::string m_value;
};

template<typename T, int key>
std::ostream& operator<<(std::ostream& ost, const T& rhs)
{
  return ost << static_cast<std::string>(rhs);
}

// NOLINTBEGIN
#define DEF_OST(typename) \
  inline std::ostream& operator<<(std::ostream& ost, const typename& rhs) \
  { \
    return ost << static_cast<std::string>(rhs); \
  }
// NOLINTEND

DEF_OST(Initlist)
DEF_OST(Function)
DEF_OST(FunctionD)
DEF_OST(Call)
DEF_OST(Statement)
DEF_OST(Template)
DEF_OST(TemplateD)
DEF_OST(Requires)
DEF_OST(Pragma)
DEF_OST(Include)
DEF_OST(IncludeL)
DEF_OST(Namespace)
DEF_OST(Return)
DEF_OST(Declaration)
DEF_OST(String)

}  // namespace cemplate
