#pragma once
template <typename T> class Vector2 {
 public:
   Vector2() {}
   Vector2( T x, T y ) : x( x ), y( y ) {}

   T x;
   T y;
};

typedef Vector2<int> Vector2I;