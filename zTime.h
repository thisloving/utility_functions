#ifndef _zTime_h_
#define _zTime_h_

#include <time.h>
#include <sstream>

#include "zType.h"

namespace zTime
{
	inline long GetTimeZone()
	{
		return -8*60*60;
	}

	inline void GetLocalTime(struct tm& tv, time_t t)
	{
		t -= GetTimeZone();
		gmtime_r(&t, &tv);
	}

	inline void GetGMTime(struct tm& tv, time_t t)
	{
		gmtime_r(&t, &tv);
	}

	inline int LocalDaysFromEpoch(time_t t)
	{
		return (t - GetTimeZone()) / (24*60*60);
	}

	inline int LocalWeeksFromEpoch(time_t t)
	{
		return (LocalDaysFromEpoch(t) + 3) / 7;
	}

	inline int LocalDaysBetween(time_t t1, time_t t2)
	{
		return LocalDaysFromEpoch(t1) - LocalDaysFromEpoch(t2);
	}

	inline int LocalWeeksBetween(time_t t1, time_t t2)
	{
		return LocalWeeksFromEpoch(t1) - LocalWeeksFromEpoch(t2);
	}

	inline bool IsSameLocalDay(time_t t1, time_t t2)
	{
		return LocalDaysFromEpoch(t1) == LocalDaysFromEpoch(t2);
	}

	inline bool IsSameLocalWeek(time_t t1, time_t t2)
	{
		return LocalWeeksFromEpoch(t1) == LocalWeeksFromEpoch(t2);
	}

	inline bool IsSameLocalMonth(time_t t1, time_t t2)
	{
		struct tm tv1;
		struct tm tv2;

		GetLocalTime(tv1, t1);
		GetLocalTime(tv2, t2);
	
		return (tv1.tm_year == tv2.tm_year && tv1.tm_mon == tv2.tm_mon);
	}

	inline bool IsSameLocalYear(time_t t1, time_t t2)
	{
		struct tm tv1;
		struct tm tv2;

		GetLocalTime(tv1, t1);
		GetLocalTime(tv2, t2);
	
		return (tv1.tm_year == tv2.tm_year);
	}

	inline int GetLocalDayStart(time_t t)
	{
		return LocalDaysFromEpoch(t) * (24*60*60) + GetTimeZone();
	}
}

struct CycleEvent
{
	public:
		CycleEvent(DWORD runtime = 0) : runtime_(runtime)
		{}

		DWORD GetRunTime() { return runtime_; }

		bool opetator() (const zRTime& current) 
		{
			if (!CheckCondition())
			{
				return false;
			}

			if (InEventPeriod(current))
			{
				if (0 == runtime_ || !InSameCycle(current))
				{
					this->Run(current);

					runtime_ = current.sec();

					return true;
				}
			}

			return false;
		}

	public:
		virtual bool InEventPeriod(const zRTime& current) = 0;
		virtual bool InSameCycle(const zRTime& current) = 0;
		virtual void Run(const zRTime& current) = 0;
		virtual bool CheckCondition() { return true; }

	protected:
		DWORD runtime_;
};


class CycleEvents
{
	public:
		typedef std::vector<CycleEvent*> Events;
	
		~CycleEvent()
		{
			for (Events::itreator it = events_.begin(); it != events_.end(); ++it)
			{
				SAFE_DELETE(*it);
			}

			events_.clear();
		}

		size_t size() const { return events_.size(); }
		
		bool Reg(CycleEvent* event)
		{
			if (!event)	return false;

			events_.push_back(event);

			return true;
		}

		void operator() (const zRTime& current)
		{
			for (size_t i = 0; i < events_.size(); ++i)
			{
				if ((*events_[i])(current))
				{
					// ....
				}
			}
		}

	private:
		Events events_;
};

struct NullCycleEvent
{
	void operator() (const zRTime& current)
	{}
};

templates <typename EventFuntor = NullCycleEvent>
class MinuteCycleEvent : public CycleEvent
{
	public:
		MinuteCycleEvent(int sec = 0, int interval = 30)
		{
			second_ = abs(sec);
			second_ = std::min(59, second_);

			interval_ = abs(interval);
		}

		virtual bool InEventPeriod(const zRTime& current)
		{
			struct tm tm;
			zTime::GetLocalTime(tm, current.sec());	

			int c_sec = tm.tm_sec;
			int e_begin = second_;
			int e_end = e_begin + interval_;

			return (c_sec >= e_begin && c_sec <= e_end)
		}

		virtual bool InSameCycle(const zRTime& current)
		{
			struct tm c_tm;
			struct tm r_tm;

			zTime::GetLocalTime(c_tm, current.sec());
			zTime::GetLocalTime(r_tm, runtime_);

			return (c_tm.tm_year == r_tm.tm_year &&
					c_tm.tm_yday == r_tm.tm_yday &&
					c_tm.tm_hour == r_tm.tm_hour &&
					c_tm.tm_min  == r_tm.tm_min);
		}

		virtual void Run(const zRTime& current)
		{
			events_(current);
		}

	protected:
		int second_;
		int interval_;

		EventFuntor events_;
};

templates <typename EventFuntor = NullCycleEvent>
class HourCycleEvent : public CycleEvent
{
	public:
		HourCycleEvent(int min = 0, int sec = 0, int interval = 60)
		{
			minute_ = abs(min);
			minute_ = std::min(59, minute_);
			
			second_ = abs(sec);
			second_ = std::min(59, second_);

			interval_ = abs(interval);
		}

		virtual bool InEventPeriod(const zRTime& current)
		{
			struct tm tm;
			zTime::GetLocalTime(tm, current.sec());	

			int c_sec = tm.tm_min * 60 + tm.tm_sec;
			int e_begin = minute_ * 60 + second_;
			int e_end = e_begin + interval_;

			return (c_sec >= e_begin && c_sec <= e_end)
		}

		virtual bool InSameCycle(const zRTime& current)
		{
			struct tm c_tm;
			struct tm r_tm;

			zTime::GetLocalTime(c_tm, current.sec());
			zTime::GetLocalTime(r_tm, runtime_);

			return (c_tm.tm_year == r_tm.tm_year &&
					c_tm.tm_yday == r_tm.tm_yday &&
					c_tm.tm_hour == r_tm.tm_hour);
		}

		virtual void Run(const zRTime& current)
		{
			events_(current);
		}

	protected:
		int minute_;
		int second_;
		int interval_;

		EventFuntor events_;
};

templates <typename EventFuntor = NullCycleEvent>
class DayCycleEvent : public CycleEvent
{
	public:
		DayCycleEvent(int hour = 0, int min = 0, int sec = 0, int interval = 300)
		{
			hour_	= abs(hour);
			hour_	= std::min(23, hour_);

			second_ = abs(sec);
			second_ = std::min(59, second_);
			
			second_ = abs(sec);
			second_ = std::min(59, second_);

			interval_ = abs(interval);
		}

		virtual bool InEventPeriod(const zRTime& current)
		{
			struct tm tm;
			zTime::GetLocalTime(tm, current.sec());	

			int c_sec = tm.tm_hour * 3600 + tm.tm_min * 60 + tm.tm_sec;
			int e_begin = hour_ * 3600 + minute_ * 60 + second_;
			int e_end = e_begin + interval_;

			return (c_sec >= e_begin && c_sec <= e_end)
		}

		virtual bool InSameCycle(const zRTime& current)
		{
			return zTime::IsSameLocalDay(current.sec(), runtime_);
		}

		virtual void Run(const zRTime& current)
		{
			events_(current);
		}

	protected:
		int hour_;
		int minute_;
		int second_;
		int interval_;

		EventFuntor events_;
};

templates <typename EventFuntor = NullCycleEvent>
class WeekCycleEvent : public CycleEvent
{
	public:
		WeekCycleEvent(int weekday = 1, int hour = 0, int min = 0, int sec = 0, int interval = 300)
		{
			wday_   = abs(weekday);
			if (wday_ > 6)
			{	
				wday_ = 0;
			}

			hour_	= abs(hour);
			hour_	= std::min(23, hour_);

			second_ = abs(sec);
			second_ = std::min(59, second_);
			
			second_ = abs(sec);
			second_ = std::min(59, second_);

			interval_ = abs(interval);
		}

		virtual bool InEventPeriod(const zRTime& current)
		{
			struct tm tm;
			zTime::GetLocalTime(tm, current.sec());	

			int c_sec = tm.tm_hour * 3600 + tm.tm_min * 60 + tm.tm_sec;
			int e_begin = hour_ * 3600 + minute_ * 60 + second_;
			int e_end = e_begin + interval_;

			return (wday_ == tm.tm_wday && c_sec >= e_begin && c_sec <= e_end)
		}

		virtual bool InSameCycle(const zRTime& current)
		{
			return zTime::IsSameLocalWeek(current.sec(), runtime_);
		}

		virtual void Run(const zRTime& current)
		{
			events_(current);
		}

	protected:
		int wday_;

		int hour_;
		int minute_;
		int second_;
		int interval_;

		EventFuntor events_;
};

templates <typename EventFuntor = NullCycleEvent>
class MonthCycleEvent : public CycleEvent
{
	public:
		MonthCycleEvent(int mday = 1, int hour = 0, int min = 0, int sec = 0, int interval = 300)
		{
			mday_   = abs(weekday);

			hour_	= abs(hour);
			hour_	= std::min(23, hour_);

			second_ = abs(sec);
			second_ = std::min(59, second_);
			
			second_ = abs(sec);
			second_ = std::min(59, second_);

			interval_ = interval;
		}

		virtual bool InEventPeriod(const zRTime& current)
		{
			struct tm tm;
			zTime::GetLocalTime(tm, current.sec());	

			int c_sec = tm.tm_hour * 3600 + tm.tm_min * 60 + tm.tm_sec;
			int e_begin = hour_ * 3600 + minute_ * 60 + second_;
			int e_end = e_begin + interval_;

			return (mday_ == tm.tm_mday && c_sec >= e_begin && c_sec <= e_end)
		}

		virtual bool InSameCycle(const zRTime& current)
		{
			return zTime::IsSameLocalWeek(current.sec(), runtime_);
		}

		virtual void Run(const zRTime& current)
		{
			events_(current);
		}

	protected:
		int mday_;

		int hour_;
		int minute_;
		int second_;
		int interval_;

		EventFuntor events_;
};


#endif  // _zTime_h_
