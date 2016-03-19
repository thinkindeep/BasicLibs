#ifndef __GENERIC_HEADER__
#define __GENERIC_HEADER__

#ifdef __cplusplus    /*  For cpp applications */
#define XXX_CPP_START extern "C" {
#define XXX_CPP_END }
#else
#define XXX_CPP_START
#define XXX_CPP_END
#endif
#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS  /* For c++ applications, defining the macro to use PRId64 format */
#endif
