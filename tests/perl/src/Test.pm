use strict;
use File::Spec;
use libgmsec_perl;


*isa = \&UNIVERSAL::isa;


sub Util::printStackTrace
{
	my $i = 0;
	print "--- begin stack trace ---\n";
	while (my @details = caller(++$i))
	{
		print "$details[1] line $details[2] in function $details[3]\n";
	}
	print "--- end stack trace ---\n";
}


=begin UNSUPPORTED
# Define custom LogHandler
#
{
    package MyLogHandler;
    use base 'libgmsec_perl::LogHandler';

    sub new
    {
        my $class = shift;
        my $self  = $class->SUPER::new();

        $self->{MUTEX} = libgmsec_perl::Mutex->new();

        return $self;
    }

    sub onMessage
    {
        my ($self, $entry) = @_;

        $self->{MUTEX}->enterMutex();

        # Check to see if the logging macro was called from the API, or from the SWIG wrapper code
        my $isFromSwig = index($entry->swig_file_get, "libgmsec_perl_wrap") != -1;
        my $file = ($isFromSwig) ? (caller(1))[1] : $entry->swig_file_get;
        my $line = ($isFromSwig) ? (caller(1))[2] : $entry->swig_line_get;
        my $time = libgmsec_perl::TimeUtil::formatTime($entry->swig_time_get);

        my $result = rindex($file, File::Spec->catfile('', ''));
        if ($result != -1)
        {
            $file = substr($file, $result+1);
        }

        my $message = $time . " [" . libgmsec_perl::Log::toString($entry->swig_level_get) . "] "
                            . "[" . $file . ":" . $line . "] "
                            . $entry->swig_message_get . "\n";

        my $find = "\n";
        my $replace = "\n\t";
        $message =~ s/$find/$replace/g;

        $message .= "\n";

        print $message;

        $self->{MUTEX}->leaveMutex();
    }
}
=end UNSUPPORTED
=cut


{
	package Test::Log;

	sub new
	{
		my $self = bless({});
		return $self;
	}


	sub report
	{
		shift;
		print @_, "\n";
	}


	sub warning
	{
		shift->report('warning:', @_);
	}


	sub debug
	{
		shift->report('debug:', @_);
	}


	sub error
	{
		shift->report('error:', @_);
	}


	sub fatal
	{
		my $text = "fatal: @_";
		shift->report($text);
		Util::printStackTrace;
		die($text);
	}
}

{
	package Test;

	use FileHandle;
	use libgmsec_perl;

	sub new
	{
		my ($first, %args) = @_;

		$0 =~ m/(.+)[\/\\](.+)$/;
		$args{DIRECTORY} = $1;

		$args{N_OK} |= 0;
		$args{N_BAD} |= 0;

		my $self = bless(\%args, $first);
		$self->{LOG} = Test::Log->new();

		Test::getConfig();

		$self->initStandardFields();

=begin UNSUPPORTED
		$self->{LOG_HANDLER} = MyLogHandler->new();
		libgmsec_perl::Log::registerHandler($self->{LOG_HANDLER});
=end UNSUPPORTED
=cut

		return $self;
	}


	sub driver
	{
		my $log = Test::Log->new();

		my $value = eval
		{
			&driverAux;
		};

		if ($@)
		{
			$log->report("caught: $@");
		}
		else
		{
			#$log->debug("driver => $value");
		}

		exit $value;
	}


	sub driverAux
	{
		my $log = Test::Log->new();
		my $function = undef;

		foreach my $x (@_)
		{
			if (ref($x) eq 'CODE')
			{
				$function = $x;
			}
		}

		if (not $function)
		{
			$log->fatal("missing test function");
		}

		my $name = $0;
		my @argv;
		my $signal = undef;

		foreach my $arg (@main::ARGV)
		{
			if ($arg =~ /^-/)
			{
				if ($arg =~ /^-signal=(.+)/)
				{
					$signal = $1;
				}
				else
				{
					$log->warning("test driver ignoring $arg");
				}
			}
			else
			{
				push(@argv, $arg);
			}
		}

		my $id = '0000';
        my $tmp = substr( $name, rindex($name, "/") + 1, length($name) - rindex($name, "/") - 1);
		if ($tmp =~ /^.*?(\d+)/)
		{
		    $id = $1;
		}

		my $test = Test->new(TEST_ID => $id,
				TEST_NAME => $name,
				ARGV => \@argv,
				FUNCTION => $function,
				SIGNAL => $signal,
				);

		#my $i = 0;
		#foreach my $x (@argv)
		#{
		#	$log->debug("ARGV[$i]: $x");
		#	++$i;
		#}

		$test->setPrefix();

		my $value = eval
		{
			$function->($test);
		};

		if ($@)
		{
			$value = -1;
			$log->report("caught: $@");
		}

		libgmsec_perl::Connection::shutdownAllMiddlewares();

		$test->finish;

		return $value;
	}


	sub log
	{
		my ($self, $in) = @_;
		my $log = $self->{LOG};
		if (ref($in))
		{
			$self->{LOG} = $in;
		}
		return $log;
	}


	sub setPrefix
	{
		my ($self, $extra) = @_;

		my $prefix = 'NIGHTRUN.PERL.';

		my $pid = $$;
		$prefix .= $pid;

		require Sys::Hostname;
		my $host = Sys::Hostname::hostname();
		$prefix .= '_' . uc($host);

		$prefix .= '.T' . $self->{TEST_ID};

		$prefix .= $extra;

		$self->{PREFIX} = $prefix;
	}


	sub initStandardFields
	{
		my ($self) = @_;

		$self->{STANDARD_FIELDS} = libgmsec_perl::FieldArray->new();

		my $field = libgmsec_perl::StringField->new("MISSION-ID", "MY-MISSION", 1);
		$self->{STANDARD_FIELDS}->push($field);
		$field->DISOWN();

		$field = libgmsec_perl::StringField->new("CONSTELLATION-ID", "MY-CONSTELLATION-ID", 1);
		$self->{STANDARD_FIELDS}->push($field);
		$field->DISOWN();

		$field = libgmsec_perl::StringField->new("SAT-ID-PHYSICAL", "MY-SAT-ID", 1);
		$self->{STANDARD_FIELDS}->push($field);
		$field->DISOWN();

		$field = libgmsec_perl::StringField->new("SAT-ID-LOGICAL", "MY-SAT-ID", 1);
		$self->{STANDARD_FIELDS}->push($field);
		$field->DISOWN();

		$field = libgmsec_perl::StringField->new("COMPONENT", "MY-COMPONENT", 1);
		$self->{STANDARD_FIELDS}->push($field);
		$field->DISOWN();

		$field = libgmsec_perl::StringField->new("FACILITY", "MY-FACILITY", 1);
		$self->{STANDARD_FIELDS}->push($field);
		$field->DISOWN();

		$field = libgmsec_perl::StringField->new("DOMAIN1", "MY-DOMAIN-1", 1);
		$self->{STANDARD_FIELDS}->push($field);
		$field->DISOWN();

		$field = libgmsec_perl::StringField->new("DOMAIN2", "MY-DOMAIN-2", 1);
		$self->{STANDARD_FIELDS}->push($field);
		$field->DISOWN();
	}


	sub getStandardFields
	{
		my ($self) = @_;

		return $self->{STANDARD_FIELDS};
	}


	sub setStandardFields
	{
		my ($self, $factory) = @_;

		$factory->setStandardFields( $self->{STANDARD_FIELDS} );
	}


	sub verifyStandardFields
	{
		my ($self, $msg) = @_;

		for (my $i = 0; $i < $self->{STANDARD_FIELDS}->size(); $i++)
		{
			my $field = $self->{STANDARD_FIELDS}->get($i);

			$self->check("Message is missing standard field", $msg->hasField($field->getName()));
			$self->check("Message unexpected standard field type", $msg->getField($field->getName())->getType() == $field->getType());
			$self->check("Message unexpected standard field value", $msg->getField($field->getName())->getStringValue() == $field->getStringValue());
		}
	}


	sub getConfig
	{
		my ($self) = @_;
		if (not $self->{CONFIG})
		{
			my @argv;
			foreach my $arg (@main::ARGV)
			{
				if ($arg =~ /^-/)
				{
					# skip options that begin with a hyphen
				}
				else
				{
					push(@argv, $arg);
				}
			}
			if ($#argv ge 0)
			{
				$self->{CONFIG} = libgmsec_perl::Config::create(\@argv);
			}
			else
			{
				$self->{CONFIG} = libgmsec_perl::Config->new();
			}

			my $val = $self->{CONFIG}->getValue("loglevel", "info");
			$self->{CONFIG}->addValue("loglevel", $val);

			$val = $self->{CONFIG}->getValue("logfile", "stderr");
			$self->{CONFIG}->addValue("logfile", $val);
		}
		return $self->{CONFIG};
	}


	sub getSubject
	{
		my ($self, $extra) = @_;
		my $subject = $self->{PREFIX};
		if (defined($extra))
		{
			$subject .= ".$extra";
		}
		return $subject;
	}


	sub getUniqueComponent
	{
		my ($self) = @_;
		my $component = $self->{PREFIX};
		$component =~ tr/./-/;
		return $component;
	}


	sub getDataFile
	{
		my ($self, $filename) = @_;
		my $os = $^O;
		my $addons_dir = $self->getConfig()->getValue("addons");
		if (not defined $addons_dir)
		{
			if ($os eq "linux" or $os eq "darwin")
			{
				$addons_dir = $self->{DIRECTORY} . "/" . "addons";
			}
			else
			{
				$addons_dir = $self->{DIRECTORY} . "\\" . "addons";
			}
		}
		if ($os eq "linux" or $os eq "darwin")
		{
			return $addons_dir . "/" . $filename;
		}
		return $addons_dir . "\\" . $filename;
	}


	sub check
	{
		my ($self, $what, $item, $throw) = @_;
		if ($self->{DEBUG_CHECK})
		{
			libgmsec_perl::logDebug("check $what");
		}

		my $ok = undef;
		my $flag = undef;

		if (ref($item))
		{
			# item is Status
			my $bad = $item->isError;
			$flag = $bad ? 0 : 1;
			if ($bad)
			{
				$what .= ': ' . $item->get();
			}
		}
		else
		{
			# item is a flag
			$flag = $item;
		}

		if ($flag)
		{
			++$self->{N_OK};
		}
		else
		{
			++$self->{N_BAD};
			my $str = "'$what' failed";
			libgmsec_perl::logWarning($str);
			if ($throw)
			{
				Util::printStackTrace;
				die($str);
			}
		}

		return $flag;
	}


	sub require
	{
		my ($self, $what, $item) = @_;
		return $self->check($what, $item, 1);
	}


	sub excuse
	{
		my ($self, $reason) = @_;
		$self->{REASON} = $reason;
	}


	sub getDescription
	{
		my ($self) = @_;
		if (not $self->{DESCRIPTION})
		{
			$self->{DESCRIPTION} = "Unknown Perl 4.X Unit Test";
		}
		return $self->{DESCRIPTION};
	}


	sub setDescription
	{
		my ($self, $desc) = @_;
		$self->{DESCRIPTION} = $desc;
		$self->start;
	}


	sub start
	{
		#
		# Same as sub finish(), but only put out initial signal file with
		# a valid description and that's all.  Meant to be called from
		# setDescription() so presumably it will be called at the start
		# of every unit test.
		#

		my ($self) = @_;

		my @summary = (
			'DESCRIPTION=' . $self->getDescription()
		);

		if ($self->{SIGNAL})
		{
			my $fh = FileHandle->new($self->{SIGNAL}, 'w');
			my $text = join("\n", @summary, '');
			$fh->print($text);
			$fh->close;
		}
	}


	sub finish
	{
		my ($self) = @_;

		my $result = 'FAIL';

		if ($self->{N_BAD} == 0)
		{
			if ($self->{N_OK} > 0)
			{
				$result = 'PASS';
			}
		}

		if ($self->{REASON})
		{
			$result = 'EXCUSE';
		}

		my @summary = (
			'RESULT=' . $result,
			'DESCRIPTION=' . $self->getDescription(),
			'N_OK=' . $self->{N_OK},
			'N_BAD=' . $self->{N_BAD},
			'PREFIX=' . $self->{PREFIX},
		);

		if ($self->{REASON})
		{
			push(@summary, 'EXCUSE=' . $self->{REASON});
		}


		my $summary = join("\n", 'summary:', @summary);

		libgmsec_perl::logInfo($summary);

		if ($self->{SIGNAL})
		{
			my $fh = FileHandle->new($self->{SIGNAL}, 'w');
			my $text = join("\n", @summary, '');
			$fh->print($text);
			$fh->close;
		}
	}
}

1;

