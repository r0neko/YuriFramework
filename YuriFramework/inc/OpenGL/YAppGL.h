#pragma once
#include <YApp.h>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

// A YuriFramework app window with OpenGL support and a core frame limiter.
class YAppGL : public YApp {
 public:
   YAppGL(){};
   ~YAppGL(){};

 protected:
   bool onCreate();
   void onUpdate();
   void onPostRender();
   void onResize( Vector2I r );
   void onTitleChange( std::string &title );
   void onDestroy();

   // events
   virtual bool onGLInit() = 0;

 private:
   GLFWwindow * mAppWindow;
   unsigned int mVBO;
};