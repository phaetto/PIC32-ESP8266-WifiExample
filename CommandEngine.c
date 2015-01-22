
#include "user.h"
#include "CommandEngine.h"
#include "../Commands/command_help.h"
#include "../Commands/command_clear.h"
#include "../ESP8266/ESP8266.h"
#include "../ESP8266/WifiGetCommand.h"
#include "../ESP8266/WifiSendCommand.h"

/*
 * Example wifi service 2, that is not set up yet
 *
 * To actually use it you need to add data to its buffer using
 */

WifiServiceData Wifi2ServiceData = {
    DefaultWifiWriteString
};
Service Wifi2Service = {
    "Wifi #2 service",
    "Background wifi orchestration (#2)",
    ServiceWifiImplementation,
    Starting,
    (void*)&Wifi2ServiceData
};

/*
 * Command-engine setup for this project
 */

#define MAX_BUFFER 0xFF
byte CommandBuffer[MAX_BUFFER];

const Command* Commands[] = {
    &HelpCommand,
    &ClearCommand,
    &WifiGetCommand,
    &WifiSendCommand,
    NULL
};

const Application* Applications[] = {
    NULL
};

Service* Services[] = {
    &DefaultWifiService,
    &Wifi2Service,
    NULL
};

CommandEngine CurrentCommandEngine = {
    CommandBuffer,
    MAX_BUFFER,
    Commands,
    Applications,
    Services,
    (WriterMethodType)WriteString,
    (WriterMethodType)DefaultErrorToOutput,
    "$> ",
};