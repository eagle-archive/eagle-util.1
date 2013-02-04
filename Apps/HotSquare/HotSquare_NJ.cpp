// HotSquare_NJ.cpp : Defines the entry point for the console application.
//

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <fstream>
#include "HotSquare.h"
#include "SquareManager.h"

extern SegManager gSegManager;
extern TileManager gTileManager;
extern SquareManager gSquareManager;

using namespace std;


typedef struct {
    string      vechid;
    string      lng;
    string      lat;
    string      speed;
    short       heading; // double
    string      gpstime;
    short       inload;
    short       inservice;
} VEHICLE_RECORD_NJ;


static 
bool ParseRecord(string &line, VEHICLE_RECORD_NJ &r) {
    /*
      Input line example:
      3584043109,118.500844,31.886752000000001,27,134,"2011-11-29 11:37:10.0000000",0,1
    */
    char buffs[8][64];
    if (8 != sscanf(line.c_str(), "%[^,],%[^,],%[^,],%[^,],%[^,],\"%[^\"]\",%[^,],%s", buffs[0], buffs[1], buffs[2], buffs[3], buffs[4], buffs[5], buffs[6], buffs[7])) {
        return false;
    }
    r.vechid = buffs[0];
    r.lng = buffs[1];
    r.lat = buffs[2];
    r.speed = buffs[3];
    r.heading = atoi(buffs[4]);
    r.gpstime = buffs[5];
    r.inload = atoi(buffs[6]);
    r.inservice = atoi(buffs[7]);

    return true;
}

static void PrintNjAssignStatus(int rec_count)
{
    static CONSOLE_SCREEN_BUFFER_INFO sConsoleInfo;
    if (sConsoleInfo.dwSize.X == 0) {
        ::GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &sConsoleInfo);
    }
    ::SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), sConsoleInfo.dwCursorPosition);
    printf("\r%s: processing #%d\n", ElapsedTimeStr().c_str(), rec_count);
}

// divide 24 hours into 144 slots
// return [0, 143]
static int GetTimeSlot(const string &gpstime)
{
    // gpstime example "2011-11-29 16:30:09.0000000"
    const char *p_time = gpstime.c_str() + 11;
    int hour, min;
    if (2 != sscanf(p_time, "%d:%d", &hour, &min)) {
        cout << ElapsedTimeStr() << ": Invalid GPSTIME: " << gpstime << endl;
        return -1;
    }
    return (hour * 60 + min) / 10;
}

static bool NanjingAssign_UsingTileManager()
{
    const int TOP = 0; //20000;
    const char *infile = "Data\\Nanjing\\1129NJ\\data.csv";
    const char *outfile = "Data\\Nanjing\\1129NJ_ASSIGNMENT.csv";

    cout << ElapsedTimeStr() << ": Enter NanjingAssign_UsingTileManager()\n";
    std::ifstream in(infile);
    if (!in.good()) {
        cout << "NanjingAssign_UsingTileManager: cannot open file " << infile << " for reading." << endl;
        return false;
    }
    remove(outfile);
    std::ofstream out(outfile);
    if (!out.good()) {
        cout << "NanjingAssign_UsingTileManager: cannot open file " << infile << " for writing." << endl;
        return false;
    }

    int count = 0;
    int no_hit_count = 0;

    std::string line;
    while (GetLine(in, line)) {
        VEHICLE_RECORD_NJ record;
        if (true == ParseRecord(line, record)) {
            if (count % 100000 == 0) {
                PrintNjAssignStatus(count);
            }
            count++;

            COORDINATE_T coord;
            coord.lng = atof(record.lng.c_str());
            coord.lat = atof(record.lat.c_str());

            SEG_ID_T assigned_seg_id1 = gTileManager.AssignSegment(coord, record.heading);
            if (INVALID_SEG_ID == assigned_seg_id1) {
                no_hit_count++;
            }
            int time_slot = GetTimeSlot(record.gpstime);

            char buff[1024 * 4];
            sprintf(buff, "%s,%s,%s,%s,%d,\"%s\",%d,%d,%lld,%d\n",
                record.vechid.c_str(), record.lng.c_str(), record.lat.c_str(), record.speed.c_str(),
                (int)record.heading, record.gpstime.c_str(), (int)record.inload, (int)record.inservice,
                assigned_seg_id1, time_slot);
            out << buff;
            if (TOP > 0 && count == TOP) {
                break;
            }
        }
    }
    PrintNjAssignStatus(count);

    cout << ElapsedTimeStr() << ": Exit NanjingAssign_UsingTileManager(), total count = " << count
         << " , hit failure count = " << no_hit_count <<endl;

    return true;
}

int main_nanjing()
{
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

    NanjingAssign_UsingTileManager();

    printf("%s: Done!\n", ElapsedTimeStr().c_str());
	return 0;
}
