#include "pre.h"
#include <Python.h>
#include <iostream>

void fuse_images(const std::string& input_path, const std::string& output_path) {
    // Optionally set Python home
    Py_SetPythonHome(L"F:/miniconda3/envs/csc");

    // Initialize the Python Interpreter
    Py_Initialize();

    // Import sys module and append the module path
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append(\"C:/Users/kyy/Desktop/GES-GSP_vc16_Modified/CSC-MEFN/\")");

    // Load the Python module
    PyObject* pName = PyUnicode_DecodeFSDefault("fusion_module");
    PyObject* pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != nullptr) {
        // Get the function from the module
        PyObject* pFunc = PyObject_GetAttrString(pModule, "fuse_images");
        std::cout << "fusing..." << std::endl;
        if (pFunc && PyCallable_Check(pFunc)) {
            // Prepare the arguments
            PyObject* pArgs = PyTuple_New(2);
            PyObject* pValue1 = PyUnicode_FromString(input_path.c_str());
            PyObject* pValue2 = PyUnicode_FromString(output_path.c_str());
            PyTuple_SetItem(pArgs, 0, pValue1);
            PyTuple_SetItem(pArgs, 1, pValue2);

            // Call the Python function
            PyObject* pResult = PyObject_CallObject(pFunc, pArgs);
            if (pResult != nullptr) {
                std::cout << "Image fusion completed successfully." << std::endl;
                Py_DECREF(pResult);
            }
            else {
                PyErr_Print();
                std::cerr << "Failed to call 'fuse_images'." << std::endl;
            }
            Py_DECREF(pArgs);
            Py_DECREF(pFunc);
        }
        else {
            if (PyErr_Occurred()) PyErr_Print();
            std::cerr << "Cannot find function 'fuse_images'." << std::endl;
        }
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        std::cerr << "Failed to load module 'fusion_module'." << std::endl;
    }

    // Finalize the Python Interpreter
    Py_Finalize();
}
