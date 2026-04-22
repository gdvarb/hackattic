#include "json.hpp"
#include <cstdio>
#include <curl/curl.h>
#include <curl/easy.h>
#include <iostream>
#include <string.h>
#include <zip.h>
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  size_t totalSize{size * nmemb};

  std::string *str{(std::string *)userp};
  char *char_ptr{static_cast<char *>(contents)};
  str->append(char_ptr, totalSize);

  return totalSize;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
  size_t written{fwrite(ptr, size, nmemb, (FILE *)stream)};
  return written;
}

int main() {
  CURL *curl;
  CURLcode res;
  std::string readBuffer{};
  curl = curl_easy_init();
  curl_global_init(CURL_GLOBAL_DEFAULT);

  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL,
                     "https://hackattic.com/challenges/brute_force_zip/"
                     "problem?access_token=840aa5334d071243");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
      std::cerr << "curl_easy_preform() failed: " << curl_easy_strerror(res)
                << '\n';
      curl_easy_cleanup(curl);
      return 1;
    }

    std::cout << "Raw JSON: " << readBuffer << '\n';
    using json = nlohmann::json;
    json data = json::parse(readBuffer);
    std::string zip_url{data.at("zip_url").get<std::string>()};
    std::cout << "URL is: " << zip_url << '\n';

    // get file
    const char *outfilename{"downloaded_file.zip"};
    FILE *fp = fopen(outfilename, "wb");
    curl_easy_setopt(curl, CURLOPT_URL, zip_url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // Follow redirects
    CURLcode res2;
    res2 = curl_easy_perform(curl);
    fclose(fp);

    int err{0};
    zip *z = zip_open("downloaded_file.zip", 0, &err);

    // todo: need to iterate and find correct password. characters are all
    // lowercase and numeric (4-6) ascii only
    const char *guess = "1234";
    zip_set_default_password(z, guess);

    // search for file
    const char *name{"secret.txt"};
    struct zip_stat st;
    zip_stat_init(&st);
    zip_stat(z, name, 0, &st);

    // allocate memory for contents
    char *contents = new char[st.size];

    // read compressed file
    zip_file *f = zip_fopen(z, name, 0);
    long long bytes_read{zip_fread(f, contents, st.size)};
    if (bytes_read > 0) {
      std::cout << "success, password is: " << guess << '\n';
    } else {
      std::cout << "wrong password\n";
    }
    // zip_fread(f, contents, st.size);
    zip_fclose(f);
    zip_close(z);
  }
  return 0;
}
