#include "SentimentAnalyzer.h"
#include <Python.h>
#include <iostream>
#include <stdexcept>

SentimentAnalyzer::SentimentAnalyzer() 
    : analyzeFunction_(nullptr), sentimentModule_(nullptr) {
    
    Py_Initialize();

    // Point the Python interpreter to the application's source directory
    PyRun_SimpleString(
        "import sys\n"
        "sys.path.insert(0, '/app/python')\n"
    );

    PyObject* moduleName = PyUnicode_FromString("sentiment");
    PyObject* module = PyImport_Import(moduleName);
    Py_DECREF(moduleName);

    if (!module) {
        PyErr_Print();
        Py_Finalize();
        throw std::runtime_error("Could not import sentiment.py");
    }

    PyObject* function = PyObject_GetAttrString(module, "analyze");
    if (!function || !PyCallable_Check(function)) {
        PyErr_Print();
        Py_XDECREF(function);
        Py_DECREF(module);
        Py_Finalize();
        throw std::runtime_error("Could not find analyze() function");
    }

    sentimentModule_ = module;
    analyzeFunction_ = function;
}

SentimentAnalyzer::~SentimentAnalyzer() {
    Py_XDECREF(static_cast<PyObject*>(analyzeFunction_));
    Py_XDECREF(static_cast<PyObject*>(sentimentModule_));
    Py_Finalize();
}

std::pair<std::string, double> SentimentAnalyzer::analyze(const std::string& text) {
    PyObject* pyText = PyUnicode_FromString(text.c_str());
    PyObject* args = PyTuple_Pack(1, pyText);
    Py_DECREF(pyText);

    PyObject* result = PyObject_CallObject(static_cast<PyObject*>(analyzeFunction_), args);
    Py_DECREF(args);

    if (!result) {
        PyErr_Print();
        throw std::runtime_error("Python analyze() execution failed");
    }

    PyObject* pyLabel = PyTuple_GetItem(result, 0);
    PyObject* pyProbability = PyTuple_GetItem(result, 1);

    const char* label = PyUnicode_AsUTF8(pyLabel);
    double probability = PyFloat_AsDouble(pyProbability);

    std::pair<std::string, double> output{label, probability};
    Py_DECREF(result);

    return output;
}
