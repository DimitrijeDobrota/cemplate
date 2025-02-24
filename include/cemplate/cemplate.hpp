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
std::string accumulate(
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

struct initlist_elem;

struct initlist
{
  std::string format(std::uint64_t lvl) const;
  friend std::ostream& operator<<(std::ostream& ost, const initlist& rhs);

  template<typename... Args>
  void emplace_back(Args... args)
  {
    values.emplace_back(std::forward<Args>(args)...);
  }

  std::vector<initlist_elem> values;  // NOLINT
};

struct initlist_elem
{
  initlist_elem(std::string value)  // NOLINT
      : m_value(std::move(value))
  {
  }

  initlist_elem(std::string_view value)  // NOLINT
      : m_value(std::in_place_type<std::string>, value)
  {
  }

  initlist_elem(const char* value)  // NOLINT
      : m_value(value)
  {
  }

  initlist_elem(std::initializer_list<initlist_elem> list)  // NOLINT
      : m_value(std::in_place_type<initlist>, list)
  {
  }

  initlist_elem(initlist list)  // NOLINT
      : m_value(std::move(list))
  {
  }

  const auto& value() const { return m_value; }

private:
  std::variant<std::string, initlist> m_value;
};

class func
{
public:
  class param_t
  {
  public:
    param_t(std::string type, std::string name)
        : m_value(std::move(type) + " " + std::move(name))
    {
    }

    explicit operator std::string() const { return m_value; }

    const auto& value() const { return m_value; }

  private:
    std::string m_value;
  };

  explicit func(std::string name)
      : m_name(std::move(name))
  {
  }

  func(std::string name, std::string ret, const std::vector<param_t>& params)
      : m_name(std::move(name))
      , m_ret(std::move(ret))
      , m_params(params.begin(), params.end())
  {
  }

  func(std::string name, std::string ret, std::vector<std::string> params = {})
      : m_name(std::move(name))
      , m_ret(std::move(ret))
      , m_params(std::move(params))
  {
  }

  const auto& name() const { return m_name; }
  const auto& ret() const { return m_ret; }
  const auto& params() const { return m_params; }

  friend std::ostream& operator<<(std::ostream& ost, const func& rhs);

private:
  std::string m_name;
  std::string m_ret;
  std::vector<std::string> m_params;
};

class func_decl : public func
{
public:
  func_decl(std::string name,
            std::string ret,
            const std::vector<param_t>& params)
      : func(std::move(name), std::move(ret), params)
  {
  }

  func_decl(std::string name,
            std::string ret,
            std::vector<std::string> params = {})
      : func(std::move(name), std::move(ret), std::move(params))
  {
  }

  friend std::ostream& operator<<(std::ostream& ost, const func_decl& rhs);
};

class call
{
public:
  call(std::string func, std::string args)
      : m_func(std::move(func))
      , m_args(std::move(args))
  {
  }

  operator std::string() const;  // NOLINT
  friend std::ostream& operator<<(std::ostream& ost, const call& rhs);

protected:
  const auto& func() const { return m_func; }
  const auto& args() const { return m_args; }

private:
  std::string m_func;
  std::string m_args;
};

class call_s : public call
{
public:
  call_s(std::string func, std::string args)
      : call(std::move(func), std::move(args))

  {
  }

  operator std::string() const;  // NOLINT
  friend std::ostream& operator<<(std::ostream& ost, const call_s& rhs);
};

class tmplate
{
public:
  explicit tmplate(std::vector<std::string> params)
      : m_params(std::move(params))
  {
  }

  operator std::string() const;  // NOLINT
  friend std::ostream& operator<<(std::ostream& ost, const tmplate& rhs);

private:
  std::vector<std::string> m_params;
};

class tmplate_spec
{
public:
  explicit tmplate_spec(std::string var, std::string param)
      : m_var(std::move(var))
      , m_param(std::move(param))
  {
  }

  operator std::string() const;  // NOLINT
  friend std::ostream& operator<<(std::ostream& ost, const tmplate_spec& rhs);

private:
  std::string m_var;
  std::string m_param;
};

class rquires
{
public:
  explicit rquires(std::string value)
      : m_value(std::move(value))
  {
  }

  operator std::string() const;  // NOLINT
  friend std::ostream& operator<<(std::ostream& ost, const rquires& rhs);

private:
  std::string m_value;
};

std::string pragma_once();

std::string include(const std::string& header, bool local = false);

std::string nspace(const std::string& name);

std::string ret(const std::string& val);

std::string string(const std::string& string);

std::string decl(const std::string& type, const std::string& name);

}  // namespace cemplate
