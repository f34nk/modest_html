[![Build status](https://travis-ci.org/f34nk/modest_html.svg?branch=master)](https://travis-ci.org/f34nk/modest_html)

# modest_html

This library wraps functionality of lexborisovs **Modest**. It is written in in pure C99 and exposes a lot of useful features to parse and manipulate a html DOM. Internally it makes use of rxis excellent **vec**, **map** and **dmt** libraries.

>Modest is a fast HTML renderer implemented as a pure C99 library with no outside dependencies.

- [Modest](https://github.com/lexborisov/Modest)
  - HTML5 parsing library in pure C99
  - fully conformant with the HTML5 spec

- [vec](https://github.com/rxi/vec)
  - A type-safe dynamic array implementation for C

- [map](https://github.com/rxi/map)
  - A type-safe generic hashmap implementation for C.
  
- [dmt](https://github.com/rxi/dmt)
  - Dynamic Memory Tracker (DMT) Library

## Example
For more examples please checkout [tests](https://github.com/f34nk/modest_html/tree/master/test).
```C
#include "modest_html.h"

/*
modest_html is build around a workspace struct.
The workspace holds all allocated memory and frees it in html_destroy().
*/
html_workspace_t *w = html_init();

// parse some html
const char *html = "<p>Hello</p><p>World</p>";
int tree_index = html_parse_tree(w, html, strlen(html));

// serialize the tree into a buffer
int buffer_index = html_serialize_tree(w, tree_index, "body");

// get buffer and join into a string
html_vec_str_t *buffer = html_get_buffer(w, buffer_index);
char* result = html_vec_join(buffer, "");
printf("%s\n", result);

// test result
if(result != NULL && strcmp(result, "<body><p>Hello</p><p>World</p></body>") != 0){
  fprintf(stderr, "Failed\n");
  html_free(result);
  html_destroy(w);
  return 1;
}

// free result
html_free(result);

// destroy our workspace
html_destroy(w);
```

## Dependencies
```
cmake 3.x
```

## Compile and test
The script compiles `Modest` and configures the library.
```
./configure
```
Build `libmodest_html.a` into `build/`.
```
cd build
make
```
Execute tests.
```
make test
```
Check for leaks.
```
valgrind --leak-check=yes test/example_test
```

## Cloning
```
git clone git@github.com:f34nk/modest_html.git
cd modest_html
```
All dependencies are added as submodules in the `libs/` folder.
```
git submodule update --init --recursive --remote
```

## Roadmap

See [CHANGELOG](https://github.com/f34nk/modest_html/blob/master/CHANGELOG.md).

- [ ] Features
  - [x] Find nodes using a CSS selector
  - [x] Serialize any string with valid or broken html
  - [x] Get attribute with optional CSS selector
  - [x] Set attribute with optional CSS selector
  - [x] Get text with optional CSS selector
  - [x] Set text with optional CSS selector
  - [x] Remove a node from html
  - [x] Append node to another node
  - [x] Prepend node to another node
  - [x] Insert node before another node
  - [x] Insert node after another node
  - [x] Replace node with another node
  - [x] Slice html to a subset of nodes
  - [x] Get position of node in relation to its parent
  - [x] Wrap node with another node
  - [x] Pretty print html
  - [x] Compare two html strings
- [ ] Documentation

