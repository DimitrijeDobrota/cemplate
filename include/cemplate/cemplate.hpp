#pragma once

#include <string>

#include "cemplate/cemplate_export.hpp"

class CEMPLATE_EXPORT exported_class
{
public:
  exported_class();

  auto name() const -> char const*;

private:
  CEMPLATE_SUPPRESS_C4251
  std::string m_name;
};
