/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni.mist;

import gov.nasa.gsfc.gmsec.api.field.Field;

import gov.nasa.gsfc.gmsec.api.mist.Device;
import gov.nasa.gsfc.gmsec.api.mist.DeviceParam;
import gov.nasa.gsfc.gmsec.api.mist.Mnemonic;
import gov.nasa.gsfc.gmsec.api.mist.MnemonicSample;
import gov.nasa.gsfc.gmsec.api.mist.ProductFile;
import gov.nasa.gsfc.gmsec.api.mist.ServiceParam;

import gov.nasa.gsfc.gmsec.api.jni.field.JNIField;


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


	public static long[] listToDevicePtrs(java.util.List<Device> list)
	{
		long[] array = null;

		if (list != null && list.size() > 0)
		{
			array = new long[list.size()];

			for (int i = 0; i < list.size(); ++i)
			{
				JNIDevice device = Device.getInternal(list.get(i));

				array[i] = JNIDevice.getCPtr(device);
			}
		}

		return array;
	}


	public static JNIDevice[] listToJNIDevices(java.util.List<Device> list)
	{
		JNIDevice[] array = null;

		if (list != null && list.size() > 0)
		{
			array = new JNIDevice[list.size()];

			for (int i = 0; i < list.size(); ++i)
			{
				array[i] = Device.getInternal(list.get(i));
			}
		}

		return array;
	}


	public static long[] listToDeviceParamPtrs(java.util.List<DeviceParam> list)
	{
		long[] array = null;

		if (list != null && list.size() > 0)
		{
			array = new long[list.size()];

			for (int i = 0; i < list.size(); ++i)
			{
				JNIDeviceParam param = DeviceParam.getInternal(list.get(i));

				array[i] = JNIDeviceParam.getCPtr(param);
			}
		}

		return array;
	}


	public static JNIDeviceParam[] listToJNIDeviceParams(java.util.List<DeviceParam> list)
	{
		JNIDeviceParam[] array = null;

		if (list != null && list.size() > 0)
		{
			array = new JNIDeviceParam[list.size()];

			for (int i = 0; i < list.size(); ++i)
			{
				array[i] = DeviceParam.getInternal(list.get(i));
			}
		}

		return array;
	}


	public static long[] listToMnemonicPtrs(java.util.List<Mnemonic> list)
	{
		long[] array = null;

		if (list != null && list.size() > 0)
		{
			array = new long[list.size()];

			for (int i = 0; i < list.size(); ++i)
			{
				JNIMnemonic mnemonic = Mnemonic.getInternal(list.get(i));

				array[i] = JNIMnemonic.getCPtr(mnemonic);
			}
		}

		return array;
	}


	public static JNIMnemonic[] listToJNIMnemonics(java.util.List<Mnemonic> list)
	{
		JNIMnemonic[] array = null;

		if (list != null && list.size() > 0)
		{
			array = new JNIMnemonic[list.size()];

			for (int i = 0; i < list.size(); ++i)
			{
				array[i] = Mnemonic.getInternal(list.get(i));
			}
		}

		return array;
	}


	public static long[] listToMnemonicSamplePtrs(java.util.List<MnemonicSample> list)
	{
		long[] array = null;

		if (list != null && list.size() > 0)
		{
			array = new long[list.size()];

			for (int i = 0; i < list.size(); ++i)
			{
				JNIMnemonicSample sample = MnemonicSample.getInternal(list.get(i));

				array[i] = JNIMnemonicSample.getCPtr(sample);
			}
		}

		return array;
	}


	public static JNIMnemonicSample[] listToJNIMnemonicSamples(java.util.List<MnemonicSample> list)
	{
		JNIMnemonicSample[] array = null;

		if (list != null && list.size() > 0)
		{
			array = new JNIMnemonicSample[list.size()];

			for (int i = 0; i < list.size(); ++i)
			{
				array[i] = MnemonicSample.getInternal(list.get(i));
			}
		}

		return array;
	}


	public static long[] listToProductFilePtrs(java.util.List<ProductFile> list)
	{
		long[] array = null;

		if (list != null && list.size() > 0)
		{
			array = new long[list.size()];

			for (int i = 0; i < list.size(); ++i)
			{
				JNIProductFile pf = ProductFile.getInternal(list.get(i));

				array[i] = JNIProductFile.getCPtr(pf);
			}
		}

		return array;
	}


	public static JNIProductFile[] listToJNIProductFiles(java.util.List<ProductFile> list)
	{
		JNIProductFile[] array = null;

		if (list != null && list.size() > 0)
		{
			array = new JNIProductFile[list.size()];

			for (int i = 0; i < list.size(); ++i)
			{
				array[i] = ProductFile.getInternal(list.get(i));
			}
		}

		return array;
	}


	public static long[] listToServiceParamPtrs(java.util.List<ServiceParam> list)
	{
		long[] array = null;

		if (list != null && list.size() > 0)
		{
			array = new long[list.size()];

			for (int i = 0; i < list.size(); ++i)
			{
				JNIServiceParam param = ServiceParam.getInternal(list.get(i));

				array[i] = JNIServiceParam.getCPtr(param);
			}
		}

		return array;
	}


	public static JNIServiceParam[] listToJNIServiceParams(java.util.List<ServiceParam> list)
	{
		JNIServiceParam[] array = null;

		if (list != null && list.size() > 0)
		{
			array = new JNIServiceParam[list.size()];

			for (int i = 0; i < list.size(); ++i)
			{
				array[i] = ServiceParam.getInternal(list.get(i));
			}
		}

		return array;
	}
}
