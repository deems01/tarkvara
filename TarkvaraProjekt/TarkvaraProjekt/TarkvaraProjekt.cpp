#define CURL_STATICLIB
#include <iostream>
#include "curl/curl.h"
#include <cstring>
#include <fstream>
#include <chrono>
#include <cstdio>
#include <Windows.h>
#include <thread>
#include <ctime>
#pragma comment (lib, "Normaliz.lib")
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Crypt32.lib")
#pragma comment (lib, "Wldap32.lib")
#pragma comment (lib, "advapi32.lib")

using namespace std;

bool fileExists(const string & fileName) {
    ifstream infile(fileName);
    return infile.good();
}

void createFileFromUrl(string url) {
    CURL* curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        std::time_t t = std::time(nullptr);
        std::tm tm = {};
        localtime_s(&tm, &t);
        std::ostringstream oss;
        oss << "NPS-" << std::put_time(&tm, "%Y-%m-%d") << ".csv";
        std::string fileName = oss.str();


        FILE* fp;
        errno_t err = fopen_s(&fp, fileName.c_str(), "w");
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



int main()
{
    while (true) {

        // Get current time
        time_t now = time(0);
        tm* ltm = localtime(&now);

        // Check if it's 15:00
        if (ltm->tm_hour == 14 && ltm->tm_min == 19 && ltm->tm_sec == 0) {

            // Get today's date
            time_t now = time(0);
            tm* ltm = localtime(&now);
            int today = ltm->tm_mday;

            // Get tomorrow's date
            time_t tomorrow = now + (24 * 60 * 60);
            tm* ltm_tomorrow = localtime(&tomorrow);
            int tomorrow_date = ltm_tomorrow->tm_mday; 

            string url = "https://dashboard.elering.ee/api/nps/price/csv?start=2023-04-" + to_string(today) + "T21%3A00%3A00Z&end=2023-04-" + to_string(tomorrow_date) + "T20%3A00%3A00Z&fields=ee";
            createFileFromUrl(url);

            cout << url << endl;


            // See file contents in terminal
            std::time_t t = std::time(nullptr);
            std::tm tm = {};
            localtime_s(&tm, &t);
            std::ostringstream oss;
            oss << "NPS-" << std::put_time(&tm, "%Y-%m-%d") << ".csv";
            std::string fileName = oss.str();

            ifstream file(fileName);
            string line;
            while (getline(file, line)) {
                cout << line << endl;
            }
            
        }
        this_thread::sleep_for(chrono::seconds(1));
    }
    return 0;
}