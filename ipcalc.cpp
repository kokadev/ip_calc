#include "ipcalc.h"

void SetDigits(const vector<int>& digits, ipv4& ip)
{
    for(int i = 0; i<4; ++i)
    {ip.digit[i]=digits[i];}
}

void Show(const ipv4& a)
{
    for(int i = 0; i<3; ++i)
    {
        cout << (int)a.digit[i];
        cout << '.';
    }
    cout << (int)a.digit[3] << "\t\t\t";


    for(int i = 0; i<3; ++i)
    {
        for(int j = 7; j >-1; --j)
        {
            cout<< (bool)(a.digit[i]& 1<<j);
        }
        cout << '.';
    }

    for(int l = 7; l >-1; --l)
    {
        cout<<(bool)(a.digit[3]& 1<<l);
    }
    cout << "\t";
}

vector<int> SplitIntoDigits(const string& text)
{
    vector<int> digits;
    string word;
    for (int i = 0; i < text.length(); ++i)
    {
        if (text[i] == '.' || text[i] == ',' || text[i] == ' ')
        {
            digits.push_back(stoi(word));
            word = "";
        }
        else
        {
            word += text[i];
        }
    }
    digits.push_back(stoi(word));
    return digits;
}

bool IsIpv4Address(const string& text)
{
    regex regular_ipv4("([0-9]{1,3}[\\.|\\,|\\ ]){3}[0-9]{1,3}");
    return regex_match(text.c_str(), regular_ipv4);
}

ipv4 GetIpAddress(ipv4& ip)
{
    vector<int> address;
    string ip_address;
    bool valid_ip;
    do
    {
        valid_ip = true;
        ip_address = "";
        cout << "Enter the IPv4 address: ";
        getline(cin, ip_address);

        if (IsIpv4Address(ip_address))
        {
            address = SplitIntoDigits(ip_address);
            for (int i : address)
            {
                if (i < 0 || i > 255 || address[0] == 0)
                {
                    valid_ip = false;
                    cout << "Wrong IP Address!" << endl;
                    break;
                }
            }
        }
        else
        {
            valid_ip = false;
            cout << "Wrong IP Address!" << endl;
            break;
        }

    }while (valid_ip != true);

    SetDigits(address, ip);
    return ip;
}

ipv4 GetNetmask(ipv4& mask, unsigned char& netmask_length)
{
    vector<int> netmask;
    string maska;
    bool valid_netmask;
    do
    {
        valid_netmask = true;
        maska = "";
        cout << "Enter the Netmask: ";
        getline(cin, maska);

        if(IsIpv4Address(maska))
        {
            netmask = SplitIntoDigits(maska);
            netmask_length = GetNetmaskLength(netmask);

            for (int i : netmask)
            {
                if
                (
                    i!=0   &&
                    i!=128 &&
                    i!=192 &&
                    i!=224 &&
                    i!=240 &&
                    i!=248 &&
                    i!=252 &&
                    i!=254 &&
                    i!=255
                )
                {
                    valid_netmask = false;
                    cout << "Wrong Netmask!" << endl;
                    break;
                }

                if
                (
                    (netmask[0]<255 && netmask[1]!=0) ||
                    (netmask[1]<255 && netmask[2]!=0) ||
                    (netmask[2]<255 && netmask[3]!=0)
                )
                {
                    valid_netmask = false;
                    cout << "Wrong Netmask!" << endl;
                    break;
                }
            }
        }
        else
        {
            if(maska[0]=='/' && maska.size()<4)
            {
                string mas = "";
                for(int i = 1; i<maska.size(); ++i)
                {mas+=maska[i];}
                netmask_length = stoi(mas);


                if(netmask_length<0 && netmask_length>32)
                {
                valid_netmask = false;
                cout << "Wrong IPv4 Mask!" << endl;
                break;
                }

                if(netmask_length<33 && netmask_length>24)
                {
                    netmask.push_back(255);
                    netmask.push_back(255);
                    netmask.push_back(255);
                    netmask.push_back(256-(1 << (32-netmask_length)));
                }

                if(netmask_length<25 && netmask_length>16)
                {
                    netmask.push_back(255);
                    netmask.push_back(255);
                    netmask.push_back(256-(1 << (24-netmask_length)));
                    netmask.push_back(0);
                }

                if(netmask_length<17 && netmask_length>8)
                {
                    netmask.push_back(255);
                    netmask.push_back(256-(1 << (16-netmask_length)));
                    netmask.push_back(0);
                    netmask.push_back(0);
                }

                if (netmask_length < 9 && netmask_length > -1)
                {
                    netmask.push_back(256 - (1 << (8 - netmask_length)));
                    netmask.push_back(0);
                    netmask.push_back(0);
                    netmask.push_back(0);
                }
            }
            else
            {
                valid_netmask = false;
                cout << "Wrong Netmask!" << endl;
                break;
            }

        }
    }
    while (valid_netmask != true);

    SetDigits(netmask,mask);
    return mask;
}

ipv4 GetNetwork(const ipv4& ip, const ipv4& mask, ipv4& network)
{
    for(int i=0;i<4;++i)
    {
        network.digit[i] = ip.digit[i] & mask.digit[i]; 
    }
    return network;
}

ipv4 GetWildcard(const ipv4& netmask, ipv4& wildcard)
{
    for(int i=0;i<4;++i)
    {
        wildcard.digit[i] = ~netmask.digit[i];
    }
    return wildcard;
}

ipv4 GetBroadcast(const ipv4& network, const ipv4& wildcard, ipv4& broadcast)
{
    for(int i=0;i<4;++i)
    {
        broadcast.digit[i] = network.digit[i] | wildcard.digit[i];
    }
    return broadcast;
}

ipv4 GetLastAddress(const ipv4& broadcast, ipv4& last)
{
    last = broadcast;
    last.digit[3]-=1; 
    return last;
}

ipv4 GetFirstAddress(const ipv4& network, ipv4& first)
{
    first = network;
    first.digit[3]+=1; 
    return first;
}

unsigned char GetNetmaskLength(const vector<int>& digits)
{
    unsigned char c = 0;

    for(char k = 0; k<4; ++k)
    {
    if(digits[k]<255)
    {
        for(int i = 0; i<8; ++i)
        {
            if((digits[0] & 1>>i)!=0)
            {c++;}
        }   
        return c;
    }
    else{c += 8;}
    }

    return c;
}
