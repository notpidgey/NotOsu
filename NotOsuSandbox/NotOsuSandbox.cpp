#include <chrono>
#include <MapParser.h>

int main(int argc, char* argv[])
{
    MapParser parser( "D:/osu/Songs" );

    std::chrono::steady_clock::time_point start = std::chrono::high_resolution_clock::now();
    Beatmap* map = parser.Parse("325307 Stonebank - The Pressure", "Stonebank - The Pressure (Asserin) [Advanced].osu");
    std::chrono::steady_clock::time_point stop = std::chrono::high_resolution_clock::now();

    std::cout << "Parsed in: " << std::chrono::duration_cast< std::chrono::microseconds >( stop - start ).count( ) << " microseconds" << std::endl;
    std::cout <<
    	"[Song Name]: " << map->General->audioFileName << std::endl <<
    	"[Epilepsy Warn]: " << map->General->epilepsyWarning << std::endl <<
    	"[Stack Leniency]: " << map->General->stackLeniency << std::endl <<
    	"[Widescreen]: " << map->General->alwaysShowPlayfield << std::endl;

    return 0;
}
