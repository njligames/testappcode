
// %module evr

%include "std_string.i"

using namespace std;

%include "_Ignore.swg"

%include "swig.swg"

#if defined(SWIGGO)
#endif

#if defined(SWIGJAVA)
#endif

#if defined(SWIGLUA)
  %import <lua/carrays.i>
  %import <lua/factory.i>
  %import <lua/lua_fnptr.i>
  %import <lua/std_common.i>
  %import <lua/std_deque.i>
  %import <lua/std_except.i>
  %import <lua/std_map.i>
  %import <lua/std_pair.i>
  %import <lua/std_string.i>
  %import <lua/std_vector.i>
  %import <lua/stl.i>
  %import <lua/typemaps.i>
  %import <lua/wchar.i>
#endif

#if defined(SWIGOCTAVE)
  %import <octave/attribute.i>
  %import <octave/boost_shared_ptr.i>
  %import <octave/carrays.i>
  %import <octave/cdata.i>
  %import <octave/cmalloc.i>
  %import <octave/exception.i>
  %import <octave/factory.i>
  %import <octave/implicit.i>
  %import <octave/std_alloc.i>
  %import <octave/std_basic_string.i>
  %import <octave/std_carray.i>
  %import <octave/std_char_traits.i>
  %import <octave/std_common.i>
  %import <octave/std_complex.i>
  %import <octave/std_container.i>
  %import <octave/std_deque.i>
  %import <octave/std_except.i>
  %import <octave/std_list.i>
  %import <octave/std_map.i>
  %import <octave/std_pair.i>
  %import <octave/std_string.i>
  %import <octave/std_vector.i>
  %import <octave/std_wstring.i>
  %import <octave/stl.i>
  %import <octave/typemaps.i>
#endif

#if defined(SWIGPYTHON)
  // %import <python/argcargv.i>
  // %import <python/attribute.i>
  // %import <python/boost_shared_ptr.i>
  // %import <python/carrays.i>
  // %import <python/ccomplex.i>
  // %import <python/cdata.i>
  // %import <python/cmalloc.i>
  // %import <python/cni.i>
  // %import <python/complex.i>
  // %import <python/cpointer.i>
  // %import <python/cstring.i>
  // %import <python/cwstring.i>
  // %import <python/embed.i>
  // %import <python/exception.i>
  // %import <python/factory.i>
  // %import <python/file.i>
  // %import <python/implicit.i>
  // %import <python/jstring.i>
  // %import <python/pyabc.i>
  // %import <python/pybuffer.i>
  // %import <python/pyname_compat.i>
  // %import <python/std_alloc.i>
  // %import <python/std_array.i>
  // %import <python/std_auto_ptr.i>
  // %import <python/std_basic_string.i>
  // %import <python/std_carray.i>
  // %import <python/std_char_traits.i>
  // %import <python/std_common.i>
  // %import <python/std_complex.i>
  // %import <python/std_container.i>
  // %import <python/std_deque.i>
  // %import <python/std_except.i>
  // %import <python/std_ios.i>
  // %import <python/std_iostream.i>
  // %import <python/std_list.i>
  // %import <python/std_map.i>
  // %import <python/std_multimap.i>
  // %import <python/std_multiset.i>
  // %import <python/std_pair.i>
  // %import <python/std_set.i>
  // %import <python/std_shared_ptr.i>
  // %import <python/std_sstream.i>
  // %import <python/std_streambuf.i>
  %import <python/std_string.i>
  // %import <python/std_unordered_map.i>
  // %import <python/std_unordered_multimap.i>
  // %import <python/std_unordered_multiset.i>
  // %import <python/std_unordered_set.i>
  // %import <python/std_vector.i>
  // %import <python/std_vectora.i>
  // %import <python/std_wios.i>
  // %import <python/std_wiostream.i>
  // %import <python/std_wsstream.i>
  // %import <python/std_wstreambuf.i>
  // %import <python/std_wstring.i>
  // %import <python/stl.i>
  // %import <python/typemaps.i>
  // %import <python/wchar.i>
#endif

#if defined(SWIGRUBY)
#endif

// %include "Array.swg"
// %include "Cast.swg"
// %include "Typemaps.swg"
// %include "SwigOverwrite.swg"

// %include "_Apply.swg"

// %include "_Rename.swg"

// %include "_Defines.swg"

#if defined(SWIGGO)
// %include "_GoCpp.swg"
#endif

#if defined(SWIGJAVA)
// %include "_JavaCpp.swg"
#endif

#if defined(SWIGLUA)
// %include "_LuaCpp.swg"
#endif

#if defined(SWIGOCTAVE)
// %include "_OctaveCpp.swg"
#endif

#if defined(SWIGPYTHON)
// %include "_PythonCpp.swg"
#endif

#if defined(SWIGRUBY)
// %include "_RubyCpp.swg"
#endif

%insert("runtime")

%{
  #include "swig.h"
%}

// %include "_Template.swg"

// %include "_Array.swg"

%include "swig.swg"

#if defined(SWIGGO)
// %native(native_function) int native_function(lua_State*L);  // registers native_function() with SWIG
// %{
//   int native_function(lua_State*L) // my native code
//   {
//       printf("native_function code")
//   }
// %}
#endif

#if defined(SWIGJAVA)
// %native(native_function) int native_function(lua_State*L);  // registers native_function() with SWIG
// %{
//   int native_function(lua_State*L) // my native code
//   {
//       printf("native_function code")
//   }
// %}
#endif

#if defined(SWIGLUA)
%native(native_function) int native_function(lua_State*L);  // registers native_function() with SWIG
%{
  int native_function(lua_State*L) // my native code
  {
      printf("native_function code")
  }
%}
#endif

#if defined(SWIGOCTAVE)
// %native(native_function) int native_function(lua_State*L);  // registers native_function() with SWIG
// %{
//   int native_function(lua_State*L) // my native code
//   {
//       printf("native_function code")
//   }
// %}
#endif

#if defined(SWIGPYTHON)
// %native(native_function) int native_function(lua_State*L);  // registers native_function() with SWIG
// %{
//   int native_function(lua_State*L) // my native code
//   {
//       printf("native_function code")
//   }
// %}
#endif

#if defined(SWIGRUBY)
// %native(native_function) int native_function(lua_State*L);  // registers native_function() with SWIG
// %{
//   int native_function(lua_State*L) // my native code
//   {
//       printf("native_function code")
//   }
// %}
#endif

%include "swig.swg"


