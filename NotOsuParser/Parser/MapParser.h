#pragma once
#include <fstream>
#include <vector>
#include <iostream>
#include <string>

#include "../Beatmap/Beatmap.h"
#include "../Utils/Utils.h"

class MapParser
{
public:
    const char* osuDirectory;
    explicit MapParser( const char* osuDirectory );
    explicit MapParser(  );
    
    Beatmap* Parse(const char* fullPath);
    Beatmap* Parse( const char* songName, const char* mapName );

private:
    Beatmap* Parse(std::string absolutePath);
    
    bool ReadFile( std::vector<char>* buffer, const char* path );
    bool GetSection( std::vector<char>* mapData, std::vector<char>* sectionData, std::string name );
    template <class T> bool ParseKvp(std::vector<char>* sectionData, std::string kvpName, void* outValue, int extraValuePadding);
    bool ParseNextHitObject( std::vector<char>* sectionData, HitObject*& hitObject ) const;
    bool ParseNextTimingPoint( std::vector<char>* sectionData, TimingPoint*& timingPoint ) const;

    bool ParseGeneral(std::vector<char>* sectionData, General* generalSection);
    bool ParseEditor(std::vector<char>* sectionData, Editor* generalSection);
    bool ParseMetadata(std::vector<char>* sectionData, Metadata* generalSection);
    bool ParseDifficulty(std::vector<char>* sectionData, Difficulty* generalSection);
    bool ParseEvents(std::vector<char>* sectionData, Events* generalSection);
    bool ParseTimingPoints( std::vector<char>* sectionData, std::vector<TimingPoint*>& generalSection );
    bool ParseColours(std::vector<char>* sectionData, Colours* generalSection);
    bool ParseHitObjects( std::vector<char>* sectionData, std::vector<HitObject*>& hitObjects );
};
