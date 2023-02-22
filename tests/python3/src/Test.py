#!/usr/bin/env python3

import time
import platform
import os
import sys
import libgmsec_python3 as libgmsec_python
from inspect import getsourcefile

import libgmsec_python3 as lp

class Test(object):
    def __init__(self):
        self.nOk=0
        self.nBad=0
        self.prefix=""
        self.signal=""
        self.reason=""
        self.exitReason=""
        self.description=""
        self.config= libgmsec_python.Config()
        self.listStandardFields = []
        self.standardFields = libgmsec_python.FieldList()


    def init_standard_fields(self):
        self.listStandardFields.append( libgmsec_python.StringField("MISSION-ID", "MY-MISSION", True) )
        self.listStandardFields.append( libgmsec_python.StringField("CONSTELLATION-ID", "MY-CONSTELLATION", True) )
        self.listStandardFields.append( libgmsec_python.StringField("SAT-ID-PHYSICAL", "MY-SAT-ID", True) )
        self.listStandardFields.append( libgmsec_python.StringField("SAT-ID-LOGICAL", "MY-SAT-ID", True) )
        self.listStandardFields.append( libgmsec_python.StringField("FACILITY", "MY-FACILITY", True) )
        self.listStandardFields.append( libgmsec_python.StringField("DOMAIN1", "MY-DOMAIN-1", True) )
        self.listStandardFields.append( libgmsec_python.StringField("DOMAIN2", "MY-DOMAIN-2", True) )
        self.listStandardFields.append( libgmsec_python.StringField("COMPONENT", "MY-COMPONENT", True) )
        
        for field in self.listStandardFields:
            self.standardFields.push_back(field)


    def get_config(self):
        return self.config


    def get_standard_fields(self):
        return self.standardFields


    def set_standard_fields(self, msgFactory):
        msgFactory.set_standard_fields(self.standardFields)


    def verify_standard_fields(self, msg):
        for field in self.standardFields:
            self.require("Message is missing standard field", msg.has_field(field.get_name()))
            self.check("Message has expected standard field type", msg.get_field(field.get_name()).get_type() == field.get_type())
            self.check("Message has expected standard field value", msg.get_field(field.get_name()).get_string_value() == field.get_string_value())
            self.check("Message is not a header field", msg.get_field(field.get_name()).is_header() == True)


    def check(self, what, flag=None, status=None):
        if status is None:
            return self._check(what, flag)
        else:
            return self._check(what+': '+status.get(), not status.is_error())


    def require(self, what, flag=None, status=None):
        if status is None:
            if(not self._check(what,flag)):
                raise Exception(what)
        else:
            if(not self._check(what, not status.is_error())):
                raise Exception(what+': '+status.get())


    def millisleep(self, ms):
        self.ms=ms
        self.ms=self.ms/1000
        try:
            time.sleep(self.ms)
        except Exception as e:
            print(e)


    def get_subject(self, tag=None):
        if tag is None:
            subject = self.prefix
        else:
            subject = self.prefix+"."+tag
        return subject.upper()


    def get_unique_component(self):
        component = self.prefix
        component.replace(".", "-")
        return component


    def getDataFile(self, file):
        path=None
        try:
            self.addonDir=self.config.get_value('addons')
            if(self.addonDir):
                path=self.addonDir+'/'+file;
            else:
                path=self.directory + "/../src/addons/" + file
        except ValueError as e:
            print(str(e))
        return path


    def _check(self, what, flag):
        if(flag):
            self.nOk+=1
        else:
            self.nBad+=1
            libgmsec_python.log_error(what)
        return flag


    def set_prefix(self, s):
        # Set up prefix
        self.prefix ='NIGHTRUN.PYTHON3.'

        # Add PID
        pid = os.getpid()
        self.prefix += str(pid)
        self.prefix  = self.prefix.upper()
        self.prefix += '_'

        # Add hostname
        host = platform.node()
        self.prefix += host
        self.prefix += '.'

        tmp = os.path.realpath(s)
        dir,file = os.path.split(tmp)

        # Add test number
        self.prefix += 'T'
        for c in os.path.basename(file):
            if c >= '0' and c <= '9':
                self.prefix += c

        self.prefix = self.prefix.upper()

        # Also capture our current working directory
        self.directory = dir


    def forceExitAtCleanup(self, reason):
        self.exitReason=reason


    @staticmethod
    def run(desc, cls):
        cls.__setDescription(desc)
        cls.initialize(sys.argv)

        libgmsec_python.log_info('Testing ' + cls.__class__.__name__ + '...')

        try:
            cls.test()
        except Exception as e:
            cls.check('exception: '+str(e), False)
            cls.nBad+=1
        finally:
            if(cls.exitReason):
                print ('Forcing exit due to: ' + cls.exitReason)
        cls.finish()
        libgmsec_python.Connection.shutdown_all_middlewares()


    def initialize(self, args):
        for s in args:
            if (s[0]=='-' and s.startswith('-signal=')):
                self.signal=s[8:]

        self.set_prefix(args[0])
        
        self.config = libgmsec_python.Config(args)
        self.config.add_value('LOGLEVEL', self.config.get_value('LOGLEVEL', 'INFO'))
        self.config.add_value('LOGFILE', self.config.get_value('LOGFILE', 'STDERR'))

        libgmsec_python.Log.register_handler( libgmsec_python.Log.defaultLogHandler )

        self.init_standard_fields()


    def test(self):
        try:
            self.check('this is not a complete test case',False)
        except Exception as e:
            print(e)


    def excuse(self, s):
        self.reason=s;


    def comment(sef, comment):
        libgmsec_python.log_info(comment)


    def __setDescription(self, desc):
        self.description=desc;
        self.start()


    def get_description(self):
        if(len(self.description) >0):
            return self.description
        else:
            return 'Unknown python unit test'


    def start(self):
        summary='DESCRIPTION='
        summary+=self.get_description()

        if self.signal:
            try:
                f=open(self.signal,'w')
                f.write('\n',summary, ' ')
                f.close()
            except IOError as e:
                print(e)


    def finish(self):
        result = 'FAIL'

        if self.nBad == 0:
            if self.nOk > 0:
                result='PASS'
        if self.reason:
            result='EXCUSE'

        summary  = 'RESULT=' + result
        summary += '\nDESCRIPTION=' + self.get_description()
        summary += '\nN_OK=' + str(self.nOk)
        summary += '\nN_BAD=' + str(self.nBad)
        summary += '\nPREFIX=' + self.prefix
        
        if self.reason:
            summary += '\nEXCUSE=' + self.reason
    
        libgmsec_python.log_info('summary:\n' + summary + '\n')
        
        if self.signal:
            try:
                f = open(self.signal, 'w')
                f.write(summary)
                f.close()
            except IOError as e:
                print(e)


    def allowMiddleware(self, allowed):
        mw_allowed = False
        mw_id = self.get_config().get_value("mw-id")

        if mw_id != None:
            for mw in allowed:
                mw_allowed = (mw in mw_id)
                if mw_allowed == True:
                    break

            if mw_allowed == False:
                self.excuse("Test is being excused for " + mw_id + ".")
        else:
            self.excuse("Test excused; mw-id not defined in configuration.")

        return mw_allowed
