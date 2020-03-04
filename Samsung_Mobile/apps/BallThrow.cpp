/*
 *	BallThrow.cpp
 *
 *	Created on:			Jan 10, 2018
 *	Last Updated on:	Feb 07, 2018
 *
 *      Author: Paul Christopher Gloumeau
 */

#include "BallThrow.h"

using namespace Dali;
using namespace Dali::Toolkit;

BallThrow::BallThrow(Dali::Application &application)
        : mApplication(application)
        , mTimer(Dali::Timer::New(1000 / 60))
		, mWorld()
        , mLeapData(std::make_shared<LeapData>()) {
    mApplication.InitSignal().Connect(this, &BallThrow::Create);
    mApplication.TerminateSignal().Connect(this, &BallThrow::Terminate);
    mTimer.TickSignal().Connect(this, &BallThrow::Update);
    mAttempts = 1;
    mEOTest = false;
}

void BallThrow::Create(Dali::Application &application) {
    // Get a handle to the stage
    Stage stage = Stage::GetCurrent();

    // Add 3D model Layer
    // 3D models require 3D based rendering method, so it can use depth buffer, etc.
    m3DLayer = Layer::New();
    stage.GetRootLayer().Add(m3DLayer);
    m3DLayer.SetBehavior(Layer::LAYER_3D);
    m3DLayer.SetParentOrigin(ParentOrigin::CENTER);
    m3DLayer.SetAnchorPoint(AnchorPoint::CENTER);

    mFileName = "SangBin1";
	char* path;
	storage_get_directory(0, STORAGE_DIRECTORY_DOWNLOADS, &path);
	mFile.open ((std::string(path) + "/" + mFileName + ".txt").c_str());
	mFile << "Test: " << mFileName << "\n\n";
	mTotalTime = Clock::now();
	mAttemptTime = Clock::now();

    std::shared_ptr<PhysicsObject> newobj;

    //create hands
    //left palm
    mJointList.push_back(newobj = std::make_shared<BoxObject>(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, Vector3::ONE * 40.f));
    (*newobj).setMaterial("cube.obj.txt", "joint_left.mtl");
    m3DLayer.Add(newobj->getModel());

    //left finger joints
    for (int i = 0 ; i < 15 ; ++i){
        mJointList.push_back(newobj = std::make_shared<BoxObject>(glm::vec3(0.0f, 700.0f, 0.0f), 0.0f, Vector3::ONE * 20.f));
        (*newobj).setMaterial("cube.obj.txt", "joint_left.mtl");
        m3DLayer.Add(newobj->getModel());
    }

    //right palm
    mJointList.push_back(newobj = std::make_shared<BoxObject>(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, Vector3::ONE * 40.f));
   (*newobj).setMaterial("cube.obj.txt", "joint_right.mtl");
   m3DLayer.Add(newobj->getModel());

   //right finger joints
   for (int i = 0 ; i < 15 ; ++i){
	   mJointList.push_back(newobj = std::make_shared<BoxObject>(glm::vec3(0.0f, 700.0f, 0.0f), 0.0f, Vector3::ONE * 20.f));
	   (*newobj).setMaterial("cube.obj.txt", "joint_right.mtl");
	   m3DLayer.Add(newobj->getModel());
   }

    //create objects

	mObjList.push_back(newobj = std::make_shared<BoxObject>(glm::vec3(0.0f, -1800.0f, 0.0f),
																0.0f,
																Vector3::ONE * 2600.f));
		(*newobj).setMaterial("cube.obj.txt", "cube.mtl")
				 .setRestitution(1.0f);

	mObjList.push_back(newobj = std::make_shared<BoxObject>(glm::vec3(0.0f, -650.0f, -1050.0f),
																0.0f,
																Vector3::ONE * 500.f));
		(*newobj).setMaterial("cube.obj.txt", "cube1.mtl")
				 .setRestitution(1.0f);

	mObjList.push_back(newobj = std::make_shared<BoxObject>(glm::vec3(0.0f, -650.0f, 250.0f),
																	0.0f,
																	Vector3::ONE * 500.f));
			(*newobj).setMaterial("cube.obj.txt", "cube1.mtl")
					 .setRestitution(1.0f);

	mObjList.push_back(newobj = std::make_shared<BoxObject>(glm::vec3(0.0f, -350.0f, -1050.0f),
															1.0f,
															Vector3::ONE * 100.f));
	(*newobj).setMaterial("cube.obj.txt", "cube2.mtl")
			 .setRestitution(0.0f);

	mObjList.push_back(newobj = std::make_shared<BoxObject>(glm::vec3(-150.0f, -350.0f, -1050.0f),
															1.0f,
															Vector3::ONE * 100.f));
	(*newobj).setMaterial("cube.obj.txt", "cube2.mtl")
			 .setRestitution(0.0f);

	mObjList.push_back(newobj = std::make_shared<BoxObject>(glm::vec3(150.0f, -350.0f, -1050.0f),
																1.0f,
																Vector3::ONE * 100.f));
	(*newobj).setMaterial("cube.obj.txt", "cube2.mtl")
			 .setRestitution(0.0f);

	mObjList.push_back(newobj = std::make_shared<BoxObject>(glm::vec3(-75.0f, -250.0f, -1050.0f),
															1.0f,
															Vector3::ONE * 100.f));
	(*newobj).setMaterial("cube.obj.txt", "cube2.mtl")
			 .setRestitution(0.0f);

	mObjList.push_back(newobj = std::make_shared<BoxObject>(glm::vec3(75.0f, -250.0f, -1050.0f),
																1.0f,
																Vector3::ONE * 100.f));
		(*newobj).setMaterial("cube.obj.txt", "cube2.mtl")
				 .setRestitution(0.0f);

	mObjList.push_back(newobj = std::make_shared<BoxObject>(glm::vec3(0.0f, -150.0f, -1050.0f),
																1.0f,
																Vector3::ONE * 100.f));
		(*newobj).setMaterial("cube.obj.txt", "cube2.mtl")
				 .setRestitution(0.0f);

	mObjList.push_back(newobj = std::make_shared<SphereObject>(glm::vec3(0.0f, -300.0f, 250.0f), //pos
																   1.0f, //mass
																   100.0f)); //size: radius
		(*newobj).setLinearVelocity(0.0f, 0.0f, 0.0f)
				 .setMaterial("sphere.obj", "tetrahedron.mtl")
				 .setRestitution(0.7f);

	mWorld = std::make_shared<DynamicsWorld>();
    mWorld->setGravity(glm::vec3(0.0f, -500.0f, 0.0f));

    for (auto &obj : mObjList) {
		m3DLayer.Add(obj->getModel());
		mWorld->addObject(obj->getShape().get());
	}

    mWorld->addIndexFinger(0, mJointList[4]->getShape().get());
    mWorld->addIndexFinger(1, mJointList[20]->getShape().get());

    mLabel = TextLabel::New();
    mLabel.SetProperty(TextLabel::Property::TEXT, "THIS\nMESSAGE\nSHOULD\nDISAPPEARE!");
    mLabel.SetProperty(TextLabel::Property::TEXT_COLOR, Color::RED);
    mLabel.SetAnchorPoint(AnchorPoint::TOP_LEFT);
    mLabel.SetResizePolicy(ResizePolicy::FILL_TO_PARENT, Dimension::WIDTH);
    mLabel.SetResizePolicy(ResizePolicy::FILL_TO_PARENT, Dimension::HEIGHT);
    mLabel.SetProperty(TextLabel::Property::MULTI_LINE, true);
    stage.GetRootLayer().Add(mLabel);

    stage.KeyEventSignal().Connect(this, &BallThrow::OnKeyEvent);

    KuMedia::Receiver::Instance()->run();
    mTimer.Start();
}

void BallThrow::Terminate(Dali::Application &application) {

}


bool BallThrow::Update()
{
	mEOTest = true;
    std::stringstream stream("");		//Displays on the screen

    int	gesture				= 0;
    glm::vec3 pose			= glm::vec3(0.0f, 700.0f, 0.0f);
	glm::vec3 lVelocity		= glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotationAngle	= glm::vec3(0.0f, 0.0f, 0.0f);

    std::string msg = KuMedia::Receiver::Instance()->getMsg();		//Get information from leapmotion
	mLeapData->parseLeapFrame(msg);									//parse information into frame class

	bool hand_detected = not(
			KuMedia::Receiver::Instance()->gesture_left == 'a' and
			KuMedia::Receiver::Instance()->gesture_right == 'a'
	);
	if(hand_detected)
	{
		for(int i = 0; i < mLeapData->mFrame.numHand; i++)			//loop for each hand recognized
		{
			auto& handInfo = mLeapData->mFrame.hands[i];
			int hand;
			std::string material;
			if (handInfo.name == "Left"){							//determine which hand
				hand = 0;
				material = "joint_left.mtl";
			}
			else {
				hand = 1;
				material = "joint_right.mtl";
			}

			//set the joint positions
			std::vector<glm::vec3> joint_poses
			{
				handInfo.palmPosition,
				handInfo.fingers[4].bones[3].end,
				handInfo.fingers[3].bones[3].end,
				handInfo.fingers[2].bones[3].end,
				handInfo.fingers[1].bones[3].end,
				handInfo.fingers[0].bones[3].end,
				handInfo.fingers[4].bones[2].end,
				handInfo.fingers[3].bones[2].end,
				handInfo.fingers[2].bones[2].end,
				handInfo.fingers[1].bones[2].end,
				handInfo.fingers[0].bones[2].end,
				handInfo.fingers[4].bones[1].end,
				handInfo.fingers[3].bones[1].end,
				handInfo.fingers[2].bones[1].end,
				handInfo.fingers[1].bones[1].end,
				handInfo.fingers[0].bones[1].end
			};

			//if only one hand recognized
			if(mLeapData->mFrame.numHand < 2)
			{
				int otherHand = hand == 0 ? 1 : 0;

				if(mPrevFrame.hands[otherHand].palmPosition != pose)		//if not already in the default position?
				{
					for (int j = 0; j < joint_poses.size(); j++)
					{
						//set positions for unrecognized hand to the default in dynamic world
						mJointList[j + (otherHand * joint_poses.size())]->getShape()->getWorldTransform()->setOrigin(pose);
						mWorld->updateIndexFinger(otherHand, gesture, pose, lVelocity, rotationAngle);
						mJointList[j + (otherHand * joint_poses.size())]->getShape()->setSelected(mJointList[4 + (otherHand * joint_poses.size())]->getShape()->isSelected());
						mJointList[j + (otherHand * joint_poses.size())]->update("joint_left.mtl");
						mPrevFrame.hands[otherHand].palmPosition = pose;
					}
				}
			}

			SetOffset(joint_poses[0]);
			//set positions for this hand
			for (int j = 0; j < joint_poses.size(); j++)
			{
				 pose = (joint_poses[j] * glm::vec3(1.0f, -1.0f, 1.0f)) + mOffset;

				//update positions in dynamic world
				mJointList[j + (hand * joint_poses.size())]->getShape()->getWorldTransform()->setOrigin(pose * glm::vec3(1.0f, -1.0f, 1.0f));
				mJointList[j + (hand * joint_poses.size())]->getShape()->setSelected(mJointList[4 + (hand * joint_poses.size())]->getShape()->isSelected());
				mJointList[j + (hand * joint_poses.size())]->update(material);
			}

			//set the gesture, velocity and rotationAngle
			gesture = hand == 0 ? KuMedia::Receiver::Instance()->gesture_left - '0' : KuMedia::Receiver::Instance()->gesture_right - '0';
			lVelocity = (handInfo.fingers[1].bones[3].end - mPrevFrame.hands[hand].fingers[1].bones[3].end) * 40.0f;	//calculate the linear velocity
			rotationAngle = glm::vec3((handInfo.pitch - mPrevFrame.hands[hand].pitch),
									  (handInfo.yaw - mPrevFrame.hands[hand].yaw),
									  (handInfo.roll - mPrevFrame.hands[hand].roll));						//calculate the rotation angle
			rotationAngle = rotationAngle/150.0f;															//scale the rotation angle

			//update the collision information
			mWorld->updateIndexFinger(hand, gesture, mJointList[4 + (hand * joint_poses.size())]->getShape()->getWorldTransform()->getOrigin(), lVelocity, rotationAngle);

			//update previous frame information
			mPrevFrame.hands[hand] = handInfo;
		}

		mWorld->updateScale();								//calculate the scaling factor

//		stream	<< "LHand:" << "\t"
//				<< GestureInterpret(KuMedia::Receiver::Instance()->gesture_left) << "\n"
//				<< "RHand:" << "\t"
//				<< GestureInterpret(KuMedia::Receiver::Instance()->gesture_right) << "\n";
	}
	else		//if no new information
	{
		for(int i = 0; i < 2; i++)								//for both hands
		{
			if(mPrevFrame.hands[i].palmPosition != pose)		//if not already in the default position?
			{
				for (int j = 0; j < 16; j++)
				{
					mJointList[(i * 16) + j]->getShape()->getWorldTransform()->setOrigin(pose);
					mJointList[(i * 16) + j]->getShape()->setSelected(false);
					mJointList[(i * 16) + j]->update("joint_left.mtl");
				}
				mWorld->updateIndexFinger(i, gesture, pose, lVelocity, rotationAngle);
				mPrevFrame.hands[i].palmPosition = pose;
			}
		}
		stream << "No Hands Detected\n";
	}

	mWorld->stepSimulation(1 / 60.0f);

	//update objects
	for (auto &obj : mObjList)
	{
		obj->update();
		if(obj->getShape()->getWorldTransform()->getOrigin().y > - 300)
		{
			mEOTest = false;
		}
	}

	if(mEOTest)
	{
		mFile << "Attempt " << mAttempts << ": \n";
		mFile << "Time: " <<  std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - mAttemptTime).count() << "\n";
		mFile << "\nNumber of Attempts: " << mAttempts << "\n";
		mFile << "Total Time: " <<  std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - mTotalTime).count() << "\n";
		mFile.close();
		mApplication.Quit();
	}

//	stream << "Time: " <<  std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - mClock).count() << "\n";
//	stream << "Attempts: " << mAttempts << "\n";
    mLabel.SetProperty(TextLabel::Property::TEXT, stream.str().c_str());
    return true;
}

void BallThrow::SetOffset(glm::vec3 leapPos)
{
	glm::vec3 scaleFactor = glm::vec3(1.0f, 3.0f, 1.0f);

	mOffset = leapPos * glm::vec3(1.0f, -1.0f, 1.0f);
	mOffset = mOffset + glm::vec3(0.0f, 260.0f, 250.0f);
	mOffset = mOffset * scaleFactor;
	mOffset = mOffset - (leapPos * glm::vec3(1.0f, -1.0f, 1.0f));
}

std::string BallThrow::GestureInterpret(char c)
{
	if ((c - '0') == 0)
		return "default";
	if ((c - '0') == 1)
		return "pinch";
	if ((c - '0') == 2)
		return "spread";
	if ((c - '0') == 3)
		return "point";
	else
		return "missing";
}

void BallThrow::OnKeyEvent(const Dali::KeyEvent &event) {
    if (event.state == KeyEvent::Down) {

        if (IsKey(event, Dali::DALI_KEY_BACK)) {
        	mFile.close();
            mApplication.Quit();
        }

        if (event.keyPressed == " ")
        {
        	mFile << "Attempt " << mAttempts << ": \n";
			mFile << "Time: " <<  std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - mAttemptTime).count() << "\n";
			mAttemptTime = Clock::now();
			mAttempts += 1;

        	for (auto &obj : mObjList)
			{
				if(obj->getShape()->getShapeType() == SPHERE_SHAPE)
				{
					obj->getShape()->setLinearVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
					obj->getShape()->setAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
					obj->getShape()->getWorldTransform()->setOrigin(glm::vec3(0.0f, -300.0f, 250.0f));
				}
			}
        }

        if (IsKey(event, Dali::DALI_KEY_SHIFT_LEFT))
		{
        	mAttemptTime = Clock::now();
		}
    }
}



