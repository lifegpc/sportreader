#ifndef _LIBSR_PUBLIC_H
#define _LIBSR_PUBLIC_H

#if defined(_WIN32)
#    if defined(__GNUC__)
#        define LIBSR_PUBLIC  __attribute__((dllexport))
#    else
#        define LIBSR_PUBLIC  __declspec(dllexport)
#    endif
#else
#	 define LIBSR_PUBLIC
#endif

#endif
