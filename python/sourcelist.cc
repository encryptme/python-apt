// -*- mode: cpp; mode: fold -*-
// Description								/*{{{*/
// $Id: sourcelist.cc,v 1.2 2003/12/26 17:04:22 mdz Exp $
/* ######################################################################

   SourcesList - Wrapper for the SourcesList functions

   ##################################################################### */
									/*}}}*/
// Include Files							/*{{{*/
#include "generic.h"
#include "apt_pkgmodule.h"

#include <apt-pkg/sourcelist.h>

#include <Python.h>
									/*}}}*/



// PkgsourceList Class							/*{{{*/
// ---------------------------------------------------------------------

static char *doc_PkgSourceListFindIndex = "xxx";
static PyObject *PkgSourceListFindIndex(PyObject *Self,PyObject *Args)
{
   pkgSourceList *list = GetCpp<pkgSourceList*>(Self);
   PyObject *pyPkgFileIter;
   PyObject *pyPkgIndexFile;

   if (PyArg_ParseTuple(Args, "O!", &PackageFileType,&pyPkgFileIter) == 0)
      return 0;

   pkgCache::PkgFileIterator &i = GetCpp<pkgCache::PkgFileIterator>(pyPkgFileIter);
   pkgIndexFile *index;
   if(list->FindIndex(i, index))
   {
      pyPkgIndexFile = CppOwnedPyObject_NEW<pkgIndexFile*>(pyPkgFileIter,&PackageIndexFileType,index);
      return pyPkgIndexFile;
   }

   //&PackageIndexFileType,&pyPkgIndexFile)

   Py_INCREF(Py_None);
   return Py_None;
}

static char *doc_PkgSourceListReadMainList = "xxx";
static PyObject *PkgSourceListReadMainList(PyObject *Self,PyObject *Args)
{
   pkgSourceList *list = GetCpp<pkgSourceList*>(Self);
   bool res = list->ReadMainList();

   return HandleErrors(Py_BuildValue("b",res));
}

static char *doc_PkgSourceListGetIndexes = "Load the indexes into the fetcher";
static PyObject *PkgSourceListGetIndexes(PyObject *Self,PyObject *Args)
{
   pkgSourceList *list = GetCpp<pkgSourceList*>(Self);

   PyObject *pyFetcher;
   char all = 0;
   if (PyArg_ParseTuple(Args, "O!|b",&PkgAcquireType,&pyFetcher, &all) == 0)
      return 0;

   pkgAcquire *fetcher = GetCpp<pkgAcquire*>(pyFetcher);
   bool res = list->GetIndexes(fetcher, all);

   return HandleErrors(Py_BuildValue("b",res));
}

static PyMethodDef PkgSourceListMethods[] =
{
   {"FindIndex",PkgSourceListFindIndex,METH_VARARGS,doc_PkgSourceListFindIndex},
   {"ReadMainList",PkgSourceListReadMainList,METH_VARARGS,doc_PkgSourceListReadMainList},
   {"GetIndexes",PkgSourceListGetIndexes,METH_VARARGS,doc_PkgSourceListGetIndexes},
   {}
};

static PyObject *PkgSourceListGetList(PyObject *Self,void*)
{
   pkgSourceList *list = GetCpp<pkgSourceList*>(Self);
   PyObject *List = PyList_New(0);
   for (vector<metaIndex *>::const_iterator I = list->begin();
        I != list->end(); I++)
   {
      PyObject *Obj;
      Obj = CppPyObject_NEW<metaIndex*>(&MetaIndexType,*I);
      PyList_Append(List,Obj);
   }
   return List;
}

static PyGetSetDef PkgSourceListGetSet[] = {
    {"List",PkgSourceListGetList,0,"A list of MetaIndex() objects.",0},
    {}
};

PyTypeObject PkgSourceListType =
{
   PyObject_HEAD_INIT(&PyType_Type)
   0,			                // ob_size
   "pkgSourceList",                          // tp_name
   sizeof(CppPyObject<pkgSourceList*>),   // tp_basicsize
   0,                                   // tp_itemsize
   // Methods
   CppDealloc<pkgSourceList*>,   // tp_dealloc
   0,                                   // tp_print
   0,                                   // tp_getattr
   0,                                   // tp_setattr
   0,                                   // tp_compare
   0,                                   // tp_repr
   0,                                   // tp_as_number
   0,                                   // tp_as_sequence
   0,                                   // tp_as_mapping
   0,                                   // tp_hash
   0,                                   // tp_call
   0,                                   // tp_str
   0,                                   // tp_getattro
   0,                                   // tp_setattro
   0,                                   // tp_as_buffer
   Py_TPFLAGS_DEFAULT,                  // tp_flags
   "pkgSourceList Object",              // tp_doc
   0,                                   // tp_traverse
   0,                                   // tp_clear
   0,                                   // tp_richcompare
   0,                                   // tp_weaklistoffset
   0,                                   // tp_iter
   0,                                   // tp_iternext
   PkgSourceListMethods,                // tp_methods
   0,                                   // tp_members
   PkgSourceListGetSet,                 // tp_getset
};

PyObject *GetPkgSourceList(PyObject *Self,PyObject *Args)
{
   return CppPyObject_NEW<pkgSourceList*>(&PkgSourceListType,new pkgSourceList());
}

