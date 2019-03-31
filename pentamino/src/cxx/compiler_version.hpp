#pragma once


#define REDEMPTION_VERSION_NUMBER(major, minor, patch) \
  ( (((major)%100)*10000000) + (((minor)%100)*100000) + ((patch)%100000) )

#define REDEMPTION_VERSION_NUMBER_NOT_AVAILABLE 0

// Clang

#ifdef __clang__
# define REDEMPTION_COMP_CLANG REDEMPTION_VERSION_NUMBER( \
    __clang_major__,                              \
    __clang_minor__,                              \
    __clang_patchlevel__)
#else
# define REDEMPTION_COMP_CLANG REDEMPTION_VERSION_NUMBER_NOT_AVAILABLE
#endif


// Gcc

#ifdef __GNUC__
# ifdef __GNUC_PATCHLEVEL__
#   define REDEMPTION_COMP_GNUC REDEMPTION_VERSION_NUMBER( \
      __GNUC__,                                    \
      __GNUC_MINOR__,                              \
      __GNUC_PATCHLEVEL__)
# else
#   define REDEMPTION_COMP_GNUC REDEMPTION_VERSION_NUMBER( \
      __GNUC__,                                    \
      __GNUC_MINOR__,                              \
      0)
# endif
#else
# define REDEMPTION_COMP_GNUC REDEMPTION_VERSION_NUMBER_NOT_AVAILABLE
#endif


// MSVC

#if defined(_MSC_VER)
# if !defined (_MSC_FULL_VER)
#   define REDEMPTION_COMP_MSVC_BUILD 0
# else
    /* how many digits does the build number have? */
#   if _MSC_FULL_VER / 10000 == _MSC_VER
      /* four digits */
#     define REDEMPTION_COMP_MSVC_BUILD (_MSC_FULL_VER % 10000)
#   elif _MSC_FULL_VER / 100000 == _MSC_VER
      /* five digits */
#     define REDEMPTION_COMP_MSVC_BUILD (_MSC_FULL_VER % 100000)
#   else
#     error "Cannot determine build number from _MSC_FULL_VER"
#   endif
# endif
  /*
  VS2014 was skipped in the release sequence for MS. Which
  means that the compiler and VS product versions are no longer
  in sync. Hence we need to use different formulas for
  mapping from MSC version to VS product version.
  */
# if (_MSC_VER >= 1900)
#   define REDEMPTION_COMP_MSVC REDEMPTION_VERSION_NUMBER( \
      _MSC_VER/100-5,                              \
      _MSC_VER%100,                                \
      REDEMPTION_COMP_MSVC_BUILD)
# else
#   define REDEMPTION_COMP_MSVC REDEMPTION_VERSION_NUMBER( \
      _MSC_VER/100-6,                              \
      _MSC_VER%100,                                \
      REDEMPTION_COMP_MSVC_BUILD)
# endif
#else
# define REDEMPTION_COMP_MSVC REDEMPTION_VERSION_NUMBER_NOT_AVAILABLE
#endif
