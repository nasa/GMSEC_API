/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api5.jni;

import gov.nasa.gsfc.gmsec.api5.field.Field;

import gov.nasa.gsfc.gmsec.api5.jni.field.JNIField;


public class ArrayListConverter
{
	public static long[] listToFieldPtrs(java.util.List<Field> list)
	{
		long[] array = null;

		if (list != null && list.size() > 0)
		{
			array = new long[list.size()];

			for (int i = 0; i < list.size(); ++i)
			{
				JNIField field = Field.getInternal(list.get(i));

				array[i] = JNIField.getCPtr(field);
			}
		}

		return array;
	}


	public static JNIField[] listToJNIFields(java.util.List<Field> list)
	{
		JNIField[] array = null;

		if (list != null && list.size() > 0)
		{
			array = new JNIField[list.size()];

			for (int i = 0; i < list.size(); ++i)
			{
				array[i] = Field.getInternal(list.get(i));
			}
		}

		return array;
	}
}
