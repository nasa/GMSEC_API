
/*
	This is a utility for modifying project files for use in
	Visual Studio 2010 Express.

	Since the Visual Studio 2010 Express project upgrader does not support
	64 bit targets, this tool will strip 64 bit references out of the
	specified project files.

	Usage:
		java VS2010e <path> ...

	Example:
		java VS2010e `find GMSEC_API -name '*.vcproj'`
*/

import java.io.*;

import org.w3c.dom.*;
import javax.xml.transform.*;
import javax.xml.transform.dom.*;
import javax.xml.transform.stream.*;


class VS2010e
{
	private java.io.PrintStream out = System.out;


	public static void main (String[] args) 
			throws Exception {

		if (args.length < 1)
			System.err.println("usage: java VS2010e <path> ...\n");
		else
			for (String arg : args)
				new VS2010e().process(arg);
	}


	private void process(String path)
			throws Exception {

		File infile = new File(path);
		File bakfile = temporary(infile, ".old");
		File tmpfile = temporary(infile, ".new");
		File outfile = new File(path);

		InputStream input = new FileInputStream(infile);
		StreamSource insource = new StreamSource(input);

		DOMResult domresult = new DOMResult();

		TransformerFactory factory = TransformerFactory.newInstance();
		out.println("DOMResult support is " + factory.getFeature(DOMResult.FEATURE));

		Transformer transformer = factory.newTransformer();
		transformer.transform(insource, domresult);
		input.close();

		Document document = (Document) domresult.getNode();
		// dump("document", document, true);

		remove(document, "Platform", "x64");
		remove(document, "Configuration", "\\w+\\|x64");
		remove(document, "FileConfiguration", "\\w+\\|x64");

		transformer = factory.newTransformer();
		Source domsource = new DOMSource(document);

		OutputStream output = new FileOutputStream(tmpfile);
		StreamResult outresult = new StreamResult(output);
		transformer.transform(domsource, outresult);
		output.close();

		rename(infile, bakfile);
		rename(tmpfile, outfile);
	}

	void dump(String prefix, Node node, boolean recursive) {

		out.println(prefix + ":\n\t" + node);

		NamedNodeMap attributes = node.getAttributes();
		if (attributes != null && attributes.getLength() > 0) {
			out.println("\tattributes " + attributes);
			for (int i = 0; i < attributes.getLength(); ++i) {
				Node a = attributes.item(i);
				out.println("\t\t" + a.getNodeName() + " = '" + a.getNodeValue() + "'");
			}
		}

		if (recursive) {
			NodeList children = node.getChildNodes();
			for (int i = 0; i < children.getLength(); ++i)
				dump(prefix + "[" + i + "]", children.item(i), true);
		}
	}


	void remove(Document document, String tag, String pattern) {

		NodeList nodes = document.getElementsByTagName(tag);
		out.println("found " + nodes.getLength() + " " + tag + " elements");
		for (int i = nodes.getLength()-1; i >= 0; --i) {
			Element element = (Element) nodes.item(i);
			String name = element.getAttribute("Name");
			if (name.matches(pattern)) {
				dump("remove " + tag + "[" + i + "]", element, false);
				element.getParentNode().removeChild(element);
			}
		}

	}


	File temporary (File infile, String extbase) {

		File tmpfile = null;

		int i = 0;

		while (tmpfile == null) {
			++i;
			tmpfile = new File(infile.getParent(), infile.getName() + extbase + i);
			if (tmpfile.exists())
				tmpfile = null;
		}

		return tmpfile;
	}


	void rename (File from, File to) {
		out.println("renaming " + from + " to " + to);
		if (!from.renameTo(to))
			out.println("unable to rename " + from + " to " + to);
	}
}


