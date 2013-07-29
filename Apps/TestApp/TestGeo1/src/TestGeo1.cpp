// TestHanaOdbc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "insert_sim_utils.h"

bool bulk_insert(const char *csv);

int main()
{
    if (false == init_globals()) {
        printf("Error: cannot init config");
        return 10;
    }


	return 0;
}
