#include "html_string.h"
#include "html_memory.h"

// concat str1 and str2
char *concat_string(const char *str1, const char *str2)
{
  char *result = NULL;
  size_t n = 0;

  if(str1) n += strlen(str1);
  if(str2) n += strlen(str2);

  if((str1 || str2) && (result = html_malloc(n + 1)) != NULL)
  {
    *result = '\0';

    if(str1) strcpy(result, str1);
    if(str2) strcat(result, str2);
  }

  return result;
}

/**
 * https://stackoverflow.com/a/9210560
 * @param  string   [description]
 * @param  delimiter [description]
 * @return         [description]
 */
char** split_string(char* string, const char delimiter)
{
  char** result    = 0;
  size_t count     = 0;
  char* tmp        = string;
  char* last_comma = 0;
  char delim[3];
  delim[0] = delimiter;
  delim[1] = 0;

  /* Count how many elements will be extracted. */
  while (*tmp)
  {
    if (delimiter == *tmp)
    {
      count++;
      last_comma = tmp;
    }
    tmp++;
  }

  /* Add space for trailing token. */
  count += last_comma < (string + strlen(string) - 1);

  /* Add space for terminating null string so caller
     knows where the list of returned strings ends. */
  count++;

  result = html_malloc(sizeof(char*) * count);

  if (result)
  {
    size_t idx  = 0;
    char* token = strtok(string, delim);

    while (token)
    {
      // assert(idx < count);
      *(result + idx++) = strdup(token);
      token = strtok(0, delim);
    }
    // assert(idx == count - 1);
    *(result + idx) = 0;
  }

  return result;
}

// char* html_string_copy(const char* string)
// {
//   if(string == NULL) {
//     return NULL;
//   }

//   char *data = html_malloc(strlen(string) + 1 * sizeof(char*));
//   if(data == NULL) {
//     return NULL;
//   }
//   *data = '\0';
//   strcpy(data, string);

//   return data;
// }