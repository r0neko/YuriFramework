#ifdef TARGET_OPENGL
#include <YShader.h>
#include <iostream>

void ShaderParam<bool>::set( GLuint shaderProgramId ) {
   glUniform1i( glGetUniformLocation( shaderProgramId, mTag.c_str() ), mVal );
}

void ShaderParam<int>::set( GLuint shaderProgramId ) {
   glUniform1i( glGetUniformLocation( shaderProgramId, mTag.c_str() ), mVal );
}

void ShaderParam<float>::set( GLuint shaderProgramId ) {
   glUniform1f( glGetUniformLocation( shaderProgramId, mTag.c_str() ), mVal );
}

void ShaderParam<Vector2<bool>>::set( GLuint shaderProgramId ) {
   glUniform2i( glGetUniformLocation( shaderProgramId, mTag.c_str() ), mVal.x, mVal.y );
}

void ShaderParam<Vector2<int>>::set( GLuint shaderProgramId ) {
   glUniform2i( glGetUniformLocation( shaderProgramId, mTag.c_str() ), mVal.x, mVal.y );
}

void ShaderParam<Vector2<float>>::set( GLuint shaderProgramId ) {
   glUniform2f( glGetUniformLocation( shaderProgramId, mTag.c_str() ), mVal.x, mVal.y );
}

void ShaderParam<glm::mat4>::set( GLuint shaderProgramId ) {
   glUniformMatrix4fv( glGetUniformLocation( shaderProgramId, mTag.c_str() ), 1, GL_FALSE, &mVal[0][0] );
}

void YShader::addProperty( GLchar *name, GLenum type ) {
   switch ( type ) {
   case GL_BOOL:
      mProperties.insert( { { name, std::make_shared<ShaderParam<bool>>( name ) } } );
      break;
   case GL_INT:
      mProperties.insert( { { name, std::make_shared<ShaderParam<int>>( name ) } } );
      break;
   case GL_FLOAT:
      mProperties.insert( { { name, std::make_shared<ShaderParam<float>>( name ) } } );
      break;
   case GL_BOOL_VEC2:
      mProperties.insert( { { name, std::make_shared<ShaderParam<Vector2<bool>>>( name ) } } );
      break;
   case GL_INT_VEC2:
      mProperties.insert( { { name, std::make_shared<ShaderParam<Vector2<int>>>( name ) } } );
      break;
   case GL_FLOAT_VEC2:
      mProperties.insert( { { name, std::make_shared<ShaderParam<Vector2<float>>>( name ) } } );
      break;
   case GL_FLOAT_MAT4:
      mProperties.insert( { { name, std::make_shared<ShaderParam<glm::mat4>>( name ) } } );
      break;
   default:
      break;
   }
}

bool YShader::compile() {
   if ( mId != -1 )
      return false; // why would we recompile something that exists? lol

   if ( mParts.size() == 0 )
      return false; // c'mon what we're gonna link? nothing?

   int ret;
   mId = glCreateProgram();

   for ( YShaderPart p : mParts ) {
      if ( !p.compiled() )
         p.compile();
      glAttachShader( mId, p.id() );
   }

   glBindAttribLocation( mId, 0, "m_Position" );
   glBindAttribLocation( mId, 1, "m_Colour" );
   glBindAttribLocation( mId, 2, "m_TexCoord" );
   glBindAttribLocation( mId, 3, "m_Time" );
   glBindAttribLocation( mId, 4, "m_Direction" );

   std::cout << "YShader::compile - linking shader program..." << std::endl;

   glLinkProgram( mId );

   glGetProgramiv( mId, GL_LINK_STATUS, &ret );
   bLoaded = ret == GL_TRUE;

   if ( !bLoaded ) {
      // some trickery to get full message size;
      int msgLen = -1;
      glGetProgramiv( mId, GL_INFO_LOG_LENGTH, &msgLen );

      if ( msgLen != -1 ) {
         // now malloc a buffer with our error message and get it in there
         char *errBuf = (char *) malloc( msgLen );

         if ( errBuf ) {
            glGetProgramInfoLog( mId, msgLen, NULL, errBuf );

            // print our message
            std::cout << "YShader::compile - shader program linking failed! :(" << std::endl << errBuf << std::endl;

            // free!!!
            free( errBuf );
         }
         else {
            std::cout << "YShader::compile - shader program linking failed! :(" << std::endl;
         }
      }
      else {
         std::cout << "YShader::compile - shader program linking failed! :(" << std::endl;
      }
   }

   for ( YShaderPart p : mParts )
      if ( p.compiled() )
         glDetachShader( mId, p.id() );

   if ( bLoaded ) {
      std::cout << "YShader::compile - Shader Program '" << mName << "' compiled/linked successfully!" << std::endl;

      int cntUniform = 0;
      glGetProgramiv( mId, GL_ACTIVE_UNIFORMS, &cntUniform );

      std::cout << "YShader::compile - active uniforms: " << std::to_string( cntUniform ) << std::endl;

      for ( int i = 0; i < cntUniform; i++ ) {
         GLchar uniformName[255];
         GLenum type;
         GLint  size;

         glGetActiveUniform( mId, i, 255, NULL, &size, &type, uniformName );

         std::cout << "YShader::compile - uniform name: " << uniformName << std::endl;

         addProperty( uniformName, type );
      }
   }

   // allShaders.push_back( { std::make_shared<YShader>( this ) } );

   return bLoaded;
}

YShader::~YShader() {
   if ( mId >= 0 )
      glDeleteProgram( mId );
}

void YShader::use() {
   if ( bUsed || !bLoaded )
      return;

   glUseProgram( mId );

   prevShader = currShader;
   currShader = mId;

   for ( auto prop : mProperties )
      prop.second->set( mId );

   for ( auto prop : globalParams )
      prop.second->set( mId );

   bLoaded = true;
}

void YShader::discard() {
   if ( !bUsed )
      return;

   glUseProgram( prevShader );
   currShader = prevShader;

   bLoaded = false;
}
#endif