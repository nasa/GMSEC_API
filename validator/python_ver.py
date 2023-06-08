#!/usr/bin/env python3


# Copyright 2007-2023 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

import os
import platform

def versiontuple(v):
    return tuple(map(int, (v.split("."))))

if versiontuple(platform.python_version()) >= versiontuple("3.8.5"):
    os._exit(0)

os._exit(1)

