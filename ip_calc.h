#ifndef ip_calc_h
#define ip_calc_h

#include <iostream>
#include <string>

using namespace std;

typedef struct 
{
    unsigned char ip[4];
    unsigned char netmask[4];
    unsigned char wildcard[4];
    unsigned char network[4];
    unsigned char broadcast[4];
    unsigned char first_address[4];
    unsigned char last_address[4];
    unsigned char netmask_length;
    
}ipv4;

int*    SplitIntoDigits(const string& text);    // split text like ip address
bool    IsValidIpAddress(const int * ip);       //check valid address
bool    IsValidNetmask(const int * netmask);    //check valid netmask

void    SetIpAddress(ipv4& a);     //entered ip
void    SetNetmask(ipv4& a);       //entered netmask and set netmask_length
void    SetWildcard(ipv4& a);      //compute wildcard
void    SetNetwork(ipv4& a);       //compute network
void    SetBroadcast(ipv4& a);     //compute broadcast
void    SetFirstAddress(ipv4& a);  //compute first_address
void    SetLastAddress(ipv4& a);   //compute last_address
void    ShowAddress(ipv4& a);      //show address
void    ComputeIpv4(ipv4& a);      //calling all functions

#endif // ip_calc_hpp