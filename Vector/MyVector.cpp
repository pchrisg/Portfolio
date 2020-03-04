#include <iostream>
#include "MyVector.h"

//Constructors, Operators and Destructor
MyVector::MyVector() : MyVector(0) { }

MyVector::MyVector(const int capacity) : _vectorSize(0), _vectorCapacity(capacity)
{
	//std::cout << "Perameterised Constructor" << std::endl;

	_vectorObjects = new MyObject[_vectorCapacity]{};
}

MyVector::MyVector(const MyVector& other)
{
	//std::cout << "Copy Constructor" << std::endl;

	_vectorCapacity = other._vectorCapacity;
	_vectorSize = other._vectorSize;

	_vectorObjects = new MyObject[_vectorCapacity]{};
	for (auto i = 0; i < _vectorSize; ++i)
		_vectorObjects[i] = other._vectorObjects[i];

	return;
}

void MyVector::operator=(const MyVector& other)
{
	//std::cout << "Assignment Operator" << std::endl;

	_vectorCapacity = other._vectorCapacity;
	_vectorSize = other._vectorSize;

	if (_vectorObjects != nullptr)
		delete[] _vectorObjects;

	_vectorObjects = new MyObject[_vectorCapacity]{};
	for (auto i = 0; i < _vectorSize; ++i)
		_vectorObjects[i] = other._vectorObjects[i];

	return;
}

MyObject& MyVector::operator[](size_t index)
{
	if (index >= _vectorSize)
		throw "Error: index out of range \n";

	return _vectorObjects[index];
}

MyVector::~MyVector()
{
	delete[] _vectorObjects;
}

//Getters
int MyVector::GetCapacity() const
{
	return _vectorCapacity;
}

int MyVector::GetSize() const
{
	return _vectorSize;
}

//Vector Functions
void MyVector::Add(int id)
{
	std::cout << "Adding: " << id << std::endl;
	if (_vectorSize == _vectorCapacity)
		Grow();

	_vectorObjects[_vectorSize]._id = id;
	_vectorSize++;
}

void MyVector::Grow()
{
	MyObject* tempPtr = _vectorObjects;

	_vectorCapacity = _vectorCapacity < 2 ?
		_vectorCapacity += 1 :
		_vectorCapacity *= 1.5;

	_vectorObjects = new MyObject[_vectorCapacity]{};
	for (auto i = 0; i < _vectorSize; ++i)
		_vectorObjects[i] = tempPtr[i];

	delete[] tempPtr;
}

void MyVector::TrimToSize()
{
	MyObject* tempPtr = _vectorObjects;

	_vectorCapacity = _vectorSize;

	_vectorObjects = new MyObject[_vectorCapacity]{};
	for (auto i = 0; i < _vectorSize; ++i)
		_vectorObjects[i] = tempPtr[i];

	delete[] tempPtr;
} 

void MyVector::RemoveAll(int myObjectId)
{
	unsigned int count = 0;
	for (auto i = 0; (i + count) < _vectorSize; ++i)
	{
		_vectorObjects[i] = _vectorObjects[i + count];

		if (_vectorObjects[i]._id == myObjectId)
		{
			++count;
			--i;
		}
	}
	_vectorSize -= count;
}

//Display
std::string MyVector::ToString() const
{
	std::string vectorString{};
	vectorString += "IDs: ";

	for (auto i = 0; i < _vectorSize; ++i)
	{
		vectorString += std::to_string(_vectorObjects[i]._id);
		vectorString += ", ";
	}
	vectorString += "\n";

	return vectorString;
}

//Utility Functions
MyObject* MyVector::FindById(int MyObjectId) const
{
	for (auto i = 0; i < _vectorSize; ++i)
	{
		if (_vectorObjects[i]._id == MyObjectId)
			return &_vectorObjects[i];
	}

	return nullptr;
}

MyVector* MyVector::GroupById(int* numGroups)
{
	*numGroups = 0;

	if (_vectorSize == 0)
		return nullptr;

	MyVector* tempVec = new MyVector[_vectorSize]{};
	tempVec[*numGroups].Add(_vectorObjects[*numGroups]._id);
	++* numGroups;

	for (auto i = 1; i < _vectorSize; ++i)
	{
		bool inGroup = false;
		for (auto j = 0; j < *numGroups; ++j)
		{
			if (tempVec[j][0]._id == _vectorObjects[i]._id)
			{
				tempVec[j].Add(_vectorObjects[i]._id);
				inGroup = true;
			}
		}
		if (!inGroup)
		{
			tempVec[*numGroups].Add(_vectorObjects[i]._id);
			++* numGroups;
		}
	}

	MyVector* groupVec = new MyVector[*numGroups]{};

	for (auto i = 0; i < *numGroups; ++i)
	{
		tempVec[i].TrimToSize();
		groupVec[i] = tempVec[i];
	}
	delete[] tempVec;

	return groupVec;
}
