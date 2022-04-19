#pragma once
#include <YShader.h>
#include <string>

class YShaderManager {
 public:
   static YShader Load( const std::string &vertex, const std::string &fragment );
};