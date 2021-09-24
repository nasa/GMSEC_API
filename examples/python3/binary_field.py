#!/usr/bin/env python3

# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


""" 
 @file binary_field.py
 
 A Python example demonstration for the following:
    1. Creating a BinaryField
    2. Adding BinaryField to a Message
    3. Retrieve BinaryField from the Message using Field Iterator
    4. Verify BinaryField and Message Field are the same
"""

import libgmsec_python3 as lp
import binascii


# 1. Create BinaryField, and display information regarding it
#
blob=bytearray()
for i in range(0, 256):
    blob.append(i)

binField = lp.BinaryField('BIN-FIELD-1', blob, len(blob))

print("\nBinaryField...\n")
print("Field Name is    : " + binField.get_name())
print("Field Type is    : " + str(binField.get_type()))
print("Field Len        : " + str(binField.get_length()))
print("Field as string  : ")
print(binField.get_string_value())
print("Field as raw data:")
binData = binField.get_value()
print(binascii.hexlify(binData))
print("\n")


# 2. Create Message and add existing BinaryField, and an anonymous one
#
msg = lp.Message('GMSEC.FOO.BAR', lp.Message.PUBLISH)
msg.add_field(binField)
msg.add_field("BIN-FIELD-2", blob, len(blob))


# 3. Retrieve Field(s) from the Message
#
msgFieldIter = msg.get_field_iterator()

print("BinaryField from the Message...\n")
while msgFieldIter.has_next():
    msgField = msgFieldIter.next()

    print("Field Name is    : " + msgField.get_name())
    print("Field Type is    : " + str(msgField.get_type()))

    if msgField.get_type() == lp.Field.BIN_TYPE:

        msgBinField = lp.to_BinaryField(msgField) # Convenience method for casting Field object to BinaryField

        print("Field Len        : " + str(msgBinField.get_length()))
        print("Field as string  : ")
        print(binField.get_string_value())
        print("Field as raw data:")
        msgBinData = msgBinField.get_value()
        print(binascii.hexlify(msgBinData))

        # Assert data in message field is the same as original blob data
        assert msgBinData == blob, "Message blob has different value from original BinaryField"

        print("\nData from message and original data are the same!\n\n")
