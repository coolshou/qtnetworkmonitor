#ifndef MISC_H
#define MISC_H

/*!
This file reassemble all the little useful functions that don't necessarly
belong to any of the other modules specifically.
*/

#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include <stdio.h>

#include <assert.h>

using namespace std;

//Conversions:
    string int_to_string(int Iin);
    string ip_to_String(int a,int b, int c, int d);
    string float_to_string(float Fin);
    float string_to_float(string Sin);

//!Gets a string representing the current time.
string get_time();

/*!
Process the string reveiced from 'get_time()' and returns true if it is the same day,
False if not.

//1212: Might not be correct on some system. (Relies on the string representation of time,
which isn't a very smart thing to do. It seems OK though).
*/
bool is_today(string);

#endif
