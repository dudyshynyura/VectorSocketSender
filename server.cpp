#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>



#define PORT 8080
using namespace std;
void setData(const string fileName, const vector<string> &data);
int min(int a, int b);

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }


    vector<string> vec;
    long size = 0;
    read(new_socket, &size, sizeof(size));
    size = ntohl(size);
    for(int i = 0; i < size; ++i){
      string rd;
      long len = 0;
      read(new_socket, &len, sizeof(len));
      len = ntohl(len);
      while(0 < len){
        char buf[1024];
        int cread;
        cread = read(new_socket, buf, min(sizeof(buf), len));
        rd.append(buf, cread);
        len -= cread;
      }
      vec.push_back(rd);
    }

    setData("out.txt", vec);
}


void setData(const string fileName, const vector<string> &data){
  ofstream out(fileName.c_str(), ofstream::out);
  if(!out.is_open()) cout << "Error opening output file\n";
  for(int i = 0; i < data.size(); i++){
    out << data[i] << endl;
  };
}

int min(int a, int b){
  if(a < b) return a;
  else if(b < a) return b;
  return 0;
}
