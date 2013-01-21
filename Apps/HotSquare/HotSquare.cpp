// HotSquare.cpp : Defines the entry point for the console application.
//

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "stdafx.h"
#include "HotSquare.h"
#include "SquareManager.h"


using namespace std;

extern bool Test_Main();

SegManager gSegManager;
TileManager gTileManager;
SquareManager gSquareManager;

bool CheckSettings()
{
    if (HEADING_LEVEL_NUM < 1 || HEADING_LEVEL_NUM > 360 || (360 % HEADING_LEVEL_NUM) != 0) {
        printf("Error: invalid HEADING_LEVEL_NUM: %d\n", HEADING_LEVEL_NUM);
        return false;
    }
    
    return true;
}

int main()
{
    if (false == CheckSettings()) {
        return 1;
    }
    gSquareManager.SetZoomLevel(SQUARE_ZOOM_LEVEL);
    double fLngSpan, fLatSpan;
    gSquareManager.GetSquareSpansInMeter(&fLngSpan, &fLatSpan);
    printf("%s: Square Zoom Level: %lf (%lf M x %lf M)\n", ElapsedTimeStr().c_str(),
        gSquareManager.GetZoomLevel(), fLngSpan, fLatSpan);

    const char *SEGMENTS_CSV_PATH = "Data\\WAY_SEGMENTS\\data.csv";
    if (false == gSegManager.LoadFromCsvFile(SEGMENTS_CSV_PATH)) {
        printf("Error: cannot read Segments CSV file: %s\n", SEGMENTS_CSV_PATH);
        return 10;
    }
    printf("%s: Found %d segments.\n",
        ElapsedTimeStr().c_str(), gSegManager.GetSegArrayCount());

    if (false == gTileManager.GenerateTiles(gSegManager)) {
        printf("Error: cannot generate tiles\n");
        return 20;
    }
    printf("%s: Generated %d tiles.\n", ElapsedTimeStr().c_str(), gTileManager.GetTileCount());
    //gTileManager.SaveToHanaExportFiles("Data\\Tiles-Z17");
    //printf("%s: Tiles for zoom level %d saved to file Data\\Tiles-Z17\\\n", ElapsedTimeStr().c_str(), TILE_ZOOM_LEVEL);

    gSquareManager.BuildSquareMap_Multi(gSegManager, gTileManager, 4);
    printf("\n%s: Generated %d squares, %d records.\n", ElapsedTimeStr().c_str(),
        gSquareManager.GetSquareCount(), gSquareManager.CalcCsvLineCount());

    Test_Main();

    const char *square_table = "SQUARE_SEGMEN_10X10";
    gSquareManager.SaveToHanaExportFiles((std::string("Data\\") + square_table).c_str(), "ITRAFFIC_TEST", square_table);

    printf("%s: Done!\n", ElapsedTimeStr().c_str());
	return 0;
}
