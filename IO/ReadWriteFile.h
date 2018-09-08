#ifndef READWRITEFILE_H
#define READWRITEFILE_H

#include <string>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <vector>
#include <stdlib.h>

#include "../Misc.h"

using namespace std;

/*!
This class can be written to a file or read from a file.
*/
class InfoReadWrite
{
    public:
    InfoReadWrite();

    //!Read the class from a file
    void Read(string filename);

    //!Write the class to a file
    void Write(string filename);

    //!Returns the expected data
    vector<float> getData();

    //!Saves the members to a file
    void setData(vector<float> in);

    //!Returns the timestamp of the file read.
    string get_timestamp();

    private:

    //!Loads the whole file in a string
    string load_txt(string filename_in);

    //!Save a string in a file
    void save_txt(string filename_out, string out);

    /*!
    mess_in = a message
    left    = the left separator
    left    = the right separator
    ...will give the last occurence of the match
    */
    string find_subs_between(string mess_in, string left, string right);

    //Data that is read/written
    float DeviceNo;
    float DownloadB;
    float UploadB;
    string Timestamp;
};

#endif
