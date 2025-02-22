#pragma once

#include <string>
#include <vector>

namespace cemplate
{

std::string pragma_once();

std::string include(const std::string& header, bool local = false);

std::string nspace(const std::string& name);

std::string nspace_close(const std::string& name);

struct param_t
{
  std::string type;
  std::string name;
};

std::string func(const std::string& ret,
                 const std::string& name,
                 std::vector<param_t> params);

std::string func_close();

}  // namespace cemplate
