#pragma once
#include "IYError.h"
#include "Vector2.h"
#include "YClock.h"

#include <string>

// A core YuriFramework app window class, with a core frame limiter.
class YApp : public IYError {
 public:
   YApp(){};
   ~YApp(){};

   bool create();
   void destroy();

   void handleEvents();
   void render();

   void setWindowSize( Vector2I s );
   void setWindowTitle( const std::string &title );

   float fps() const;
   float delta() const;

   bool running() const;

 protected:
   // events
   // called when the app is created
   virtual bool onCreate() = 0;
   // called before rendering a frame
   virtual void onUpdate() = 0;
   // called when rendering a frame
   virtual void onRender() = 0;
   // called after rendering a frame
   virtual void onPostRender() = 0;
   // called when window is resized
   virtual void onResize( Vector2I ) = 0;
   // called when window title is changed
   virtual void onTitleChange( std::string & ) = 0;
   // called when the app is destroyed
   virtual void onDestroy() = 0;

   void updateTimings();
   bool isRightFrame() const;

   Vector2I    mSize{ 800, 600 }; // defaults to 800x600
   std::string mTitle = "Yuri Framework";

 private:
   bool bDestroy = false;
   bool bRunning = false;

   // frame limiter
   float mFrameCap   = 60.f;
   float mFrameDelta = 0.f;

   YClock mClock;
};