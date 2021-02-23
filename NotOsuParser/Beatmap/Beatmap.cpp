#include "Beatmap.h"

Beatmap::Beatmap( ) {
	General = new ::General;
	Editor = new ::Editor;
	Metadata = new::Metadata;
	Difficulty = new ::Difficulty;
	Events = new ::Events;
	TimingPoints = std::vector<TimingPoint*>();
	Colours = new ::Colours;
	HitObjects = std::vector<HitObject*>();
}

TimingPoint* Beatmap::GetTimingAtMapTime( const int time )
{
	TimingPoint* closest = nullptr;
	for (auto timingPoint : TimingPoints)
	{
		if(timingPoint->time <= time)
		{
			closest = timingPoint;
		}
		else
		{
			return closest;
		}
	}
	
	return nullptr;
}

TimingPoint* Beatmap::GetParent(TimingPoint* childTimingPoint)
{
	TimingPoint* parent = nullptr;
	for (auto timingPoint : TimingPoints)
	{
		if(timingPoint == childTimingPoint)
			return parent;

		if(timingPoint->uninherited)
			parent = timingPoint;
	}

	return nullptr;
}