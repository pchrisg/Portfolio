/*
 * LeapData.h
 *
 *  Created on: Apr 24, 2017
 *      Author: kululu
 */

#ifndef LEAPDATA_H_
#define LEAPDATA_H_

#include "Frame.h"
#include <string>
#include <json-glib/json-glib.h>

class LeapData {
public:
    LeapData() = default;

    ~LeapData() = default;

    Frame       mFrame;
    std::string mInfo;

    bool ParseLeapFrame(const std::string &msg);

};//LeapData

#endif /* GLOBAL_H_ */
