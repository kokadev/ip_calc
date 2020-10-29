#ifndef IPCALC_H
#define IPCALC_H

#include <vector>
#include <iostream>
#include <string>
#include <regex>

using namespace std;

struct ipv4
{unsigned char digit[4];};

void            SetDigits(const vector<int>& digits, const ipv4& ip);
void            Show(const ipv4& address);
vector<int>     SplitIntoDigits(const string& text);
bool            IsIpv4Address(const string& text);
ipv4            GetIpAddress(ipv4& ip);    
ipv4            GetNetmask(ipv4& netmask, unsigned char& temp);
ipv4            GetWildcard(const ipv4& netmask, ipv4& wildcard);
ipv4            GetNetwork(const ipv4& ip, const ipv4& mask, ipv4& network);
ipv4            GetBroadcast(const ipv4& network, const ipv4& wildcard, ipv4& broadcast);
ipv4            GetLastAddress(const ipv4& broadcast, ipv4& last);
ipv4            GetFirstAddress(const ipv4& network, ipv4& first);
unsigned char   GetNetmaskLength(const vector<int>& digits);

#endif