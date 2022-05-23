#ifdef _WIN32
    #ifndef PASOCSLIB_EXPORT_H
    #define PASOCSLIB_EXPORT_H

    #ifdef PASOCSLIB_STATIC_DEFINE
    #  define PASOCSLIB_EXPORT
    #  define PASOCSLIB_NO_EXPORT
    #else
    #  ifndef PASOCSLIB_EXPORT
    #    ifdef pasocslib_EXPORTS
            /* We are building this library */
    #      define PASOCSLIB_EXPORT __declspec(dllexport)
    #    else
            /* We are using this library */
    #      define PASOCSLIB_EXPORT __declspec(dllimport)
    #    endif
    #  endif

    #  ifndef PASOCSLIB_NO_EXPORT
    #    define PASOCSLIB_NO_EXPORT 
    #  endif
    #endif

    #ifndef PASOCSLIB_DEPRECATED
    #  define PASOCSLIB_DEPRECATED __declspec(deprecated)
    #endif

    #ifndef PASOCSLIB_DEPRECATED_EXPORT
    #  define PASOCSLIB_DEPRECATED_EXPORT PASOCSLIB_EXPORT PASOCSLIB_DEPRECATED
    #endif

    #ifndef PASOCSLIB_DEPRECATED_NO_EXPORT
    #  define PASOCSLIB_DEPRECATED_NO_EXPORT PASOCSLIB_NO_EXPORT PASOCSLIB_DEPRECATED
    #endif

    #if 0 /* DEFINE_NO_DEPRECATED */
    #  ifndef PASOCSLIB_NO_DEPRECATED
    #    define PASOCSLIB_NO_DEPRECATED
    #  endif
    #endif

    #endif /* PASOCSLIB_EXPORT_H */

#else
    #define PASOCSLIB_EXPORT
#endif