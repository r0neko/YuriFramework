#include <YShader.h>
#include <iostream>

#include "YTestApp.h"

#include <Windows.h>

YTestApp app;

int main() {
   std::cout << "Welcome to YuriFramework test app!" << std::endl;

   if ( !app.create() ) {
      //std::cout << "APP CREATION ERROR: " << app.getLastError() << std::endl;
      MessageBox( NULL, ( "The app could not start properly.\n" + app.getLastError() ).c_str(), "YuriFramework", 16 );
      return -1;
   }

   std::cout << "App was created!\n";

   while ( app.running() ) {
      app.handleEvents();
      app.render();
   }

   app.destroy();

   return 0;
}
