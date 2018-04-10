#ifndef MODEST_HTML_H
#define MODEST_HTML_H

#define MODEST_HTML_VERSION "1.0.0"

// #define MODEST_HTML_DEBUG
// #define MODEST_HTML_USE_DMT

#ifdef MODEST_HTML_DEBUG
#define MODEST_HTML_LOG fprintf(stdout, "[LOG]\t%s()\n", __func__);
#define MODEST_HTML_LOG_ERROR fprintf(stderr, "[ERROR]\t%s()\n\t%s:%d\n", __func__, __FILE__, __LINE__);
#else
// nothing
#define MODEST_HTML_LOG
#define MODEST_HTML_LOG_ERROR
#endif

#include "html_memory.h"
#include "html_vec.h"
#include "html_map.h"
#include "html_string.h"
#include "html_node.h"
#include "html_workspace.h"
#include "html_parse.h"
#include "html_select.h"
#include "html_serialize.h"
#include "html_attribute.h"
#include "html_text.h"
#include "html_remove.h"
#include "html_replace.h"
#include "html_wrap.h"
#include "html_insert_after.h"
#include "html_insert_before.h"
#include "html_slice.h"
#include "html_prepend.h"
#include "html_append.h"
#include "html_position.h"
#include "html_compare.h"
#include "html_pretty_print.h"

#endif