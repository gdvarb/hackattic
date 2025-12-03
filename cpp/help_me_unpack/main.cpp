#include <curl/curl.h>
#include <curl/easy.h>

#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>

#include "Base64.h"
#include "json.hpp"

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
  // calculate total size of data
  size_t totalSize = size * nmemb;

  // cast userp back to a string pointer so we can use it
  std::string* str = (std::string*)userp;

  char* char_ptr = static_cast<char*>(contents);
  str->append(char_ptr, totalSize);

  return totalSize;
}

using json = nlohmann::json;

int main() {
  CURL* curl;
  CURLcode res;
  std::string readBuffer{};

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL,
                     "https://hackattic.com/challenges/help_me_unpack/"
                     "problem?access_token=840aa5334d071243");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
      std::cerr << "curl_easy_perform() failed" << curl_easy_strerror(res)
                << '\n';

      curl_easy_cleanup(curl);
      return 1;
    }

    std::cout << "Raw Json: " << readBuffer << '\n';

    try {
      json data = json::parse(readBuffer);
      std::string base64_str{data.at("bytes").get<std::string>()};
      std::cout << "Raw str: " << base64_str << '\n';

      std::string raw_binary{};
      int offset{0};

      // decode entire string
      macaron::Base64::Decode(base64_str, raw_binary);

      // extract data:
      int decoded_int{};
      std::memcpy(&decoded_int, raw_binary.data() + offset, sizeof(int));
      offset += sizeof(int);
      std::cout << "Decoded int is: " << decoded_int << '\n';

      unsigned int decoded_int_unsigned{};
      std::memcpy(&decoded_int_unsigned, raw_binary.data() + offset,
                  sizeof(unsigned int));
      offset += sizeof(unsigned int);
      std::cout << "Decoded unsigned int is: " << decoded_int_unsigned << '\n';

      short decoded_short{};
      std::memcpy(&decoded_short, raw_binary.data() + offset, sizeof(short));
      offset += sizeof(short);
      std::cout << "Decoded short is: " << decoded_short << '\n';

      float decoded_float{};
      std::memcpy(&decoded_float, raw_binary.data() + offset, sizeof(float));
      offset += sizeof(float);
      std::cout << "Decoded float is: " << decoded_float << '\n';

      double decoded_double{};
      std::memcpy(&decoded_double, raw_binary.data() + offset, sizeof(double));
      offset += sizeof(double);
      std::cout << "Decoded double is: " << decoded_double << '\n';

      // reverse for big endian
      std::string chunk{raw_binary.substr(offset, sizeof(double))};
      std::reverse(chunk.begin(), chunk.end());
      double decoded_double_bigE{};
      std::memcpy(&decoded_double_bigE, chunk.data(), sizeof(double));
      std::cout << "Decoded double_bigE is: " << decoded_double_bigE << '\n';

      // create solution
      json solution_json;
      solution_json["int"] = decoded_int;
      solution_json["uint"] = decoded_int_unsigned;
      solution_json["short"] = decoded_short;
      solution_json["float"] = decoded_float;
      solution_json["double"] = decoded_double;
      solution_json["big_endian_double"] = decoded_double_bigE;

      // convert json object to string so curl can send
      std::string json_string{solution_json.dump()};
      readBuffer.clear();

      // create post request
      struct curl_slist* headers{NULL};
      headers = curl_slist_append(headers, "Content-Type: application/json");
      curl_easy_setopt(curl, CURLOPT_URL,
                       "https://hackattic.com/challenges/help_me_unpack/"
                       "solve?access_token=840aa5334d071243");
      curl_easy_setopt(curl, CURLOPT_POST, 1L);
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_string.c_str());
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

      // capture response
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

      // send
      res = curl_easy_perform(curl);

      std::cout << "Send response: " << readBuffer << '\n';
      curl_slist_free_all(headers);

    } catch (json::parse_error& e) {
      std::cerr << "JSON Parse failed: " << e.what() << '\n';
    }
  }
  return 0;
}
