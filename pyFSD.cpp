////////////////////////////////////////////////////////////////////////////////
//
// Creator:		Daniel Speed
// Created:		Feb 2013
// Copyright:	CCP 2013
//
#include "StdAfx.h"
#include "BlueExposure/include/InterfaceDefinitions.cxx"

#include <functional>

BLUE_REGISTER_GLOBAL_AS_MODULE_OBJECT( "classes", BeClasses );

const char* g_moduleName = "pyFSD";
extern PyTypeObject PyFsdVectorType;
extern PyTypeObject PyFsdListType;
extern PyTypeObject PyFsdDictType;

// We do not use the standard module initialization here, so that we can add some extra stuff to the module
PyMODINIT_FUNC initpyFSD()
{
	BeClasses->RegisterClasses(BlueRegistration::GetClassRegs());
	PyObject* module = Py_InitModule(g_moduleName, NULL);

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
}
