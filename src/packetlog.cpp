#include "packetlog.h"

#include "util.h"

#include <fstream>
#include <map>
#include <string>

using namespace std;

struct LogInfo
{
	unsigned long count;
	unsigned long bytes;
};

map<string, LogInfo> mapSendLog;

void WriteToDisk(const map<string, LogInfo>& mapLog, const char* txtFilename, const char* csvFilename);

void LogPacketSend(char* pszCommand, int nSize)
{
	string strCommand(pszCommand);
	
	++(mapSendLog[strCommand].count);
	mapSendLog[strCommand].bytes += nSize;

	WriteToDisk(mapSendLog, "send-info.txt", "send-info.csv");
}

void WriteToDisk(const map<string, LogInfo>& mapLog, const char* txtFilename, const char* csvFilename)
{
	boost::filesystem::path pDataDir = GetDataDir();
	ofstream osTxtLogFile((pDataDir / txtFilename).string().c_str(), ios::out | ios::trunc);

	ofstream osCsvLogFile((pDataDir / csvFilename).string().c_str(), ios::out | ios::trunc);
	osCsvLogFile << "Message,Count,Bytes\r\n";
	
	for (map<string, LogInfo>::const_iterator it = mapLog.begin(); it != mapLog.end(); ++it)
	{
		osTxtLogFile << "Message " << it->first << endl;
		osTxtLogFile << "\tCount " << it->second.count << endl;
		osTxtLogFile << "\tBytes " << it->second.bytes << endl;
		osTxtLogFile << endl;

		osCsvLogFile << it->first << "," << it->second.count << "," << it->second.bytes << "\r\n"; 
	}
	
	osTxtLogFile.close();
	osCsvLogFile.close();

}

