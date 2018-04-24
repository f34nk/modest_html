# modest_html Changelog

## 2.0.1

April 24, 2018

- `Modest` now builds with `MyCORE_BUILD_WITHOUT_THREADS=YES` option enabled
- Refactored html_serialize_collection, html_serialize_tree and html_serialize_node to use myhtml_serialization_tree_buffer for better performance.

## 2.0.0

April 20, 2018

- Removed feature `compare`. A proper implementation of html comparison, down to the node level, is planned for a later release.

## 1.0.0

April 8, 2018

- Added MODEST_HTML_VERSION definition
- Implemented features:  append, get and set attribute, compare, insert_after, insert_before, position, prepend, pretty_print, remove, replace, serialize, slice, get and set text, wrap
- Added feature tests
- Enabled memory tracking with `dmt` library for html_* tests