#pragma once
#include <OpenGL/YAppGL.h>
#include <YShader.h>
#include <YShaderManager.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class YTestApp : public YAppGL {
 public:
   YTestApp(){};
   ~YTestApp(){};

 protected:
   bool onGLInit() {
      glDisable( GL_DEPTH_TEST );
      glDisable( GL_STENCIL_TEST );
      glEnable( GL_BLEND );

      setWindowTitle( "YuriFramework Test Application (OpenGL)" );

      textureShader2D = YShaderManager::Load( "Texture2D", "Texture" );

      if ( !textureShader2D.compile() ) {
         setError( YError::GFX_SHADER_COMPILATION_FAIL );
         return false;
      }

      return true;
   }

   void onRender() {
      glClearColor( 0.f, 0.f, 0.f, 1.f );
      glClear( GL_COLOR_BUFFER_BIT );

      textureShader2D.use();

      textureShader2D.discard();
   }

   void onResize( Vector2<int> r ) {
      YAppGL::onResize( r );
      resetProjMatrix();
   }

 private:
   YShader   textureShader2D;
   glm::mat4 mProjMatrix;

   void resetProjMatrix() {
      // make projection matrix
      mProjMatrix = glm::orthoZO( 0, mSize.x, mSize.y, 0, -1, 1 );
      YShader::setGlobal<glm::mat4>( "g_ProjMatrix", mProjMatrix );
   }
};