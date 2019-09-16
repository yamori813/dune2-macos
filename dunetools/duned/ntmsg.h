 Message file

 The Message IDs correspond to the EVENTLOG type
//
//  Values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//


//
// Define the severity codes
//


//
// MessageId: 0x00000000L (No symbolic name defined)
//
// MessageText:
//
//  Success: %1
//


//
// MessageId: 0x00000001L (No symbolic name defined)
//
// MessageText:
//
//  Error: %1
//


//
// MessageId: 0x00000002L (No symbolic name defined)
//
// MessageText:
//
//  Warning: %1
//


//
// MessageId: 0x00000004L (No symbolic name defined)
//
// MessageText:
//
//  Information: %1
//


