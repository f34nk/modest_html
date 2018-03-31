# modest_html

This library wraps functionality of lexborisovs **Modest**. It is written in in pure C99 and exposes a lot of useful features to parse and manipulate a html DOM.

>Modest is a fast HTML renderer implemented as a pure C99 library with no outside dependencies.

- [Modest](https://github.com/lexborisov/Modest)
  - HTML5 parsing library in pure C99
  - fully conformant with the HTML5 spec

## Example
For more examples please checkout the [tests](https://github.com/f34nk/modest_html/tree/master/test).
```C
int i = 0;
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

## Cloning
```
git clone git@github.com:f34nk/modest_html.git
cd modest_html
```
All dependencies are added as submodules in the `libs/` folder.
```
git submodule update --init --recursive --remote
```