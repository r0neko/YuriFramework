#pragma once
#include <IYError.h>
#include <string>

enum class YShaderType { FRAGMENT, VERTEX };

class YShaderPart {
 public:
   YShaderPart( const std::string &name, std::string data, YShaderType type ) : mName( name ), mData( data ), mType( type ){};
   ~YShaderPart();

   static YShaderPart fromFile( std::string name, std::string path, YShaderType type );

   bool compile();

   bool compiled() const {
      return bCompiled;
   }

   unsigned int id() const {
      return mId;
   }

 private:
   unsigned int mId = -1;

   bool        bCompiled = false;
   std::string mName;
   std::string mData;
   YShaderType mType;
};