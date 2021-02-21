#pragma once
#include "../Utils/Structs/BeatmapStructures.h"

class Beatmap {
public:
	General* General;
	Editor* Editor;
	Metadata* Metadata;
	Difficulty* Difficulty;
	Events* Events;
	std::vector<TimingPoint*> TimingPoints;
	Colours* Colours;
	std::vector<HitObject*> HitObjects;

	Beatmap();
};
