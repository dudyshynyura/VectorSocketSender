#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <vector>
#include <iostream>
#include <fstream>

#define PORT 8080

using namespace std;
vector<string> getData(const string fileName);

int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);


    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

  

    vector<string> vec = getData("in.txt");
    long len = htonl(vec.size());
    send(sock, &len, sizeof(len), 0);
    for(int i = 0; i < vec.size(); ++i){
      len = htonl(vec[i].length());
      send(sock, &len, sizeof(len), 0);
      send(sock, vec[i].data(), vec[i].length(), 0);
    }

}

vector<string> getData(const string fileName){
  ifstream in(fileName.c_str(), ifstream::in);
  if(!in.is_open()) cout << "Error opening input file\n";
  vector<string> data;
  string str;
  while(in >> str) data.push_back(str);
  in.close();
  return data;
}
