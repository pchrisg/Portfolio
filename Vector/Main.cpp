#include "MyVector.h"
#include <iostream>
using std::cout;

int main() {
	cout << "Start Program\n";

	MyVector vec1, vec2(5);

	cout << "V1:  Size: " << vec1.GetSize() << " Capacity: " << vec1.GetCapacity() << "\n";
	cout << vec1.ToString();

	vec2.Add(1);
	vec2.Add(2);
	vec2.Add(3);
	cout << "V2:  Size: " << vec2.GetSize() << " Capacity: " << vec2.GetCapacity() << "\n";
	cout << vec2.ToString();

	vec1 = vec2;
	cout << "V1:  Size: " << vec1.GetSize() << " Capacity: " << vec1.GetCapacity() << "\n";
	cout << vec1.ToString();

	vec1.Add(4);

	MyVector vec3(vec1);

	try {
		vec3[10]._id = 5;
	}
	catch (const char * e) {
		cout << e;
	}
	
	vec3[0]._id = 5;

	cout << "V3:  Size: " << vec3.GetSize() << " Capacity: " << vec3.GetCapacity() << "\n";
	cout << vec3.ToString();

	MyVector vec4(vec3);
	cout << "V4:  Size: " << vec4.GetSize() << " Capacity: " << vec4.GetCapacity() << "\n";
	vec4.Add(6);
	cout << "V4:  Size: " << vec4.GetSize() << " Capacity: " << vec4.GetCapacity() << "\n";
	vec4.Add(7);
	cout << "V4:  Size: " << vec4.GetSize() << " Capacity: " << vec4.GetCapacity() << "\n";
	vec4.Add(8);
	cout << "V4:  Size: " << vec4.GetSize() << " Capacity: " << vec4.GetCapacity() << "\n";
	vec4.Add(9);
	cout << "V4:  Size: " << vec4.GetSize() << " Capacity: " << vec4.GetCapacity() << "\n";
	cout << vec4.ToString();

	
	try {
		vec4[2]._id = 5;
	}
	catch (const char* e) {
		cout << e;
	}
	try {
		vec4[4]._id = 5;
	}
	catch (const char* e) {
		cout << e;
	}
	vec4.RemoveAll(5);
	vec4.TrimToSize();
	cout << "V4:  Size: " << vec4.GetSize() << " Capacity: " << vec4.GetCapacity() << "\n";
	cout << vec4.ToString();

	MyObject* pMyObject = vec4.FindById(8);
	if (pMyObject != nullptr)
	{
		pMyObject->_id = 3;
		pMyObject = nullptr;
	}
	delete pMyObject;

	cout << vec4.ToString();




	cout << "\n\n\n";
	MyVector vec5(20);
	vec5.Add(0);
	vec5.Add(0);
	vec5.Add(2);
	vec5.Add(0);
	vec5.Add(2);
	vec5.Add(0);
	vec5.Add(0);
	vec5.Add(4);
	vec5.Add(5);
	vec5.Add(5);
	vec5.Add(0);
	vec5.Add(4);
	vec5.Add(4);
	vec5.Add(2);
	vec5.Add(0);
	vec5.Add(2);
	vec5.Add(0);
	vec5.Add(1);
	vec5.Add(1);
	vec5.Add(1);

	int numGroups = 0;
	MyVector* groupVec = vec5.GroupById(&numGroups);
	for (auto i = 0; i < numGroups; ++i)
	{
		cout << "Group Vec " << i <<":  Size: " << groupVec[i].GetSize() << " Capacity: " << groupVec[i].GetCapacity() << "\n";
		cout << groupVec[i].ToString();
	}
	delete[] groupVec;

	return 0;
}