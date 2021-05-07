#ifndef P3PE_VERSION_INCLUDED
#define P3PE_VERSION_INCLUDED

#define MAKE_STR_HELPER(x) #x
#define MAKE_STR(x) MAKE_STR_HELPER(x)

#define P3PE_VERSION_MAJOR 4
#define P3PE_VERSION_MINOR 3
#define P3PE_VERSION_PATCH 7
#define P3PE_VERSION_BETA 0
#define P3PE_VERSION_VERSTRING   \
	MAKE_STR(P3PE_VERSION_MAJOR) \
	"." MAKE_STR(P3PE_VERSION_MINOR) "." MAKE_STR(P3PE_VERSION_PATCH) "." MAKE_STR(P3PE_VERSION_BETA)
#define P3PE_VERSION_MAJOR_MINOR   \
	MAKE_STR(P3PE_VERSION_MAJOR) "." MAKE_STR(P3PE_VERSION_MINOR)

#endif