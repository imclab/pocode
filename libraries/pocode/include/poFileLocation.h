//
//  poFileLocation.h
//  pocode
//
//  Created by Steve Varga on 9/6/12.
//  Copyright (c) 2012 Potion Design. All rights reserved.
//

#pragma once

#define BOOST_FILESYSTEM_VERSION 3


<<<<<<< HEAD
#include <curl/curl.h>
=======
//#include <curl/curl.h>
>>>>>>> 5f543812c9a5a076538977af53616f8576a3719e

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

//Path Utils
namespace po {
    //------------------------------------------------------------------
    //FilePath
    class FilePath {
    public:
        FilePath();
        FilePath(const char* path);
        FilePath(const std::string &path);
        FilePath(const fs::path &filePath);
        ~FilePath();
        
        void set(std::string path);
        void setFromBoostPath(fs::path filePath);
        bool isSet() const;
        
        bool exists() const;
        
        std::string toString() const;
        fs::path    toBoostPath();
        
        FilePath getScaled(float scale) const;
    private:
        fs::path filePath;
    };


    //------------------------------------------------------------------
    //URL
    class URL {
    public:
        struct curl_option {
<<<<<<< HEAD
            curl_option(CURLoption option, void* value) : option(option), value(value) {};
            CURLoption option;
=======
            curl_option(int option, void* value) : option(option), value(value) {};
            int option;
>>>>>>> 5f543812c9a5a076538977af53616f8576a3719e
            void*  value;
        };
        
        URL();
        explicit URL(std::string path);
        ~URL();
        
        void set(std::string url);
        bool isSet();
        
<<<<<<< HEAD
        void addOption(CURLoption option, void*value);
=======
        void addOption(int option, void*value);
>>>>>>> 5f543812c9a5a076538977af53616f8576a3719e
        std::vector<curl_option> &getOptions();
        
        void addHeader(std::string value);
        std::vector <std::string> &getHeaders();
        
        void setUsername(std::string username);
        std::string getUsername();
        
        void setPassword(std::string password);
        std::string getPassword();
        
        void setTimeout(float timeInSeconds);
        float getTimeout();
        
        std::string toString();
        
    private:
        
        std::string url;
        std::vector <std::string> headers;
        std::vector <curl_option> options;
        
        std::string username, password;
        
        float timeOut;
    };
    
    
    
    //------------------------------------------------------------------
    //Utils
    FilePath getCurrentPath();
    
    // search up the filesystem from pwd for folder
    bool pathToFolder(const std::string &folder_name, fs::path *path);
    
    // search up the filesystem from pwd and set to a folder if its there
    bool lookUpAndSetPath(const std::string &folder_name);
    bool lookUpAndSetPathNextTo(const std::string &folder_name);
    std::string getApplicationSupportDirectory();
    
    
    //List directory
    void listDirectory(FilePath dir);

}/* End po Namespace */