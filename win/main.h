
#ifdef __APPLE__
	#define POTION_APPLE
	#include "TargetConditionals.h"
	#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
		#include <OpenGLES/ES2/gl.h>
		#include <OpenGLES/ES2/glext.h>
		#define OPENGL_ES
		#define POTION_IOS
	#elif TARGET_OS_MAC
		#define POTION_MAC
		#include <OpenGL/OpenGL.h>
		#include <OpenGL/glu.h>
	#endif
#elif defined(_WIN32)
	#define POTION_WIN32
	#include "glew.h"
	#include "wglew.h"
#elif defined(__linux)
// include whatever linux uses
#endif

#include <map>
#include <set>
#include <list>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include <iostream>

#include <glm/ext.hpp>

#include <boost/any.hpp>
#include <boost/bind.hpp>
#include <boost/variant.hpp>
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/unordered_map.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/format.hpp>

#define BOOST_FILESYSTEM_VERSION 3
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

typedef unsigned int uint;
typedef unsigned char ubyte;
typedef unsigned short ushort;