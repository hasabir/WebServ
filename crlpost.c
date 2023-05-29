/*
 * HTTP POST with chunked encoding.
 *
 * Usage:
 * cc chunkedpost.c -lcurl -o chunkedpost
 * ./chunkedpost
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

struct WriteThis {
  const char *readptr;
  long sizeleft;
};

static size_t _upload_read_function(void *ptr, size_t size, size_t nmemb, void *data) {
  struct WriteThis *pooh = (struct WriteThis *)data;
  size_t max_bytes = size*nmemb;
  if (max_bytes < 1)
    return 0;

  if (pooh->sizeleft) {
    size_t bytes = max_bytes < pooh->sizeleft ? max_bytes : pooh->sizeleft;
    memcpy(ptr, pooh->readptr, bytes);
    pooh->readptr += bytes;
    pooh->sizeleft -= bytes;
    printf("upload_read_function: %zu bytes (%ld left)\n", bytes, pooh->sizeleft);
    return size*nmemb;
  }

  return 0;
}

int main(int argc, char const *argv[]) {
  curl_version_info_data *curl_version = curl_version_info(CURLVERSION_NOW);
  printf("libcurl: %s\n", curl_version->version);

  size_t buffer_size = 65535;
  unsigned char *ogg_buffer = calloc(buffer_size, 1);
  memset(ogg_buffer, 'x', buffer_size);

  struct WriteThis pooh;
  pooh.readptr = (const char *)ogg_buffer;
  pooh.sizeleft = buffer_size;

  CURL *curl = curl_easy_init();
  if (curl) {
    //curl_easy_setopt(curl, CURLOPT_URL, "https://ameriwether.com/cgi-bin/info.pl");
    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8090");
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    struct curl_slist *chunk = NULL;
    chunk = curl_slist_append(chunk, "Transfer-Encoding: chunked");
    chunk = curl_slist_append(chunk, "Accept: text/plain"); // Example output easier to read as plain text.
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    // curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, buffer_size);   // <-- if this line is enabled, data is sent correctly
    curl_easy_setopt(curl, CURLOPT_READDATA, &pooh);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, _upload_read_function);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // Make the example URL work even if your CA bundle is missing.
    CURLcode res = curl_easy_perform(curl);

    if (res == CURLE_OK) {
      long response_code;
      double request_size;
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
      curl_easy_getinfo(curl, CURLINFO_SIZE_UPLOAD, &request_size);
      printf("Server responeded with %ld, request was %f bytes\n", response_code, request_size);
    } else {
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    // Cleanup
    curl_slist_free_all(chunk);
    curl_easy_cleanup(curl);
  }
  return 0;
}
