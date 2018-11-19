#include <stdio.h> /* needed for vsnprintf */
#include <stdlib.h> /* needed for malloc-free */
#include <stdarg.h> /* needed for va_list */

#ifndef _vscprintf
/* For some reason, MSVC fails to honour this #ifndef. */
/* Hence function renamed to _vscprintf_so(). */
int _vscprintf_so(const char * format, va_list pargs) {
	int retval;
	va_list argcopy;
	va_copy(argcopy, pargs);
	retval = vsnprintf(NULL, 0, format, argcopy);
	va_end(argcopy);
	return retval;
}
#endif // _vscprintf


#ifndef asprintf
int asprintf(char *strp[], const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	int r = asprintf(strp, fmt, ap);
	va_end(ap);
	return r;
}
#endif // asprintf