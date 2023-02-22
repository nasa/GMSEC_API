#!/usr/bin/env perl

use strict;
use libgmsec_perl;


{
	package T010_Responder;

	*isa = \&UNIVERSAL::isa;

	sub new
	{
		my $class = shift;
		my $self = { CONFIG => shift };
		bless $self, $class;
		return $self;
	}


	sub getConfig
	{
		my ($self) = @_;

		return $self->{CONFIG};
	}


	sub getStandardFields
	{
		my ($self) = @_;

		$self->{FIELDS} = libgmsec_perl::FieldArray->new();

		my $field = libgmsec_perl::StringField->new("MISSION-ID", "MY-MISSION", 1);
		$self->{FIELDS}->push($field);
		$field->DISOWN();

		$field = libgmsec_perl::StringField->new("CONSTELLATION-ID", "MY-CONSTELLATION-ID", 1);
		$self->{FIELDS}->push($field);
		$field->DISOWN();

		$field = libgmsec_perl::StringField->new("SAT-ID-PHYSICAL", "MY-SAT-ID", 1);
		$self->{FIELDS}->push($field);
		$field->DISOWN();

		$field = libgmsec_perl::StringField->new("SAT-ID-LOGICAL", "MY-SAT-ID", 1);
		$self->{FIELDS}->push($field);
		$field->DISOWN();

		$field = libgmsec_perl::StringField->new("COMPONENT", "MY-COMPONENT", 1);
		$self->{FIELDS}->push($field);
		$field->DISOWN();

		$field = libgmsec_perl::StringField->new("FACILITY", "MY-FACILITY", 1);
		$self->{FIELDS}->push($field);
		$field->DISOWN();

		$field = libgmsec_perl::StringField->new("DOMAIN1", "MY-DOMAIN-1", 1);
		$self->{FIELDS}->push($field);
		$field->DISOWN();

		$field = libgmsec_perl::StringField->new("DOMAIN2", "MY-DOMAIN-2", 1);
		$self->{FIELDS}->push($field);
		$field->DISOWN();

		return $self->{FIELDS};
	}


	sub respondToRequest
	{
		my ($self) = @_;

		libgmsec_perl::logInfo("Responder is seeking config options...");
		my $request_subject = $self->getConfig()->getValue("req-subject", "GMSEC.UNKNOWN");
		my $reply_subject   = $self->getConfig()->getValue("rep-subject", "GMSEC.UNKNOWN");

		my $conn;

		eval
		{
			libgmsec_perl::logInfo("Responder is creating Connection...");
			$conn = libgmsec_perl::Connection::create( $self->getConfig() );

			libgmsec_perl::logInfo("Responder is connecting...");
			$conn->connect();

			$conn->getMessageFactory()->setStandardFields( $self->getStandardFields() );

			libgmsec_perl::logInfo("Responder is subscribing to: $request_subject");
			$conn->subscribe( $request_subject );

			my $iter = 0;

			while ($iter < 3)
			{
				libgmsec_perl::logInfo("Responder is waiting to receive message...");
				my $reqMsg = $conn->receive(10000);

				if ($reqMsg == undef)
				{
					libgmsec_perl::logWarning("Responder failed to receive message");
				}
				else
				{
					libgmsec_perl::logInfo("Responder got request message");

					if ($iter == 0 || $iter == 2)
					{
						libgmsec_perl::logInfo("Responder is preparing response message...");

						# prepare response message
						my $repMsg = $conn->getMessageFactory()->createMessage("RESP.DIR");

						$repMsg->setSubject( $reply_subject );

						if ($reqMsg->hasField("REQUEST-ID"))
						{
							$repMsg->addField( $reqMsg->getField("REQUEST-ID") );
						}

						if ($reqMsg->hasField("COMPONENT"))
						{
							$repMsg->addField( libgmsec_perl::StringField->new("DESTINATION-COMPONENT", $reqMsg->getStringValue("COMPONENT")) );
						}

						$repMsg->addField( libgmsec_perl::I16Field->new("RESPONSE-STATUS", 1) );

						# send response message
						$conn->reply($reqMsg, $repMsg);
					}
					else
					{
						libgmsec_perl::logInfo("Responder is ignoring the request message");
					}
				}

				$iter += 1;
			}

			libgmsec_perl::logInfo("Responder is cleaning up...");
			$conn->disconnect();
		};
		if (isa($@, 'libgmsec_perl::GmsecException'))
		{
			my $error = $@;
			libgmsec_perl::logError("T010_Responder Error: " . $error->what());
		}

		if (defined $conn)
		{
			libgmsec_perl::Connection::destroy($conn);
		}

		libgmsec_perl::Connection::shutdownAllMiddlewares();
	}
}


# Main entry point
#
my $config = libgmsec_perl::Config->new();

foreach my $arg (@main::ARGV)
{
	my @argVals = split /=/, $arg;

	if (scalar @argVals == 2)
	{
		$config->addValue($argVals[0], $argVals[1]);
	}
}

my $responder = T010_Responder->new( $config );
$responder->respondToRequest();
exit 0;
