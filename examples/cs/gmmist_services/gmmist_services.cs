/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/*
 * @file gmmist_services.cs
 * 
 * A C# example demonstration of GMSEC the ConnectionManager class services
 * for logging and heartbeats, as well as general message validation. 
 * 
 */

namespace gmmist_services
{
    using GMSEC.API;
    using GMSEC.API.UTIL;
    using GMSEC.API.MIST;
    using GMSEC.API.Example.Common;
    using System;

    class gmmist_services : GmsecExample
    {

        private Config config;
        private ConnectionManager connManager;

        private void PrintUsage()
        {
            Log.Error("\nusage: gmmist_services_cs connectiontype=<middleware> " +
                      "[ <parameter>=<value> ]\n" +
                      "\n\tNote that the parameter 'connectiontype' is required. " +
                      "\n\tThe rest of other parameters are optional.\n" +
                      "\n\tserver=<server name> " +
                      "(required if middleware is not bolt/MB locally)" +
                      "\n\tsubject=<subject name>" +
                      "\n\titerations=<iterations>" +
                      "\n\tinterval_ms=<interval (milliseconds)>" +
                      "\n\tloglevel=<log level>" +
                      "\n\tcfgfile=<config_filename.xml>" +
                      "\n\nFor more information, see API User's Guide\n");
        }

        private void Run(string[] args)
        {
            config = new Config(args);

            Initialize(config);
            AddToConfigFromFile(config);

            if (IsOptionInValid(config, args, false))
            {
                PrintUsage();
                return;
            }

            //Output GMSEC API version
            Log.Info(Connection.GetAPIVersion());

            try
            {
                connManager = new ConnectionManager(config);

                connManager.Initialize();

                //o SetStandardFields()
                Log.Info("Setting fields which will be applied globally to message from this ConnectionManager");
                Log.Info("These fields are usually certain values from the GMSEC Header which apply broadly");
                
                F32Field fieldVersion = new F32Field("HEADER-VERSION", (float)2010.0);
                StringField fieldMission = new StringField("MISSION-ID", "GMSEC");
                StringField fieldFacility = new StringField("FACILITY", "GMSEC Lab");
                StringField fieldComponent = new StringField("COMPONENT", "gmmist");

                System.Collections.Generic.List<Field> definedFields = new System.Collections.Generic.List<Field>
                { fieldVersion, fieldMission, fieldFacility, fieldComponent };

                connManager.SetStandardFields(definedFields);

                StringField fieldSubclass = new StringField("SUBCLASS", "TAC");
                StringField fieldOccurType = new StringField("OCCURRENCE-TYPE", "NORM");

                System.Collections.Generic.List<Field> logDefinedFields = new System.Collections.Generic.List<Field>
                { fieldSubclass, fieldOccurType};
                
                Log.Info("No need to add MESSAGE-TYPE and MESSAGE-SUBTYPE for a log, we do that for you!");
                Log.Info("Adding general log fields");

                connManager.SetLoggingDefaults("GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.LOG", logDefinedFields);


                I16Field fieldLogSeverity = new I16Field("SEVERITY", 1);
                Log.Info("Sending log message regarding imminent start of heartbeat service");
                connManager.PublishLog("About to start heartbeat service", fieldLogSeverity);

                I16Field fieldPubRate  = new I16Field("PUB-RATE", 1);
                I16Field fieldCounter  = new I16Field("COUNTER", 0);
                StringField fieldMsgId = new StringField("MSG-ID", "My heartbeat identifier");

                System.Collections.Generic.List<Field> hbDefinedFields = new System.Collections.Generic.List<Field>
                { fieldPubRate, fieldCounter, fieldMsgId};
                
                Log.Info("No need to add MESSAGE-TYPE, MESSAGE-SUBTYPE, and C2CX-SUBTYPE for a heartbeat, we do that for you!");
                Log.Info("Adding general heartbeat fields");
                
                Log.Info("Starting heartbeat service");
                connManager.StartHeartbeatService("GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.C2CX.HB", hbDefinedFields);
                Log.Info("Heartbeat service has been started.Publish rate is defined by PUB - RATE, defaulting to 30 seconds if not defined. "
                    + "Component status will be reported as nominal unless otherwise instructed. The counter will automatically increment.");


                connManager.PublishLog("Heartbest service has been started!", fieldLogSeverity);

                
                Log.Info("Time to start up the resource service. This will automatically publish resource information");
                connManager.StartResourceMessageService("GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.C2CX.EXAMPLE.RSRC.EXAMPLE2", 1, 1, 1);
                
                Log.Info("Waiting for 60 seconds to allow heartbeats and resources to publish.");
                Log.Info("The user may wish to use gmsub to observe message traffic.");
                new System.Threading.AutoResetEvent(false).WaitOne(60000);
                
                Log.Info("Changing the example component status to RED");
                I16Field compStatus = new I16Field("COMPONENT-STATUS", 4);
                connManager.ChangeComponentStatus(compStatus);
                
                Log.Info("Wait another 60 seconds to observe the change");
                new System.Threading.AutoResetEvent(false).WaitOne(60000);


                connManager.PublishLog("End of demonstration, cleaning up ConnectionManager", fieldLogSeverity);
                Log.Info("End of demonstration, cleaning up ConnectionManager");


                connManager.StopHeartbeatService();
                Log.Info("Stopping heartbeat service. This will happen automatically when the ConnectionManager cleans up resources, "
                     + "should the user not perform so manually.");

                Log.Info("Demo complete!");

            }
            catch (GMSEC_Exception e)
	        {
                Log.Error(e.ToString());
            }
            finally
            {
                connManager.Cleanup();
            }
            
        }

        static void Main(string[] args)
        {
            new gmmist_services().Run(args);
        }
    }
}
