#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <fstream>
#include <vector>
#include <cstring>
#include <sstream>
#include <regex>
#include <filesystem>

/* ~ ARGS ~
  | -ext / -extension
  | -h / -help 
  | -host / --host
  | -p /-port
  | -wl / --wordlist
*/
class argsParser{
    public:
     std::string wordlist{};
     std::string ext{}; // extension to add to requests
     char *host = new char[10]; // target host
     char *port = new char[10]; // target port (default value)

     void help()
     {
        std::cout << "[-] Work in progress ..." << std::endl;
        exit(0);
     }

     void fileCheck(std::string *file)
     {
        std::string file_path = std::filesystem::path(*file);
        if(!std::filesystem::exists(file_path)){
            std::cout << "[!] - File " << *file << " doesn't exist" << std::endl;
            exit(0);
        }
        if(!std::filesystem::is_regular_file(file_path)){
            std::cout << "[!] - " << *file << " is not a file" << std::endl;
            exit(0);
        }
        this->wordlist = file_path;
     }
    
     argsParser(std::stringstream *args)
     {
        std::string *opt = new std::string;
        int n = 0;
        while(!args->eof()){
            *args >> *opt;
            std::cout << "PRobando => " << *opt << std::endl;
            if(*opt == "-host" || *opt == "--host"){
                *args >> *opt;
                this->host = &opt->at(0);
                std::cout << "Host => " << this->host << std::endl;
            }
            else if(*opt == "-ext" || *opt == "-extension"){
                *args >> *opt;
                this->ext = *opt;
                std::cout << "Ext => " << *opt << std::endl;
            }
            else if(*opt == "-h" || *opt == "--help"){
                this->help();
            }
            else if(*opt == "-wl" || *opt == "--wordlist"){
                *args >> *opt;
                this->fileCheck(opt);
                std::cout << "Wordlist => " << this->wordlist << std::endl;
            }
            else if(*opt == "-p" || *opt == "--port"){
                *args >> *opt;
                this->port = &opt->at(0);
                std::cout << "Port => " << this->port << std::endl;
            }
            else
            {
                std::cout << "[!] - Option " << *opt << " doesn't exist" << std::endl;
                return;
            }
            n+=1;
        }
        delete opt;
        delete args;
     }
};
int main(int argc, char** argv){
    std::stringstream *args = new std::stringstream;
    for (int i = 1 ; i < argc-1 ; i++){
        *args << argv[i] << ' ';
    }
    *args << argv[argc-1];
    argsParser target(args);
    std::cout << target.port << " " << target.host << " " << target.wordlist << " " << target.ext << std::endl;
}