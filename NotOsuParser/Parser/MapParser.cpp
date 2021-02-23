#include "MapParser.h"

MapParser::MapParser( const char* osuDirectory ) {
	this->osuDirectory = osuDirectory;
}
MapParser::MapParser()
{
	osuDirectory = "D:\osu\Songs";
}

Beatmap* MapParser::Parse( const char* fullPath)
{
	std::string absolutePath = fullPath;

	return Parse(absolutePath);
}

Beatmap* MapParser::Parse( const char* songName, const char* mapName ) {
	std::string absolutePath;
	absolutePath.append( osuDirectory );
	absolutePath.append( "/" );
	absolutePath.append( songName );
	absolutePath.append( "/" );
	absolutePath.append( mapName );

	return Parse(absolutePath);
}

Beatmap* MapParser::Parse( std::string absolutePath )
{
	std::vector< char > buffer;
	ReadFile( &buffer, absolutePath.c_str( ) );

	Beatmap* beatmap = new Beatmap( );
	std::vector< char > sectionData;
	if ( GetSection( &buffer, &sectionData, "[General]" ) ) {
		if ( !ParseGeneral( &sectionData, beatmap->General ) )
			std::cout << "Error parsing General section" << std::endl;
	}
	else std::cout << "Error reading General section" << std::endl;

	if ( GetSection( &buffer, &sectionData, "[Editor]" ) ) {
		if ( !ParseEditor( &sectionData, beatmap->Editor ) )
			std::cout << "Error parsing Editor section" << std::endl;
	}
	else std::cout << "Error reading Editor section" << std::endl;

	if ( GetSection( &buffer, &sectionData, "[Metadata]" ) ) {
		if ( !ParseMetadata( &sectionData, beatmap->Metadata )) 
			std::cout << "Error parsing Metadata section" << std::endl;		
	}
	else std::cout << "Error reading Metadata section" << std::endl;
	

	if ( GetSection( &buffer, &sectionData, "[Difficulty]" ) ) {
		if ( !ParseDifficulty( &sectionData, beatmap->Difficulty ) ) 
			std::cout << "Error parsing Difficulty section" << std::endl;
	}
	else std::cout << "Error reading Difficulty section" << std::endl;
	

	if ( GetSection( &buffer, &sectionData, "[Events]" ) ) {
		if ( !ParseEvents( &sectionData, beatmap->Events ) ) 
			std::cout << "Error parsing Events section" << std::endl;		
	}
	else std::cout << "Error reading Events section" << std::endl;

	if ( GetSection( &buffer, &sectionData, "[TimingPoints]" ) ) {
		if ( !ParseTimingPoints( &sectionData, beatmap->TimingPoints ) )
			std::cout << "Error parsing TimingPoints section" << std::endl;		
	}
	else std::cout << "Error reading TimingPoints section" << std::endl;

	if ( GetSection( &buffer, &sectionData, "[Colours]" ) ) {
		if ( !ParseColours( &sectionData, beatmap->Colours ) )
			std::cout << "Error parsing Colours section" << std::endl;		
	}
	else std::cout << "Error reading Colours section" << std::endl;

	if ( GetSection( &buffer, &sectionData, "[HitObjects]" ) ) {
		if ( !ParseHitObjects( &sectionData, beatmap->HitObjects) )
			std::cout << "Error parsing HitObjects section" << std::endl;			
	}
	else std::cout << "Error reading HitObjects section" << std::endl;

	return beatmap;
}

bool MapParser::ReadFile( std::vector< char >* buffer, const char* path ) {
	std::ifstream* fileStream = new std::ifstream( path );
	fileStream->seekg( 0, std::ios::end );
	const size_t fileSize = fileStream->tellg( );
	fileStream->seekg( 0, std::ios::beg );

	if ( fileSize > 0 ) {
		buffer->resize( fileSize );
		fileStream->read( &buffer->at( 0 ), fileSize );
	}

	return true;
}

bool MapParser::GetSection( std::vector< char >* mapData, std::vector< char >* sectionData, std::string name ) {
	int bytesRead = 0;
	sectionData->clear(  );
	while ( bytesRead != mapData->size( ) - 1 ) {
		for ( auto character : name ) {
			if ( mapData->at( bytesRead ) == character )
				bytesRead++;
			else
				goto completed;
		}

		//Section Found
		bytesRead ++;
		while ( mapData->at( bytesRead ) != '[' && bytesRead != mapData->size(  ) -  1  ) {
			sectionData->push_back( mapData->at( bytesRead ) );
			bytesRead++;
		}

		mapData->erase( mapData->begin( ), mapData->begin( ) + bytesRead );

		return true;

	completed:
		bytesRead++;
	}

	return false;
}

template<typename T>
bool MapParser::ParseKvp( std::vector<char>* sectionData, std::string kvpName, void* outValue, int extraValuePadding = 2 ) {
	int bytesRead = 0;
	std::vector<char> valueData;
	while ( bytesRead != sectionData->size( ) - 1 ) {
		for ( auto character : kvpName ) {
			if ( sectionData->at( bytesRead ) == character )
				bytesRead++;
			else
				goto completed;
		}
		
		//Found section
		bytesRead += extraValuePadding;
		while ( sectionData->at( bytesRead ) != 0x0a && bytesRead != sectionData->size(  ) -  1  ) {
			valueData.push_back( sectionData->at( bytesRead ) );
			bytesRead++;
		}

		if(std::is_same<T, int>::value || std::is_same<T, bool>::value) {
			*static_cast< int* >( outValue ) = std::stoi(valueData.data(  ));
		}
		else if(std::is_same<T, const char*>::value || std::is_same<T, std::string>::value) {
			static_cast< std::string* >( outValue )->append( valueData.data(  ) );
		}
		else if(std::is_same<T, float>::value) {
			*static_cast< float* >( outValue ) = std::stof( valueData.data(  ));
		}

		valueData.clear(  );
		sectionData->erase( sectionData->begin(  ), sectionData->begin(  ) + bytesRead );
		
		return true;
		
		completed:
		bytesRead++;
	}

	return false;
}

bool MapParser::ParseNextHitObject( std::vector< char >* sectionData, HitObject*& hitObject ) const {
	if(sectionData->at( 0 ) == 0)
		return false;
	
	std::vector<byte> hitObjectData;
	int bytesRead = 0;
	hitObject = new HitObject();
	
	while(sectionData->at( bytesRead ) != 0x0a && bytesRead != sectionData->size(  ) -  1) {
		hitObjectData.push_back( sectionData->at( bytesRead ) );
		bytesRead++;
	}
	sectionData->erase( sectionData->begin(  ), sectionData->begin(  ) + bytesRead + 1 );

	auto something =  std::string ((const char*)hitObjectData.data(  ));
	something.resize(hitObjectData.size());
	std::vector<std::string> params = Split(something, "," );

	hitObject->position.x =  std::stoi( params[0] );
	hitObject->position.y =  std::stoi( params[1] );
	hitObject->time = std::stoll( params[2] );
	hitObject->type = static_cast< HitObjectType >( std::stoll( params[3] ) );
	hitObject->hitSound = static_cast< HitObjectSound >( static_cast< HitObjectType >( std::stoi( params[4] ) ) );

	if((hitObject->type & HIT_CIRCLE) != 0) {
		
	}
	if ((hitObject->type & SLIDER) != 0 ) {
		SliderHitObject* sliderHitObject = new SliderHitObject();
		hitObject->objectParams = static_cast< void* >( sliderHitObject );

		std::vector<std::string> curves = Split( params[5], "|" );
		sliderHitObject->curveType = curves[0][0] ;
		curves.erase (curves.begin());
		for ( auto point : curves ) {
			std::vector<std::string> xyPoint = Split( point, ":" );
			sliderHitObject->curvePoints.push_back( BeatmapPoint(std::stoi( xyPoint[0] ) , std::stoi( xyPoint[1] ) ) );
		}
				
		sliderHitObject->slides = std::stoi( params[ 6 ] );
		sliderHitObject->length = std::stof( params[ 7 ] );

		if(params.size(  ) >= 9 ) {
			std::vector<std::string> edgeSounds = Split( params[8], "|" );
			for ( auto sound : edgeSounds ) {
				sliderHitObject->edgeSounds.push_back(  std::stoi( sound ) );
			}

			if(params.size(  ) == 10) {
				std::vector<std::string> edgeSets = Split( params[9], "|" );
				for ( auto set : edgeSets ) {
					sliderHitObject->edgeSounds.push_back(  std::stoi( set ) );
				}
			}
		}
	}
	if ((hitObject->type & SPINNER) != 0 ) {
		SpinnerHitObject* spinnerHitObject = new SpinnerHitObject();
		hitObject->objectParams = static_cast< void* >( spinnerHitObject );
		spinnerHitObject->endTime = std::stoi( params[ 5 ] );
		//Hit sample
	}
	if ((hitObject->type & OSU_MANIA_HOLD) != 0 ) {
		
	}

	return true;
}

bool MapParser::ParseNextTimingPoint( std::vector<char>* sectionData, TimingPoint*& timingPoint) const {
	if(sectionData->at( 0 ) == 0 || sectionData->at( 0 ) == 0x0a)
		return false;
	
	std::vector<byte> timingPointData;
	int bytesRead = 0;
	timingPoint = new TimingPoint;
	
	while(sectionData->at( bytesRead ) != 0x0a && bytesRead != sectionData->size(  ) -  1) {
		timingPointData.push_back( sectionData->at( bytesRead ) );
		bytesRead++;
	}
	sectionData->erase( sectionData->begin(  ), sectionData->begin(  ) + bytesRead + 1 );

	std::vector<std::string> params = Split( (char*)timingPointData.data(  ), "," );

	timingPoint->time = std::stoll( params[0] );
	timingPoint->beatLength = std::stod( params[1] );
	timingPoint->meter = std::stoll( params[2] );
	timingPoint->sampleSet = std::stoll( params[3] );
	timingPoint->sampleIndex = std::stoll( params[4] );
	timingPoint->volume = std::stoll( params[5] );
	timingPoint->uninherited = std::stoll( params[6] );
	timingPoint->effects = std::stoll( params[7] );

	return true;
}

bool MapParser::ParseGeneral( std::vector< char >* sectionData, General* generalSection ) {
	//I dont care enough to handle the true/falses sorry /shrug
	ParseKvp<std::string>( sectionData, "AudioFilename", &generalSection->audioFileName );
	ParseKvp<int>( sectionData, "AudioLeadIn", &generalSection->audioLeadIn );
	ParseKvp<std::string>( sectionData, "AudioHash", &generalSection->audioHash );
	ParseKvp<int>( sectionData, "PreviewTime", &generalSection->previewTime );
	ParseKvp<int>( sectionData, "Countdown", &generalSection->countdown );
	ParseKvp<std::string>( sectionData, "SampleSet", &generalSection->sampleSet );
	ParseKvp<float>( sectionData, "StackLeniency", &generalSection->stackLeniency );
	ParseKvp<int>( sectionData, "Mode", &generalSection->mode );
	ParseKvp<bool>( sectionData, "LetterboxInBreaks", &generalSection->letterboxInBreaks );
	ParseKvp<bool>( sectionData, "StoryFireInFront", &generalSection->storyFireInFront );
	ParseKvp<bool>( sectionData, "UseSkinSprites", &generalSection->useSkinSprites );
	ParseKvp<bool>( sectionData, "AlwaysShowPlayfield", &generalSection->alwaysShowPlayfield );
	ParseKvp<std::string>( sectionData, "OverlayPosition", &generalSection->overlayPosition );
	ParseKvp<std::string>( sectionData, "SkinPreference", &generalSection->skinPreference );
	ParseKvp<bool>( sectionData, "EpilepsyWarning", &generalSection->epilepsyWarning );
	ParseKvp<int>( sectionData, "CountdownOffset", &generalSection->countdownOffset );
	ParseKvp<bool>( sectionData, "SpecialStyle", &generalSection->specialStyle );
	ParseKvp<bool>( sectionData, "WidescreenStoryboard", &generalSection->widescreenStoryboard );
	ParseKvp<bool>( sectionData, "SamplesMatchPlaybackRate", &generalSection->samplesMatchPlaybackRate );

	return true;
}

bool MapParser::ParseEditor( std::vector< char >* sectionData, Editor* generalSection ) {
	return true;
}

bool MapParser::ParseMetadata( std::vector< char >* sectionData, Metadata* generalSection ) {
	return true;
}

bool MapParser::ParseDifficulty( std::vector< char >* sectionData, Difficulty* difficultySection ) {
	ParseKvp<float>( sectionData, "HPDrainRate", &difficultySection->hpDrainRate, 1  );
	ParseKvp<float>( sectionData, "CircleSize", &difficultySection->circleSize, 1 );
	ParseKvp<float>( sectionData, "OverallDifficulty", &difficultySection->overallDifficulty, 1 );
	ParseKvp<float>( sectionData, "ApproachRate", &difficultySection->approachRate, 1 );
	ParseKvp<float>( sectionData, "SliderMultiplier", &difficultySection->sliderMultiplier, 1 );
	ParseKvp<float>( sectionData, "SliderTickRate", &difficultySection->sliderTickRate, 1 );
	
	return true;
}

bool MapParser::ParseEvents( std::vector< char >* sectionData, Events* generalSection ) {
	return true;
}

bool MapParser::ParseTimingPoints( std::vector< char >* sectionData, std::vector<TimingPoint*>& timingPoints ) {
	TimingPoint* timingPoint;
	while ( ParseNextTimingPoint( sectionData, timingPoint ) ) 
		timingPoints.push_back( timingPoint );
	
	return true;
}

bool MapParser::ParseColours( std::vector< char >* sectionData, Colours* generalSection ) {
	return true;
}

bool MapParser::ParseHitObjects( std::vector< char >* sectionData, std::vector<HitObject*>& hitObjects ) {
	HitObject* object;
	while( ParseNextHitObject ( sectionData, object ) )
		hitObjects.push_back( object );
	
	return true;
}