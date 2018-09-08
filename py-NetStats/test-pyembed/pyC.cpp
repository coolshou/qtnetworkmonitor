/*!
Problem so far: Can't get values back from the functions ran.
*/

//Has to be included first
#include "Python.h"

#include <vector>
#include <string>
#include <assert.h>
#include <fstream>

#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
	Py_Initialize();

	// Access the "__main__" module and its name-space dictionary.
	PyObject *pMainModule     = PyImport_AddModule( "__main__" );
	PyObject *pMainDictionary = PyModule_GetDict( pMainModule );

	PyObject* PyFileObject = PyFile_FromString("GetNBytesOnDevice.py", "r");
	PyRun_SimpleFile(PyFile_AsFile(PyFileObject), "");
	
    //Extracts a variable from the script ran
    PyObject *pResult = PyDict_GetItemString( pMainDictionary, "NBytesOnDevice" );

	int nValue;
    PyArg_Parse( pResult, "i", &nValue );
    cout << "Script's \"returnValue\" variable was set to: " << nValue << endl;

	Py_Finalize();
}
