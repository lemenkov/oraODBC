#ifndef _TEST_DEFS__H
#define _TEST_DEFS__H
#include <stdlib.h>
#include <assert.h>
#define VERBOSE if(getenv("VERBOSE")!=NULL)printf

#define T_ASSERT( x , y ) if (!x) printf("%s\n",y); assert(x);
#define T_ASSERT2( x , y , z ) if (!x) printf("%s %s\n",y,z); assert(x);
#define T_ASSERT3( x , y , z , a ) if (!x) printf("%s %s %s\n",y,z,a); assert(x);
#define T_ASSERT4( x , y , z , a , b  ) if (!x) printf("%s %s %s %s \n",y,z,a,b); assert(x);
#define T_ASSERT5( x , y , z , a, b, c ) if (!x) printf("%s %s %s %s %s\n",y,z,a, b, c); assert(x);
#define T_ASSERT6( x , y , z , a, b, c ,d) if (!x) printf("%s %s %s %s %s %s\n",y,z,a, b, c,d); assert(x);
#endif
