#ifndef _zMisc_h_
#define _zMisc_h_

class Base
{
	public:
		virtual bool Check(const void* buf, unsigned int len) = 0
};

template<typename T>
class Polymorphism : public Base
{
	public:
		virtual bool Check(const void* buf, unsigned int len)
		{
			return len = ((T*)buf)->GetSize();
		}
};

#endif  // _zMisc_h_
