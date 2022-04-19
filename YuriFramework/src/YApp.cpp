#include <YApp.h>

#include <iostream>

bool YApp::isRightFrame() const {
   return mFrameDelta > 1000.f / mFrameCap;
}

float YApp::fps() const {
   return 1000.f / mFrameDelta;
}

float YApp::delta() const {
   return mFrameDelta;
}

bool YApp::running() const {
   return bRunning;
}

bool YApp::create() {
   if ( bRunning ) {
      setError( YError::APP_RUNNING );
      return false;
   }

   // do this to prevent errors
   mFrameDelta = 1.f / mFrameCap;

   if ( !onCreate() )
      return false;

   bRunning = true;        // now we are running :p
   setWindowSize( mSize ); // reset the window size to prevent any bugs

   return true;
}

void YApp::handleEvents() {
   if ( bDestroy ) {
      std::cout << "YApp::handleEvents(): app destroying was scheduled." << std::endl;
      bRunning = false;
      bDestroy = false;
      return;
   }
}

void YApp::updateTimings() {
   mFrameDelta = mClock.delta();

   if ( isRightFrame() )
      mClock.reset();
}

void YApp::render() {
   if ( !bRunning )
      return;

   updateTimings();

   if ( isRightFrame() ) {
      onUpdate();
      onRender();
      onPostRender();
   }
}

void YApp::setWindowSize( Vector2I s ) {
   onResize( mSize = s );
}

void YApp::setWindowTitle( const std::string &title ) {
   onTitleChange( mTitle = title );
}

void YApp::destroy() {
   if ( bRunning ) {
      std::cout << "YApp::destroy(): scheduled destroying of the app." << std::endl;
      bDestroy = true;
   }
   else
      onDestroy();
}