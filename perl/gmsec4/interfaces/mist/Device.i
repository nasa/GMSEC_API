/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Device
%{
#include <gmsec4/mist/Device.h>
#include <list>
using namespace gmsec::api::mist;
%}

%ignore gmsec::api::mist::Device::Device(const char*, gmsec::api::mist::Device::DeviceStatus, const gmsec::api::util::DataList<gmsec::api::mist::DeviceParam*>&);
%ignore gmsec::api::mist::Device::addParam(const DeviceParam&);

%ignore gmsec::api::mist::Device::operator=(const Device&);

%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/Device.h>

%include <std_list.i>
namespace std
{
    %template(DeviceParamList) list<gmsec::api::mist::DeviceParam*>;
}

%extend gmsec::api::mist::Device {
    
    Device(const char* name, gmsec::api::mist::Device::DeviceStatus status, const std::list<gmsec::api::mist::DeviceParam*>& params)
    {
        gmsec::api::util::DataList<gmsec::api::mist::DeviceParam*> list;

        for(std::list<gmsec::api::mist::DeviceParam*>::const_iterator it = params.begin(); it != params.end(); ++it)
            list.push_back(*it);

        return new Device(name, status, list);
    }

    void CALL_TYPE addParam(const gmsec::api::mist::DeviceParam& param)
    {
        self->addParam(param);
    }
}

%perlcode%{
=pod

=head1 NAME

libgmsec_perl::Device

This class is a lightweight container for holding information on Device, and is used to generate GMSEC Device messages by the ConnectionManager class

=head2 Public Types and Constants

=head3 DeviceStatus

=over

=item libgmsec_perl::Device::DEBUG

=item libgmsec_perl::Device::NOMINAL_GREEN

=item libgmsec_perl::Device::YELLOW

=item libgmsec_perl::Device::RED

=back

=head2 Public Member Subroutines

=head3 new

C<libgmsec_perl::Device-E<gt>new($name, $status)>

    Default constructor - Initializes the Device object with no Device parameter information

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $name - the name of the device
    $status - The status of the device

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    Exception if the name parameter is NULL or empty

C<libgmsec_perl::Device-E<gt>new($name, $status)>

    Constructor - Initializes the Device object with a set of Device parameter information

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $name - the name of the Device
    $status - the status of the Device
    $params - a DeviceParamList object containing the list of Device parameters

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    Exception if the name parameter is NULL or empty

=head3 DESTROY

C<libgmsec_perl::Device-E<gt>DESTROY()>

    Destructor

=head3 getName

C<libgmsec_perl::Device-E<gt>getName()>

    Retrieves the name of the Device

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    The name of the Device

=head3 numberAvailable

C<libgmsec_perl::Device-E<gt>numberAvailable()>

    Checks the availability of the Device number

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    1 if the Device number has been set, 0 otherwise

=head3 getNumber

C<libgmsec_perl::Device-E<gt>getNumber()>

    Accessor for the Device number

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    A reference to the device number Field

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br> 

    Exception if the Number field has not been set

=head3 setNumber

C<libgmsec_perl::Device-E<gt>setNumber($number)>

    Supplies a number for the Device and makes a copy of the Field

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $number - a Field containing the Device number

=head3 modelAvailable

C<libgmsec_perl::Device-E<gt>modelAvailable()>

    Checks the availability of the model number

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    1 if the Device model has been set, 0 otherwise

=head3 getModel

C<libgmsec_perl::Device-E<gt>getModel()>
    
    Accessor for the Device model

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    A string value of the Device model

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions:</b><br>

    Exception if the Device model field has not been set

=head3 setModel

C<libgmsec_perl::Device-E<gt>setModel($model)>

    Supplies a Model for the device and makes a copy of the string

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters:</b><br>

    $model - A string value containing the model for the device

=head3 serialAvailable

C<libgmsec_perl::Device-E<gt>serialAvailable()>

    Checks the availability of the Device serial number

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    1 if the Device serial number has been set, 0 otherwise

=head3 getSerial

C<libgmsec_perl::Device-E<gt>getSerial()>

    Accessor fo the Device serial number

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    A string value of the Device serial number

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions</b><br>

    Exception if the serial number field has not been set

=head3 setSerial

C<libgmsec_perl::Device-E<gt>setSerial($serial)>

    Supplies a serial number for the device and makes a copy of the string

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters</b><br>

    $serial - a string value containing the Device serial number

=head3 versionAvailable

C<libgmsec_perl::Device-E<gt>serialAvailable()>

    Checks the availability of the Device version number

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    1 if the Device version number has been set, 0 otherwise

=head3 getVersion

C<libgmsec_perl::Device-E<gt>getVersion()>

    Accessor fo the Device version number

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    A string value of the Device version number

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions</b><br>

    Exception if the version number field has not been set

=head3 setVersion

C<libgmsec_perl::Device-E<gt>setVersion($version)>

    Supplies a version number for the device and makes a copy of the string

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters</b><br>

    $version - a string value containing the Device version number
    
=head3 groupAvailable

C<libgmsec_perl::Device-E<gt>groupAvailable()>

    Checks the availability of the Device group

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    1 if the Device group has been set, 0 otherwise

=head3 getGroup

C<libgmsec_perl::Device-E<gt>getGroup()>

    Accessor of the Device group

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    A string value of the device group

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions</b><br>

    Exception if the group field has not been set

=head3 setGroup

C<libgmsec_perl::Device-E<gt>setGroup($group)>

    Supplies a group for the device and makes a copy of the string

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters</b><br>

    $group - a string value containing the Device group

=head3 roleAvailable

C<libgmsec_perl::Device-E<gt>roleAvailable()>

    Checks the availability of the Device role

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    1 if the Device role has been set, 0 otherwise

=head3 getRole

C<libgmsec_perl::Device-E<gt>getRole()>

    Accessor of the Device role

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    A string value of the device role

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions</b><br>

    Exception if the role field has not been set

=head3 setRole

C<libgmsec_perl::Device-E<gt>setRole($role)>

    Supplies a role for the device and makes a copy of the string

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters</b><br>

    $role - a string value containing the Device role

=head3 getStatus

C<libgmsec_perl::Device-E<gt>getStatus()>

    Accessor of the Device Status

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    A Device Status enumeration containing the current Device status

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions</b><br>

    Exception if the Status field has not been set

=head3 setStatus

C<libgmsec_perl::Device-E<gt>setStatus($status)>

    Supplies a status for the device and makes a copy of the string

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters</b><br>

    $status - a Device Status enumeration containing the Device status

=head3 infoAvailable

C<libgmsec_perl::Device-E<gt>infoAvailable()>

    Checks the availability of the Device info

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    1 if the Device info has been set, 0 otherwise

=head3 getInfo

C<libgmsec_perl::Device-E<gt>getInfo()>

    Accessor of the Device info

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    A reference to the Device Info field

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions</b><br>

    Exception if the info field has not been set

=head3 setInfo

C<libgmsec_perl::Device-E<gt>setInfo($info)>

    Supplies Info for the device and makes a copy of the string

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters</b><br>

    $info - a Field object containing the Device info

=head3 getParamCount

C<libgmsec_perl::Device-E<gt>getParamCount()>

    Returns the number of parameters for this device

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    The number of parameters contained in the Device

=head3 getParam

C<libgmsec_perl::Device-E<gt>getParam($idx)>

    Returns a reference to a DeviceParam held in the Device object

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters</b><br>

    $idx - the zero-based index of the DeviceParam object to access

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Returns:</b><br>

    A reference to the requested DeviceParam object

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Exceptions</b><br>

    Exception if the supplied index is out of bounds

=head3 addParam

C<libgmsec_perl::Device-E<gt>addParam($param)>

    Adds a single DeviceParam object to the current list contained in the Device object

=for html &nbsp;&nbsp;&nbsp;&nbsp;<b>Parameters</b><br>

    $param - a new DeviceParam object

=cut
%}
