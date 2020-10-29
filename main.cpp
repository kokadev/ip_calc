#include "ipcalc.h"

int main()
{
    unsigned char netmask_length = 0;
    ipv4 ip_address, netmask, network, wildcard, broadcast, last, first;

    GetIpAddress(ip_address);
    GetNetmask(netmask, netmask_length);
    GetNetwork(ip_address, netmask, network);
    GetWildcard(netmask, wildcard);
    GetBroadcast(network, wildcard, broadcast);
    GetLastAddress(broadcast, last);
    GetFirstAddress(network, first);

    Show(ip_address);   cout << "IP" << '\n';
    Show(netmask);      cout << "NETMASK" << '\n';
    Show(wildcard);     cout << "WILDCARD" << '\n';
    Show(network);      cout << "NETWORK" << '\n';
    Show(broadcast);    cout << "BROADCAST" << '\n';
    Show(first);        cout << "FIRST_ADDRESS" << '\n';
    Show(last);         cout << "LAST_ADDRESS" << "\n\n";

    cout << "HOSTS COUNT = " << (int)(1<<(32-netmask_length))-2 <<'\n';

    system("pause");
    return 0;
}