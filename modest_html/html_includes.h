#ifndef HTML_INCLUDES_H
#define HTML_INCLUDES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <myhtml/myhtml.h>
#include <modest/finder/finder.h>
#include <myhtml/serialization.h>
#include <mycss/mycss.h>
#include <mycss/selectors/init.h>
#include <mycss/selectors/serialization.h>

#include "vec.h"
// #include "vec_utils.h"

#include "html_workspace.h"

// #define MODEST_HTML_DEBUG
// #define MODEST_HTML_USE_DMT

#ifdef MODEST_HTML_USE_DMT

#include "dmt.h"

#define html_malloc(size) dmt_malloc(size)
#define html_realloc(data, size) dmt_realloc(data, size)
#define html_calloc(num, size) dmt_calloc(num, size)
#define html_free(data) dmt_free(data)

#else

#define html_malloc(size) malloc(size)
#define html_realloc(data, size) realloc(data, size)
#define html_calloc(num, size) calloc(num, size)
#define html_free(data) free(data)

#endif

#endif