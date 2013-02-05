// TestHanaOdbc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "insert_sim_utils.h"

bool bulk_insert(const char *csv);

int main()
{
    if (false == init_globals(CONFIG_FILE)) {
        printf("Error: cannot load config file %s\n", CONFIG_FILE);
        return 10;
    }

    bulk_insert();
	return 0;
}
