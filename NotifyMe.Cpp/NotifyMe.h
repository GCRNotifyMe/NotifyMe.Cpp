//
//  NotifyMe.h
//  NotifyMe.Cpp
//
//  Created by Voltmeter Amperage on 1/25/17.
//  Copyright © 2017 ReVoltApplications. All rights reserved.
//

#ifndef NotifyMe_H
#define NotifyMe_H

#include <vector>
#include <string>
#include <iostream>

#include <curl/curl.h>
#include <stdio.h>

class connectionException: public std::runtime_error {
    
};

class NotifyMe {
    // MARK: - Instance variables
    /** A vector of the given devices to send the alert to */
    std::vector<std::string> devices;
    
    
    /** The user's login token from the server */
    std::string token = "";
    
    /** The group the alert is a part of. NULL if not part of a group */
    std::string group = "";
    
    /** The longer message that is to be sent with the notification */
    std::string message = "";
    
    std::string login(std::string username, std::string password);
    
    int sendNotificationToServer(std::string alert);
        
    // MARK: - Constructors
    public:
    /** A blank constructor. All variables have default values */
    NotifyMe();
    
    /** Pass the user's username and password. Perform login to get user token */
    NotifyMe(std::string username, std::string password);
    
    /** Initiate with the user's token */
    NotifyMe(std::string userToken);
    
    
    // MARK: Accessors
    /** Get the user's login token */
    std::string getUserToken();
    
    /** Get a list of all the devices the alert will be sent to */
    std::vector<std::string> getDevices();
    
    /** Add a new device to the array */
    void addDevice(std::string newDevice);
    
    /** Remove all the devices added so far */
    void removeAllDevices();
    
    /** Get the current group */
    std::string getGroup();
    
    /** Set the current group */
    void setGroup(std::string newGroup);
    
    /** Get the current message */
    std::string getMessage();
    
    /** Set the current message */
    void setMessage(std::string newMessage);
    
    int sendNotification(std::string alert);
};

#endif
