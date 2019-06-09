#ifndef _NET_H_
#define _NET_H_

/* 方法 */
#define GET     "GET"
#define ACK     "ACK"

/* 字段 */
#define ACTION      "ACTION"
#define USER        "USER"
#define PSK         "PSK"
#define KEY         "KEY"
#define FILE_TYPE   "FILE-TYPE"
#define FILE_NAME   "FILE-NAME"
#define HAVE_DATA   "HAVE-DATA"
#define DATA_LEN    "DATA-LEN"

/* 字段值 */
/* ACTION */
#define CONNECT     "CONNECT"
#define DISCONNECT  "DISCONNECT"
#define ASK         "ASK"
#define PRINT       "PRINT"
#define SEND_FILE   "SEN-FILE"
#define GET_INFO    "GET-INFO"
#define FINISH      "FINISH"

#define ACTION_CONNECT      1
#define ACTION_DISCONNECT   2
#define ACTION_ASK          3
#define ACTION_PRINT        4
#define ACTION_SEND_FILE    5
#define ACTION_GET_INFO     6
#define ACTION_FINISH       7

/* FILE-TYPE */
#define PDF         "TEXT/PDF"
#define BMP         "IMG/BMP"
#define PNG         "IMG/PNG"
#define JPG         "IMG/JPG"

/* HAVE-DATA */
#define YES         "YES"
#define NO          "NO"

/* 响应码 */
#define RCODE_OK                100

#define RCODE_USER_OR_PSK_ERR   201
#define RCODE_FILE_TRANSFER_ERR 202

#define RCODE_BUZY              301
#define RCODE_NO_PAPER          302

#define STATUS_OK   "OK"
#define STATUS_FAIL "FAIL"

#endif //_NET_H_