/*
 * LeapData.cpp
 *
 *  Created on: May 29, 2017
 *      Author: Paul Christopher Gloumeau
 */

#include "LeapData.h"

bool LeapData::ParseLeapFrame(const std::string &msg) {
    mFrame.reset();
    mInfo = "";

    if (msg.length() > 152) {
        //Handle Json data;
        JsonParser *parser;
        JsonNode   *root;
        GError     *error;

        error  = nullptr;
        parser = json_parser_new();

        gboolean result = json_parser_load_from_data(parser, msg.c_str(), msg.size(),
                                                     &error);

        if (result) {
            unsigned long  frameID   = 0;
            unsigned long  timestamp = 0;
            int            extendedFingers;
            int            tools;
            int            gestures;
            unsigned short numHand;

            root = json_parser_get_root(parser);

            JsonObject *object;
            object = json_node_get_object(root);

            JsonObject *leapmotiondataObject = json_object_get_object_member(object, "leapmotiondata");
            JsonObject *frameObject          = json_object_get_object_member(leapmotiondataObject, "frame");

            frameID         = json_object_get_int_member(frameObject, "id");
            timestamp       = json_object_get_int_member(frameObject, "timestamp");
            numHand         = json_object_get_int_member(frameObject, "hand");
            extendedFingers = json_object_get_int_member(frameObject, "extendedFingers");
            tools           = json_object_get_int_member(frameObject, "tools");
            gestures        = json_object_get_int_member(frameObject, "gestures");

            //dlog_print(DLOG_DEBUG, "SRBD", "Size: %d", json_object_get_size(leapmotiondataObject));
            // This list will contain all the names of objects under "data"

            JsonArray *handsArray = json_object_get_array_member(frameObject, "hands");

            for (int i             = 0; i < numHand; i++) {
                std::string handName;
                int         handID;
                int         numFinger;
                float       pitch, roll, yaw;

                JsonNode   *handNode   = json_array_get_element(handsArray, i);
                JsonObject *handObject = json_node_get_object(handNode);

                handName = json_object_get_string_member(handObject, "hand");
                handID   = json_object_get_int_member(handObject, "id");

                JsonObject *palmPositionObject = json_object_get_object_member(handObject, "palmPosition");
                glm::vec3    palmPosition(json_object_get_double_member(palmPositionObject, "x"),
                                        json_object_get_double_member(palmPositionObject, "y"),
                                        json_object_get_double_member(palmPositionObject, "z"));

                pitch = json_object_get_double_member(handObject, "pitch");
                roll  = json_object_get_double_member(handObject, "roll");
                yaw   = json_object_get_double_member(handObject, "yaw");

                JsonObject *armDirectionObject = json_object_get_object_member(handObject, "armDirection");
                glm::vec3    armDirection(json_object_get_double_member(armDirectionObject, "x"),
                                        json_object_get_double_member(armDirectionObject, "y"),
                                        json_object_get_double_member(armDirectionObject, "z"));

                JsonObject *wristPositionObject = json_object_get_object_member(handObject, "wristPosition");
                glm::vec3    wristPosition(json_object_get_double_member(wristPositionObject, "x"),
                                         json_object_get_double_member(wristPositionObject, "y"),
                                         json_object_get_double_member(wristPositionObject, "z"));

                JsonObject *elbowPositionObject = json_object_get_object_member(handObject, "elbowPosition");;
                glm::vec3 elbowPosition(json_object_get_double_member(elbowPositionObject, "x"),
                                      json_object_get_double_member(elbowPositionObject, "y"),
                                      json_object_get_double_member(elbowPositionObject, "z"));

                JsonArray *fingersArray = json_object_get_array_member(handObject, "fingers");
                numFinger = json_array_get_length(fingersArray);

                for (int j                    = 0; j < numFinger; j++) {
                    std::string fingerName;
                    int         fingerID;
                    float       length;
                    float       width;
                    int         numBone;

                    JsonNode   *fingerNode   = json_array_get_element(fingersArray, j);
                    JsonObject *fingerObject = json_node_get_object(fingerNode);

                    fingerName = json_object_get_string_member(fingerObject, "finger");
                    fingerID   = json_object_get_int_member(fingerObject, "id");
                    length     = json_object_get_double_member(fingerObject, "length");
                    width      = json_object_get_double_member(fingerObject, "width");

                    JsonArray *bonesArray = json_object_get_array_member(fingerObject, "bones");
                    numBone = json_array_get_length(bonesArray);

                    for (int t                         = 0; t < numBone; t++) {
                        std::string boneName;

                        JsonNode   *boneNode   = json_array_get_element(bonesArray, t);
                        JsonObject *boneObject = json_node_get_object(boneNode);

                        boneName = json_object_get_string_member(boneObject, "bone");

                        JsonObject *startObject = json_object_get_object_member(boneObject, "start");
                        glm::vec3    start(json_object_get_double_member(startObject, "x"),
                                         json_object_get_double_member(startObject, "y"),
                                         json_object_get_double_member(startObject, "z"));

                        JsonObject *endObject = json_object_get_object_member(boneObject, "end");
                        glm::vec3    end(json_object_get_double_member(endObject, "x"),
                                       json_object_get_double_member(endObject, "y"),
                                       json_object_get_double_member(endObject, "z"));

                        JsonObject *directionObject = json_object_get_object_member(boneObject, "direction");
                        glm::vec3    direction(json_object_get_double_member(directionObject, "x"),
                                             json_object_get_double_member(directionObject, "y"),
                                             json_object_get_double_member(directionObject, "z"));

                        mFrame.hands[i].fingers[j].bones[t].name      = boneName;
                        mFrame.hands[i].fingers[j].bones[t].start     = start;
                        mFrame.hands[i].fingers[j].bones[t].end       = end;
                        mFrame.hands[i].fingers[j].bones[t].direction = direction;
                    }
                    mFrame.hands[i].fingers[j].name    = fingerName;
                    mFrame.hands[i].fingers[j].id      = fingerID;
                    mFrame.hands[i].fingers[j].length  = length;
                    mFrame.hands[i].fingers[j].width   = width;
                    mFrame.hands[i].fingers[j].numBone = numBone;
                }
                mFrame.hands[i].name          = handName;
                mFrame.hands[i].id            = handID;
                mFrame.hands[i].palmPosition  = palmPosition;
                mFrame.hands[i].pitch         = pitch;
                mFrame.hands[i].yaw           = yaw;
                mFrame.hands[i].roll          = roll;
                mFrame.hands[i].armDirection  = armDirection;
                mFrame.hands[i].wristPosition = wristPosition;
                mFrame.hands[i].elbowPosition = elbowPosition;
                mFrame.hands[i].numFinger     = numFinger;
            }
            mFrame.id              = frameID;
            mFrame.timestamp       = timestamp;
            mFrame.extendedFingers = extendedFingers;
            mFrame.tools           = tools;
            mFrame.gestures        = gestures;
            mFrame.numHand         = numHand;
        }

    } else {
        mInfo.append("No hands detected\n");
    }

    return true;
}


