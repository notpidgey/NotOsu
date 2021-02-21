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
