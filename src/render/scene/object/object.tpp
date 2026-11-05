///	Template Implementation for object.ixx

template<typename T, typename... Args>
T* Object::createPrimitive(Args&&... args)
{
	T* ptrPrimitive = new T(this, std::forward<Args>(args)...);
	this->myPrimitives.push_back(ptrPrimitive);

	return ptrPrimitive;
}