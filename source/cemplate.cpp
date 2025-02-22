#include <string>

#include "cemplate/cemplate.hpp"

exported_class::exported_class()
    : m_name {"cemplate"}
{
}

char const* exported_class::name() const
{
  return m_name.c_str();
}
