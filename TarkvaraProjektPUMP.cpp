//#define CURL_STATICLIB
#include <iostream>
#include <fstream>
#include <string>
//#include <ctime>
//#include <cstdlib>
//#include <chrono>
#include <thread>
#include <Windows.h>
//#pragma comment (lib, "Normaliz.lib")
//#pragma comment (lib, "Ws2_32.lib")
//#pragma comment (lib, "Crypt32.lib")
//#pragma comment (lib, "Wldap32.lib")
//#pragma comment (lib, "advapi32.lib")

using namespace std;

void openWebsite(string website)
{
    system(("start " + website).c_str());
}

void delay(int minutes)
{
    while (minutes > 0)
    {
        this_thread::sleep_for(chrono::minutes(1));
        minutes--;
        cout << minutes << " minutes left." << endl;
    }
}

void processFile(string filename, tm ltm)
{
    ifstream file(filename);
    if (file.is_open())
    {
        string line;
        bool startCounting = false;
        bool delayPassed = false;
        int count = 0;
        while (getline(file, line))
        {
            int num1 = (line[2] - '0') * 10 + (line[3] - '0');
            if (!startCounting)
            {
                if (line[0] == '+')
                {
                    openWebsite("https://www.bing.com");
                }
                else if (line[0] == '-')
                {
                    openWebsite("https://www.google.com");
                }
            }
            if (num1 == ltm.tm_hour)
            {
                startCounting = true;
                int num2 = (line[5] - '0') * 10 + (line[6] - '0');
                int delayTime = 60 - ltm.tm_min + num2;
                cout << delayTime << " minutes left." << endl;
                delay(delayTime);
                delayPassed = true;
            }
            if (startCounting && delayPassed)
            {
                if (line[0] == '+')
                {
                    openWebsite("https://www.bing.com");
                    printf("pump ON");
                    Sleep(36000);
                }
                else if (line[0] == '-')
                {
                    openWebsite("https://www.google.com");
                    Sleep(36000);
                    printf("pump OFF");
                }
                count++;
            }
        }
        file.close();
        cout << "Processed " << count << " lines." << endl;
    }
    else
    {
        cout << "Unable to open file\n";
    }
}

int main()
{
    while (true)
    {
        time_t now = time(0);
        tm ltm;
        localtime_s(&ltm, &now);

        if (ltm.tm_hour == 14 && ltm.tm_min == 50)
        {
            char date[20];
            strftime(date, sizeof(date), "NPS-%Y-%m-%d.txt", &ltm);

            processFile(date, ltm);
            break;
        }
    }

    return 0;
}