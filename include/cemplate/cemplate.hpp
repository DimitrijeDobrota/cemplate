#pragma once

#include <cstdint>
#include <string>
#include <variant>
#include <vector>

namespace cemplate
{

struct param_t
{
  std::string type;
  std::string name;
};

struct initlist_elem;

struct initlist
{
  std::string format(std::uint64_t lvl) const;
  friend std::ostream& operator<<(std::ostream& ost, const initlist& list);

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

std::string pragma_once();

std::string include(const std::string& header, bool local = false);

std::string nspace(const std::string& name);

std::string ret(const std::string& val);

std::string string(const std::string& string);

std::string decl(const std::string& type, const std::string& name);

std::string func(const std::string& name,
                 const std::string& ret = "",
                 const std::vector<param_t>& params = {});

std::string func_decl(const std::string& name,
                      const std::string& ret = "",
                      const std::vector<param_t>& params = {});

}  // namespace cemplate
