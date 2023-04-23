#define CURL_STATICLIB
#include <iostream>
#include "curl/curl.h"
#include <cstring>
#include <fstream>
#include <string>
#pragma comment (lib, "Normaliz.lib")
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Crypt32.lib")
#pragma comment (lib, "Wldap32.lib")
#pragma comment (lib, "advapi32.lib")

using namespace std;

void createFileFromUrl(string url) {
    CURL* curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        FILE* fp;
        errno_t err = fopen_s(&fp, "NPS-2023-04-03.csv", "w");
        if (err != 0) {
            char buf[256];
            strerror_s(buf, sizeof(buf), err);
            std::cerr << "fopen_s() failed: " << buf << std::endl;
            return;
        }
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            char buf[256];
            strerror_s(buf, sizeof(buf), res);
            std::cerr << "curl_easy_perform() failed: " << buf << std::endl;
        }
        fclose(fp);
        curl_easy_cleanup(curl);
    }
}

int main() {
    string url = "https://dashboard.elering.ee/api/nps/price/csv?fields=ee";
    createFileFromUrl(url);

    ifstream file("NPS-2023-04-03.csv");
    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }

    return 0;
}