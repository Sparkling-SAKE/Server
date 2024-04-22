#pragma once
class NetAddress
{
public:
	NetAddress() = delete;
	NetAddress(SOCKADDR_IN sockAddr);
	NetAddress(const SOCKADDR_IN& sockAddr);
	NetAddress(SOCKADDR_IN&& sockAddr);
	NetAddress(wstring ip, uint16 port);

	~NetAddress() = default;

	const SOCKADDR_IN&	GetSockAddr()  const { return _sockAddr; }
	wstring				GetIpAddress();
	uint16				GetPort()			 { return ::ntohs(_sockAddr.sin_port); }

public:
	static IN_ADDR	Ip2Address(const WCHAR* ip);

private:
	SOCKADDR_IN		_sockAddr = {};
};

