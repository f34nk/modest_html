#include "html_vec.h"
#include "html_memory.h"

char* html_vec_str_join(html_vec_str_t* vec, const char* delimiter)
{
  if(vec == NULL) {
    return NULL;
  }

#if 0
  char* data = NULL;
  data = (char*)vec_malloc(0);
  if(data == NULL) {
    return NULL;
  }
#endif

#if 0
  char* data = NULL;
  data = (char*)html_malloc(1 * sizeof(char));
  if(data == NULL) {
    return NULL;
  }
  *data = '\0';
#endif

#if 1
  char* data = (char*)html_calloc(1, 1);
#endif

  int i;
  char* value;
  html_vec_foreach(vec, value, i) {
    int prev = (int)strlen(data);
    int length = (int)strlen(value) + 1;

    char* new_data = (char*)html_realloc(data, prev + length * sizeof(char));
    if(new_data == NULL) {
      html_free(data);
      return NULL;
    }

    // strncat((char*)&new_data[prev], value, length);
    strcpy((char*)&new_data[prev], value);
    data = new_data;

    if(i < vec->length - 1 && strlen(delimiter) > 0) {
      prev = strlen(data);
      length = strlen(delimiter) + 1;

      new_data = (char*)html_realloc(data, prev + length * sizeof(char));
      if(new_data == NULL) {
        html_free(data);
        return NULL;
      }
      // strncat((char*)&new_data[prev], delimiter, length);
      strcpy((char*)&new_data[prev], delimiter);
      data = new_data;
    }
  }
  // User must free this data.
  return data;
}