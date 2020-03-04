/*
 *	BallThrow.h
 *
 *	Created on:			Jan 10, 2018
 *	Last Updated on:	Feb 07, 2018
 *
 *      Author: Paul Christopher Gloumeau
 */

#ifndef MODEL3DVIEW_BALLTHROW_H
#define MODEL3DVIEW_BALLTHROW_H

#include <memory>
#include <dali-toolkit/dali-toolkit.h>
#include <sstream>
#include <storage.h>
#include <fstream>
#include <chrono>
typedef std::chrono::high_resolution_clock Clock;

#include "../leapmotion/Frame.h"
#include "../leapmotion/LeapData.h"
#include "../leapmotion/Receiver.h"
#include "../objects/PhysicsObject.h"
#include "../objects/BoxObject.h"
#include "../objects/SphereObject.h"
#include "../interaction/Interaction.h"

#include "bullet/DynamicsWorld.h"

class PhysicsObject;
class LeapData;

class BallThrow : public Dali::ConnectionTracker {
public:
    explicit BallThrow(Dali::Application &application);

    ~BallThrow() override = default;

    void Create(Dali::Application &application);

    void Terminate(Dali::Application &application);

    bool Update();

    void OnKeyEvent(const Dali::KeyEvent &event);

private:
    Dali::Application                            &mApplication;
    Dali::Timer                                  mTimer;
    Dali::Layer                                  m3DLayer;
    Dali::Toolkit::TextLabel                     mLabel;
    std::vector<std::shared_ptr<PhysicsObject> > mJointList;
    std::vector<std::shared_ptr<PhysicsObject> > mObjList;
    std::shared_ptr<DynamicsWorld>               mWorld;
    std::shared_ptr<LeapData>                    mLeapData;
    Frame										 mPrevFrame;
    glm::vec3									 mOffset;
    std::ofstream 								 mFile;
    std::string									 mFileName;
    int 										 mAttempts;
    Clock::time_point							 mAttemptTime;
    Clock::time_point							 mTotalTime;
    bool										 mEOTest;
    std::string GestureInterpret(char);
    void SetOffset(glm::vec3);
};


#endif //MODEL3DVIEW_BALLTHROW_H
