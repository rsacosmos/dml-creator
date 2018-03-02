#ifndef CONF_EXPORT_H
#define CONF_EXPORT_H

#if defined(_MSC_VER)
#    pragma warning(disable: 4251)
#endif

/* Define the EXPORT variable */
#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
#  if defined( CONF_LIBRARY_STATIC )
#    define CONF_EXPORT
#  elif defined( CONF_LIBRARY )
#    define CONF_EXPORT   __declspec(dllexport)
#  else
#    define CONF_EXPORT   __declspec(dllimport)
#  endif
#else
#  define CONF_EXPORT
#endif

/* Set up define for whether member templates are supported by VisualStudio compilers */
#ifdef _MSC_VER
# if (_MSC_VER >= 1300)
#  define __STL_MEMBER_TEMPLATES
# endif
#endif //_MSC_VER

/* Define NULL pointer value */
#ifndef NULL
#ifdef  __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif //NULL

#endif // CONF_EXPORT_H
