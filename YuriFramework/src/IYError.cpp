#include <IYError.h>

std::string IYError::getLastError() const {
   switch ( currentError ) {
   default:
      return "Unknown";
   case YError::NONE:
      return "None";
   case YError::APP_RUNNING:
      return "The current app instance is already running!";
   case YError::WIN_CREATION_FAIL:
      return "The app window could not be created!";
   case YError::GFX_LIB_INIT_FAIL:
      return "The Graphical Library could not be initialized!";
   case YError::GFX_SHADER_COMPILATION_FAIL:
      return "One or more shaders could not be compiled properly.";
   }
}

void IYError::setError( YError error ) {
   currentError = error;
}