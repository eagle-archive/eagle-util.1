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

int main_nanjing()
{
    if (false == CheckSettings()) {
        return 1;
    }

    const char SEGMENTS_CSV_PATH[] = "Data\\Nanjing\\NJ_WAY_SEGMENTS\\data.csv";
    if (false == gSegManager.LoadFromCsvFile(SEGMENTS_CSV_PATH)) {
        printf("Error: cannot read Segments CSV file: %s\n", SEGMENTS_CSV_PATH);
        return 10;
    }
    printf("%s: Found %d segments.\n", ElapsedTimeStr().c_str(), gSegManager.GetSegArrayCount());

    if (false == gTileManager.GenerateTiles(gSegManager)) {
        printf("Error: cannot generate tiles\n");
        return 20;
    }
    printf("%s: Generated %d tiles.\n", ElapsedTimeStr().c_str(), gTileManager.GetTileCount());

    bool Test_NanjingAssign_UsingTileManager();
    Test_NanjingAssign_UsingTileManager();

    printf("%s: Done!\n", ElapsedTimeStr().c_str());
	return 0;
}

int main()
{
    return main_nanjing();

    if (false == CheckSettings()) {
        return 1;
    }
    gSquareManager.SetZoomLevel(SQUARE_ZOOM_LEVEL);
    double fLngSpan, fLatSpan;
    gSquareManager.GetSquareSpansInMeter(45.720608, &fLngSpan, &fLatSpan);
    printf("%s: Square Zoom Level: %lf (%lf M x %lf M)\n", ElapsedTimeStr().c_str(),
        gSquareManager.GetZoomLevel(), fLngSpan, fLatSpan);

    const char *SEGMENTS_CSV_PATH = "Data\\WAY_SEGMENTS\\heb_data.csv";
    if (false == gSegManager.LoadFromCsvFile(SEGMENTS_CSV_PATH)) {
        printf("Error: cannot read Segments CSV file: %s\n", SEGMENTS_CSV_PATH);
        return 10;
    }
    printf("%s: Found %d segments.\n", ElapsedTimeStr().c_str(), gSegManager.GetSegArrayCount());

    if (false == gTileManager.GenerateTiles(gSegManager)) {
        printf("Error: cannot generate tiles\n");
        return 20;
    }
    printf("%s: Generated %d tiles.\n", ElapsedTimeStr().c_str(), gTileManager.GetTileCount());
    //gTileManager.SaveToHanaExportFiles("Data\\Tiles-Z17");
    //printf("%s: Tiles for zoom level %d saved to file Data\\Tiles-Z17\\\n", ElapsedTimeStr().c_str(), TILE_ZOOM_LEVEL);

#if 0
    gSquareManager.BuildSquareMap_Multi(gSegManager, gTileManager, 8);
    printf("\n%s: Generated %d squares, %d records.\n", ElapsedTimeStr().c_str(), gSquareManager.GetSquareCount(), gSquareManager.CalcCsvLineCount());
#else
    gSquareManager.BuildSquareMap_FromCsv(gSegManager, gTileManager, "Data\\HEB_SQUARES\\index\\HEB_OSM\\HE\\HEB_SQUARES\\data.csv");
    printf("\n%s: Got %d squares, %d records from CSV file.\n", ElapsedTimeStr().c_str(), gSquareManager.GetSquareCount(), gSquareManager.CalcCsvLineCount());
#endif

    Test_Main();

    const char *square_table = "CQ_SQUARES";
    //gSquareManager.SaveToHanaExportFiles((std::string("Data\\") + square_table).c_str(), "ITRAFFIC_TEST", square_table);

    printf("%s: Done!\n", ElapsedTimeStr().c_str());
	return 0;
}

