# modest_html

This library wraps functionality of lexborisovs **Modest**. It is written in in pure C99 and exposes a lot of useful features to parse and manipulate a html DOM. Internally it makes use of rxis **vec** and **dmt** libraries.

>Modest is a fast HTML renderer implemented as a pure C99 library with no outside dependencies.

- [Modest](https://github.com/lexborisov/Modest)
  - HTML5 parsing library in pure C99
  - fully conformant with the HTML5 spec

- [vec](https://github.com/rxi/vec)
  - A type-safe dynamic array implementation for C

- [dmt](https://github.com/rxi/dmt)
  - Dynamic Memory Tracker (DMT) Library

## Example
For more examples please checkout the [tests](https://github.com/f34nk/modest_html/tree/master/test).
```C
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
vec_str_t *buffer = html_get_buffer(w, buffer_index);

// join buffer into a string
char* result = html_vec_str_join(buffer, "");
printf("%s\n", result);
if(result != NULL && strcmp(result, "<body><p>Hello</p><p>World</p></body>") != 0){
  fprintf(stderr, "Failed\n");
  html_free(result);
  html_destroy(w);
  return 1;
}
html_free(result);

// destroy our workspace
html_destroy(w);
```

## Target dependencies
```
cmake 3.x
```

## Compile and test
The script compiles `Modest` and configures the `modest_html` library.
```
./configure.sh
```
Build `build/libmodest_html.a`.
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