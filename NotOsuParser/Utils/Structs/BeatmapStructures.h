#pragma once
#include <string>
#include <vector>
#include <Windows.h>
#include "Other.h"

struct General {
	std::string audioFileName;
	int audioLeadIn;
	std::string audioHash;
	int previewTime;
	int countdown;
	std::string sampleSet;
	double stackLeniency;
	int mode;
	bool letterboxInBreaks;
	bool storyFireInFront;
	bool useSkinSprites;
	bool alwaysShowPlayfield;
	std::string overlayPosition;
	std::string skinPreference;
	bool epilepsyWarning;
	int countdownOffset;
	bool specialStyle;
	bool widescreenStoryboard;
	bool samplesMatchPlaybackRate;
};

struct Editor {
	std::vector< int > bookmarks;
	double distanceSpacing;
	double beatDivisor;
	int gridSize;
	double timelineZoom;
};

struct Metadata {
	std::string title;
	std::string titleUnicode;
	std::string artist;
	std::string artistUnicode;
	std::string creator;
	std::string version;
	std::string source;
	std::vector< std::string > tags;
	int beatmapId;
	int beatmapSetId;
};

struct Difficulty {
	double hpDrainRate;
	double circleSize;
	double overallDifficulty;
	double approachRate;
	double sliderMultiplier;
	double sliderTickRate;
};

struct Events {
	std::string eventType;
	int startTime;
	int evenParams;
};

struct TimingPoint {
	int time;
	double beatLength;
	int meter;
	int sampleSet;
	int sampleIndex;
	int volume;
	bool uninherited;
	int effects;
};

struct Colours {

};

enum HitObjectType {
	HIT_CIRCLE = 1 << 0,
	SLIDER = 1 << 1,
	NEW_COMBO = 1 << 2,
	SPINNER = 1 << 3,
	OSU_MANIA_HOLD = 1 << 7,
};

enum HitObjectSound {
	NORMAL = 0,
	WHISTLE = 1,
	FINISH = 2,
	CLAP = 3
};

struct SliderHitObject {
	char CurveType;
	std::vector<BeatmapPoint> CurvePoints;
	int Slides;
	double Length;
	std::vector<int> EdgeSounds;
	std::vector<std::string> EdgeSets;
};

struct SpinnerHitObject {
	int EndTime;
	POINT Position;
};

struct HitObject {
	POINT Position;
	int Time;
	HitObjectType Type;
	HitObjectSound HitSound;
	void* ObjectParams;
	std::vector<int> HitSample;

	SliderHitObject* GetParamsAsSlider() const {
		return static_cast< SliderHitObject* >( ObjectParams );
	}

	SpinnerHitObject* GetParamsAsSpinner() const {
		return static_cast< SpinnerHitObject* >( ObjectParams );
	}
};
