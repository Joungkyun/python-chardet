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

#define DETECTOR_TRUE 1
#define DETECTOR_FALSE 0

#define ENCODING_LENGTH 32
#ifndef SAFE_STRCPY
	#define SAFE_STRCPY(x,y,l) \
		do{ \
			(l>=ENCODING_LENGTH)? \
			strncpy(x,y,ENCODING_LENGTH-1) : strcpy(x,y); \
			x[ENCODING_LENGTH-1]=0; \
		} while (0)
#endif

typedef struct {
	PyObject_HEAD

	// Universal_members
	PyObject * result;
	char       done;

	Detect   * ptr;
	char       encoding[ENCODING_LENGTH];
	float      confidence;
} Universal;

static void Universal_set_result (Universal * self, char * encoding, float confidence) {
	PyObject * prop;

	if ( self->result == NULL )
		self->result = PyDict_New ();
	else
		PyDict_Clear (self->result);

	prop = Py_BuildValue ("s", encoding);
	PyDict_SetItemString (self->result, "encoding", prop);
	Py_DECREF (prop);
	prop = Py_BuildValue ("f", confidence);
	PyDict_SetItemString (self->result, "confidence", prop);
	Py_DECREF (prop);
}

static void Universal_dealloc (Universal * self) {
	if ( self != NULL ) {
		Py_XDECREF (self->result);
#if PY_MAJOR_VERSION >= 3
		Py_TYPE(self)->tp_free ((PyObject *) self);
#else
		self->ob_type->tp_free ((PyObject *) self);
#endif
	}
}

static PyObject * Universal_new (PyTypeObject * type, PyObject * args, PyObject *kwds) {
	Universal * self;

	if ( (self = (Universal *) type->tp_alloc (type, 0)) == NULL )
		PyErr_SetString (PyExc_ValueError, "Memory allocation failed");

	self->ptr  = NULL;

	return (PyObject *) self;
}

static int Universal_init (Universal * self, PyObject *args, PyObject *kwds) {
	PyObject * prop;

	if ( (self->ptr = detect_init ()) == NULL ) {
		PyErr_SetString (PyExc_ValueError, "failed detect_init");
		return 1;
	}

	self->done = DETECTOR_FALSE;
	memset (self->encoding, 0, ENCODING_LENGTH);
	self->confidence = 0.0;

	if ( (self->result = PyDict_New ()) == NULL ) {
		detect_destroy (&self->ptr);
		Py_DECREF (self);
#if PY_MAJOR_VERSION >= 3
		Py_TYPE(self)->tp_free ((PyObject *) self);
#else
		self->ob_type->tp_free ((PyObject*) self);
#endif
		PyErr_SetString (PyExc_ValueError, "Dictionary initionalize failed");
		return 1;
	}

	prop = Py_BuildValue ("s", "None");
	PyDict_SetItemString (self->result, "encoding", prop);
	Py_DECREF (prop);
	prop = Py_BuildValue ("f", 0.0);
	PyDict_SetItemString (self->result, "confidence", prop);
	Py_DECREF (prop);

	return 0;
}

static PyObject * Universal_close (Universal * self) {
	if ( self->ptr != NULL ) {
		detect_destroy (&self->ptr);
	}

	return Py_None;
}

static PyObject * Universal_reset (Universal * self) {
	self->done = DETECTOR_FALSE;
	memset (self->encoding, 0, ENCODING_LENGTH);
	self->confidence = 0.0;

	Universal_set_result (self, "None", 0.0);

	if ( self->ptr != NULL )
		Py_RETURN_TRUE;

	if ( (self->ptr = detect_init ()) == NULL )
		Py_RETURN_FALSE;

	Py_RETURN_TRUE;
}

/* feed 동작 여부 확인 필요, minimum confidence 가 뭐지? */
static PyObject * Universal_feed (Universal * self, PyObject * args) {
	char      * text;
	size_t      inlen;

	DetectObj *	obj;

	if ( ! PyArg_ParseTuple (args, "s#", &text, &inlen) )
		return Py_None;

	if ( inlen < 1 )
		Py_RETURN_FALSE;

	if ( self->done == DETECTOR_TRUE )
		Py_RETURN_TRUE;

	detect_reset (&self->ptr);
	if ( (obj = detect_obj_init ()) == NULL ) {
		PyErr_SetString (PyExc_ValueError, "failed detect_obj_init");
		Py_RETURN_FALSE;
	}

#ifdef CHARDET_BINARY_SAFE
	if ( detect_handledata_r (&self->ptr, text, inlen, &obj) == CHARDET_OUT_OF_MEMORY )
#else
	if ( detect_handledata (&self->ptr, text, &obj) == CHARDET_OUT_OF_MEMORY )
#endif
	{
		detect_obj_free (&obj);
		PyErr_SetString (PyExc_ValueError, "On handle processing, occured out of memory");
		Py_RETURN_FALSE;
	}

	detect_dataend (&self->ptr);

	if ( obj->confidence < 0.1 )
		Py_RETURN_FALSE;

	if ( strlen (self->encoding) < 1 ) {
		SAFE_STRCPY (self->encoding, obj->encoding, strlen (obj->encoding));
		self->confidence = obj->confidence;

		if ( obj->confidence > 0 ) {
			Universal_set_result (self, obj->encoding, obj->confidence);
			self->done = DETECTOR_FALSE;
			Py_RETURN_TRUE;
		}
	} else if ( strcmp (self->encoding, obj->encoding) == 0 ) {
		if ( self->confidence < obj->confidence )
			self->confidence = obj->confidence;
	} else {
		if ( strcmp (self->encoding, "ASCII") == 0 ) {
			if ( strcmp (obj->encoding, "ASCII") == 0 ) {
				if ( self->confidence <= obj->confidence )
					self->confidence = obj->confidence;
			} else {
				SAFE_STRCPY (self->encoding, obj->encoding, strlen (obj->encoding));
				self->confidence = obj->confidence;
			}
		} else {
			if ( strcmp (obj->encoding, "ASCII") != 0 ) {
				if ( self->confidence <= obj->confidence ) {
					SAFE_STRCPY (self->encoding, obj->encoding, strlen (obj->encoding));
					self->confidence = obj->confidence;
				}
			}
		}
	}

	if ( strcmp (self->encoding, "ASCII") != 0 && self->confidence > 0.9 ) {
		self->done = DETECTOR_TRUE;
		Universal_set_result (self, self->encoding, self->confidence);
	} else
		self->done = DETECTOR_FALSE;

	detect_obj_free (&obj);

	Py_RETURN_TRUE;
}

static PyMemberDef Universal_members[] = {
	{ "done", T_BOOL, offsetof (Universal, done), 0, "Detect complete" },
	{ "result", T_OBJECT, offsetof (Universal, result), 0, "Detected reault" },
	{ NULL }
};

static PyMethodDef Universal_methods[] = {
	{ "reset", (PyCFunction) Universal_reset, METH_NOARGS, "Chardet initionalize" },
	{ "feed", (PyCFunction) Universal_feed, METH_VARARGS, "Detect charset" },
	{ "close", (PyCFunction) Universal_close, METH_NOARGS, "Uninitionalize chardet" },
	{ NULL }
};

static PyTypeObject UniversalType = {
#if PY_MAJOR_VERSION >= 3
	PyVarObject_HEAD_INIT(NULL, 0)
#else
	PyObject_HEAD_INIT(NULL)
	0,								/*ob_size*/
#endif
	"universaldetector",			/*tp_name*/
	sizeof(Universal),				/*tp_basicsize*/
	0,								/*tp_itemsize*/
	(destructor)Universal_dealloc,	/*tp_dealloc*/
	0,								/*tp_print*/
	0,								/*tp_getattr*/
	0,								/*tp_setattr*/
	0,								/*tp_compare*/
	0,								/*tp_repr*/
	0,								/*tp_as_number*/
	0,								/*tp_as_sequence*/
	0,								/*tp_as_mapping*/
	0,								/*tp_hash */
	0,								/*tp_call*/
	0,								/*tp_str*/
	0,								/*tp_getattro*/
	0,								/*tp_setattro*/
	0,								/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,	/*tp_flags*/
	"UniversalDetector objects",	/* tp_doc */
	0,								/* tp_traverse */
	0,								/* tp_clear */
	0,								/* tp_richcompare */
	0,								/* tp_weaklistoffset */
	0,								/* tp_iter */
	0,								/* tp_iternext */
	Universal_methods,				/* tp_methods */
	Universal_members,				/* tp_members */
	0,								/* tp_getset */
	0,								/* tp_base */
	0,								/* tp_dict */
	0,								/* tp_descr_get */
	0,								/* tp_descr_set */
	0,								/* tp_dictoffset */
	(initproc)Universal_init,		/* tp_init */
	0,								/* tp_alloc */
	Universal_new,					/* tp_new */
};


#if PY_MAJOR_VERSION < 3
static struct PyMethodDef universaldetector_methods[] = { // {{{
	{ NULL, NULL }
}; // }}}
#endif

#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef universaldetector_moduledef = { // {{{
	PyModuleDef_HEAD_INIT,
	"universaldetetor",
	"Mozilla Universal charset detect C binding extension",
	-1,
	//universaldetector_methods,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
}; // }}}
#endif

MOD_INIT(universaldetector) { // {{{
	PyObject * m;

	if ( PyType_Ready (&UniversalType) < 0 )
		return MOD_ERROR_VAL;

#if PY_MAJOR_VERSION >= 3
	m = PyModule_Create (&universaldetector_moduledef);
#else
	m = Py_InitModule3 (
			"universaldetector",
			universaldetector_methods,
			"Mozilla Universal Charset detect C binding extension"
	);
#endif

	if ( m == NULL ) {
		PyErr_SetString (PyExc_ValueError, "UniversalDetector initialize error");
		return MOD_ERROR_VAL;
	}

	PyModule_AddStringConstant (m, "__version__", MOD_CHARDET_VERSION);

	Py_INCREF (&UniversalType);
	PyModule_AddObject (m, "UniversalDetector", (PyObject *) &UniversalType);

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
