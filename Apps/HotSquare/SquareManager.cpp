#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <Windows.h>
#include <fstream>
#include "HotSquare.h"
#include "SquareManager.h"

using namespace std;
using namespace stdext;


static const double LAT_MARGIN = SEG_ASSIGN_DISTANCE_MAX * 1.1 / LAT_METERS_PER_DEGREE;
static const double LNG_MARGIN = SEG_ASSIGN_DISTANCE_MAX * 1.1 / LNG_METERS_PER_DEGREE;
static const double LAT_STEP = SQUARE_LAT_SPAN / 4.0 / LAT_METERS_PER_DEGREE;
static const double LNG_STEP = SQUARE_LNG_SPAN / 4.0 / LNG_METERS_PER_DEGREE;

static
bool GetSegmentNeighboringSquareIds(const SEGMENT_T *pSegment, hash_set<SQUARE_ID_T> &sqIdSet)
{
    double lat1 = pSegment->from.lat;
    double lng1 = pSegment->from.lng;
    double lat2 = pSegment->to.lat;
    double lng2 = pSegment->to.lng;
    if (lat1 > lat2) {
        double t = lat1; lat1 = lat2; lat2 = t;
    }
    if (lng1 > lng2) {
        double t = lng1; lng1 = lng2; lng2 = t;
    }

    lat1 -= LAT_MARGIN;
    lat2 += LAT_MARGIN;
    lng1 -= LNG_MARGIN;
    lng2 += LNG_MARGIN;

    sqIdSet.clear();
    COORDINATE_T coord;
    for (coord.lat = lat1; coord.lat <= lat2; coord.lat += LAT_STEP) {
        for (coord.lng = lng1; coord.lng < lng2; coord.lng += LNG_STEP) {
            SQUARE_ID_T id = SquareManager::CoordinateToSquareId(coord);
            if (SegManager::CalcDistanceSquareMeters(coord, *pSegment) < SEG_ASSIGN_DISTANCE_MAX * SEG_ASSIGN_DISTANCE_MAX) {
                if (sqIdSet.find(id) == sqIdSet.end()) {
                        sqIdSet.insert(id);
                }
            }
        }
    }

    return !sqIdSet.empty();
}


bool GenerateSquareIds(const SEGMENT_T segments[], int count, int nThreadId, hash_set<SQUARE_ID_T> &squareIdSet)
{
    hash_set<SQUARE_ID_T> subSet;
    squareIdSet.clear();

    for (int i=0; i<count; i++) {
        if ((i % 500) == 0) {
            printf("\r%s: thread #%d - Generating square IDs, %d/%d, %2.2lf%%\t",
				ElapsedTimeStr().c_str(), nThreadId, i, count, (double)i/count * 100);
        }

        subSet.clear();
        GetSegmentNeighboringSquareIds(&segments[i], subSet);

        for (hash_set<SQUARE_ID_T>::iterator it = subSet.begin(); it != subSet.end(); it++) {
            if (squareIdSet.find(*it) == squareIdSet.end()) {
                squareIdSet.insert(*it);
            }
        }
    }

    return !squareIdSet.empty();
}


typedef struct {
    int nThreadId;
    SEGMENT_T *pSegStart;
    int nSegCount;
    hash_set<SQUARE_ID_T> squareIdSet;
} THREAD_DATA1;

static unsigned long WINAPI ThreadFun_GenSquareIds( LPVOID lpParam ) 
{ 
    THREAD_DATA1 *pData = (THREAD_DATA1 *)lpParam;
    GenerateSquareIds(pData->pSegStart, pData->nSegCount, pData->nThreadId, pData->squareIdSet);
    return 0; 
}

bool GenerateSquareIds_Multi(const SEGMENT_T segments[], int nSegs,
    int nThreadCount, hash_set<SQUARE_ID_T> &squareIdSet)
{
    if (segments == NULL || nThreadCount <= 0 || nSegs == 0) {
        printf("CalculateSquareIds_Multi: invalid parameter passed in\n");
        return false;
    }

    vector<THREAD_DATA1> dataArray;
    vector<DWORD> dwThreadIdArray;
    vector<HANDLE> hThreadArray;

    dataArray.resize(nThreadCount);
    dwThreadIdArray.resize(nThreadCount);
    hThreadArray.resize(nThreadCount);

    const int nAverageCount = int(nSegs / (double)nThreadCount + 0.5);
    for (int i = 0; i < nThreadCount; i++) {
        dataArray[i].nThreadId = i;
        dataArray[i].pSegStart = (SEGMENT_T *)segments + nAverageCount * i;
        dataArray[i].nSegCount = nAverageCount;
        if (i == nThreadCount - 1) {
            dataArray[i].nSegCount = nSegs - nAverageCount * i;
        }

        hThreadArray[i] = ::CreateThread(NULL, 0, ThreadFun_GenSquareIds,
            &dataArray[i], 0, &dwThreadIdArray[i]);
    }

    // Wait until all threads have terminated.
    ::WaitForMultipleObjects(nThreadCount, &hThreadArray[0], TRUE, INFINITE);
    for(int i=0; i<nThreadCount; i++) {
        CloseHandle(hThreadArray[i]);
    }

    // combine the result set
    squareIdSet.clear();
	const hash_set<SQUARE_ID_T>::iterator squareIdSet_End = squareIdSet.end();

    for (int i=0; i<nThreadCount; i++) {
        hash_set<SQUARE_ID_T> &subSet = dataArray[i].squareIdSet;
		const hash_set<SQUARE_ID_T>::iterator subSet_End = subSet.end();
        for (stdext::hash_set<SQUARE_ID_T>::iterator it = subSet.begin(); it != subSet_End; it++) {
            if (squareIdSet_End == squareIdSet.find(*it)) {
                squareIdSet.insert(*it);
            }
        }
    }

    return !squareIdSet.empty();
}

static void SquareSetToArray(hash_set<SQUARE_ID_T> &squareIdSet, vector<SQUARE_ID_T> &arrIds)
{
    arrIds.reserve(squareIdSet.size());
	for (hash_set<SQUARE_ID_T>::iterator it = squareIdSet.begin(); it != squareIdSet.end(); it++) {
        arrIds.push_back(*it);
    }
}

static bool GenerateSquarePtrArray(TileManager &tileMgr, SQUARE_ID_T squareIds[], int num,
    int nThreadId, vector<SQUARE_T *> &arrSquarePtr)
{
    arrSquarePtr.clear();
    arrSquarePtr.reserve(num);

    for (int i = 0; i < num; i++) {
        SQUARE_T *psq = new SQUARE_T;
        psq->square_id = squareIds[i];
        if ((i % 20000) == 0) {
            printf("\r%s: thread #%d - Pre-calculating %d/%d, %2.2lf%%\t",
                ElapsedTimeStr().c_str(), nThreadId, i, num, (double)i/num * 100);
        }

        COORDINATE_T centerCoord;
        SquareManager::SquareIdToCenterCoordinate(psq->square_id, &centerCoord);
        SEG_ID_T seg_id_heading_levels[HEADING_LEVEL_NUM];
        // Get seg ID for each heading for the coordinate
        for (int level = HEADING_LEVEL_NUM - 1; level >= 0; level--) {
            seg_id_heading_levels[level] =
                tileMgr.AssignSegment(centerCoord, level * (360 / HEADING_LEVEL_NUM));
        }

        psq->arr_headings_seg_id.clear();
        psq->arr_headings_seg_id.reserve(HEADING_LEVEL_NUM);

        // compress seg_id_heading_levels[] into psq->arr_headings_seg_id
        for (int i1 = 0; i1 < HEADING_LEVEL_NUM;) {
            int i2 = i1;
            while(i2 < HEADING_LEVEL_NUM && seg_id_heading_levels[i2] == seg_id_heading_levels[i1]) {
                i2++;
            }
            i2--;
            if (seg_id_heading_levels[i1] != 0) {
                HEADINGS_TO_SEG_IDS_T heads_segid;
                heads_segid.from_level = i1;
                heads_segid.to_level = i2;
                heads_segid.seg_id = seg_id_heading_levels[i1];
                psq->arr_headings_seg_id.push_back(heads_segid);
            }
            i1 = i2 + 1;
        }

#ifdef CPP11_SUPPORT
		psq->arr_headings_seg_id.shrink_to_fit();
#endif
        arrSquarePtr.push_back(psq);
    }

    return !arrSquarePtr.empty();
}

typedef struct {
    int nThreadId;
    TileManager *pTileManager;
    SQUARE_ID_T *pSqStart;
    int nSqCount;
    vector<SQUARE_T *> arrSquarePtr;
} THREAD_DATA2;

static unsigned long WINAPI ThreadFun_GenSquareArray( LPVOID lpParam ) 
{ 
    THREAD_DATA2 *pData = (THREAD_DATA2 *)lpParam;
    GenerateSquarePtrArray(*pData->pTileManager, pData->pSqStart, pData->nSqCount,
        pData->nThreadId, pData->arrSquarePtr);
    return 0; 
}

static bool GenerateSquareArray_Multi(TileManager &tileMgr, int nThreadCount, 
    vector<SQUARE_ID_T> &squareIdArr, SQUARE_MAP_T &sqMap)
{
    vector<THREAD_DATA2> dataArray;
    vector<DWORD> dwThreadIdArray;
    vector<HANDLE> hThreadArray;
    dataArray.resize(nThreadCount);
    dwThreadIdArray.resize(nThreadCount);
    hThreadArray.resize(nThreadCount);

    const int nAverageCount = int(squareIdArr.size() / (double)nThreadCount + 0.5);
    for (int i = 0; i < nThreadCount; i++) {
        dataArray[i].nThreadId = i;
        dataArray[i].pTileManager = &tileMgr;
        dataArray[i].pSqStart = (SQUARE_ID_T *)&squareIdArr[0] + nAverageCount * i;
        dataArray[i].nSqCount = nAverageCount;
        if (i == nThreadCount - 1) {
            dataArray[i].nSqCount = squareIdArr.size() - nAverageCount * i;
        }

        hThreadArray[i] = ::CreateThread(NULL, 0, ThreadFun_GenSquareArray,
            &dataArray[i], 0, &dwThreadIdArray[i]);
    }

    // Wait until all threads have terminated.
    ::WaitForMultipleObjects(nThreadCount, &hThreadArray[0], TRUE, INFINITE);
    for(int i=0; i<nThreadCount; i++) {
        ::CloseHandle(hThreadArray[i]);
    }

    // build mSquareMap
    sqMap.clear();
    for (size_t n = 0; n < dataArray.size(); n++) {
        vector<SQUARE_T *> &arrSquarePtr = dataArray[n].arrSquarePtr;
        for (size_t i = 0; i < arrSquarePtr.size(); i++) {
            sqMap.insert(SQUARE_MAP_T::value_type(arrSquarePtr[i]->square_id, arrSquarePtr[i]));
        }

        arrSquarePtr.clear();
    }

    return true;
}

bool SquareManager::BuildSquareMap_Multi(SegManager &segMgr, TileManager &tileMgr, int nThreadCount)
{
    mpSegMgr = &segMgr;
    mpTileMgr = &tileMgr;

    printf("%s: To generate square Ids, waiting ...\n", ElapsedTimeStr().c_str());
    hash_set<SQUARE_ID_T> squareIdSet;
    bool res = GenerateSquareIds_Multi(segMgr.GetSegArray(), segMgr.GetSegArrayCount(),
        nThreadCount, squareIdSet);
    if (res == false)
        return res;
    printf("\n%s: Generated square Ids count: %d\n", ElapsedTimeStr().c_str(), squareIdSet.size());

    //printf("%s: before SetToArray\n", ElapsedTimeStr().c_str());
    vector<SQUARE_ID_T> squareIdArr;
    SquareSetToArray(squareIdSet, squareIdArr);
    squareIdSet.clear();
    //printf("%s: after SetToArray\n", ElapsedTimeStr().c_str());

    GenerateSquareArray_Multi(tileMgr, nThreadCount, squareIdArr, mSquareMap);
    squareIdArr.clear();

    return !mSquareMap.empty();
}

void SquareManager::ClearSquareMap()
{
	for (SQUARE_MAP_T::iterator it = mSquareMap.begin(); it != mSquareMap.end(); it++) {
        if (it->second != NULL) {
            delete it->second;
            it->second = NULL;
        }
    }
    mSquareMap.clear();
}

bool SquareManager::SaveToCsvFile(const char *filename)
{
    std::ofstream out(filename);
    if (!out.good())
        return false;

	for (SQUARE_MAP_T::iterator it = mSquareMap.begin(); it != mSquareMap.end(); it++) {
        SQUARE_T *pSq = it->second;
        for (size_t i = 0; i < pSq->arr_headings_seg_id.size(); i++) {
            char buff[512];
            // "seqare id, heading_from, heading_to, segment id"
            sprintf(buff, "%lld,%d,%d,%lld\n", pSq->square_id,
                pSq->arr_headings_seg_id[i].from_level, pSq->arr_headings_seg_id[i].to_level,
                pSq->arr_headings_seg_id[i].seg_id);
            out << buff;
        }
    }
    out.close();
    return true;
}

SEG_ID_T SquareManager::AssignSegment(const COORDINATE_T &coord, int nHeading)
{
    SQUARE_ID_T sqId = SquareManager::CoordinateToSquareId(coord);
    SQUARE_T *pSq = GetSquareById(sqId);
    if (!pSq) return 0;

    int headingLevel = SegManager::HeadingToLevel(nHeading);
    for (size_t i = 0; i < pSq->arr_headings_seg_id.size(); i++) {
        const HEADINGS_TO_SEG_IDS_T &head_segids = pSq->arr_headings_seg_id[i];
        if (headingLevel >= head_segids.from_level && headingLevel <= head_segids.to_level) {
            return head_segids.seg_id;
        }
    }
    return 0;
}
