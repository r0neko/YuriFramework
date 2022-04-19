#pragma once
#include <Vector2.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <YShaderPart.h>

#ifdef TARGET_OPENGL
#include <glad/glad.h>
#include <glm/glm.hpp>
#endif

class IShaderParam {
 public:
   virtual ~IShaderParam() = default;

#ifdef TARGET_OPENGL
   virtual void set( GLuint shaderProgramId ) = 0;
#else
   virtual void set() = 0;
#endif
};

template <typename T> class ShaderParam : public IShaderParam {
 public:
   ShaderParam( std::string t ) : mTag( t ) {}
   ShaderParam( std::string t, T value ) : mTag( t ), mVal( value ) {}

   void setValue( T newValue ) {
      mVal = newValue;
   }

   T value() const {
      return mVal;
   }

#ifdef TARGET_OPENGL
   void set( GLuint shaderProgramId );
#else
   void         set();
#endif

   std::string tag() const {
      return mTag;
   }

 private:
   std::string mTag;
   T           mVal;
};

class YShader;

static std::unordered_map<std::string, std::shared_ptr<IShaderParam>> globalParams;
static std::vector<std::shared_ptr<YShader>>                          allShaders;

static int prevShader;
static int currShader;

class YShader {
 public:
   YShader( const YShader &s ) : mName( s.mName ), mId( s.mId ), bLoaded( s.bLoaded ), bUsed( s.bUsed ), mParts( s.mParts ), mProperties( s.mProperties ) {}

   YShader() : mName( "" ){};
   YShader( std::string name ) : mName( name ) {}
   YShader( std::string name, std::vector<YShaderPart> parts ) : mName( name ), mParts( parts ){};
   ~YShader();

   void use();
   void discard();
   bool compile();

   bool hasProperty( const std::string key ) {
      return mProperties.find( key ) != mProperties.end();
   }

   template <typename T> void setProperty( const std::string &key, T value ) {
      if ( hasProperty( key ) )
         mProperties.erase( key );
      mProperties.insert( { key, std::make_shared<ShaderParam<T>>( key, value ) } );
   };

   template <typename T> static void setGlobal( const std::string &key, T value ) {
      if ( globalParams.find( key ) != globalParams.end() )
         globalParams.erase( key );
      globalParams.insert( { key, std::make_shared<ShaderParam<T>>( key, value ) } );

      for ( auto s : allShaders )
         s->setProperty( key, value );
   };

 private:
   unsigned int mId = -1;
   std::string  mName;
   bool         bLoaded = false;
   bool         bUsed   = false;

#ifdef TARGET_OPENGL
   void addProperty( GLchar *name, GLenum type );
#endif

   std::vector<YShaderPart>                                       mParts;
   std::unordered_map<std::string, std::shared_ptr<IShaderParam>> mProperties;
};