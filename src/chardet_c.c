/*
 * Copyfight (c) 2018 JoungKyun.Kim <hostmaster@oops.org>
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 */

#include "Python.h"

#include <chardet.h>
#include "version.h"

#if PY_MAJOR_VERSION >= 3
	#define MOD_ERROR_VAL NULL
	#define MOD_SUCCESS_VAL(val) val
	#define MOD_INIT(name) PyMODINIT_FUNC PyInit_##name(void)
	#define PyString_FromString PyBytes_FromString
#else
	#define MOD_ERROR_VAL
	#define MOD_SUCCESS_VAL(val)
	#define MOD_INIT(name) PyMODINIT_FUNC init##name(void)
#endif

static PyObject * ErrorObject;

static PyObject * py_init (PyObject * self, PyObject * args) { // {{{
	Detect *	ptr;

	if ( (ptr = detect_init ()) == NULL )
		return Py_None;

	return Py_BuildValue ("l", (long *) ptr);
} // }}}

static PyObject * py_destroy (PyObject * self, PyObject * args) { // {{{
	Detect *	ptr;
	if ( ! PyArg_ParseTuple (args, "l", (long *) &ptr) )
		return NULL;

	detect_destroy (&ptr);

	return Py_None;
} // }}}

static PyObject * py_detect (PyObject * self, PyObject * args) { // {{{
	PyObject *	err;
	char *		text;
	size_t      inlen;
	int			argc;

	Detect *	ptr;
	DetectObj *	obj;
	PyObject *	dict;
	PyObject *	prop;
	PyObject *	new;
#if PY_MAJOR_VERSION < 3
	static PyObject *	ret;
#endif

	if ( ! PyArg_ParseTuple (args, "ls#|O", (long *) &ptr, &text, &inlen, &err) )
		return NULL;

	argc = PyTuple_Size (args);

	if ( err != NULL ) {
		if ( ! PyList_Check (err) ) {
			PyErr_SetString (ErrorObject, "3th argument is must PyLis");
			return NULL;
		}
	}

	detect_reset (&ptr);
	if ( (obj = detect_obj_init ()) == NULL ) {
		if ( argc > 2 ) {
			PyObject * value = PyString_FromString ("Memory allocation failed");
			PyList_Append (err, value);
			Py_DECREF (value);
		}
		return Py_None;
	}

#ifdef CHARDET_BINARY_SAFE
	if ( detect_handledata_r (&ptr, text, inlen, &obj) == CHARDET_OUT_OF_MEMORY )
#else
	if ( detect_handledata (&ptr, text, &obj) == CHARDET_OUT_OF_MEMORY )
#endif
	{
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

#if PY_MAJOR_VERSION >= 3
	new = _PyNamespace_New (dict);
#else
	if ( ret == NULL )
		ret = PyClass_New(NULL, PyDict_New(), PyString_FromString("CHARDET_PTR"));

	new = PyInstance_NewRaw(ret, dict);
#endif
	Py_DECREF (dict);

	return new;
} // }}}

static PyObject * py_detector (PyObject * self, PyObject * args) { // {{{
	PyObject *	err;
	char *		text;
	size_t		inlen;
	int			argc;

	DetectObj *	obj;
	PyObject *	dict;
	PyObject *	prop;
	PyObject *	new;
#if PY_MAJOR_VERSION < 3
	static PyObject *	ret;
#endif

	if ( ! PyArg_ParseTuple (args, "s#|O", &text, &inlen, &err) )
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

#ifdef CHARDET_BINARY_SAFE
	if ( detect_r (text, inlen, &obj) == CHARDET_OUT_OF_MEMORY )
#else
	if ( detect (text, &obj) == CHARDET_OUT_OF_MEMORY )
#endif
	{
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

#if PY_MAJOR_VERSION >= 3
	new = _PyNamespace_New (dict);
#else
	if ( ret == NULL )
		ret = PyClass_New(NULL, PyDict_New(), PyString_FromString("CHARDET_PTR"));

	new = PyInstance_NewRaw(ret, dict);
#endif
	Py_DECREF (dict);

	return new;
} // }}}

static struct PyMethodDef chardet_c_methods[] = { // {{{
	{ "init",     py_init,     METH_NOARGS },
	{ "detect",   py_detect,   METH_VARARGS },
	{ "destroy",  py_destroy,  METH_VARARGS },
	{ "detector", py_detector, METH_VARARGS },
	{ NULL, NULL }
}; // }}}

#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef chardet_c_moduledef = { // {{{
	PyModuleDef_HEAD_INIT,
	"chardet_c",
	"Mozilla Universal charset detect C binding extension",
	-1,
	chardet_c_methods,
	NULL,
	NULL,
	NULL,
	NULL
}; // }}}
#endif

MOD_INIT(chardet_c) { // {{{
	PyObject * m;

#if PY_MAJOR_VERSION >= 3
	m = PyModule_Create (&chardet_c_moduledef);
#else
	m = Py_InitModule3 (
			"chardet_c",
			chardet_c_methods,
			"Mozilla Universal charset detect C binding extension"
	);
#endif

	if ( m == NULL )
		return MOD_ERROR_VAL;

	PyModule_AddStringConstant (m, "__version__", MOD_CHARDET_VERSION);
	ErrorObject = Py_BuildValue ("s", "chardet_c initialize error");

	return MOD_SUCCESS_VAL(m);
} // }}}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
