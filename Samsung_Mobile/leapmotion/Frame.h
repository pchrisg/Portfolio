/*
 * Frame.h
 *
 *  Created on: Jun 4, 2017
 *      Author: Paul Christopher Gloumeau
 */

#ifndef FRAME_H_
#define FRAME_H_

#include <glm/glm.hpp>
#include <string>

struct Bone {
    std::string   name      = "";
    glm::vec3 start     = glm::vec3(-1000, 0, 0);
    glm::vec3 end       = glm::vec3(-1000, 0, 0);
    glm::vec3 direction = glm::vec3(0, 0, 0);

    void reset() {
        name      = "";
        start     = glm::vec3(-1000, 0, 0);
        end       = glm::vec3(-1000, 0, 0);
        direction = glm::vec3(0, 0, 0);
    }
};

struct Finger {
    std::string    name    = "";
    unsigned long  id      = 0;
    float          length  = 0.0;
    float          width   = 0.0;
    unsigned short numBone = 0;
    Bone           bones[4];

    void reset() {
        name    = "";
        id      = 0;
        length  = 0.0;
        width   = 0.0;
        numBone = 0;
        for (auto &bone : bones) {
            bone.reset();
        }
    }
};

struct Hand {
    std::string		name			= "";
    unsigned long	id				= 0;
    glm::vec3		palmPosition	= glm::vec3(-1000, 0, 0);
    float			pitch			= 0.0;
    float			roll			= 0.0;
    float			yaw				= 0.0;
    glm::vec3		armDirection	= glm::vec3(0, 0, 0);
    glm::vec3		wristPosition	= glm::vec3(-1000, 0, 0);
    glm::vec3		elbowPosition	= glm::vec3(-1000, 0, 0);
    unsigned short	numFinger		= 0;
    Finger			fingers[5];

    void reset() {
        name          = "";
        id            = 0;
        palmPosition  = glm::vec3(-1000, 0, 0);
        pitch         = 0.0;
        roll          = 0.0;
        yaw           = 0.0;
        armDirection  = glm::vec3(0, 0, 0);
        wristPosition = glm::vec3(-1000, 0, 0);
        elbowPosition = glm::vec3(-1000, 0, 0);
        numFinger     = 0;
        for (auto &finger : fingers) {
            finger.reset();
        }
    }
};

struct Frame {
    unsigned long  id              = 0;
    unsigned long  timestamp       = 0;
    int            extendedFingers = 0;
    int            tools           = 0;
    int            gestures        = 0;
    unsigned short numHand         = 0;
    Hand           hands[2];

    void reset() {
        id              = 0;
        timestamp       = 0;
        extendedFingers = 0;
        tools           = 0;
        gestures        = 0;
        numHand         = 0;
        for (auto &hand : hands) {
            hand.reset();
        }
    }
};

#endif /* FRAME_H_ */
