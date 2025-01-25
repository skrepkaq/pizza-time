#pragma once
#include <HTTPClient.h>
#include <config.h>
#include <ArduinoStreamParser.h>

class Request {
  public:
    char* request(const char* method, const char* path);
    char* request(const char* method, const char* path, const char* payload);
    void request(const char* method, char* path, ArudinoStreamParser& parser);
  private:
    HTTPClient* _get_client(const char* method, char* path);
    char* _get_url(const char* path);
};

