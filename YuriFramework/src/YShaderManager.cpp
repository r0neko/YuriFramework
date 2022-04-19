#include <YShaderManager.h>

#include <fstream>
#include <iostream>
#include <sstream>

std::string getExtension( YShaderType t ) {
   switch ( t ) {
   default:
   case YShaderType::FRAGMENT:
      return "fs";
   case YShaderType::VERTEX:
      return "vs";
   }
}

YShaderPart getShaderPart( std::string name, YShaderType type ) {
   std::cout << "YShaderManager::getShaderPart - loading shader file: "
             << "../Shaders/sh_" + name + "." + getExtension( type ) << std::endl;

   return YShaderPart::fromFile( name, "../Shaders/sh_" + name + "." + getExtension( type ), type );
}

YShader YShaderManager::Load( const std::string &vertex, const std::string &fragment ) {
   return { vertex + '/' + fragment, { getShaderPart( vertex, YShaderType::VERTEX ), getShaderPart( fragment, YShaderType::FRAGMENT ) } };
}