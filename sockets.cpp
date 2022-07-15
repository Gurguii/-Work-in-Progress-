#include <stdio.h> // printf, sprintf
//#include <stdlib.h> // exit
#include <sys/socket.h> // socket, connect
//#include <sys/types.h>
#include <netinet/in.h> // struct sockaddr_in, struct sockaddr
#include <arpa/inet.h> // inet_pton
#include <string.h> // memcpy, memset
#include <unistd.h> // read, write, close
#include <string>  // string
#include <iostream> // cout, cin, endl
#include <fstream> // ifstream
#include <filesystem> // path(),is_regular_file(), exists()
#include <cstring>  // c_str()

int main(int argc, char** argv){
    if(argc != 4){
        std::cout << "Usage: " << argv[0] << " <target> <port> <wordlist>" << std::endl;
        exit(0);
    }
    // WORDLIST FILE CHECK
    auto wordlist = std::filesystem::path(argv[3]);
    if (!std::filesystem::exists(wordlist)){
        std::cout << "File doesn't exist" << std::endl;
        exit(0);
    }
    if (!std::filesystem::is_regular_file(wordlist)){
        std::cout << "It's not a file" << std::endl;
        exit(0);
    }
    // CONNECTION CREATION
    int connection; // Stores socket
    char* target_host = argv[1];
    char* target_port = argv[2];
    struct sockaddr_in target; // Stores target info
    char buffer[1000]{0}; // Will store response
    // Create socket
    if((connection = socket(AF_INET,SOCK_STREAM,0)) < 0){
        std::cout << "[-] Socket creation failed" << std::endl;
        exit(0);
    }
    // Build target struct
    target.sin_family = AF_INET;
    target.sin_port = htons(atoi(target_port));
    inet_pton(AF_INET,(const char*)target_host,&target.sin_addr);
    // Attempt connection
    if(connect(connection,(struct sockaddr*)&target,sizeof(target)) < 0){
        std::cout << "[-]Couldn't establish connection..." << std::endl;
        exit(0);
    }
    std::cout << "[+]Connected to target" << std::endl;
    // WORD ITERATION
    std::ifstream file(wordlist);
    if(!file.is_open()){
        std::cout << "Couldn't open file" << std::endl;
        exit(0);
    }

    std::string word;
    char *directory = new char[30];
    char *request = new char[100];

    while(getline(file,word)){
        strcpy(directory,word.c_str());
        sprintf(request,"GET /%s HTTP/1.1\r\nHost: test\r\n\r\n",directory);
        std::cout << "Request => " << request << std::endl;
        // Send GET request
        if(send(connection,request,strlen(request),0) < 0 ){
            std::cout << "[-]Couldn't send message" << std::endl;
            exit(0);
        }
        // Get response
        if(recv(connection,&buffer,sizeof(buffer),0) < 0 ){
            std::cout << "[-]Could't receive message" << std::endl;
            exit(0);
        }
        std::cout << "~ Response ~" << std::endl << buffer << std::endl;
        std::cout << "Size => " << strlen(buffer) << std::endl;
    }
    close(connection);
}   