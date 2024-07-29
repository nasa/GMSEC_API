import gov.nasa.gsfc.gmsec.api5.U8;
import gov.nasa.gsfc.gmsec.api5.U16;
import gov.nasa.gsfc.gmsec.api5.U32;
import gov.nasa.gsfc.gmsec.api5.U64;
import gov.nasa.gsfc.gmsec.api5.GmsecException;

import gov.nasa.gsfc.gmsec.api5.util.Log;

import java.math.BigInteger;


public class T027_Types extends TestCase
{
	public static void main(String[] args)
	{
		T027_Types test = new T027_Types();
		test.setDescription("Test Types");
		test.run(args);
	}


	public void testCase()
		throws Exception
	{
		try
		{
			test_U8();
			test_U16();
			test_U32();
			test_U64();
		}
		catch (Exception e)
		{
			require(e.getMessage(), false);
		}
	}


	private void test_U8()
		throws GmsecException
	{
		Log.info("Test U8");

		//o Test constructor limits
		{
			U8 def = new U8();
			U8 lo  = new U8(U8.MIN_VALUE);
			U8 hi  = new U8(U8.MAX_VALUE);
			U8 mid = new U8(127);

			check("Expected U8 default value of 0", def.get() == 0);
			check("Expected U8 low value of 0", lo.get() == 0);
			check("Expected U8 high value of 255", hi.get() == 255);
			check("Expected U8 mid value of 127", mid.get() == 127);
		}

		//o Test copy constructor
		{
			U8 mid  = new U8(127);
			U8 copy = new U8(mid);
			check("Expected U8 copy value of 127", copy.get() == 127);
		}

		//o Test set()
		{
			U8 lo  = new U8();
			U8 hi  = new U8();
			U8 mid = new U8();
			lo.set(U8.MIN_VALUE);
			hi.set(U8.MAX_VALUE);
			mid.set(127);
			check("Expected U8 set low value of 0", lo.get() == 0);
			check("Expected U8 set high value of 255", hi.get() == 255);
			check("Expected U8 set mid value of 127", mid.get() == 127);
		}

		//o Off-nominal
		try {
			new U8(U8.MIN_VALUE - 1);
			check("Expected an exception", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Value is less than U8.MIN_VALUE"));
		}

		try {
			new U8(U8.MAX_VALUE + 1);
			check("Expected an exception", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Value is greater than U8.MAX_VALUE"));
		}

		try {
			U8 badset = new U8();
			badset.set(U8.MIN_VALUE - 1);
			check("Expected an exception", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Value is less than U8.MIN_VALUE"));
		}

		try {
			U8 badset = new U8();
			badset.set(U8.MAX_VALUE + 1);
			check("Expected an exception", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Value is greater than U8.MAX_VALUE"));
		}
	}

	private void test_U16()
		throws GmsecException
	{
		Log.info("Test U16");

		//o Test constructor limits
		{
			U16 def = new U16();
			U16 lo  = new U16(U16.MIN_VALUE);
			U16 hi  = new U16(U16.MAX_VALUE);
			U16 mid = new U16(14000);

			check("Expected U16 default value of 0", def.get() == 0);
			check("Expected U16 low value of 0", lo.get() == 0);
			check("Expected U16 high value of 65535", hi.get() == 65535);
			check("Expected U16 mid value of 14000", mid.get() == 14000);
		}

		//o Test copy constructor
		{
			U16 mid  = new U16(14000);
			U16 copy = new U16(mid);
			check("Expected U16 copy value of 14000", copy.get() == 14000);
		}

		//o Test set()
		{
			U16 lo  = new U16();
			U16 hi  = new U16();
			U16 mid = new U16();
			lo.set(U16.MIN_VALUE);
			hi.set(U16.MAX_VALUE);
			mid.set(14000);
			check("Expected U16 set low value of 0", lo.get() == 0);
			check("Expected U16 set high value of 65535", hi.get() == 65535);
			check("Expected U16 set mid value of 14000", mid.get() == 14000);
		}

		//o Off-nominal
		try {
			new U16(U16.MIN_VALUE - 1);
			check("Expected an exception", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Value is less than U16.MIN_VALUE"));
		}

		try {
			new U16(U16.MAX_VALUE + 1);
			check("Expected an exception", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Value is greater than U16.MAX_VALUE"));
		}

		try {
			U16 badset = new U16();
			badset.set(U16.MIN_VALUE - 1);
			check("Expected an exception", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Value is less than U16.MIN_VALUE"));
		}

		try {
			U16 badset = new U16();
			badset.set(U16.MAX_VALUE + 1);
			check("Expected an exception", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Value is greater than U16.MAX_VALUE"));
		}
	}


	private void test_U32()
		throws GmsecException
	{
		Log.info("Test U32");

		//o Test constructor limits
		{
			U32 def = new U32();
			U32 lo  = new U32(U32.MIN_VALUE);
			U32 hi  = new U32(U32.MAX_VALUE);
			U32 mid = new U32(14000);

			check("Expected U32 default value of 0", def.get() == 0);
			check("Expected U32 low value of 0", lo.get() == 0);
			check("Expected U32 high value of 4,294,967,295", hi.get() == 4294967295L);
			check("Expected U32 mid value of 14000", mid.get() == 14000);
		}

		//o Test copy constructor
		{
			U32 mid  = new U32(14000);
			U32 copy = new U32(mid);
			check("Expected U32 copy value of 14000", copy.get() == 14000);
		}

		//o Test set()
		{
			U32 lo  = new U32();
			U32 hi  = new U32();
			U32 mid = new U32();
			lo.set(U32.MIN_VALUE);
			hi.set(U32.MAX_VALUE);
			mid.set(14000);
			check("Expected U32 low value of 0", lo.get() == 0);
			check("Expected U32 high value of 4,294,967,295", hi.get() == 4294967295L);
			check("Expected U32 mid value of 14000", mid.get() == 14000);
		}

		//o Off-nominal
		try {
			new U32(U32.MIN_VALUE - 1);
			check("Expected an exception", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Value is less than U32.MIN_VALUE"));
		}

		try {
			new U32(U32.MAX_VALUE + 1);
			check("Expected an exception", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Value is greater than U32.MAX_VALUE"));
		}

		try {
			U32 badset = new U32();
			badset.set(U32.MIN_VALUE - 1);
			check("Expected an exception", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Value is less than U32.MIN_VALUE"));
		}

		try {
			U32 badset = new U32();
			badset.set(U32.MAX_VALUE + 1);
			check("Expected an exception", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Value is greater than U32.MAX_VALUE"));
		}
	}


	private void test_U64()
		throws GmsecException
	{
		Log.info("Test U64");

		//o Test constructor limits
		{
			U64 def = new U64();
			U64 lo  = new U64(U64.MIN_VALUE);
			U64 hi  = new U64(U64.MAX_VALUE);
			U64 mid = new U64(new BigInteger("14000"));

			check("Expected U64 default value", def.get().equals(new BigInteger("0")));
			check("Expected U64 low value", lo.get().equals(new BigInteger("0")));
			check("Expected U64 high value", hi.get().equals(new BigInteger("18446744073709551615")));
			check("Expected U64 mid value", mid.get().equals(new BigInteger("14000")));
		}

		//o Test copy constructor
		{
			U64 mid  = new U64(new BigInteger("14000"));
			U64 copy = new U64(mid);
			check("Expected U64 copy value", mid.get().equals(new BigInteger("14000")));
		}

		//o Test set()
		{
			U64 lo  = new U64();
			U64 hi  = new U64();
			U64 mid = new U64();
			lo.set(U64.MIN_VALUE);
			hi.set(U64.MAX_VALUE);
			mid.set(new BigInteger("14000"));
			check("Expected U64 low value", lo.get().equals(new BigInteger("0")));
			check("Expected U64 high value", hi.get().equals(new BigInteger("18446744073709551615")));
			check("Expected U64 mid value", mid.get().equals(new BigInteger("14000")));
		}

		//o Off-nominal
		try {
			new U64(new BigInteger("-1"));
			check("Expected an exception", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Value is less than U64.MIN_VALUE"));
		}

		try {
			new U64(new BigInteger("18446744073709551616"));
			check("Expected an exception", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Value is greater than U64.MAX_VALUE"));
		}

		try {
			U64 badset = new U64();
			badset.set(new BigInteger("-1"));
			check("Expected an exception", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Value is less than U64.MIN_VALUE"));
		}

		try {
			U64 badset = new U64();
			badset.set(new BigInteger("18446744073709551616"));
			check("Expected an exception", false);
		}
		catch (IllegalArgumentException e) {
			check(e.getMessage(), e.getMessage().contains("Value is greater than U64.MAX_VALUE"));
		}
	}
}
