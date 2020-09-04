"""This module reads the GMSEC message templates from the GMSEC_API templates directory and generates sample GMSEC messages based on the templates"""

import optparse
import xml.etree.ElementTree as ET
import xmltodict
import os
import json
from xml.dom import minidom
import unidecode

SUBJECT_TEMPLATE = "GMSEC.TEST-MISSION-ID.SAT-ID."

def get_fields(doc):
    fields = []
    for field in doc['SCHEMA']['FIELD']:
        if field['@MODE'] != 'CONTROL':
            fields.append({'NAME': field['@NAME'],
                           'TYPE': field['@TYPE'],
                           'MODE': field['@MODE'],
                           'VALUE': field['@VALUE'],
                           'DESCRIPTION': field['@DESCRIPTION']})
    return fields


def add_field_json(message, field):
    f = []
    f.append(field)


def add_field_xml(message, field):
    f = ET.SubElement(message, 'FIELD')
    f.set('NAME', field['NAME'])
    f.set('TYPE', field['TYPE'])
    f.set('MODE', field['MODE'])
    f.set('DESCRIPTION', field['DESCRIPTION'])
    f.text = field['VALUE']


def process_header(header_file):
    with open(os.path.join(header_file)) as fd:
        doc = xmltodict.parse(fd.read())
    return get_fields(doc)


def get_message_attr(doc):
    schema_name = doc['SCHEMA']['@NAME'].split(".")
    type = schema_name[3]
    subtype = schema_name[4]
    if type == 'MSG':
        kind = 'PUBLISH'
    elif type == 'REQ':
        kind = 'REQUEST'
    else:
        kind = 'RESPONSE'

    return {'kind': kind, 'type': type, 'subtype': subtype}


def process_content_json(header_fields, doc):

    message_attr = get_message_attr(doc)

    message = {}
    message['MESSAGE'] = {}
    message['MESSAGE']['KIND'] = message_attr['kind']
    message['MESSAGE']['FIELD'] = list(header_fields)

    content_fields = get_fields(doc)
    message['MESSAGE']['FIELD'].extend(content_fields)

    for field in message['MESSAGE']['FIELD']:
        if field['NAME'] == 'MESSAGE-TYPE':
            field['VALUE'] = message_attr['type']
            msg_type = message_attr['type']
        if field['NAME'] == 'MESSAGE-SUBTYPE':
            field['VALUE'] = message_attr['subtype']
            msg_subtype = message_attr['type']

    message['MESSAGE']['SUBJECT'] = SUBJECT_TEMPLATE + msg_type + "." + msg_subtype + ".TEST-COMPONENT"

    return message


def process_content_xml(header_fields, doc):

    message_attr = get_message_attr(doc)

    content_fields = get_fields(doc)
    message = ET.Element('MESSAGE')

    message.set('KIND', message_attr['kind'])
    
    for field in header_fields:
        add_field_xml(message, field)
        if field['NAME'] == 'MESSAGE-TYPE':
            msg_type = field['VALUE']
        if field['NAME'] == 'MESSAGE-SUBTYPE':
            msg_subtype = field['VALUE']

    message.set('SUBJECT', SUBJECT_TEMPLATE + msg_type + "." + msg_subtype + ".TEST-COMPONENT")

    for field in content_fields:
        add_field_xml(message, field)


    return message


def process_template(options, args):

    header_fields = process_header(os.path.join(
        options.input_dir, "GMSEC.HEADER.xml"))

    for fn in os.listdir(options.input_dir):

        if not os.path.exists(options.output_dir):
            os.makedirs(options.output_dir)

        filename = os.path.join(options.input_dir, fn)
        xmlname = os.path.join(options.output_dir, fn)
        jsonname = os.path.join(
            options.output_dir, fn).replace(".xml", ".json")

        if os.path.isfile(filename) and fn.startswith(('GMSEC.MSG', 'GMSEC.REQ', 'GMSEC.RESP')):
            print filename
            with open(filename) as fd:
                doc = xmltodict.parse(fd.read())

            message = process_content_json(header_fields, doc)
            with open(jsonname, 'w') as file:
                file.write(json.dumps(message, indent=4,
                                      separators=(',', ': '), sort_keys=False))

            message = process_content_xml(header_fields, doc)
            with open(xmlname, 'w') as file:
                xml_string = minidom.parseString(ET.tostring(message, 'utf-8'))
                file.write(unidecode.unidecode(
                    xml_string.toprettyxml(indent="    ")))


if __name__ == "__main__":

    usage = "usage: %prog [options]"
    parser = optparse.OptionParser()
    parser.add_option("-i", "--input", dest="input_dir",
                      help="input directory containing the GMSEC templates", metavar="DIR")
    parser.add_option("-o", "--output", dest="output_dir",
                      help="output directory for json and xml", metavar="DIR")
    parser.add_option("-q", "--quiet", action="store_false", dest="verbose",
                      default=True, help="don't print status messages to stdout")

    (options, args) = parser.parse_args()

    process_template(options, args)
