#ifndef _UTIL_HPP
#define _UTIL_HPP

extern "C" {
#include <unistd.h>
}


template <class T>
class Buffer {
public:
   size_t count;
   T *buffer;
   operator T*() {
      return buffer;
   }
   size_t size() {
      return sizeof(T) * count;
   }
   void assign(T *buf, size_t len) {
      buffer = buf;
      count = len;
   }

   void alloc(size_t size) {
      buffer = new T[size];
   }

   void zalloc(size_t size) {
      buffer = new T[size];
      memset(buffer, 0, sizeof(T) * size);
   }

   Buffer<T>(): count(0), buffer(NULL) {};
   Buffer<T>(size_t _count):count(_count), buffer(new T[_count]) {}
   ~Buffer() {
      if (buffer) {
         delete [] buffer;
      }
   }
};


#endif
