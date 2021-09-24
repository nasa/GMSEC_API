
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






package bolt;

public class Topic
{
	public static boolean isValid (String topic) {
		return areValidElements(getElements(topic), false);
	}

	public static boolean isValidSubscription (String topic) {
		return areValidElements(getElements(topic), true);
	}

	public static String[] getElements (String topic) {
		// this might be more efficient looking for periods?
		return topic.split("\\.", -1);
	}

	public static boolean areValidElements (String[] elements,
			boolean subscription) {
		for (int i = 0; i < elements.length; ++i)
			if (!isValidElement(elements[i], subscription,
					i == elements.length - 1))
				return false;
		return true;
	}

	static boolean isValidElement (String element, boolean subscription,
			boolean last) {
		boolean valid = false;

		if (element.length() < 1)
			valid = false;
		else if (subscription && last && (element.equals(">") || element.equals("+") ))
			valid = true;
		else if (subscription && element.equals("*"))
			valid = true;
		else {
			valid = true;
			for (char c : element.toCharArray())
				if (!isValidCharacter(c))
					valid = false;
		}

		return valid;
	}

	// since lowercase characters are prevalent in topic, allow for now
	final private static boolean ALLOW_LOWERCASE = true;
	static boolean[] validTopicChar;
	static {
		validTopicChar = new boolean[128];

		for (int i = 0; i < 10; ++i)
			validTopicChar['0' + i] = true;

		for (int i = 0; i < 26; ++i)
			validTopicChar['A' + i] = true;

		validTopicChar['-'] = true;
		validTopicChar['_'] = true;

		if (ALLOW_LOWERCASE)
			for (int i = 0; i < 26; ++i)
				validTopicChar['a' + i] = true;

	}

	static boolean isValidCharacter (char c) {
		if (c >= 0 && c < validTopicChar.length)
			return validTopicChar[c];
		return false;
	}

	static void test (String s) {
		boolean valid = isValid(s);
		System.out.println("test(" + s + ") => " + valid);
	}

	static void test (String s, boolean expected) {
		boolean valid = isValid(s);

		System.out.println("test(" + s + ") => " + valid);
		if (valid != expected)
			System.out.println("WRONG");
	}

	static void test () {
		test("A.B", true);
		test("AB.", false);
		test("A>", false);
		test(">A", false);
		test(">.A", false);
		test("..", false);
		test("*.A", true);
		test("A.*.B", true);
		test("A.**.B", false);
		test("A.>.B", false);
		test("a.>", false);
		test("HELLo", false);

	}

	public static void main (String[] args) {
		if (args.length > 0)
			for (String s : args)
				test(s);
		else
			test();
	}
}
