// -*- mode: cpp; mode: fold -*-
// Description								/*{{{*/
// $Id: sourcelist.cc,v 1.1 2003/07/23 02:20:24 mdz Exp $
/* ######################################################################

   Package Records - Wrapper for the package records functions

   ##################################################################### */
									/*}}}*/
// Include Files							/*{{{*/
#include "generic.h"
#include "apt_pkgmodule.h"

#include <apt-pkg/sourcelist.h>

#include <Python.h>
									/*}}}*/

struct PkgSourceListStruct
{
   pkgSourceList List;
};
    
// PkgsourceList Class							/*{{{*/
// ---------------------------------------------------------------------

static char *doc_PkgSourceListFindIndex = "xxx";
static PyObject *PkgSourceListFindIndex(PyObject *Self,PyObject *Args)
{   
   PkgSourceListStruct &Struct = GetCpp<PkgSourceListStruct>(Self);
   return Py_BuildValue("i", 1);
}

static char *doc_PkgSourceListReadMainList = "xxx";
static PyObject *PkgSourceListReadMainList(PyObject *Self,PyObject *Args)
{
   PkgSourceListStruct &Struct = GetCpp<PkgSourceListStruct>(Self);
   Struct.List.ReadMainList();

   return Py_None;
}

static PyMethodDef PkgSourceListMethods[] = 
{
   {"FindIndex",PkgSourceListFindIndex,METH_VARARGS,doc_PkgSourceListFindIndex},
   {"ReadMainList",PkgSourceListReadMainList,METH_VARARGS,doc_PkgSourceListReadMainList},
   {}
};

static PyObject *PkgSourceListAttr(PyObject *Self,char *Name)
{
     return Py_FindMethod(PkgSourceListMethods,Self,Name);
}
PyTypeObject PkgSourceListType =
{
   PyObject_HEAD_INIT(&PyType_Type)
   0,			                // ob_size
   "pkgSourceList",                          // tp_name
   sizeof(CppOwnedPyObject<PkgSourceListStruct>),   // tp_basicsize
   0,                                   // tp_itemsize
   // Methods
   CppOwnedDealloc<PkgSourceListStruct>,   // tp_dealloc
   0,                                   // tp_print
   PkgSourceListAttr,                      // tp_getattr
   0,                                   // tp_setattr
   0,                                   // tp_compare
   0,                                   // tp_repr
   0,                                   // tp_as_number
   0,                                   // tp_as_sequence
   0,                                   // tp_as_mapping
   0,                                   // tp_hash
};

PyObject *GetPkgSourceList(PyObject *Self,PyObject *Args)
{
   PyObject *Owner;
//   if (PyArg_ParseTuple(Args,"O!",&PkgCacheType,&Owner) == 0)
//      return 0;

   return HandleErrors(CppOwnedPyObject_NEW<PkgSourceListStruct>(Owner,
                                                                 &PkgSourceListType));
}
