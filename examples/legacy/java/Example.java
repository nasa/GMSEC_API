import gov.nasa.gsfc.gmsecapi.Config;
import gov.nasa.gsfc.gmsecapi.Connection;
import gov.nasa.gsfc.gmsecapi.ConnectionFactory;
import gov.nasa.gsfc.gmsecapi.Status;
import gov.nasa.gsfc.gmsecapi.util.*;

interface Example
{

	abstract boolean run() throws ExampleException;

	abstract boolean cleanup() throws ExampleException;

}
