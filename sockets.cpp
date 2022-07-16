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
#include <regex>

int wordlist_Check(char *wordlist){
    auto file_path = std::filesystem::path(wordlist);
    if (!std::filesystem::exists(file_path)){
        std::cout << "[-] File " << file_path << " doesn't exist, exiting...";
        return(0);
    }
    if (!std::filesystem::is_regular_file(file_path)){
        std::cout << "[-] " << file_path << " is not a file, exiting...";
        return(0);
    }
    return(1);
}
/*
int status_Code(char* buff){
    int status_code{};
    std::regex rule("[0-9]{3}");
    std::regex_search(buff,status_code,rule);
}
*/
int main(int argc, char** argv){
    
    if(argc != 4){
        std::cout << "Usage: " << argv[0] << " <target> <port> <wordlist>" << std::endl;
        exit(0);
    }

    int connection; // Stores socket
    char* target_host = argv[1]; // Target host 
    char* target_port = argv[2]; // Target port
    auto wordlist = std::filesystem::path(argv[3]);
    struct sockaddr_in target; // Stores target info
    char buffer[12]{0}; // Will store response

    if (atoi(target_port) <= 0 || atoi(target_port) > 65535){
        std::cout << "[-]Target port must be in-between 1-65535(both included)" << std::endl;
        exit(0);
    }

    if (!wordlist_Check(argv[3])){
        exit(0);
    }

    // Build target struct
    target.sin_family = AF_INET;
    target.sin_port = htons(atoi(target_port));
    inet_pton(AF_INET,(const char*)target_host,&target.sin_addr);

    // Wordlist check
    std::ifstream file(wordlist);
    if(!file.is_open()){
        std::cout << "Couldn't open file" << std::endl;
        exit(0);
    }

    std::string word;
    char *directory = new char[30];
    char *request = new char[100];

    while(getline(file,word)){
        if((connection =  socket(AF_INET,SOCK_STREAM,0)) < 0){
            std::cout << "[-]Socket creation failed" << std::endl;
            exit(0);
        }
        if(connect(connection,(struct sockaddr*)&target,sizeof(target)) < 0){
            std::cout << "[-]Connection failed" << std::endl;
            exit(0);
        }
        strcpy(directory,word.c_str());
        sprintf(request,"GET /%s HTTP/1.1\r\nHost: %s:%s\r\n\r\n",directory,target_host,target_port);
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
        close(connection);
    }  
}   
