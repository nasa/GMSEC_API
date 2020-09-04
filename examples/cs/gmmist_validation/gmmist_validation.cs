namespace gmmist_validation
{
    using GMSEC.API;
    using GMSEC.API.UTIL;
    using GMSEC.API.MIST;
    using GMSEC.API.Example.Common;
    using System;

    class gmmist_validation : GmsecExample
    {
        private Config config;
        private ConnectionManager connManager;

        private void PrintUsage()
        {
            Log.Error("\nusage: gmmist_validation_cs connectiontype=<middleware> " +
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

            if(IsOptionInValid(config, args, false))
            {
                PrintUsage();
                return;
            }

            Log.Info(Connection.GetAPIVersion());

            try
            {
                connManager = new ConnectionManager(config, true, GMSEC_ISD.GMSEC_ISD_CURRENT);

                connManager.Initialize();

                //o setStandardFields()
                Log.Info("Setting fields which will be applied globally to message from this ConnectionManager");
                Log.Info("These fields are usually certain values from the GMSEC Header which apply broadly");

                System.Collections.Generic.List<Field> definedFields = new System.Collections.Generic.List<Field> { };

                F32Field fieldVersion = new F32Field("HEADER-VERSION", (float)2010.0);
                StringField fieldMission = new StringField("MISSION-ID", "GMSEC");
                StringField fieldFacility = new StringField("FACILITY", "GMSEC Lab");
                StringField fieldComponent = new StringField("COMPONENT", "gmmist-demo");

                definedFields.Add(fieldVersion);
                definedFields.Add(fieldMission);
                definedFields.Add(fieldFacility);
                definedFields.Add(fieldComponent);

                connManager.SetStandardFields(definedFields);


                //o createLogMessage()
                System.Collections.Generic.List<Field> logDefinedFields = new System.Collections.Generic.List<Field> { };

                Log.Info("No need to add MESSAGE-TYPE and MESSAGE-SUBTYPE for a log, we do that for you!");
                Log.Info("Adding general log fields");

                StringField fieldSubclass = new StringField("SUBCLASS", "TAC");
                StringField fieldOccurType = new StringField("OCCURRENCE-TYPE", "NORM");

                logDefinedFields.Add(fieldSubclass);
                logDefinedFields.Add(fieldOccurType);

                Message msg = connManager.CreateLogMessage("GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.LOG", logDefinedFields);

                msg.AddField(new I16Field("SEVERITY", 1));
                msg.AddField(new StringField("MSG-TEXT", "Self-published log message"));


                //o Publish valid log message
                Log.Info("Attempting to manually publish a valid log message...");
                connManager.Publish(msg);

                Log.Info("Demonstrating the fast log functionality...");
                Log.Info("First we set the defaults for all log messages called with 'publishLog'");
                connManager.SetLoggingDefaults("GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.LOG", logDefinedFields);
                I16Field severityField = new I16Field("SEVERITY", 1);
                Log.Info("...Then all we have to do is call 'publishLog' with a log string and a severity");
                connManager.PublishLog("Self-published log message", severityField);
                connManager.PublishLog("Another log, with one function call!", severityField);


                //o Attempt to publish invalid log message
                msg.AddField(new StringField("SEVERITY", "SEVERITY Field in a LOG should be an I16"));

                try
                {
                    Log.Info("Attempting to manually publish an invalid log message...");
                    connManager.Publish(msg);
                    Log.Info("Ignored improper severity and published anyway");
                }
                catch (GMSEC_Exception e)
                {
                    Log.Info("Invalid log message was NOT published; reason:\n"
                        + e.ToString());
                }


                //o Create Product message
                Log.Info("Creating PROD message...");
                Message prodMsg = new Message("GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.PROD.MIST-DEMO.1.AUTO.DM", Message.MessageKind.PUBLISH);
                
                Log.Info("Setting fields related to PROD message header...");
                connManager.AddStandardFields(prodMsg);
                prodMsg.AddField(new StringField("MESSAGE-TYPE", "MSG"));
                prodMsg.AddField(new StringField("MESSAGE-SUBTYPE", "PROD"));
                prodMsg.AddField(new F32Field("CONTENT-VERSION", (float)2013.0));
                prodMsg.AddField(new I16Field("RESPONSE-STATUS", 6));
                prodMsg.AddField(new StringField("PROD-TYPE", "AUTO"));
                prodMsg.AddField(new StringField("PROD-SUBTYPE", "DM"));


                //o Publish valid PROD message
                Log.Info("Publishing PROD message...");
                connManager.Publish(prodMsg);


                //o Alter PROD message to make it invalid
                prodMsg.AddField(new F32Field("PROD-SUBTYPE", 45));


                //o Attempt to publish invalid PROD message
                try
                {
                    Log.Info("Attempting to publish invalid PROD message...");
                    connManager.Publish(prodMsg);
                    Log.Warning("Ignored improper PROD-SUBTYPE and published anyway");
                }
                catch (GMSEC_Exception e)
                {
                    Log.Info("Invalid PROD message was NOT published; reason:\n"
                        + e.ToString());
                }
                
                Log.Info("But wait, publishing a product is even easier!");

                ProductFileMessage productFileMessage = new ProductFileMessage(
                    "GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.PROD.MIST-DEMO.1.AUTO.DM", 
                    ResponseStatus.FINAL_MESSAGE, "AUTO", "DM", GMSEC_ISD.GMSEC_ISD_CURRENT);

                ProductFile myFile = new ProductFile("My Product", "My Product Description", "Version",
                    "TXT", "A path to somewhere");

                productFileMessage.AddProductFile(myFile);
                connManager.AddStandardFields(productFileMessage);

                try
                {
                    Log.Info("Attempting to publish PROD message with ProductFileMessage");
                    connManager.Publish(productFileMessage);
                }
                catch (GMSEC_Exception e)
                {
                    Log.Info("PROD message was NOT published; reason:\n"
                        + e.ToString());
                }
                
                Log.Info("Mnemonic Messages are also easier!");

                MnemonicSample ms1 = new MnemonicSample("Timestamp 1", new I32Field("Timestamp 1", 15));
                ms1.SetEUValue(new F32Field("My EU", (float)15.0));
                ms1.SetFlags(1);
                ms1.SetLimit(MnemonicSample.LimitFlag.RED_HIGH);
                //implicitly set limit enable/disable with setting of limit
                ms1.SetQuality(true);
                ms1.SetStalenessStatus(false);
                ms1.SetTextValue("15");

                MnemonicSample ms2 = new MnemonicSample("Timestamp 2", new I32Field("Timestamp 2", 16));
                MnemonicSample ms3 = new MnemonicSample("Timestamp 3", new I32Field("Timestamp 3", 17));
                MnemonicSample ms4 = new MnemonicSample("Timestamp 4", new I32Field("Timestamp 4", 18));

                System.Collections.Generic.List<MnemonicSample> mnemonic_samples = new System.Collections.Generic.List<MnemonicSample>
                { ms1, ms2, ms3, ms4 };

                Mnemonic m1 = new Mnemonic("MyMnemonic", mnemonic_samples);
                I16Field statusField = new I16Field("My status", 5);
                m1.SetStatus(statusField);
                m1.SetUnits("MW");

                MnemonicMessage mMessage = new MnemonicMessage(
                    "GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.MVAL.EXAMPLE-PROG", 
                    GMSEC_ISD.GMSEC_ISD_CURRENT);
                mMessage.AddMnemonic(m1);
                connManager.AddStandardFields(mMessage);

                if (GMSEC_ISD.GMSEC_ISD_CURRENT == GMSEC_ISD.GMSEC_ISD_2014_00)
                {
                    //MSG-ID was required in GMSEC messages before the 2016 specification.
                    mMessage.AddField(new StringField("MSG-ID", "MYMSG"));
                }
                
                Log.Info("MnemonicMessage formats my GMSEC data automatically!");
                Log.Info(mMessage.ToXML());
                connManager.Publish(mMessage);
                
                Log.Info("Device Messages have their own subclass, as well");
                
                Device device1 = new Device("Device 1", Device.DeviceStatus.RED);
                I32Field paramVal = new I32Field("Param1Val", 79);
                DeviceParam param = new DeviceParam("DEV Param 1", "Param 1 Timestamp", paramVal);
                device1.AddParam(param);

                Device device2 = new Device("Device 2", Device.DeviceStatus.NOMINAL_GREEN);
                I32Field param1Val = new I32Field("Param1Val", 79);
                DeviceParam param1 = new DeviceParam("DEV Param 1", "Param 1 Timestamp", param1Val);
                StringField param2Val = new StringField("Param2Val", "79");
                DeviceParam param2 = new DeviceParam("DEV Param 2", "Param 2 Timestamp", param2Val);
                device2.AddParam(param1);
                device2.AddParam(param2);

                DeviceMessage deviceMessage = new DeviceMessage(
                    "GMSEC.GMSEC-MISSION.NOT-A-SPACECRAFT.MSG.C2CX.EXAMPLE-PROG.DEV.EXAMPLE-PROG-2", 
                    GMSEC_ISD.GMSEC_ISD_CURRENT);
                deviceMessage.AddDevice(device1);
                deviceMessage.AddDevice(device2);
                
                Log.Info("DeviceMessage formats my GMSEC data automatically!");
                Log.Info(deviceMessage.ToXML());

                connManager.AddStandardFields(deviceMessage);
                connManager.Publish(deviceMessage);
                
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
            new gmmist_validation().Run(args);
        }
    }
}
