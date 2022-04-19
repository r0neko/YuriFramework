#include <YShaderPart.h>
#include <iostream>

#include <fstream>
#include <iostream>
#include <sstream>

#ifdef TARGET_OPENGL
#include <glad/glad.h>

GLenum mapShaderType( YShaderType type ) {
   switch ( type ) {
   default:
      return GL_NONE;
   case YShaderType::FRAGMENT:
      return GL_FRAGMENT_SHADER;
   case YShaderType::VERTEX:
      return GL_VERTEX_SHADER;
   }
}

bool YShaderPart::compile() {
   int ret;

   if ( mId == -1 ) {
      std::cout << "YShaderPart::compile - no shader found, time to create a new one!" << std::endl;
      mId = glCreateShader( mapShaderType( mType ) );
   }

   const char *shaderCode = mData.c_str();

   std::cout << "YShaderPart::compile - proceeding to compile shader..." << std::endl;
   glShaderSource( mId, 1, &shaderCode, NULL );
   glCompileShader( mId );

   glGetShaderiv( mId, GL_COMPILE_STATUS, &ret );
   bCompiled = ret == GL_TRUE;

   if ( !bCompiled )
      this->~YShaderPart();

   if ( !bCompiled ) {
      // some trickery to get full message size;
      GLint msgLen;
      glGetShaderiv( mId, GL_INFO_LOG_LENGTH, &msgLen );

      if ( msgLen != -1 ) {
         // now malloc a buffer with our error message and get it in there
         char *errBuf = (char *) malloc( msgLen );

         if ( errBuf ) {
            glGetShaderInfoLog( mId, msgLen, NULL, errBuf );

            // print our message
            std::cout << "YShaderPart::compile - shader compilation failed! :(" << std::endl << errBuf << std::endl;

            // free!!!
            free( errBuf );
         }
         else {
            std::cout << "YShaderPart::compile - shader compilation failed! :(" << std::endl;
         }
      }
      else {
         std::cout << "YShaderPart::compile - shader compilation failed! :(" << std::endl;
      }
   }

   if ( bCompiled )
      std::cout << "YShaderPart::compile - shader '" << mName << "' compiled successfully!" << std::endl;

   return bCompiled;
}

YShaderPart ::~YShaderPart() {
   if ( mId >= 0 )
      glDeleteShader( mId );
}

YShaderPart YShaderPart::fromFile( std::string name, std::string path, YShaderType type ) {
   std::ifstream     inFile;
   std::stringstream strStream;

   std::cout << "YShaderPart::fromFile - " << name << ": loading shader file "
             << path << std::endl;

   inFile.open( path );
   strStream << inFile.rdbuf();

   return { name, strStream.str(), type };
}

#endif