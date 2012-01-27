#include "global.h"

GLint GLmax(GLint a, GLint b)
{
#ifdef DEBUG_FUNC
    static int max_count;
    max_count++;
    printf("Rotate count: %i\n", max_count);
#endif
    return a > b ? a : b;
}
