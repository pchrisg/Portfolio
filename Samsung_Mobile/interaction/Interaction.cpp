/*
 *	Interaction.cpp
 *
 *	Created on:			Apr 19, 2017
 *	Last Updated on:	Feb 07, 2018
 *
 *      Author: Paul Christopher Gloumeau
 */

#include "Interaction.h"

/*
							INITIALIZATION
*/

/*
Name: Interaction (Constructor)
Purpose: Innitializes the variables for the Interaction class
*/
Interaction::Interaction()
{
	for(int i = 0; i < 2; i++)
	{
		mHand[i].mInteracting	= false;
		mHand[i].mSelected		= false;
		mHand[i].mGesture		= 0;
		mHand[i].mIndexFinger	= NULL;
		mHand[i].mObject		= NULL;
		mHand[i].mOffset		= glm::vec3(0.0f, 0.0f, 0.0f);
		mHand[i].mPrevPos		= glm::vec3(0.0f, 0.0f, 0.0f);
		mHand[i].mLVelocity		= glm::vec3(0.0f, 0.0f, 0.0f);
		mHand[i].mRotationAngle	= glm::vec3(0.0f, 0.0f, 0.0f);
	}

	errorMessage = "";
}//Interaction

/*
							ACCESS FUNCTIONS
 */

void Interaction::SetGesture(int hand, int gesture)
{
	if (mHand[hand].mGesture != gesture)
	{
		mHand[hand].mGesture = gesture;

		setInteracting(hand, false);
	}
}//setGesture

void Interaction::SetInteracting(int hand, bool status)
{
	mHand[hand].mInteracting = status;

	if(!status)
	{
		if(mHand[hand].mObject != NULL)
		{
			mHand[hand].mObject->setScale(1.0f);
			mHand[hand].mObject = NULL;
			mHand[hand].mSelected = false;
		}
	}
}

void Interaction::SetOffset(int hand)
{
	mHand[hand].mOffset = mHand[hand].mObject->getWorldTransform()->getOrigin() - mHand[hand].mIndexFinger->getWorldTransform()->getOrigin();
}//setOffset

void Interaction::SetObject(int hand, CollisionShape* object)
{
	mHand[hand].mObject = object;
	setOffset(hand);
}//setObject

void Interaction::SetScale()
{
	float prevSize = 0.0f;
	prevSize += glm::abs(mHand[0].mPrevPos.x - mHand[1].mPrevPos.x);
	prevSize += glm::abs(mHand[0].mPrevPos.y - mHand[1].mPrevPos.y);

	float curSize = 0.0f;
	curSize += glm::abs(mHand[0].mIndexFinger->getWorldTransform()->getOrigin().x - mHand[1].mIndexFinger->getWorldTransform()->getOrigin().x);
	curSize += glm::abs(mHand[0].mIndexFinger->getWorldTransform()->getOrigin().y - mHand[1].mIndexFinger->getWorldTransform()->getOrigin().y);

	mHand[0].mPrevPos = mHand[0].mIndexFinger->getWorldTransform()->getOrigin();
	mHand[1].mPrevPos = mHand[1].mIndexFinger->getWorldTransform()->getOrigin();

	mScale = curSize/prevSize;
}//setScale

/*
							MAIN FUNCTION
*/

/*
Name: handleInteractions
Purpose: Controls the flow of the Interaction class
Input:
output:
*/
void Interaction::HandleInteractions()
{
	switch (GetInteractions()){
		case 0:
			errorMessage = "handleInteractions: No interaction recognized";
			break;
		case 1:				//two hands interacting on same object
			Interact();
			break;
		case 2:				//two hands interacting on different objects
			Interact(0);
			Interact(1);
			break;
		case 3:				//left hand interaction
			Interact(0);
			break;
		case 4:				//right hand interaction
			Interact(1);
			break;
		default:
			errorMessage = "Undefined gesture: interact";
	}//end switch
}//handleInteractions

/*
							INTERACTION PROCESSING
*/

/*
Name: getInteractions
Purpose: To determine which gestures will result in interactions with objects
Input:
output: An integer value which represents where interactions will take place
*/
int Interaction::GetInteractions()
{
	bool leftHand  = false;
	bool rightHand = false;

	if ((mHand[0].mGesture > 0) && (mHand[0].mObject != NULL)) leftHand  = true;	//left hand interaction
	if ((mHand[1].mGesture > 0) && (mHand[1].mObject != NULL)) rightHand = true;	//right hand interaction

	if (!leftHand && !rightHand) return 0;			//no interactions

	if (leftHand && rightHand)						//two interactions
	{
		if (mHand[0].mObject == mHand[1].mObject)	//interacting on the same object
			return 1;
		else										//interacting on different objects
			return 2;
	}												//one interaction
	if (leftHand)									//left hand interaction
		return 3;
	else											//right hand interaction
		return 4;
}//getInteractions

/*
Name:
Purpose:
Input:
output:
*/
void Interaction::Interact(int hand)
{
	// 0 = open hand
	// 1 = pinch
	// 2 = pinch spread
	// 3 = point
    switch (mHand[hand].mGesture){
        case 2:
        	if(!mHand[hand].mObject->isSelected()) //break if object isn't selected
        		break;

			mHand[hand].mObject->setLinearVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
			mHand[hand].mObject->setAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
			Rotate(hand);
			Translate(hand);
            break;
        case 3:
        	if(!mHand[hand].mSelected)
        		Select(hand);
            break;
        default:
            errorMessage = "Undefined gesture: interact";
    }//end switch
}//interact

/*
Name:
Purpose:
Input:
output:
*/
void Interaction::Interact()
{
	int lHand = 0;
	int rHand = 1;

	// 0 = open hand
	// 1 = pinch
	// 2 = pinch spread
	// 3 = point
	switch (mHand[lHand].mGesture) //left hand gesture switch
	{
		case 2:
			if(!mHand[lHand].mObject->isSelected()) //break if object isn't selected
				break;

			mHand[lHand].mObject->setLinearVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
			mHand[lHand].mObject->setAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));

			switch (mHand[rHand].mGesture) //right hand gesture switch
			{
				case 2:
					Scale();
					break;
				default:
					SetInteracting(rHand, false);
					Rotate(lHand);
					Translate(lHand);
			}
			break;

		case 3:
			switch (mHand[rHand].mGesture) //right hand gesture switch
			{
				case 2:
					if(!mHand[rHand].mObject->isSelected()) //break if object isn't selected
						break;

					mHand[rHand].mObject->setLinearVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
					mHand[rHand].mObject->setAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));

					SetInteracting(lHand, false);
					Rotate(rHand);
					Translate(rHand);
					break;
				default:
					SetInteracting(rHand, false);
					if(!mHand[lHand].mSelected)
						Select(lHand);
					break;
			}
			break;

		default:
			switch (mHand[rHand].mGesture) //right hand gesture switch
			{
				case 2:
					if(!mHand[rHand].mObject->isSelected()) //break if object isn't selected
						break;

					mHand[rHand].mObject->setLinearVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
					mHand[rHand].mObject->setAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));

					SetInteracting(lHand, false);
					Rotate(rHand);
					Translate(rHand);
					break;
				case 3:
					SetInteracting(lHand, false);
					if(!mHand[lHand].mSelected)
						Select(rHand);
					break;
				default:
					errorMessage = "Undefined gestures: 2 hand interact";
			}
	}
}//interact

/*
							SELECT
*/

void Interaction::Select(int hand)
{
	if (!mHand[hand].mObject->isSelected())
		mHand[hand].mObject->setSelected(true);

	else
		mHand[hand].mObject->setSelected(false);

	mHand[hand].mSelected = true;
}//select

/*
							TRANSFORAMTIONS
*/

/*
Name: translate
Purpose: calculates the translation and updates the world transformation matrix
Input:
output:
*/
void Interaction::Translate(int hand)
{
	mHand[hand].mObject->setLinearVelocity(mHand[hand].mLVelocity);
	mHand[hand].mObject->getWorldTransform()->setOrigin(mHand[hand].mIndexFinger->getWorldTransform()->getOrigin() + mHand[hand].mOffset);
}//translate


/*
Name: rotate
Purpose: calculates the rotation and updates the world transformation matrix
Input:
output:
*/
void Interaction::Rotate(int hand)
{
	glm::mat3 xRotation = glm::mat3(1.0f,				   0.0f,				   0.0f,
									0.0f, cos(mHand[hand].mRotationAngle.x), -sin(mHand[hand].mRotationAngle.x),
									0.0f, sin(mHand[hand].mRotationAngle.x),  cos(mHand[hand].mRotationAngle.x));

	glm::mat3 yRotation = glm::mat3( cos(mHand[hand].mRotationAngle.y), 0.0f, sin(mHand[hand].mRotationAngle.y),
													  0.0f, 1.0f,				   0.0f,
									-sin(mHand[hand].mRotationAngle.y), 0.0f, cos(mHand[hand].mRotationAngle.y));

	glm::mat3 zRotation = glm::mat3(cos(mHand[hand].mRotationAngle.z), -sin(mHand[hand].mRotationAngle.z), 0.0f,
									sin(mHand[hand].mRotationAngle.z),  cos(mHand[hand].mRotationAngle.z), 0.0f,
													 0.0f, 					 0.0f, 1.0f);

	mHand[hand].mObject->getWorldTransform()->setBasis(zRotation * yRotation * xRotation * mHand[hand].mObject->getWorldTransform()->getBasis());
}//rotate


/*
Name: scale
Purpose: calculates the scaling and updates the world transformation matrix
Input:
output:
*/
void Interaction::Scale()
{
	mHand[0].mObject->scale(mScale);
	mHand[0].mObject->setScale(mScale);
}//scale
