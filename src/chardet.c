/*
 * Copyfight 2021. JoungKyun.Kim all rights reserevd.
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

#include <Python.h>
#include <structmember.h>

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

static PyObject * py_detect (PyObject * self, PyObject * args) { // {{{
	PyObject        * err = NULL;
	char            * text;
	char            * buf;
	size_t            inlen;
	int               argc;

	DetectObj       * obj;
	PyObject        * dict;
	PyObject        * prop;

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

	if ( strcmp (obj->encoding, "UTF-8") == 0 && obj->bom == 1 ) {
		int buflen = sizeof (char) * (strlen (obj->encoding) + 5);
		buf = malloc (buflen);
		sprintf (buf, "%s-SIG", obj->encoding);
		buf[buflen - 1] = 0;
	} else {
		buf = strdup (obj->encoding);
	}

	prop = Py_BuildValue ("s", buf);
	PyDict_SetItemString (dict, "encoding", prop);
	Py_DECREF (prop);
	free (buf);

	prop = Py_BuildValue ("f", obj->confidence);
	PyDict_SetItemString (dict, "confidence", prop);
	Py_DECREF (prop);

#ifdef CHARDET_BOM_CHECK
	prop = Py_BuildValue ("d", obj->bom);
	PyDict_SetItemString (dict, "bom", prop);
	Py_DECREF (prop);
#endif

	detect_obj_free (&obj);

	return dict;
} // }}}

static struct PyMethodDef chardet_methods[] = { // {{{
	{ "detect",   py_detect,   METH_VARARGS },
	{ NULL, NULL }
}; // }}}

#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef chardet_moduledef = { // {{{
	PyModuleDef_HEAD_INIT,
	"_chardet",
	"Mozilla Universal charset detect C binding extension",
	-1,
	chardet_methods,
	NULL,
	NULL,
	NULL,
	NULL
}; // }}}
#endif

MOD_INIT(_chardet) { // {{{
	PyObject * m;

#if PY_MAJOR_VERSION >= 3
	m = PyModule_Create (&chardet_moduledef);
#else
	m = Py_InitModule3 (
			"_chardet",
			chardet_methods,
			"Mozilla Universal charset detect C binding extension"
	);
#endif

	if ( m == NULL )
		return MOD_ERROR_VAL;

	PyModule_AddStringConstant (m, "__version__", MOD_CHARDET_VERSION);
	ErrorObject = Py_BuildValue ("s", "_chardet initialize error");

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
