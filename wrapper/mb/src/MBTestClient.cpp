
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







#include "TCPSocket.h"

#define TC_S_SUBJECT "SMB.Test"

/*
   * <MESSAGE SUBJECT="MB.Test" KIND="PUBLISH">
         *      <FIELD TYPE="CHAR" NAME="char_field">c</FIELD>
         *      <FIELD TYPE="BOOL" NAME="bool_field">TRUE</FIELD>
         *      <FIELD TYPE="SHORT" NAME="short_field">123</FIELD>
         *      <FIELD TYPE="USHORT" NAME="ushort_field">123</FIELD>
         *      <FIELD TYPE="LONG" NAME="long_field">123</FIELD>
         *      <FIELD TYPE="ULONG" NAME="ulong_field">123</FIELD>
         *      <FIELD TYPE="STRING" NAME="string_field">This is a test</FIELD>
         *      <FIELD TYPE="FLOAT" NAME="float_field">123</FIELD>
         *      <FIELD TYPE="DOUBLE" NAME="double_field">123</FIELD>
         *      <FIELD TYPE="BIN" NAME="bin_field">4a4c4d4e4f5051</FIELD>
         * </MESSAGE>
*/
#define TC_P_MSG     "P<MESSAGE SUBJECT=\"MB.Test\" KIND=\"PUBLISH\">\n\t<FIELD TYPE=\"CHAR\" NAME=\"char_field\">c</FIELD>\n\t<FIELD TYPE=\"BOOL\" NAME=\"bool_field\">TRUE</FIELD>\n\t<FIELD TYPE=\"SHORT\" NAME=\"short_field\">123</FIELD>\n\t<FIELD TYPE=\"USHORT\" NAME=\"ushort_field\">123</FIELD>\n\t<FIELD TYPE=\"LONG\" NAME=\"long_field\">123</FIELD>\n\t<FIELD TYPE=\"ULONG\" NAME=\"ulong_field\">123</FIELD>\n\t<FIELD TYPE=\"STRING\" NAME=\"string_field\">This is a test</FIELD>\n\t<FIELD TYPE=\"FLOAT\" NAME=\"float_field\">123</FIELD>\n\t<FIELD TYPE=\"DOUBLE\" NAME=\"double_field\">123</FIELD>\n\t<FIELD TYPE=\"BIN\" NAME=\"bin_field\">4a4c4d4e4f5051</FIELD>\n</MESSAGE>"

#define TC_U_SUBJECT "UMG.Test"

using namespace gmsec::messagebus;


int main()
{
	int d;

	int inStrLen;
	char * inStr;


	CTCPSocketClient sClient;

	sClient.Connect(10004);

	sClient.SetDebug(0);

	sClient.Read(inStr, inStrLen);
	printf("%s\n", inStr);

	sClient.Write(TC_S_SUBJECT,strlen(TC_S_SUBJECT)+1);

	scanf("%d", &d);

	sClient.Write(TC_P_MSG,strlen(TC_P_MSG)+1);
	scanf("%d", &d);
	sClient.Write(TC_U_SUBJECT,strlen(TC_U_SUBJECT)+1);


	sClient.Read(inStr, inStrLen);
	printf("%s\n", inStr);

	sClient.Disconnect();

	return 0;
}
