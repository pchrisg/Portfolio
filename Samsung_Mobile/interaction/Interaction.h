/*
 *	Interaction.h
 *
 *	Created on:			Apr 19, 2017
 *	Last Updated on:	Feb 07, 2018
 *
 *      Author: Paul Christopher Gloumeau
 */

#ifndef INTERACTION_H_
#define INTERACTION_H_

#include "../../inc/bullet/CollisionShape.h"
#include <string>
#include <glm/glm.hpp>
#include <math.h>

class Interaction {

private:
	std::string errorMessage;
	float mScale;

	struct Hand
	{
		bool mInteracting;
		bool mSelected;
		int mGesture;
		CollisionShape* mIndexFinger;
		CollisionShape* mObject;
		glm::vec3 mOffset;
		glm::vec3 mPrevPos;
		glm::vec3 mLVelocity;
		glm::vec3 mRotationAngle;
	} mHand[2];

	bool ValidInput();
	int GetInteractions();

protected:
	void Interact(int hand);
	void Interact();
	void Translate(int hand);
	void Rotate(int hand);
	void Scale();
	void Select(int hand);

public:
	Interaction();
	void SetInteracting(int hand, bool status);
	void SetGesture(int hand, int gesture);
	void SetIndexFinger(int hand, CollisionShape* indexFinger)	{mHand[hand].mIndexFinger = indexFinger;};
	void SetObject(int hand, CollisionShape* object);
	void SetOffset(int hand);
	void SetLVelocity(int hand, glm::vec3 lVelocity)			{mHand[hand].mLVelocity = (mHand[hand].mLVelocity + lVelocity) / 2.0f;};
	void SetRotationAngle(int hand, glm::vec3 rotationAngle)	{mHand[hand].mRotationAngle = rotationAngle;};
	void SetScale();

	int GetInteracting(int hand)								{return mHand[hand].mInteracting;};
	int GetGesture(int hand)									{return mHand[hand].mGesture;};
	CollisionShape* GetIndexFinger(int hand)					{return mHand[hand].mIndexFinger;};
	CollisionShape* GetObject(int hand)							{return mHand[hand].mObject;};

	void HandleInteractions();
};//Interaction

#endif /* INTERACTION_H_ */
