// TestHanaOdbc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "insert_sim_utils.h"

bool bulk_insert(const char *csv);

int main()
{
#if 1
    bool TestHdb_Main();
    TestHdb_Main();
#else
    if (false == init_globals(CONFIG_FILE)) {
        printf("Error: cannot load config file %s\n", CONFIG_FILE);
        return 10;
    }
    bulk_insert(GLOBALS.CSV_PATH.c_str());
#endif
	return 0;
}
