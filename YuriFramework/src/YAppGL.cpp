#include <OpenGL/YAppGL.h>
#include <iostream>

bool YAppGL::onCreate() {
   glfwInit();
   glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
   glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
   glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
   // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

   mAppWindow = glfwCreateWindow( mSize.x, mSize.y, "Yuri Framework", NULL, NULL );

   if ( mAppWindow == NULL ) {
      setError( YError::WIN_CREATION_FAIL );
      glfwTerminate();
      return false;
   }

   glfwMakeContextCurrent( mAppWindow );

   if ( !gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress ) ) {
      setError( YError::GFX_LIB_INIT_FAIL );
      return false;
   }

   glfwSetWindowUserPointer( mAppWindow, this );
   glfwSetFramebufferSizeCallback( mAppWindow, []( GLFWwindow *win, int w, int h ) {
      YApp *appPtr = static_cast<YApp *>( glfwGetWindowUserPointer( win ) );
      if ( appPtr )
         appPtr->setWindowSize( { w, h } );
   } );

   // make vbo
   glGenBuffers( 1, &mVBO );
   glBindBuffer( GL_ARRAY_BUFFER, mVBO );

   return onGLInit();
}

void YAppGL::onUpdate() {
   if ( mAppWindow != NULL && glfwWindowShouldClose( mAppWindow ) )
      destroy();
}

void YAppGL::onPostRender() {
   glfwSwapBuffers( mAppWindow );
   glfwPollEvents();
}

void YAppGL::onResize( Vector2I r ) {
   std::cout << "YAppGL::onResize: " << std::to_string( r.x ) << "x" << std::to_string( r.y ) << std::endl;
   glViewport( 0, 0, r.x, r.y );
}

void YAppGL::onTitleChange( std::string &title ) {
   std::cout << "YAppGL::onTitleChange: " << title << std::endl;
   glfwSetWindowTitle( mAppWindow, title.c_str() );
}

void YAppGL::onDestroy() {
   glfwTerminate();
}