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
	float stackLeniency;
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
	float distanceSpacing;
	float beatDivisor;
	int gridSize;
	float timelineZoom;
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
	float hpDrainRate;
	float circleSize;
	float overallDifficulty;
	float approachRate;
	float sliderMultiplier;
	float sliderTickRate;
};

struct Events {
	std::string eventType;
	int startTime;
	int evenParams;
};

struct TimingPoint {
	int time;
	float beatLength;
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
	char curveType;
	std::vector<BeatmapPoint> curvePoints;
	int slides;
	float length;
	std::vector<int> edgeSounds;
	std::vector<std::string> edgeSets;
};

struct SpinnerHitObject {
	int endTime;
	BeatmapPoint $position;
};

struct HitObject {
	BeatmapPoint position;
	int time;
	HitObjectType type;
	HitObjectSound hitSound;
	void* objectParams;
	std::vector<int> hitSample;
	
	SliderHitObject* GetParamsAsSlider() const {
		return static_cast< SliderHitObject* >( objectParams );
	}

	SpinnerHitObject* GetParamsAsSpinner() const {
		return static_cast< SpinnerHitObject* >( objectParams );
	}
};
