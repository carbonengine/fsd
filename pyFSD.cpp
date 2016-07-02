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
//extern PyTypeObject PyFsdVector2Type;
//extern PyTypeObject PyFsdVector3Type;
//extern PyTypeObject PyFsdVector4Type;

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

	/*PyType_Ready(&PyFsdVector2Type);
	Py_INCREF(&PyFsdVector2Type);
	PyModule_AddObject(module, "FsdVector", (PyObject*)&PyFsdVector2Type);

	PyType_Ready(&PyFsdVector3Type);
	Py_INCREF(&PyFsdVector3Type);
	PyModule_AddObject(module, "FsdVectord", (PyObject*)&PyFsdVector3Type);
	*/
}
