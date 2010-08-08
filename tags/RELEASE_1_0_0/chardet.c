/*
 * $Id: chardet.c,v 1.1.1.1 2010-07-05 19:03:19 oops Exp $
 */
#include "Python.h"

#include <chardet.h>
#include "version.h"

static PyObject * ErrorObject;

PyObject * py_init (PyObject * self, PyObject * args) { // {{{
	Detect *	ptr;

	if ( (ptr = detect_init ()) == NULL )
		return Py_None;

	return Py_BuildValue ("i", (int *) ptr);
} // }}}

PyObject * py_destroy (PyObject * self, PyObject * args) { // {{{
	int *		_ptr;
	Detect *	ptr;
	if ( ! PyArg_ParseTuple (args, "i", &_ptr) )
		return NULL;

	ptr = (Detect *) _ptr;

	detect_destroy (&ptr);

	return Py_None;
} // }}}

PyObject * py_detect (PyObject * self, PyObject * args) { // {{{
	PyObject *	err;
	int *		_ptr;
	char *		text;
	int			argc;

	Detect *	ptr;
	DetectObj *	obj;
	PyObject *	dict;
	PyObject *	prop;
	static PyObject *	new;
	static PyObject *	ret;

	if ( ! PyArg_ParseTuple (args, "is|O", &_ptr, &text, &err) )
		return NULL;

	argc = PyTuple_Size (args);

	if ( err != NULL ) {
		if ( ! PyList_Check (err) ) {
			PyErr_SetString (ErrorObject, "3th argument is must PyLis");
			return NULL;
		}
	}

	ptr = (Detect *) _ptr;

	detect_reset (&ptr);
	if ( (obj = detect_obj_init ()) == NULL ) {
		if ( argc > 2 ) {
			PyObject * value = PyString_FromString ("Memory allocation failed");
			PyList_Append (err, value);
			Py_DECREF (value);
		}
		return Py_None;
	}

	if ( detect_handledata (&ptr, text, &obj) == CHARDET_OUT_OF_MEMORY ) {
		if ( argc > 2 ) {
			PyObject * value = PyString_FromString ("On handle processing, occured out of memory");
			PyList_Append (err, value);
			Py_DECREF (value);
		}
		detect_obj_free (&obj);
		return Py_None;
	}
	detect_dataend (&ptr);

	dict = PyDict_New ();

	prop = Py_BuildValue ("s", obj->encoding);
	PyDict_SetItemString (dict, "encoding", prop);
	Py_DECREF (prop);

	prop = Py_BuildValue ("f", obj->confidence);
	PyDict_SetItemString (dict, "confidence", prop);
	Py_DECREF (prop);

	detect_obj_free (&obj);

	if ( ret == NULL )
		ret = PyClass_New(NULL, PyDict_New(), PyString_FromString("CHARDET_PTR"));

	new = PyInstance_NewRaw(ret, dict);
	Py_DECREF (dict);

	return new;
} // }}}

PyObject * py_detector (PyObject * self, PyObject * args) { // {{{
	PyObject *	err;
	char *		text;
	int			argc;

	DetectObj *	obj;
	PyObject *	dict;
	PyObject *	prop;
	static PyObject *	new;
	static PyObject *	ret;

	if ( ! PyArg_ParseTuple (args, "s|O", &text, &err) )
		return NULL;

	argc = PyTuple_Size (args);

	if ( err != NULL ) {
		if ( ! PyList_Check (err) ) {
			PyErr_SetString (ErrorObject, "3th argument is must PyLis");
			return NULL;
		}
	}

	if ( (obj = detect_obj_init ()) == NULL ) {
		if ( argc > 1 ) {
			PyObject * value = PyString_FromString ("Memory allocation failed");
			PyList_Append (err, value);
			Py_DECREF (value);
		}
		return Py_None;
	}

	if ( detect (text, &obj) == CHARDET_OUT_OF_MEMORY ) {
		if ( argc > 1 ) {
			PyObject * value = PyString_FromString ("On handle processing, occured out of memory");
			PyList_Append (err, value);
			Py_DECREF (value);
		}
		detect_obj_free (&obj);
		return Py_None;
	}

	dict = PyDict_New ();

	prop = Py_BuildValue ("s", obj->encoding);
	PyDict_SetItemString (dict, "encoding", prop);
	Py_DECREF (prop);

	prop = Py_BuildValue ("f", obj->confidence);
	PyDict_SetItemString (dict, "confidence", prop);
	Py_DECREF (prop);

	detect_obj_free (&obj);

	if ( ret == NULL )
		ret = PyClass_New(NULL, PyDict_New(), PyString_FromString("CHARDET_PTR"));

	new = PyInstance_NewRaw(ret, dict);
	Py_DECREF (dict);

	return new;
} // }}}

static struct PyMethodDef chardet_methods[] = { // {{{
	{ "init",     py_init,     METH_NOARGS },
	{ "detect",   py_detect,   METH_VARARGS },
	{ "destroy",  py_destroy,  METH_VARARGS },
	{ "detector", py_detector, METH_VARARGS },
	{ NULL, NULL }
}; // }}}

void initchardet () { // {{{
	PyObject *  m;

	m = Py_InitModule ("chardet", chardet_methods);
	ErrorObject = Py_BuildValue ("s", "chardet initialize error");
} // }}}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
