//
//  NotifyMe.Cpp
//  NotifyMe.Cpp
//
//  Created by Voltmeter Amperage on 1/25/17.
//  Copyright © 2017 ReVoltApplications. All rights reserved.
//

#include "NotifyMe.h"

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

std::string NotifyMe::login(std::string username, std::string password) {
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    std::string postData = ("username=" + username + "&password=" + password);
    
    
    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);
    
    /* get a curl handle */
    curl = curl_easy_init();
    if(curl) {
        /* First set the URL that is about to receive our POST. This URL can
         just as well be a https:// URL if that is what should receive the
         data. */
        curl_easy_setopt(curl, CURLOPT_URL, "http://notifyme.revoltapplications.com/phpapi/Login.php");
        /* Now specify the POST data */
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        
        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK) {
            // Failed to make connection
            throw std::runtime_error("Failed to connect to the web. Please try again later.");
        }
        
        /* always cleanup */
        curl_easy_cleanup(curl);
    } else {
        // Failed to init curl
        throw std::runtime_error("Failed to initalize curl. Please try again.");
    }
    curl_global_cleanup();
    
    return readBuffer;
}

int NotifyMe::sendNotificationToServer(std::string alert) {
    if (token.size() == 0) {
        std::cout << "Error";
        return -7;
    }
    
    replaceAll(alert, "\\", "\\\\");
    
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    
    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);
    
    /* get a curl handle */
    curl = curl_easy_init();
    
    std::string deviceString = "[";
    std::string flag = "";
    
    for (std::string device : devices) {
        deviceString += flag + "\"" + device + "\"";
        flag = ", ";
    }
    
    deviceString += "]";
    
    std::string json = "{ \"token\":\"" + token + "\", \"source\":\"C++\", \"alert\":\"" + curl_easy_escape(curl, alert.c_str(), alert.size()) + "\", \"devices\":" + deviceString;
    
    if (message.size() != 0) {
        json += ", \"message\":\"" + message + "\"";
    }
    
    if (group.size() != 0) {
        json += ", \"group\":\"" + group + "\"";
    }
    
    json += "}";
    
    std::cout << json << "\n";
    
//    return 0;
    std::string postData = ("json=" + json);
    
    if(curl) {
        /* First set the URL that is about to receive our POST. This URL can
         just as well be a https:// URL if that is what should receive the
         data. */
        curl_easy_setopt(curl, CURLOPT_URL, "http://notifyme.revoltapplications.com/phpapi/SendNotification.php");
        /* Now specify the POST data */
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        
        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK) {
            // Failed to make connection
            throw std::runtime_error("Failed to connect to the web. Please try again later.");
        }
        
        /* always cleanup */
        curl_easy_cleanup(curl);
    } else {
        // Failed to init curl
        throw std::runtime_error("Failed to initalize curl. Please try again.");
    }
    curl_global_cleanup();
    
    std::cout << "The return data is " << readBuffer << "\n";
    
    return 0;
}

NotifyMe::NotifyMe() {}

NotifyMe::NotifyMe(std::string username, std::string password) {
    std::string ret = login(username, password);
    
    /*
     Possible token values -
        -1
            The user has not confirmed their email. They need to.
        -2
            That username is not registered.
        -3
            The password is incorrect.
        Anything else:
            Its the user's login token.
     */
    
    if (ret.compare("-1") == 0) {
        throw new std::invalid_argument("The user has not confirmed their email.");
    } else if (ret.compare("-2") == 0) {
        throw new std::invalid_argument("That username is not registered. Please register at notifyme.revoltapplications.com");
    } else if (ret.compare("-3") == 0) {
        throw new std::invalid_argument("That password is incorrect. If you forgot your password, you can reset is at notifyme.revoltapplications.com");
    } else {
        token = ret;
    }
}

NotifyMe::NotifyMe(std::string userToken) : token(userToken) {}

std::string NotifyMe::getUserToken() {
    return token;
}

std::vector<std::string> NotifyMe::getDevices() {
    return devices;
}

void NotifyMe::addDevice(std::string newDevice) {
    devices.insert(devices.end(), newDevice);
}

void NotifyMe::removeAllDevices() {
    devices.clear();
}

std::string NotifyMe::getGroup() {
    return group;
}

void NotifyMe::setGroup(std::string newGroup) {
    group = newGroup;
}

std::string NotifyMe::getMessage() {
    return message;
}

void NotifyMe::setMessage(std::string newMessage) {
    message = newMessage;
}

int NotifyMe::sendNotification(std::string alert) {
    sendNotificationToServer(alert);
    return 0;
}
