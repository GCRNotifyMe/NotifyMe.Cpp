//
//  main.cpp
//  NotifyMe.Cpp
//
//  Created by Voltmeter Amperage on 1/25/17.
//  Copyright © 2017 ReVoltApplications. All rights reserved.
//

#include <iostream>
#include "NotifyMe.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    NotifyMe NM = NotifyMe("wowza7125", "gabe7125");
    std::cout << NM.getUserToken() << "\n";
    NM.addDevice("all");
    NM.sendNotification("\"");
    return 0;
}
