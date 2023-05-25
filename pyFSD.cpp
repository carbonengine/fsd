////////////////////////////////////////////////////////////////////////////////
//
// Creator:		Daniel Speed
// Created:		Feb 2013
// Copyright:	CCP 2013
//
#include "StdAfx.h"

#include <functional>

BLUE_REGISTER_GLOBAL_AS_MODULE_OBJECT( "classes", BeClasses );

const char* g_moduleName = "_pyfsd";
extern PyTypeObject PyFsdVectorType;
extern PyTypeObject PyFsdListType;
extern PyTypeObject PyFsdDictType;
extern PyTypeObject PyFsdUnsignedIntegerKeyMapType;

// We do not use the standard module initialization here, so that we can add some extra stuff to the module
#ifndef _WIN32
__attribute__((visibility ("default")))
#endif
PyMODINIT_FUNC CCP_CONCATENATE( PyInit__pyfsd, CCP_BUILD_FLAVOR )()
{
	BeClasses->RegisterClasses(BlueRegistration::GetClassRegs());
        static struct PyModuleDef moduleDef {
            PyModuleDef_HEAD_INIT,
            CCP_STRINGIZE( CCP_CONCATENATE( _pyfsd, CCP_BUILD_FLAVOR ) ),
            "",
            -1,
            nullptr
        };
	PyObject* module = PyModule_Create( &moduleDef );
        if (!module) {
          return nullptr;
        }

	BlueRegisterToModule(
		module,
		BlueRegistration::GetClassRegs(),
		BlueRegistration::GetFuncRegs(),
		BlueRegistration::GetEnumRegs(),
		BlueRegistration::GetTestRegs(),
		BlueRegistration::GetThunkerRegs()
		);

	BlueRegisterObjectsToModule(module, BlueRegistration::GetObjectRegs());
	PyModule_AddObject(module, "BlueWrapper", (PyObject*)BePyTypePtr);

	// Extra, manually wrapped types
	PyType_Ready(&PyFsdVectorType);
	Py_INCREF(&PyFsdVectorType);
	PyModule_AddObject(module, "FsdVector", (PyObject*)&PyFsdVectorType);

	PyType_Ready(&PyFsdListType);
	Py_INCREF(&PyFsdListType);
	PyModule_AddObject(module, "FsdList", (PyObject*)&PyFsdListType);

	PyType_Ready(&PyFsdDictType);
	Py_INCREF(&PyFsdDictType);
	PyModule_AddObject(module, "FsdDict", (PyObject*)&PyFsdDictType);

	if ( PyType_Ready(&PyFsdUnsignedIntegerKeyMapType) == -1 ) {
        return nullptr;
    }
	Py_INCREF(&PyFsdUnsignedIntegerKeyMapType);
	PyModule_AddObject(module, "FsdUnsignedIntegerKeyMap", (PyObject*)&PyFsdUnsignedIntegerKeyMapType);

	return module;
}
