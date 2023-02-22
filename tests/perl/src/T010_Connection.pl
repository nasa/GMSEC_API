#!/usr/bin/env perl

use strict;
use Test;
use libgmsec_perl;


sub Test_Connection
{
	my ($test) = @_;

	$test->setDescription("Connection");

	test_create($test);
	test_getAPIVersion($test);
	test_connect($test);
	test_disconnect($test);
	test_getLibraryName($test);
	test_getLibraryVersion($test);
	test_getMessageFactory($test);
	test_registerEventCallback($test);
	test_subscribe($test);
	test_unsubscribe($test);
	test_publish($test);                    # also calls on subscribe() and receive()
	test_publish_with_mwconfig($test);
	test_request($test);                    # also calls on reply()
	test_reply($test);
	test_dispatch($test);
	test_receive($test);
	test_excludeSubject($test);             # also tests removeExcludedSubject()
	test_removeExcludedSubject($test);
	test_getName_setName($test);
	test_getID($test);
	test_getMWInfo($test);
	test_getConnectionEndpoint($test);
	test_getPublishQueueMessageCount($test);
}


sub test_create
{
	libgmsec_perl::logInfo("Test create");

	my ($test) = @_;

	# Nominal test
	eval
	{
		my $conn = libgmsec_perl::Connection::create( $test->getConfig() );
		$test->check("Okay", 1);
		libgmsec_perl::Connection::destroy($conn);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->require($error->what(), 0);
	}

	# Off-Nominal test
	eval
	{
		# missing mw-id
		my $conn = libgmsec_perl::Connection::create( libgmsec_perl::Config->new() );
		$test->check("Exception expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "mw-id in Config is not specified") != -1);
	}

	eval
	{
		# bogus middleware
		my $conn = libgmsec_perl::Connection::create( libgmsec_perl::Config->new("mw-id=bogus", $libgmsec_perl::DataType_KEY_VALUE_DATA) );
		$test->check("Exception expected", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Unable to load") != -1);
	}
}


sub test_getAPIVersion
{
	libgmsec_perl::logInfo("Test getAPIVersion()");

	my ($test) = @_;

	my $version = "GMSEC API v" . $libgmsec_perl::GMSEC_VERSION_NUMBER;
	$test->check("Expected to get API version info", index(libgmsec_perl::Connection::getAPIVersion(), $version) != -1);
}


sub test_connect
{
	libgmsec_perl::logInfo("Test connect()");

	my ($test) = @_;

	my $conn;

	# Nominal test
	eval
	{
		$conn = libgmsec_perl::Connection::create( $test->getConfig() );
		$conn->connect();
		$test->check("Okay", 1);
		$conn->disconnect();
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->require($error->what(), 0);
	}
	if (defined $conn)
	{
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}

	# Off-nominal test
	eval
	{
		my $config = libgmsec_perl::Config->new( "mw-id=bolt server=gs580s-gc764mw1:1234", $libgmsec_perl::DataType_KEY_VALUE_DATA );
		$conn = libgmsec_perl::Connection::create($config);
		$conn->connect();
		$test->check("Expected error to indicate unable to connect", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;

		my $result = ($error->getErrorClass() == $libgmsec_perl::MIDDLEWARE_ERROR)  &&
		             ($error->getErrorCode()  == $libgmsec_perl::CUSTOM_ERROR_CODE) &&
		             ($error->getCustomCode() == 7);

		$test->check($error->what(), $result);
	}
	if (defined $conn)
	{
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}
}


sub test_disconnect
{
	libgmsec_perl::logInfo("Test disconnect()");

	my ($test) = @_;

	my $conn;

	eval
	{
		$conn = libgmsec_perl::Connection::create( $test->getConfig() );
		$conn->connect();
		$conn->disconnect();
		$test->check("Okay", 1);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), 0);
	}
	if (defined $conn)
	{
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}
}


sub test_getLibraryName
{
	libgmsec_perl::logInfo("Test getLibraryName()");

	my ($test) = @_;

	my $conn;

	eval
	{
		$conn = libgmsec_perl::Connection::create( $test->getConfig() );
		$test->check("Unexpected library name", index($conn->getLibraryName(), $test->getConfig()->getValue("mw-id", "unknown")) != -1);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), 0);
	}
	if (defined $conn)
	{
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}
}


sub test_getLibraryVersion
{
	libgmsec_perl::logInfo("Test getLibraryVersion()");

	my ($test) = @_;

	my $conn;

	eval
	{
		$conn = libgmsec_perl::Connection::create( $test->getConfig() );
		$test->check("Expected to get a library version", defined $conn->getLibraryVersion());
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), 0);
	}
	if (defined $conn)
	{
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}
}


sub test_getMessageFactory
{
	libgmsec_perl::logInfo("Test getMessageFactory()");

	my ($test) = @_;

	my $conn;

	eval
	{
		$conn = libgmsec_perl::Connection::create( $test->getConfig() );
		$test->check("Expected to get a MessageFactory for the CURRENT specification", $libgmsec_perl::GMSEC_MSG_SPEC_CURRENT == $conn->getMessageFactory()->getSpecification()->getVersion());
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), 0);
	}
	if (defined $conn)
	{
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}
}


sub test_registerEventCallback
{
	libgmsec_perl::logInfo("Test registerEventCallback()");

	my ($test) = @_;

	my $mw = $test->getConfig()->getValue("mw-id", "unknown");

	if ($mw eq "unknown" or $mw eq "mb" or $mw eq "generic_jms" or $mw eq "zeromq413" or $mw eq "zeromq432")
    {
		# Test is being skipped because of lame middleware wrapper
		# which does not support event notification upon connect
		# and/or disconnect to/from the GMSEC Bus.
		return;
    }

	my $mec = T010_EventCallback->new();

	my $conn;

	# Nominal case
	eval
	{
		$conn = libgmsec_perl::Connection::create( $test->getConfig() );
		$conn->registerEventCallback( $libgmsec_perl::Connection::Event_ALL_EVENTS, $mec );

		$conn->connect();
		$test->check("Expected EventCallback to be called", $mec->wasCalled());
		$test->check("Expected to be connected", $libgmsec_perl::Connection::Event_CONNECTION_SUCCESSFUL_EVENT == $mec->getEvent());

		$mec.reset();

		$conn->disconnect();
		$test->check("Expected EventCallback to be called", $mec->wasCalled());
		$test->check("Expected to be disconnected", $libgmsec_perl::Connection::Event_CONNECTION_BROKEN_EVENT == $mec->getEvent());
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), 0);
	}
	if (defined $conn)
	{
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}

	# Off-nominal case
	eval
	{
		$conn = libgmsec_perl::Connection::create( $test->getConfig() );
		$conn->registerEventCallback( $libgmsec_perl::Connection::Event_ALL_EVENTS, undef );
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "EventCallback cannot be NULL") != -1);
	}
	if (defined $conn)
	{
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}
}


sub test_subscribe
{
	libgmsec_perl::logInfo("Test subscribe()");

	my ($test)= @_;

	my $conn = undef;

	# Nominal test
	eval
	{
		$conn = libgmsec_perl::Connection::create( $test->getConfig() );
		$conn->connect();
		$conn->subscribe( $test->getSubject("*.BAR") );
		$conn->subscribe( $test->getSubject("FOO.BAZ") );

		my $msg1 = $conn->getMessageFactory()->createMessage("LOG");
		my $msg2 = $conn->getMessageFactory()->createMessage("LOG");
		my $msg3 = $conn->getMessageFactory()->createMessage("LOG");

		$msg1->setSubject( $test->getSubject("GIN.BAR") );
		$msg2->setSubject( $test->getSubject("FOO.BAZ") );
		$msg3->setSubject( $test->getSubject("FOO.GAZ") );

		$conn->publish($msg1);

		my $msg = $conn->receive(5000);
		$test->check("Was expecting to receive a message", defined $msg);
		$test->check("Unexpected message subject", $msg->getSubject() eq $test->getSubject("GIN.BAR"));
		libgmsec_perl::Message::destroy($msg);

		$conn->publish($msg2);

		$msg = $conn->receive(5000);
		$test->check("Was expecting to receive a message", defined $msg);
		$test->check("Unexpected message subject", $msg->getSubject() eq $test->getSubject("FOO.BAZ"));
		libgmsec_perl::Message::destroy($msg);

		$conn->publish($msg3);

		$msg = $conn->receive(5000);
		$test->check("Was not expecting to receive a message", ! defined $msg);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), 0);
	}
	if (defined $conn)
	{
		$conn->disconnect();
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}

	# Off-nominal tests
	eval
	{
		# duplicate subscription
		$conn = libgmsec_perl::Connection::create( $test->getConfig() );
		$conn->connect();
		$conn->subscribe("GMSEC.>");
		$conn->subscribe("GMSEC.>");
		$test->check("Was expecting an exception", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Duplicate subscription") != -1);
	}
	if (defined $conn)
	{
		$conn->disconnect();
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}

	eval
	{
		# subscribe() before connect()
		$conn = libgmsec_perl::Connection::create( $test->getConfig() );
		$conn->subscribe("GMSEC.>");
		$test->check("Was expecting an exception", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Connection has not been initialized") != -1);
	}
	if (defined $conn)
	{
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}

	eval
	{
		# null subscription subject
		$conn = libgmsec_perl::Connection::create( $test->getConfig() );
		$conn->connect();
		$conn->subscribe(undef);
		$test->check("Was expecting an exception", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Subject is NULL or is empty string") != -1);
	}
	if (defined $conn)
	{
		$conn->disconnect();
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}

	eval
	{
		# illegal subscription subject
		$conn = libgmsec_perl::Connection::create( $test->getConfig() );
		$conn->connect();
		$conn->subscribe("GMSEC.MY MISSION.*.*.>");
		$test->check("Was expecting an exception", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Subject is invalid") != -1);
	}
	if (defined $conn)
	{
		$conn->disconnect();
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}
}


sub test_unsubscribe
{
	libgmsec_perl::logInfo("Test unsubscribe()");

	my ($test) = @_;

	my $config = libgmsec_perl::Config->new( $test->getConfig() );  # make copy!
	$config->addValue("gmsec-msg-content-validate-recv", "1");

	my $conn = undef;

	# Nominal tests
	eval
	{
		$conn = libgmsec_perl::Connection::create($config);
		$conn->connect();
		my $info = $conn->subscribe( $test->getSubject("FOO.BAR") );
		$conn->unsubscribe($info);

		my $hb = $conn->getMessageFactory()->createMessage("HB");
		$hb->setSubject( $test->getSubject("FOO.BAR") );

		$conn->publish($hb);

		my $msg = $conn->receive(5000);

		$test->check("Did not expect to receive a message", ! defined $msg);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), 0);
	}
	if (defined $conn)
	{
		$conn->disconnect();
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}

	# Off-nominal tests
	eval
	{
		# Attempt to unsubscribe using NULL object
		$conn = libgmsec_perl::Connection::create($config);
		$conn->connect();
		my $info = undef;
		$conn->unsubscribe($info);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Cannot unsubscribe using NULL SubscriptionInfo object") != -1);
	}
	if (defined $conn)
	{
		$conn->disconnect();
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}

	my $conn2;

	eval
	{
		# Attempt to unsubscribe using wrong Connection object
		$conn  = libgmsec_perl::Connection::create($config);
		$conn2 = libgmsec_perl::Connection::create($config);

		$conn->connect();
		$conn2->connect();

		my $info1 = $conn->subscribe("GMSEC.FOO.BAR");

		$conn2->unsubscribe($info1);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "SubscriptionInfo object is not associated with this Connection") != -1);
	}
	if (defined $conn)
	{
		$conn->disconnect();
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}
	if (defined $conn2)
	{
		$conn2->disconnect();
		libgmsec_perl::Connection::destroy($conn2);
		$conn2 = undef;
	}
}


sub test_publish
{
	libgmsec_perl::logInfo("Test publish()");

	my ($test) = @_;

	# Nominal test
	my $conn;

	eval
	{
		$conn = libgmsec_perl::Connection::create($test->getConfig());

		$conn->connect();
		$conn->subscribe( $test->getSubject(">") );

		$test->setStandardFields($conn->getMessageFactory());

		my $msg = $conn->getMessageFactory()->createMessage("HB");
		$msg->setSubject( $test->getSubject("HB") );

		$conn->publish($msg);

		my $rcvd = $conn->receive(5000);

		if (defined $rcvd)
		{
			$test->check("Unexpected subject", $rcvd->getSubject() eq $test->getSubject("HB"));
			$test->check("Unexpected message kind", $rcvd->getKind() == $libgmsec_perl::Message::Kind_PUBLISH);

			libgmsec_perl::Message::destroy($rcvd);
		}
		else
		{
			$test->check("Failed to receive published message", 0);
		}
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), 0);
	}
	if (defined $conn)
	{
		$conn->disconnect();
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}

	# Off-nominal tests
	eval
	{
		# Message validation fails (missing standard fields)
		my $config = libgmsec_perl::Config->new( $test->getConfig() );  # make a copy!
		$config->addValue("gmsec-msg-content-validate", "true");

		$conn = libgmsec_perl::Connection::create($config);
		my $msg = $conn->getMessageFactory()->createMessage("HB");
		$conn->connect();
		$conn->publish($msg);
		$test->check("Was expecting an exception", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Message Validation Failed") != -1);
	}
	if (defined $conn)
	{
		$conn->disconnect();
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}

	eval
	{
		# Connection not initialized
		$conn = libgmsec_perl::Connection::create($test->getConfig());
		my $msg = $conn->getMessageFactory()->createMessage("HB");
		$conn->publish($msg);
		$test->check("Was expecting an exception", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Connection has not been initialized") != -1);
	}
	if (defined $conn)
	{
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}

	eval
	{
		# Non-PUBLISH kind message
		$conn = libgmsec_perl::Connection::create($test->getConfig());
		my $msg = $conn->getMessageFactory()->createMessage("REQ.DIR");
		$conn->connect();
		$conn->publish($msg);
		$test->check("Was expecting an exception", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Cannot publish message with non-PUBLISH message kind") != -1);
	}
	if (defined $conn)
	{
		$conn->disconnect();
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}
}


sub test_publish_with_mwconfig
{
	libgmsec_perl::logInfo("Test publish() with mwConfig");

	my ($test) = @_;

	my $conn;

	eval
	{
		my $config  = $test->getConfig();
		my $subject = $test->getSubject("MW-CONFIG");
        my $mwConfig = libgmsec_perl::Config->new();

		$conn = libgmsec_perl::Connection::create( $config );

		$test->setStandardFields( $conn->getMessageFactory() );

		my $msg = $conn->getMessageFactory()->createMessage("HB");
		$msg->setSubject( $subject );

        $conn->connect();

        $conn->subscribe( $subject );

        $conn->publish( $msg, $mwConfig );

        my $rcv = $conn->receive( 5000 );

        if (defined $rcv)
        {
            $test->check("Eeceived a message", 1);
        }
        else
        {
            $test->check("Failed to receive a message", 0);
        }

        $conn->disconnect();
    };
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), 0);
	}
	if (defined $conn)
	{
		$conn->disconnect();
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}
}


sub test_request
{
	libgmsec_perl::logInfo("Test synchronous request()");

	my ($test) = @_;

	my $conn;

	# Nominal case
	eval
	{
		my $config = $test->getConfig();

		my $responseSubject = $test->getSubject("RESP.DIR");
		my $requestSubject  = $test->getSubject("REQ.DIR");

		# Setup crap to launch sub-process that will handle issuing response messages
		my $responder = $test->getDataFile("T010_Responder.pl");

		my @cmd_args = ();

		my $perl_cmd = $ENV{'PERL5_HOME'};
		my $os = $^O;

		if (defined $perl_cmd)
		{
			if ($os eq "linux" or $os eq "darwin")
			{
				$perl_cmd .= "/bin/perl";
			}
			else
			{
				$perl_cmd .= "/perl/bin/perl";
			}
		}
		else
		{
			$perl_cmd = "perl";
		}

		push @cmd_args, $perl_cmd;
		push @cmd_args, $responder;

		my $cp = libgmsec_perl::ConfigPair->new();
		my $hasPair = $config->getFirst($cp);

		while ($hasPair)
		{
			my $name  = $cp->getName();
			my $value = $cp->getValue();

			my @args = ( "$name=$value" );

			push @cmd_args, @args;

			$hasPair = $config->getNext($cp);
		}

		my @other_args = ("req-subject=$requestSubject", "rep-subject=$responseSubject", "loglevel=none");

		push @cmd_args, @other_args;

		# Launch responder sub-process
		unless (fork) {
			libgmsec_perl::logInfo("Forking... @cmd_args");
			exec(@cmd_args);
			exit 0;
		}

		# Delay to allow sub-process to fully start
		libgmsec_perl::TimeUtil::millisleep(5000);

		$conn = libgmsec_perl::Connection::create( $config );

		$test->setStandardFields( $conn->getMessageFactory() );

		my $reqMsg = $conn->getMessageFactory()->createMessage("REQ.DIR");
		$reqMsg->setSubject( $requestSubject );

		$reqMsg->addField( libgmsec_perl::StringField->new("DIRECTIVE-STRING", "Do something!") );
		$reqMsg->addField( libgmsec_perl::StringField->new("DESTINATION-COMPONENT", "RESPONDER") );

		# Setup connection to the GMSEC Bus
		$conn->connect();

		# Subscribe to receive the response message
		$conn->subscribe( $responseSubject );

		# Verify response is returned
		$reqMsg->addField( libgmsec_perl::U16Field->new("REQUEST-ID", 0) );
		my $repMsg = $conn->request($reqMsg, 5000, $libgmsec_perl::GMSEC_REQUEST_REPUBLISH_NEVER);

		if (! defined $repMsg)
		{
			$test->check("Expected to receive response message", 0);
		}
		else
		{
			$test->check("Unexpected response message kind", $repMsg->getKind() == $libgmsec_perl::Message::Kind_REPLY);
			$test->check("Unexpected response message subject", index($repMsg->getSubject(), $test->getSubject("RESP.DIR")) != -1);
			libgmsec_perl::Message::destroy($repMsg);
		}

		# Verify no response is returned
		$reqMsg->addField( libgmsec_perl::U16Field->new("REQUEST-ID", 1) );
		$repMsg = $conn->request($reqMsg, 5000, $libgmsec_perl::GMSEC_REQUEST_REPUBLISH_NEVER);

		if (defined $repMsg)
		{
			$test->check("Was not expecting to receive response message", 0);
			libgmsec_perl::Message::destroy($repMsg);
		}

		# TODO
		# If the spawned task is not running, then this test has the potential
		# to 'fail' in the sense that it will never receive a response message.
		# The API will continue reissuing the request message indefinitely, and
		# this will give the appearance that the unit test is hung.
		# This is NOT good. For now, the test is commented out.
		#
		# Verify response is returned
		#$repMsg = $conn->request($reqMsg, 5000, 5000);

		#if (! defined $repMsg)
		#{
		#	$test->check("Expected to receive response message", 0);
		#}
		#else
		#{
		#	libgmsec_perl::Message::destroy($repMsg);
		#}
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), 0);
	}
	if (defined $conn)
	{
		$conn->disconnect();
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}

	# Off-nominal tests
	eval
	{
		# Connection not initialized
		$conn = libgmsec_perl::Connection::create( $test->getConfig() );
		my $reqMsg = $conn->getMessageFactory()->createMessage("REQ.DIR");
		$conn->request($reqMsg, 5000, $libgmsec_perl::GMSEC_REQUEST_REPUBLISH_NEVER);
		$test->check("Connection not initialized", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Connection has not been initialized") != -1);
	}
	if (defined $conn)
	{
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}

	eval
	{
		# Request using non-REQUEST kind message
		$conn = libgmsec_perl::Connection::create( $test->getConfig() );
		$conn->connect();
		my $badMsg = $conn->getMessageFactory()->createMessage("RESP.DIR");
		$conn->request($badMsg, 5000, $libgmsec_perl::GMSEC_REQUEST_REPUBLISH_NEVER);
		$test->check("Was expecting bad message error", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Cannot issue request with non-REQUEST kind message") != -1);
	}
	if (defined $conn)
	{
		$conn->disconnect();
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}
}


sub test_reply
{
	libgmsec_perl::logInfo("Test reply()");

	my ($test) = @_;

	# Nominal tests for Connection.reply() tested earlier

	my $conn;

	# Off-nominal tests
	eval
	{
		# Connection not initialized
		$conn = libgmsec_perl::Connection::create( $test->getConfig() );
		my $reqMsg = $conn->getMessageFactory()->createMessage("REQ.DIR");
		my $repMsg = $conn->getMessageFactory()->createMessage("RESP.DIR");
		$conn->reply($reqMsg, $repMsg);
		$test->check("Was expecting an exception since we ain't initialized", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Connection has not been initialized") != -1);
	}
	if (defined $conn)
	{
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}

	eval
	{
		# Bad reply message
		$conn = libgmsec_perl::Connection::create( $test->getConfig() );
		$conn->connect();
		my $reqMsg = $conn->getMessageFactory()->createMessage("REQ.DIR");
		my $badMsg = $conn->getMessageFactory()->createMessage("REQ.DIR");
		$conn->reply($reqMsg, $badMsg);
		$test->check("Was expecting an exception since we have a bad reply message", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Cannot issue reply with non-REPLY kind message") != -1);
	}
	if (defined $conn)
	{
		$conn->disconnect();
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}
}


sub test_dispatch
{
	libgmsec_perl::logInfo("Test dispatch()");

	my ($test) = @_;

	my $conn;

	eval
	{
		my $mcb1 = T010_Callback->new();
		my $mcb2 = T010_Callback->new();
		my $mcb3 = T010_Callback->new();

		$conn = libgmsec_perl::Connection::create( $test->getConfig() );
		$conn->connect();

		$test->setStandardFields($conn->getMessageFactory());

		my $msg = $conn->getMessageFactory()->createMessage("HB");

		$msg->setSubject( $test->getSubject("HB") );

		my $info1 = $conn->subscribe( $msg->getSubject(), $mcb1 );
		my $info2 = $conn->subscribe( $msg->getSubject(), $mcb2 );
		my $info3 = $conn->subscribe( $test->getSubject("FOO.BAR"),  $mcb3 );

		$conn->publish($msg);

		my $rcvd = $conn->receive(5000);

		if (! defined $rcvd)
		{
			$test->check("Failed to receive message", 0);
		}
		else
		{
			$conn->dispatch($rcvd);

			libgmsec_perl::TimeUtil::millisleep(1000);

			$test->check("Expected Callback 1 to receive a message", $mcb1->wasCalled());
			$test->check("Expected Callback 2 to receive a message", $mcb2->wasCalled());
			$test->check("Expected Callback 3 to not receive a message", $mcb3->wasCalled() == 0);

			libgmsec_perl::Message::destroy($rcvd);
		}

		$conn->unsubscribe($info1);
		$conn->unsubscribe($info2);
		$conn->unsubscribe($info3);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), 0);
	}
	if (defined $conn)
	{
		$conn->disconnect();
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}
}


sub test_receive
{
	libgmsec_perl::logInfo("Test receive()");

	my ($test) = @_;

	my $conn = undef;

	# Nominal tests
	eval
	{
		$conn = libgmsec_perl::Connection::create( $test->getConfig() );
		$conn->connect();

		# Timeout occurs (no subscription)
		my $rcvd = $conn->receive(10);
		$test->check("Received unexpected message", ! defined $rcvd);

		$test->setStandardFields($conn->getMessageFactory());

		# Receive message
		my $msg = $conn->getMessageFactory()->createMessage("HB");
		$msg->setSubject( $test->getSubject("HB") );

		$conn->subscribe($msg->getSubject());
		$conn->publish($msg);

		$rcvd = $conn->receive(5000);
		$test->check("Did not receive expected message", defined $rcvd);
		libgmsec_perl::Message::destroy($rcvd);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), 0);
	}
	if (defined $conn)
	{
		$conn->disconnect();
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}

	# Off-nominal tests
	eval
	{
		# Connection not initialized
		$conn = libgmsec_perl::Connection::create( $test->getConfig() );
		$conn->receive(10);
		$test->check("Expected exception since we're not initialized", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Connection has not been initialized") != -1);
	}
	if (defined $conn)
	{
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}
}


sub test_excludeSubject
{
	libgmsec_perl::logInfo("Test excludeSubject()");

	my ($test) = @_;

	my $conn;

	eval
	{
		$conn = libgmsec_perl::Connection::create( $test->getConfig() );
		$conn->connect();

		$test->setStandardFields( $conn->getMessageFactory() );

		my $msg1 = $conn->getMessageFactory()->createMessage("HB");
		my $msg2 = $conn->getMessageFactory()->createMessage("LOG");

		$msg1->setSubject( $test->getSubject("HB") );
		$msg2->setSubject( $test->getSubject("LOG") );

		$msg2->addField( libgmsec_perl::StringField->new("SUBCLASS", "NO-CLASS") );
		$msg2->addField( libgmsec_perl::StringField->new("OCCURRENCE-TYPE", "FREQUENTLY") );
		$msg2->addField( libgmsec_perl::I16Field->new("SEVERITY", 1) );

		$conn->subscribe( $test->getSubject(">") );
		$conn->excludeSubject( $test->getSubject("LOG.+") );

		$conn->publish($msg1);

		my $rcvd = $conn->receive(5000);
		$test->check("Was expecting to receive an HB message", defined $rcvd);
		libgmsec_perl::Message::destroy($rcvd);

		$conn->publish($msg2);
		$rcvd = $conn->receive(5000);
		$test->check("Was not expecting to receive a LOG message", ! defined $rcvd);

		$conn->removeExcludedSubject( $test->getSubject("LOG.+") );

		$conn->publish($msg2);
		$rcvd = $conn->receive(5000);
		$test->check("Was expecting to receive a LOG message", defined $rcvd);
		libgmsec_perl::Message::destroy($rcvd);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), 0);
	}
	if (defined $conn)
	{
		$conn->disconnect();
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}

	# Off-nominal tests
	eval
	{
		# Connection not initialized
		$conn = libgmsec_perl::Connection::create( $test->getConfig() );
		$conn->excludeSubject("GMSEC.FOO.BAR");
		$test->check("Expected exception since we're not initialized", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Connection has not been initialized") != -1);
	}
	if (defined $conn)
	{
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}

	eval
	{
		# Invalid subject
		$conn = libgmsec_perl::Connection::create( $test->getConfig() );
		$conn->connect();
		$conn->excludeSubject("GMSEC FOO BAR");
		$test->check("Expected exception for invalid subject", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Subject is invalid") != -1);
	}
	if (defined $conn)
	{
		$conn->disconnect();
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}
}


sub test_removeExcludedSubject
{
	libgmsec_perl::logInfo("Test removeExcludedSubject()");

	my ($test) = @_;

	# Nominal test for removeExcludedSubject() are conducted witin test_excludeSubject()

	my $conn;

	# Off-nominal tests
	eval
	{
		# Connection not initialized
		$conn = libgmsec_perl::Connection::create( $test->getConfig() );
		$conn->removeExcludedSubject("GMSEC.FOO.BAR");
		$test->check("Expected exception since we're not initialized", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Connection has not been initialized") != -1);
	}
	if (defined $conn)
	{
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}

	eval
	{
		# Invalid subject
		$conn = libgmsec_perl::Connection::create( $test->getConfig() );
		$conn->connect();
		$conn->removeExcludedSubject("GMSEC FOO BAR");
		$test->check("Expected exception for invalid subject", 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), index($error->what(), "Subject is invalid") != -1);
	}
	if (defined $conn)
	{
		$conn->disconnect();
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}
}


sub test_getName_setName
{
	libgmsec_perl::logInfo("Test getName() and setName()");

	my ($test) = @_;

	my $conn;

	eval
	{
		$conn = libgmsec_perl::Connection::create( $test->getConfig() );
		$conn->connect();

		$test->check("Expected a connection name", defined $conn->getName());

		$conn->setName("FOOBAR");

		$test->check("Expected a connection name of FOOBAR", $conn->getName() eq "FOOBAR");
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), 0);
	}
	if (defined $conn)
	{
		$conn->disconnect();
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}
}


sub test_getID
{
	libgmsec_perl::logInfo("Test getID()");

	my ($test) = @_;

	my $conn;

	eval
	{
		$conn = libgmsec_perl::Connection::create( $test->getConfig() );
		$conn->connect();
		$test->check("Expected a connection ID", defined $conn->getID());
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), 0);
	}
	if (defined $conn)
	{
		$conn->disconnect();
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}
}


sub test_getMWInfo
{
	libgmsec_perl::logInfo("Test getMWInfo()");

	my ($test) = @_;

	my $conn;

	eval
	{
		$conn = libgmsec_perl::Connection::create( $test->getConfig() );
		$test->check("Expected mwInfo to contain mw-id", index($conn->getMWInfo(), $test->getConfig()->getValue("mw-id")) != -1);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), 0);
	}
	if (defined $conn)
	{
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}
}


sub test_getConnectionEndpoint
{
	libgmsec_perl::logInfo("Test getConnectionEndpoint()");

	my ($test) = @_;

	my $conn;

	eval
	{
		$conn = libgmsec_perl::Connection::create( $test->getConfig() );
		$conn->connect();
		$test->check("Unexpected connection endpoint", defined $conn->getConnectionEndpoint());
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), 0);
	}
	if (defined $conn)
	{
		$conn->disconnect();
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}
}


sub test_getPublishQueueMessageCount
{
	libgmsec_perl::logInfo("Test getPublishQueueMessageCount()");

	my ($test) = @_;

	my $config = libgmsec_perl::Config->new( $test->getConfig() );  # make a copy!
	$config->addValue("gmsec-async-publish", "1");
	$config->addValue("gmsec-async-publish-queue-depth", "20");
	$config->addValue("gmsec-async-publish-teardown-wait", "2000");

	my $conn;

	eval
	{
		$conn = libgmsec_perl::Connection::create( $config );
		$conn->connect();

		$test->setStandardFields( $conn->getMessageFactory() );

		my $msg = $conn->getMessageFactory()->createMessage("LOG");

		$msg->addField( libgmsec_perl::StringField->new("SUBCLASS", "TEST") );
		$msg->addField( libgmsec_perl::StringField->new("OCCURRENCE-TYPE", "FREQUENT") );
		$msg->addField( libgmsec_perl::I16Field->new("SEVERITY", 1) );

		for (my $i = 0; $i < 10; $i++)
		{
			$conn->publish($msg);
			$test->check("Unexpected message count < 0", $conn->getPublishQueueMessageCount() >= 0);
		}

		libgmsec_perl::TimeUtil::millisleep(2000);

		$test->check("Unexpected message count != 0", $conn->getPublishQueueMessageCount() == 0);
	};
	if (isa($@, 'libgmsec_perl::GmsecException'))
	{
		my $error = $@;
		$test->check($error->what(), 0);
	}
	if (defined $conn)
	{
		$conn->disconnect();
		libgmsec_perl::Connection::destroy($conn);
		$conn = undef;
	}
}


{
	package T010_EventCallback;
	use base 'libgmsec_perl::EventCallback';

	sub new
	{
		# Initialize the base package (libgmsec_perl::EventCallback)
		my $class = shift;
		my $self = $class->SUPER::new();

        $self->{CALLED} = 0;
        $self->{EVENT}  = $libgmsec_perl::Connection::Event_ALL_EVENTS;
	}

    sub onEvent
	{
		my ($self, $conn, $status, $event) = @_;

        $self->{CALLED} = 1;
        $self->{EVENT}  = $event;
	}

    sub wasCalled
	{
		my ($self) = @_;

        return $self->{CALLED};
	}

    sub getEvent
	{
		my ($self) = @_;

        return $self->{EVENT};
	}

    sub reset
	{
		my ($self) = @_;

    	$self->{CALLED} = 0;
        $self->{EVENT}  = $libgmsec_perl::Connection::Event_ALL_EVENTS;
	}
}


{
	package T010_Callback;
	use base 'libgmsec_perl::Callback';

    sub new
	{
		# Initialize the base package (libgmsec_perl::Callback)
		my $class = shift;
		my $self  = $class->SUPER::new();

        $self->{CALLED} = 0;

		return $self;
	}

    sub onMessage
	{
		my ($self, $conn, $msg) = @_;

        $self->{CALLED} = 1;
	}

    sub wasCalled
	{
		my ($self) = @_;

        return $self->{CALLED};
	}

    sub reset
	{
		my ($self) = @_;

        $self->{CALLED} = 0;
	}
}


=begin USELESS
class T010_RequestCallback(libgmsec_perl::Callback):
    def __init__(self):
        super().__init__()
        m_provideResponse = 1

    def on_message(self, conn, msg):
        try:
            if m_provideResponse:
                repMsg = conn.getMessageFactory().createMessage("RESP.DIR")

                repMsg.setFieldValue("RESPONSE-STATUS", libgmsec_perl::Message.ResponseStatus_SUCCESSFUL_COMPLETION)

                if msg.hasField("COMPONENT"):
                    repMsg.addField( libgmsec_perl::StringField("DESTINATION-COMPONENT", msg.getStringValue("COMPONENT")) )

                if msg.hasField("DESTINATION-COMPONENT"):
                    repMsg.addField( libgmsec_perl::StringField("COMPONENT", msg.getStringValue("DESTINATION-COMPONENT")) )
                else:
                    repMsg.addField( libgmsec_perl::StringField("COMPONENT", "RESPONDER") )

                if msg.hasField("REQUEST-ID"):
                    repMsg.addField( msg.getField("REQUEST-ID") )

                conn.reply(msg, repMsg)
            else:
                m_provideResponse = 1
        except Exception as e:
            libgmsec_perl::logError(str(e), 0)

    def provide_response(self, flag):
        m_provideResponse = flag


class T010_ReplyCallback(libgmsec_perl::ReplyCallback):
    def __init__(self):
        super().__init__()
        m_gotReply = 0
        m_gotEvent = 0

    def on_reply(self, conn, request, reply):
        m_gotReply = 1

    def on_event(self, conn, status, event):
        m_gotEvent = (event == libgmsec_perl::Connection.REQUEST_TIMEOUT_EVENT)

    def got_reply(self):
        return m_gotReply

    def got_event(self):
        return m_gotEvent

    def reset(self):
        m_gotReply = 0
        m_gotEvent = 0
=end USELESS
=cut


Test::driver(\&Test_Connection);
