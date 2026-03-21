#ifndef TOOLKIT_DEBUG_H
#define TOOLKIT_DEBUG_H


/*  TODO:
    Replace asserts with errors. I mean it isn't that hard to have a conditional
    if (err) error(-2);
    if (err) errorf(-2, "you messed up, %s", name);
    if (err) fatal();
    if (err) fatalf("hello world");
    if (err) fail(_exitlabel);
    if (err) failf(_exitlabel, "on noes!");

    Then again, I should keep assertions for when I want to know what check failed
    assert(a != b)

    But overall encourage the use of the errors, and only use asserts when 
    the conditional check actually matters, rather than a simple error
    assert(!err) // don't do this. Just use `if (err) error(err);

*/






//#ifdef __DEBUG__
#include <stdlib.h>
#include <stdio.h>
//#endif
// TODO: copy some of the macros from this to remove it as a header dependancy
#include <assert.h>
// https://github.com/lattera/glibc/blob/master/assert/assert.h

// TODO move debug.h and macros.h to their own library like env or something
#include "macros.h"
//#include "utils.h"


// Caution! not static!
//#define __FILENAME__ filename_from_path(__FILE__)
#define __FILENAME__ __FILE__


#define ANSI_DEFAULT   "\033[0m"
#define ANSI_BLACK     "\033[30m"
#define ANSI_RED       "\033[31m"
#define ANSI_YELLOW    "\033[33m"
#define ANSI_BLUE      "\033[34m"
#define ANSI_GREEN     "\033[32m"

#define ANSI_BG_RED     "\033[41m"

#define ANS_BLACK_REDBG "\033[1;30;41m"

#define STR_RED(__str)    ANSI_RED    __str ANSI_DEFAULT
#define STR_YELLOW(__str) ANSI_YELLOW __str ANSI_DEFAULT
#define STR_BLUE(__str)   ANSI_BLUE   __str ANSI_DEFAULT
#define STR_GREEN(__str)  ANSI_GREEN  __str ANSI_DEFAULT

//#define STR_FATAL   ANSI_BLACK ANSI_REDBG "[fatal]"   ANSI_DEFAULT
#define STR_FATAL   ANS_BLACK_REDBG "[fatal]"   ANSI_DEFAULT
#define STR_ERROR   ANSI_RED        "[error]"   ANSI_DEFAULT
#define STR_WARN    ANSI_YELLOW     "[warn] "   ANSI_DEFAULT
#define STR_PASS    ANSI_GREEN      "[pass] "   ANSI_DEFAULT
#define STR_DEBUG                   "[debug]"
#define STR_MSG                     "[msg]"


#define SYSTEM_MSG(__status, __format, ...) do {                                \
        fprintf(stderr, __status "\t" STR_BLUE("%s") ":" STR_BLUE("%s") ": "    \
            __format "\n",                                                      \
            __FILENAME__,  STRINGIFY(__LINE__) __VA_OPT__(,) __VA_ARGS__);      \
    } while(0)


#define SYSTEM_ERR(__status, __format, ...) do {                        \
        fprintf(stderr, __status "\t" STR_BLUE(__FILENAME__) ":"        \
            STR_BLUE(STRINGIFY(__LINE__)) ": "                          \
            STR_BLUE("%s(") "..." STR_BLUE(")") ": " __format "\n",     \
            __func__ __VA_OPT__(,) __VA_ARGS__);                        \
    } while(0)


#define IGNORE_UNUSED(__expr)                               \
    _Pragma("GCC diagnostic push")                          \
    _Pragma("GCC diagnostic ignored \"-Wunused-value\"")    \
    __expr                                                  \
    _Pragma("GCC diagnostic pop")



// TODO: Implement later
const char* filename_from_path(const char* path);





#ifdef __DEBUG__

//  NOTE: doesn't work if statement contains commas
#   define DEBUG(__statement) __statement

#   define debug(__statement) do {      \
            __statement                 \
        } while(0)

#else

#   define DEBUG(__statement)
#   define debug(__statement) NOP()

#endif





#if defined(__DEBUG__) || defined(__ENABLE_SYSMESSAGES__)


    //extern char *assert_format;


#   define fatal(__ecode) do { IGNORE_UNUSED(                                       \
        SYSTEM_ERR(STR_FATAL, "\"fatal(%s)\" returned '%d'", #__ecode, __ecode);    \
        fflush(stdout);                                                             \
        exit(__ecode);                                                              \
    )} while(0)



    // TODO: add some way to implement errorf
#   define error(__ecode) do { IGNORE_UNUSED(                                       \
        SYSTEM_ERR(STR_ERROR, "\"error(%s)\" returned '%d'", #__ecode, __ecode);    \
        return __ecode;                                                             \
    )} while(0)



    // "fatal assertion '%s' failed."
#   define fassert(__expr) do { IGNORE_UNUSED(                                  \
        if (!(__expr)) {                                                        \
            SYSTEM_ERR(STR_FATAL, "fatal assertion '%s'", #__expr);             \
            fflush(stdout);                                                     \
            abort();                                                            \
        }                                                                       \
    )} while(0)

    // TODO: use __VA_OPT__ to merge assert and vassert
    // TODO: I should probably change these from macros to functions at this point
    // "assertion '%s' failed. return code %d."
    // TODO: change assert to be this: assert(__expr, __rcode, __formatopt, ...)
#   undef  assert
#   define assert(__expr, __rcode) do { IGNORE_UNUSED(                          \
        if (!(__expr)) {                                                        \
            SYSTEM_ERR(STR_ERROR, "assertion '%s'", #__expr);                   \
            return (__rcode);                                                   \
        }                                                                       \
    )} while(0)

    // "assertion '%s' failed."
#   define vassert(__expr) do { IGNORE_UNUSED(                                  \
        if (!(__expr)) {                                                        \
            SYSTEM_ERR(STR_ERROR, "assertion '%s'", #__expr);                   \
            return;                                                             \
        }                                                                       \
    )} while(0)

    // "warning assertion '%s' failed."
#   define wassert(__expr) do { IGNORE_UNUSED(                                  \
        if (!(__expr)) {                                                        \
            SYSTEM_ERR(STR_WARN, "warning assertion '%s'", #__expr);            \
        }                                                                       \
    )} while(0)


#   define jassert(__expr, __label) do { IGNORE_UNUSED(                         \
        if (!(__expr)) {                                                        \
            SYSTEM_ERR(STR_ERROR, "assertion '%s'", #__expr);                   \
            goto __label;                                                       \
        }                                                                       \
    )} while(0)

/*#   define fatal(__msg) do {            \
        printf(STR_FATAL "\t" __msg);   \
        abort();                        \
    } while(0)

#   define error(__msg, ...) _error(__msg, __VA_ARGS__, ((void)0))
#   define _error(__msg, __rcode, ...) do {     \
        printf(STR_ERROR "\t" __msg);           \
        return __rcode;                         \
    } while(0)*/

//#   define warn(__msg) warnf(__msg)

//#   define ferror(...) do {} while(0)




/*#   define debugf(__format, ...) do {                               \
        printf(STR_DEBUG "\t%s:%s: " __format "\n", __FILENAME__,   \
            STRINGIFY(__LINE_C__) __VA_OPT__(,) __VA_ARGS__);         \
    } while(0)*/


#   ifdef __DEBUG__

#       define alertf(__type, __format, ...)                                \
            SYSTEM_ERR(__type, __format __VA_OPT__(,) __VA_ARGS__);

#   else

#       define alertf(__type, __format, ...)                                \
            SYSTEM_MSG(__type, __format __VA_OPT__(,) __VA_ARGS__);

#   endif

#       define noticef(__type, __format, ...)                               \
            SYSTEM_MSG(__type, __format __VA_OPT__(,) __VA_ARGS__);


/*
    // WARNING: Requires static literal for __format
    // TODO: make it so that this accepts runtime format strings
#   define warnf(__format, ...) do {                                \
        SYSTEM_MSG(STR_WARN, __format __VA_OPT__(,) __VA_ARGS__);   \
    } while(0)


    // WARNING: Requires static literal for __format
#   define debugf(__format, ...)                                    \
        SYSTEM_MSG(STR_DEBUG, __format __VA_OPT__(,) __VA_ARGS__);


    // WARNING: Requires static literal for __format
#define msgf(__format, ...)                                         \
        SYSTEM_MSG(STR_MSG, __format __VA_OPT__(,) __VA_ARGS__);
*/


#   define warnf(__format, ...)  alertf(STR_WARN,  __format, __VA_ARGS__)
#   define debugf(__format, ...) alertf(STR_DEBUG, __format, __VA_ARGS__)

#   define msgf(__format, ...)   noticef(STR_MSG,   __format, __VA_ARGS__)
#   define passf(__format, ...)  noticef(STR_PASS,  __format, __VA_ARGS__)




#else



#   define fatal(__ecode) do { IGNORE_UNUSED(                   \
        exit(__ecode);                                          \
    )} while(0)


#   define error(__rcode) do { IGNORE_UNUSED(                   \
        return (__rcode);                                       \
    )} while(0)


#   define fassert(__expr) do { IGNORE_UNUSED(                  \
        if (!(__expr)) abort();                                 \
    )} while(0)

#   undef  assert
#   define assert(__expr, __rcode) do { IGNORE_UNUSED(          \
        if (!(__expr)) return (__rcode);                        \
    )} while(0)

#   define vassert(__expr) do { IGNORE_UNUSED(                  \
        if (!(__expr)) return;                                  \
    )} while(0)

#   define wassert(__expr)  NOP()

#   define jassert(__expr, __label) do { IGNORE_UNUSED(         \
        if (!(__expr)) goto __label;                            \
    )} while(0)

//#   define error(__msg)     abort()
//#   define warn(__msg)      NOP()
//#   define ffatal(...)      abort()
//#   define ferror(...)      abort()

#   define warnf(...)       NOP()
#   define debugf(...)      NOP()
#   define alertf(...)      NOP()
#   define noticef(...)     NOP()
#   define passf(...)       NOP()



#endif





#endif /* #ifndef HYPERC_DEBUG_H */
