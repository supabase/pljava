/*
 * Copyright (c) 2018 Tada AB and other contributors, as listed below.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the The BSD 3-Clause License
 * which accompanies this distribution, and is available at
 * http://opensource.org/licenses/BSD-3-Clause
 *
 * Contributors:
 *   Chapman Flack
 */
#include <postgres.h>

#include "pljava/type/Type_priv.h"
#include "pljava/VarlenaWrapper.h"

static jclass    s_SQLXML_Readable_class;
static jmethodID s_SQLXML_Readable_init;

static jvalue _SQLXML_coerceDatum(Type self, Datum arg)
{
	jvalue result;
	jobject vwi = pljava_VarlenaWrapper_Input(
		arg, TopTransactionContext, TopTransactionResourceOwner);
	result.l = JNI_newObject(
		s_SQLXML_Readable_class, s_SQLXML_Readable_init, vwi);
	JNI_deleteLocalRef(vwi);
	return result;
}

static Datum _SQLXML_coerceObject(Type self, jobject sqlxml)
{
	return 0; /* haven't got that direction yet */
}

/* Make this datatype available to the postgres system.
 */
extern void pljava_SQLXMLImpl_initialize(void);
void pljava_SQLXMLImpl_initialize(void)
{
	TypeClass cls = TypeClass_alloc("type.SQLXML");
	cls->JNISignature = "Ljava/sql/SQLXML;";
	cls->javaTypeName = "java.sql.SQLXML";
	cls->coerceDatum  = _SQLXML_coerceDatum;
	cls->coerceObject = _SQLXML_coerceObject;
	Type_registerType("java.sql.SQLXML", TypeClass_allocInstance(cls, XMLOID));

	s_SQLXML_Readable_class = JNI_newGlobalRef(PgObject_getJavaClass(
		"org/postgresql/pljava/jdbc/SQLXMLImpl$Readable"));
	s_SQLXML_Readable_init = PgObject_getJavaMethod(s_SQLXML_Readable_class,
		"<init>", "(Lorg/postgresql/pljava/internal/VarlenaWrapper$Input;)V");
}
