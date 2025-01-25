#include "requests.h"


char* Request::_get_url(const char* path) {
    size_t len = strlen(API_URL) + strlen(path) + 1;
    char* url = (char*)malloc(len);
    strcpy(url, API_URL);
    strcat(url, path);
    return url;
}

HTTPClient* Request::_get_client(const char* method, char* url) {
    HTTPClient* http = new HTTPClient();
    http->begin(url);
    if (!strcmp(method, "POST")) {
        http->addHeader("Content-Type", "application/json");
        http->addHeader("Accept", "application/json");
    } else {
        http->addHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8");
    }
    http->addHeader("Accept-Language", "en-US,en;q=0.5");
    http->setUserAgent("Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:120.0) Gecko/20100101 Firefox/120.0");
    return http;
}

void Request::request(const char* method, char* path, ArudinoStreamParser& parser) {
    char* url = _get_url(path);
    HTTPClient* http = _get_client(method, url);
    
    int httpCode = http->GET();
    if (httpCode == HTTP_CODE_OK) {
      http->writeToStream(&parser);
    }
    http->end();
    delete http;
    free(url);
}

char* Request::request(const char* method, const char* path, const char* payload) {
    char* url = _get_url(path);
    HTTPClient* http = _get_client(method, url);

    int httpCode = http->POST(payload);

    const size_t len = http->getSize() + 1;
    char* response = (char*)malloc(len);
    http->getString().toCharArray(response, len);
    http->end();
    delete http;
    free(url);
    return response;
}

char* Request::request(const char* method, const char* path) {
    char* url = _get_url(path);
    HTTPClient* http = _get_client(method, url);

    int httpCode = http->GET();

    const size_t len = http->getSize() + 1;
    char* response = (char*)malloc(len);
    http->getString().toCharArray(response, len);
    http->end();
    delete http;
    free(url);
    return response;
}
