#!/usr/bin/env python

"""
 Copyright 2007-2019 United States Government as represented by the
 Administrator of The National Aeronautics and Space Administration.
 No copyright is claimed in the United States under Title 17, U.S. Code.
 All Rights Reserved.
"""



""" 
 @file binary_field.py
 
 A Python example demonstration for the following:
    1. Creating a BinaryField
    2. Adding BinaryField to a Message
    3. Retrieve BinaryField from the Message using Field Iterator
    4. Verify BinaryField and Message Field are the same
"""

import libgmsec_python as lp
import binascii


# 1. Create BinaryField, and display information regarding it
#
blob=bytearray()
for i in range(0, 256):
    blob.append(i)

binField = lp.BinaryField('BIN-FIELD-1', blob, len(blob))

print("\nBinaryField...\n")
print("Field Name is    : " + binField.getName())
print("Field Type is    : " + str(binField.getType()))
print("Field Len        : " + str(binField.getLength()))
print("Field as string  : ")
print(binField.getStringValue())
print("Field as raw data:")
binData = binField.getValue()
print(binascii.hexlify(binData))
print("\n")


# 2. Create Message and add existing BinaryField, and an anonymous one
#
msg = lp.Message('GMSEC.FOO.BAR', lp.Message.PUBLISH)
msg.addField(binField)
msg.addField("BIN-FIELD-2", blob, len(blob))


# 3. Retrieve Field(s) from the Message
#
msgFieldIter = msg.getFieldIterator()

print("BinaryField from the Message...\n")
while msgFieldIter.hasNext():
    msgField = msgFieldIter.next()

    print("Field Name is    : " + msgField.getName())
    print("Field Type is    : " + str(msgField.getType()))

    if msgField.getType() == lp.Field.BIN_TYPE:

        msgBinField = lp.to_BinaryField(msgField) # Convenience method for casting Field object to BinaryField

        print("Field Len        : " + str(msgBinField.getLength()))
        print("Field as string  : ")
        print(binField.getStringValue())
        print("Field as raw data:")
        msgBinData = msgBinField.getValue()
        print(binascii.hexlify(msgBinData))

        # Assert data in message field is the same as original blob data
        assert msgBinData == blob, "Message blob has different value from original BinaryField"

        print("\nData from message and original data are the same!\n\n")

