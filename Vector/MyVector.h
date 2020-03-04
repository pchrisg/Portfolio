//-------------------------------------------------------------------------------------------------
// 벡터 (vector) 클래스 구현하기
//
// 본 header 파일에서는, 신입 입사지원자의 과제용으로, 간단한 벡터를 구현하기 위한,
// MyVector 라는 클래스를 정의하고 있습니다.
//
// 소스와 주석을 참고하여, 필요한 기능들을 구현하여,
// MyVector.h 파일과 MyVector.cpp, 2개의 파일을 제출해주시면 됩니다.
// (제출하시는 파일들의 인코딩을 utf-8 로 맞춰주시면 감사하겠습니다.)
//
// 주의: std::vector 와 같은, 이미 구현된 벡터를 감싸는식으로 구현하지 마시고,
//    직접 자신의 코드로 기능을 구현 부탁드립니다.
//
// 참고: 영어로 작성된 부분이 많은 것에 대한 설명을 드리자면,
//    영어 실력을 중요하게 여긴다기 보다는, 번역기를 돌리던 관련 한글 문서를 찾던,
//    어떻게 해서든 원서의 내용을, 이해/학습하는 능력이 중요하기 때문입니다.
//-------------------------------------------------------------------------------------------------
 
 
//-------------------------------------------------------------------------------------------------
// 벡터란? (http://www.cplusplus.com/reference/vector/vector/)
//
// Vectors are sequence containers representing arrays that can change in size.
// Just like arrays, vectors use contiguous storage locations for their elements,
// which means that their elements can also be accessed using offsets on regular pointers
// to its elements, and just as efficiently as in arrays. But unlike arrays,
// their size can change dynamically, with their storage being handled automatically
// by the container.
//
// Internally, vectors use a dynamically allocated array to store their elements.
// This array may need to be reallocated in order to grow in size when new elements are inserted,
// which implies allocating a new array and moving all elements to it.
// This is a relatively expensive task in terms of processing time, and thus,
// vectors do not reallocate each time an element is added to the container.
//
// Instead, vector containers may allocate some extra storage to accommodate for possible growth,
// and thus the container may have an actual capacity greater than the storage strictly needed to
// contain its elements (i.e., its size). Libraries can implement different strategies for growth
// to balance between memory usage and reallocations, but in any case,
// reallocations should only happen at logarithmically growing intervals of size
// so that the insertion of individual elements at the end of the vector can be provided with
// amortized constant time complexity.
//-------------------------------------------------------------------------------------------------
 
#include <string>

//-------------------------------------------------------------------------------------------------
// MyVector 가 관리하는 오브젝트
//-------------------------------------------------------------------------------------------------
struct MyObject
{
	int _id{};
};
 
//-------------------------------------------------------------------------------------------------
// MyVector 클래스.
//-------------------------------------------------------------------------------------------------
class MyVector
{
private: // 구현에 필요한 멤버 추가 함수/변수들을 자유롭게 아래에 정의 합니다.
 
    MyObject* _vectorObjects{};
    unsigned int _vectorCapacity{};
    unsigned int _vectorSize{};

    void Grow();
 
public: // 생성자, 복사생성자, 할당연산자, 소멸자를 .cpp 파일에 구현합니다.
 
    // Default Constructor.
    MyVector();

    //Perameterised Constructor.
    MyVector(const int capacity);
 
    // Copy constructor.
    MyVector(const MyVector& other);
 
    // Assignment operator.
    void operator=(const MyVector& other);

    // Returns the MyObject instance at the specified index.
    MyObject& operator[](size_t index);
 
    // Destructor.
    ~MyVector();
 
public: // 아래 기능 함수들을 .cpp 파일에 구현합니다.
 
    // Returns current capacity of this vector.
    int GetCapacity() const;
 
    // Returns current size of this vector.
    int GetSize() const;
 
    // Creates a new MyObject instance with the given ID, and appends it to the end of this vector.
    void Add(int id);
 
    // Trims the capacity of this vector to current size.
    void TrimToSize();
 
    // Remove all MyObject instances with the given ID in this vector.
    void RemoveAll(int myObjectId);
 
    // Returns string representation of the vector.
    std::string ToString() const;
 
    // Returns the first occurrence of MyObject instance with the given ID.
    // Returns nullptr if not found.
    MyObject* FindById(int myObjectId) const;
 
    // Returns a newly allocated array of MyVector objects,
    // each of whose elements have the same "_id" value of the MyObject struct.
    // The 'numGroups' is an out parameter, and its value should be set to
    // the size of the MyVector array to be returned.
    MyVector* GroupById(int* numGroups);
};