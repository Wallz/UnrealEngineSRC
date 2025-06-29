/*===========================================================================
    C++ class definitions exported from UnrealScript.
    This is automatically generated by the tools.
    DO NOT modify this manually! Edit the corresponding .uc files instead!
===========================================================================*/
#if SUPPORTS_PRAGMA_PACK
#pragma pack (push,4)
#endif

#ifndef IPDRV_API
#define IPDRV_API DLL_IMPORT
#endif

#ifndef NAMES_ONLY
#define AUTOGENERATE_NAME(name) extern IPDRV_API FName IPDRV_##name;
#define AUTOGENERATE_FUNCTION(cls,idx,name)
#endif

AUTOGENERATE_NAME(Accepted)
AUTOGENERATE_NAME(Closed)
AUTOGENERATE_NAME(ConnectionFailed)
AUTOGENERATE_NAME(GetMasterServer)
AUTOGENERATE_NAME(LogStatLine)
AUTOGENERATE_NAME(OnPingTimeout)
AUTOGENERATE_NAME(OnQueryFinished)
AUTOGENERATE_NAME(OnReceivedModMOTDData)
AUTOGENERATE_NAME(OnReceivedMOTDData)
AUTOGENERATE_NAME(OnReceivedOwnageItem)
AUTOGENERATE_NAME(OnReceivedPingInfo)
AUTOGENERATE_NAME(OnReceivedServer)
AUTOGENERATE_NAME(Opened)
AUTOGENERATE_NAME(ReceivedBinary)
AUTOGENERATE_NAME(ReceivedLine)
AUTOGENERATE_NAME(ReceivedText)
AUTOGENERATE_NAME(Refresh)
AUTOGENERATE_NAME(Resolved)
AUTOGENERATE_NAME(ResolveFailed)

#ifndef NAMES_ONLY

enum EReceiveMode
{
    RMODE_Manual            =0,
    RMODE_Event             =1,
    RMODE_MAX               =2,
};
enum ELineMode
{
    LMODE_auto              =0,
    LMODE_DOS               =1,
    LMODE_UNIX              =2,
    LMODE_MAC               =3,
    LMODE_MAX               =4,
};
enum ELinkMode
{
    MODE_Text               =0,
    MODE_Line               =1,
    MODE_Binary             =2,
    MODE_MAX                =3,
};

struct AInternetLink_eventResolveFailed_Parms
{
};
struct AInternetLink_eventResolved_Parms
{
    FIpAddr Addr;
};
class IPDRV_API AInternetLink : public AInternetInfo
{
public:
    BYTE LinkMode GCC_PACK(4);
    BYTE InLineMode;
    BYTE OutLineMode;
    PTRINT Socket GCC_PACK(4);
    INT Port;
    PTRINT RemoteSocket;
    PTRINT PrivateResolveInfo;
    INT DataPending;
    BYTE ReceiveMode;
    DECLARE_FUNCTION(execGetLocalIP);
    DECLARE_FUNCTION(execGameSpyGameName);
    DECLARE_FUNCTION(execGameSpyValidate);
    DECLARE_FUNCTION(execStringToIpAddr);
    DECLARE_FUNCTION(execIpAddrToString);
    DECLARE_FUNCTION(execGetLastError);
    DECLARE_FUNCTION(execResolve);
    DECLARE_FUNCTION(execParseURL);
    DECLARE_FUNCTION(execIsDataPending);
    void eventResolveFailed()
    {
        ProcessEvent(FindFunctionChecked(IPDRV_ResolveFailed),NULL);
    }
    void eventResolved(FIpAddr Addr)
    {
        AInternetLink_eventResolved_Parms Parms;
        Parms.Addr=Addr;
        ProcessEvent(FindFunctionChecked(IPDRV_Resolved),&Parms);
    }
    DECLARE_CLASS(AInternetLink,AInternetInfo,0|CLASS_Transient,IpDrv)
	AInternetLink();
	void Destroy();
	UBOOL Tick( FLOAT DeltaTime, enum ELevelTick TickType );	
	SOCKET& GetSocket() 
	{ 
		return *(SOCKET*)&Socket;
	}
	FResolveInfo*& GetResolveInfo()
	{
		return *(FResolveInfo**)&PrivateResolveInfo;
	}
};


struct AUdpLink_eventReceivedBinary_Parms
{
    FIpAddr Addr;
    INT Count;
    BYTE B[255];
};
struct AUdpLink_eventReceivedLine_Parms
{
    FIpAddr Addr;
    FString Line;
};
struct AUdpLink_eventReceivedText_Parms
{
    FIpAddr Addr;
    FString Text;
};
class IPDRV_API AUdpLink : public AInternetLink
{
public:
    INT BroadcastAddr GCC_PACK(4);
    FStringNoInit RecvBuf;
    DECLARE_FUNCTION(execReadBinary);
    DECLARE_FUNCTION(execReadText);
    DECLARE_FUNCTION(execSendBinary);
    DECLARE_FUNCTION(execSendText);
    DECLARE_FUNCTION(execBindPort);
    void eventReceivedBinary(FIpAddr Addr, INT Count, BYTE* B)
    {
        AUdpLink_eventReceivedBinary_Parms Parms;
        Parms.Addr=Addr;
        Parms.Count=Count;
        appMemcpy(&Parms.B,B,sizeof(Parms.B));
        ProcessEvent(FindFunctionChecked(IPDRV_ReceivedBinary),&Parms);
    }
    void eventReceivedLine(FIpAddr Addr, const FString& Line)
    {
        AUdpLink_eventReceivedLine_Parms Parms;
        Parms.Addr=Addr;
        Parms.Line=Line;
        ProcessEvent(FindFunctionChecked(IPDRV_ReceivedLine),&Parms);
    }
    void eventReceivedText(FIpAddr Addr, const FString& Text)
    {
        AUdpLink_eventReceivedText_Parms Parms;
        Parms.Addr=Addr;
        Parms.Text=Text;
        ProcessEvent(FindFunctionChecked(IPDRV_ReceivedText),&Parms);
    }
    DECLARE_CLASS(AUdpLink,AInternetLink,0|CLASS_Transient,IpDrv)
	AUdpLink();
	void PostScriptDestroyed();
	UBOOL Tick( FLOAT DeltaTime, enum ELevelTick TickType );	
	FSocketData GetSocketData();
};

enum ELinkState
{
    STATE_Initialized       =0,
    STATE_Ready             =1,
    STATE_Listening         =2,
    STATE_Connecting        =3,
    STATE_Connected         =4,
    STATE_ListenClosePending=5,
    STATE_ConnectClosePending=6,
    STATE_ListenClosing     =7,
    STATE_ConnectClosing    =8,
    STATE_MAX               =9,
};

struct ATcpLink_eventReceivedBinary_Parms
{
    INT Count;
    BYTE B[255];
};
struct ATcpLink_eventReceivedLine_Parms
{
    FString Line;
};
struct ATcpLink_eventReceivedText_Parms
{
    FString Text;
};
struct ATcpLink_eventClosed_Parms
{
};
struct ATcpLink_eventOpened_Parms
{
};
struct ATcpLink_eventAccepted_Parms
{
};
class IPDRV_API ATcpLink : public AInternetLink
{
public:
    BYTE LinkState GCC_PACK(4);
    FIpAddr RemoteAddr GCC_PACK(4);
    class UClass* AcceptClass;
    TArrayNoInit<BYTE> SendFIFO;
    FStringNoInit RecvBuf;
    DECLARE_FUNCTION(execReadBinary);
    DECLARE_FUNCTION(execReadText);
    DECLARE_FUNCTION(execSendBinary);
    DECLARE_FUNCTION(execSendText);
    DECLARE_FUNCTION(execIsConnected);
    DECLARE_FUNCTION(execClose);
    DECLARE_FUNCTION(execOpen);
    DECLARE_FUNCTION(execListen);
    DECLARE_FUNCTION(execBindPort);
    void eventReceivedBinary(INT Count, BYTE* B)
    {
        ATcpLink_eventReceivedBinary_Parms Parms;
        Parms.Count=Count;
        appMemcpy(&Parms.B,B,sizeof(Parms.B));
        ProcessEvent(FindFunctionChecked(IPDRV_ReceivedBinary),&Parms);
    }
    void eventReceivedLine(const FString& Line)
    {
        ATcpLink_eventReceivedLine_Parms Parms;
        Parms.Line=Line;
        ProcessEvent(FindFunctionChecked(IPDRV_ReceivedLine),&Parms);
    }
    void eventReceivedText(const FString& Text)
    {
        ATcpLink_eventReceivedText_Parms Parms;
        Parms.Text=Text;
        ProcessEvent(FindFunctionChecked(IPDRV_ReceivedText),&Parms);
    }
    void eventClosed()
    {
        ProcessEvent(FindFunctionChecked(IPDRV_Closed),NULL);
    }
    void eventOpened()
    {
        ProcessEvent(FindFunctionChecked(IPDRV_Opened),NULL);
    }
    void eventAccepted()
    {
        ProcessEvent(FindFunctionChecked(IPDRV_Accepted),NULL);
    }
    DECLARE_CLASS(ATcpLink,AInternetLink,0|CLASS_Transient,IpDrv)
	ATcpLink();
	void PostScriptDestroyed();
	UBOOL Tick( FLOAT DeltaTime, enum ELevelTick TickType );	

	void CheckConnectionAttempt();
	void CheckConnectionQueue();
	void PollConnections();
	UBOOL FlushSendBuffer();
	void ShutdownConnection();
	virtual UBOOL ShouldTickInEntry() { return true; }
};


class IPDRV_API AMasterServerGameStats : public AGameStats
{
public:
    class AMasterServerUplink* Uplink GCC_PACK(4);
    DECLARE_CLASS(AMasterServerGameStats,AGameStats,0|CLASS_Config,IpDrv)
    NO_DEFAULT_CONSTRUCTOR(AMasterServerGameStats)
};

struct IPDRV_API FtMasterServerEntry
{
    FStringNoInit Address GCC_PACK(4);
    INT Port;
};


struct AMasterServerLink_eventGetMasterServer_Parms
{
    FString OutAddress;
    INT OutPort;
};
class IPDRV_API AMasterServerLink : public AInfo
{
public:
    PTRINT LinkPtr GCC_PACK(4);
    INT LANPort;
    INT LANServerPort;
    TArrayNoInit<FtMasterServerEntry> MasterServerList;
    TArrayNoInit<FtMasterServerEntry> ActiveMasterServerList;
    INT LastMSIndex;
    DECLARE_FUNCTION(execPoll);
    void eventGetMasterServer(FString& OutAddress, INT& OutPort)
    {
        AMasterServerLink_eventGetMasterServer_Parms Parms;
        Parms.OutAddress=OutAddress;
        Parms.OutPort=OutPort;
        ProcessEvent(FindFunctionChecked(IPDRV_GetMasterServer),&Parms);
        OutAddress=Parms.OutAddress;
        OutPort=Parms.OutPort;
    }
    DECLARE_CLASS(AMasterServerLink,AInfo,0|CLASS_Transient|CLASS_Config,IpDrv)
	virtual UBOOL Poll( INT WaitTime ) { return 0; }
};

enum EHeartbeatType
{
    HB_QueryInterface       =0,
    HB_GamePort             =1,
    HB_GamespyQueryPort     =2,
    HB_MAX                  =3,
};
enum EMasterToServer
{
    MTS_ClientChallenge     =0,
    MTS_ClientAuthFailed    =1,
    MTS_Shutdown            =2,
    MTS_MatchID             =3,
    MTS_MD5Update           =4,
    MTS_UpdateOption        =5,
    MTS_CheckOption         =6,
    MTS_ClientMD5Fail       =7,
    MTS_ClientBanned        =8,
    MTS_ClientDupKey        =9,
    MTS_UTANBan             =10,
    MTS_MAX                 =11,
};
enum EServerToMaster
{
    STM_ClientResponse      =0,
    STM_GameState           =1,
    STM_Stats               =2,
    STM_ClientDisconnectFailed=3,
    STM_MD5Version          =4,
    STM_CheckOptionReply    =5,
    STM_MAX                 =6,
};
struct IPDRV_API FMD5UpdateData
{
    FString Guid GCC_PACK(4);
    FString MD5;
    INT Revision;
    friend IPDRV_API FArchive& operator<<(FArchive& Ar,FMD5UpdateData& MyMD5UpdateData)
    {
        return Ar << MyMD5UpdateData.Guid << MyMD5UpdateData.MD5 << MyMD5UpdateData.Revision;
    }
};

#define UCONST_MSUPROPNUM 3

struct AMasterServerUplink_eventLogStatLine_Parms
{
    FString StatLine;
    BITFIELD ReturnValue;
};
struct AMasterServerUplink_eventRefresh_Parms
{
};
struct AMasterServerUplink_eventConnectionFailed_Parms
{
    BITFIELD bShouldReconnect;
};
class IPDRV_API AMasterServerUplink : public AMasterServerLink
{
public:
    BITFIELD bInitialStateCached:1 GCC_PACK(4);
    FServerResponseLine ServerState GCC_PACK(4);
    FServerResponseLine FullCachedServerState;
    FServerResponseLine CachedServerState;
    FLOAT CacheRefreshTime;
    INT CachePlayerCount;
    class AMasterServerGameStats* GameStats;
    class AUdpLink* GamespyQueryLink;
    INT MatchID;
    FLOAT ReconnectTime;
    BITFIELD bReconnectPending:1 GCC_PACK(4);
    BITFIELD DoUplink:1;
    BITFIELD UplinkToGamespy:1;
    BITFIELD SendStats:1;
    BITFIELD ServerBehindNAT:1;
    BITFIELD DoLANBroadcast:1;
    BITFIELD bIgnoreUTANBans:1;
    FStringNoInit MSUPropText[3] GCC_PACK(4);
    FStringNoInit MSUPropDesc[3];
    DECLARE_FUNCTION(execLogStatLine);
    DECLARE_FUNCTION(execForceGameStateRefresh);
    DECLARE_FUNCTION(execReconnect);
    BITFIELD eventLogStatLine(const FString& StatLine)
    {
        AMasterServerUplink_eventLogStatLine_Parms Parms;
        Parms.ReturnValue=0;
        Parms.StatLine=StatLine;
        ProcessEvent(FindFunctionChecked(IPDRV_LogStatLine),&Parms);
        return Parms.ReturnValue;
    }
    void eventRefresh()
    {
        ProcessEvent(FindFunctionChecked(IPDRV_Refresh),NULL);
    }
    void eventConnectionFailed(BITFIELD bShouldReconnect)
    {
        AMasterServerUplink_eventConnectionFailed_Parms Parms;
        Parms.bShouldReconnect=((bShouldReconnect) ? FIRST_BITFIELD : 0);
        ProcessEvent(FindFunctionChecked(IPDRV_ConnectionFailed),&Parms);
    }
    DECLARE_CLASS(AMasterServerUplink,AMasterServerLink,0|CLASS_Transient|CLASS_Config,IpDrv)
	// AActor interface
	void Destroy();
	void PostScriptDestroyed();
	// AServerQueryLink interface
	UBOOL Poll( INT WaitTime );
};

enum EPingCause
{
    PC_Unknown              =0,
    PC_Clicked              =1,
    PC_AutoPing             =2,
    PC_LANBroadcast         =3,
    PC_MAX                  =4,
};
enum EQueryInterfaceCommand
{
    QI_Ping                 =0,
    QI_Rules                =1,
    QI_Players              =2,
    QI_RulesAndPlayers      =3,
    QI_SmallPing            =4,
    QI_MAX                  =5,
};

struct AServerQueryClient_eventOnPingTimeout_Parms
{
    INT ListID;
    BYTE PingCause;
};
struct AServerQueryClient_eventOnReceivedPingInfo_Parms
{
    INT ListID;
    BYTE PingCause;
    FServerResponseLine S;
};
class IPDRV_API AServerQueryClient : public AMasterServerLink
{
public:
    BITFIELD bLANQuery:1 GCC_PACK(4);
    FScriptDelegate __OnReceivedPingInfo__Delegate GCC_PACK(4);
    FScriptDelegate __OnPingTimeout__Delegate;
    DECLARE_FUNCTION(execNetworkError);
    DECLARE_FUNCTION(execCancelPings);
    DECLARE_FUNCTION(execPingServer);
    void delegateOnPingTimeout(INT ListID, BYTE PingCause)
    {
        AServerQueryClient_eventOnPingTimeout_Parms Parms;
        Parms.ListID=ListID;
        Parms.PingCause=PingCause;
        ProcessDelegate(IPDRV_OnPingTimeout,&__OnPingTimeout__Delegate,&Parms);
    }
    void delegateOnReceivedPingInfo(INT ListID, BYTE PingCause, FServerResponseLine S)
    {
        AServerQueryClient_eventOnReceivedPingInfo_Parms Parms;
        Parms.ListID=ListID;
        Parms.PingCause=PingCause;
        Parms.S=S;
        ProcessDelegate(IPDRV_OnReceivedPingInfo,&__OnReceivedPingInfo__Delegate,&Parms);
    }
    DECLARE_CLASS(AServerQueryClient,AMasterServerLink,0|CLASS_Transient|CLASS_Config,IpDrv)
	// AActor interface
	void Destroy();
	void PostScriptDestroyed();
	// MasterServerLink interface
	UBOOL Poll( INT WaitTime );
	// ServerQueryClient interface
	void Init();
};

enum EMOTDResponse
{
    MR_MOTD                 =0,
    MR_MandatoryUpgrade     =1,
    MR_OptionalUpgrade      =2,
    MR_NewServer            =3,
    MR_IniSetting           =4,
    MR_Command              =5,
    MR_MAX                  =6,
};
enum EResponseInfo
{
    RI_AuthenticationFailed =0,
    RI_ConnectionFailed     =1,
    RI_ConnectionTimeout    =2,
    RI_Success              =3,
    RI_MustUpgrade          =4,
    RI_DevClient            =5,
    RI_BadClient            =6,
    RI_BannedClient         =7,
    RI_UTANBan              =8,
    RI_MAX                  =9,
};
enum EQueryType
{
    QT_Equals               =0,
    QT_NotEquals            =1,
    QT_LessThan             =2,
    QT_LessThanEquals       =3,
    QT_GreaterThan          =4,
    QT_GreaterThanEquals    =5,
    QT_Disabled             =6,
    QT_MAX                  =7,
};
enum EClientToMaster
{
    CTM_Query               =0,
    CTM_GetMOTD             =1,
    CTM_QueryUpgrade        =2,
    CTM_GetModMOTD          =3,
    CTM_GetOwnageList       =4,
    CTM_MAX                 =5,
};
struct IPDRV_API FQueryData
{
    FString Key GCC_PACK(4);
    FString Value;
    BYTE QueryType;
    friend IPDRV_API FArchive& operator<<(FArchive& Ar,FQueryData& MyQueryData)
    {
        return Ar << MyQueryData.Key << MyQueryData.Value << MyQueryData.QueryType;
    }
};


struct AMasterServerClient_eventOnReceivedOwnageItem_Parms
{
    INT Level;
    FString ItemName;
    FString ItemDesc;
    FString ItemURL;
};
struct AMasterServerClient_eventOnReceivedModMOTDData_Parms
{
    FString Value;
};
struct AMasterServerClient_eventOnReceivedMOTDData_Parms
{
    BYTE Command;
    FString Value;
};
struct AMasterServerClient_eventOnReceivedServer_Parms
{
    FServerResponseLine S;
};
struct AMasterServerClient_eventOnQueryFinished_Parms
{
    BYTE ResponseInfo;
    INT Info;
};
class IPDRV_API AMasterServerClient : public AServerQueryClient
{
public:
    PTRINT MSLinkPtr GCC_PACK(4);
    INT OwnageLevel;
    INT ModRevLevel;
    TArrayNoInit<FQueryData> Query;
    INT ResultCount;
    FStringNoInit OptionalResult;
    FScriptDelegate __OnQueryFinished__Delegate;
    FScriptDelegate __OnReceivedServer__Delegate;
    FScriptDelegate __OnReceivedMOTDData__Delegate;
    FScriptDelegate __OnReceivedModMOTDData__Delegate;
    FScriptDelegate __OnReceivedOwnageItem__Delegate;
    DECLARE_FUNCTION(execLaunchAutoUpdate);
    DECLARE_FUNCTION(execStop);
    DECLARE_FUNCTION(execStartQuery);
    void delegateOnReceivedOwnageItem(INT Level, const FString& ItemName, const FString& ItemDesc, const FString& ItemURL)
    {
        AMasterServerClient_eventOnReceivedOwnageItem_Parms Parms;
        Parms.Level=Level;
        Parms.ItemName=ItemName;
        Parms.ItemDesc=ItemDesc;
        Parms.ItemURL=ItemURL;
        ProcessDelegate(IPDRV_OnReceivedOwnageItem,&__OnReceivedOwnageItem__Delegate,&Parms);
    }
    void delegateOnReceivedModMOTDData(const FString& Value)
    {
        AMasterServerClient_eventOnReceivedModMOTDData_Parms Parms;
        Parms.Value=Value;
        ProcessDelegate(IPDRV_OnReceivedModMOTDData,&__OnReceivedModMOTDData__Delegate,&Parms);
    }
    void delegateOnReceivedMOTDData(BYTE Command, const FString& Value)
    {
        AMasterServerClient_eventOnReceivedMOTDData_Parms Parms;
        Parms.Command=Command;
        Parms.Value=Value;
        ProcessDelegate(IPDRV_OnReceivedMOTDData,&__OnReceivedMOTDData__Delegate,&Parms);
    }
    void delegateOnReceivedServer(FServerResponseLine S)
    {
        AMasterServerClient_eventOnReceivedServer_Parms Parms;
        Parms.S=S;
        ProcessDelegate(IPDRV_OnReceivedServer,&__OnReceivedServer__Delegate,&Parms);
    }
    void delegateOnQueryFinished(BYTE ResponseInfo, INT Info)
    {
        AMasterServerClient_eventOnQueryFinished_Parms Parms;
        Parms.ResponseInfo=ResponseInfo;
        Parms.Info=Info;
        ProcessDelegate(IPDRV_OnQueryFinished,&__OnQueryFinished__Delegate,&Parms);
    }
    DECLARE_CLASS(AMasterServerClient,AServerQueryClient,0|CLASS_Transient|CLASS_Config,IpDrv)
	// AActor interface
	void Destroy();
	void PostScriptDestroyed();
	// MasterServerLink interface
	UBOOL Poll( INT WaitTime );
	// ServerQueryClient interface
	void Init();

};

#endif

AUTOGENERATE_FUNCTION(AUdpLink,-1,execReadBinary);
AUTOGENERATE_FUNCTION(AUdpLink,-1,execReadText);
AUTOGENERATE_FUNCTION(AUdpLink,-1,execSendBinary);
AUTOGENERATE_FUNCTION(AUdpLink,-1,execSendText);
AUTOGENERATE_FUNCTION(AUdpLink,-1,execBindPort);
AUTOGENERATE_FUNCTION(AInternetLink,-1,execGetLocalIP);
AUTOGENERATE_FUNCTION(AInternetLink,-1,execGameSpyGameName);
AUTOGENERATE_FUNCTION(AInternetLink,-1,execGameSpyValidate);
AUTOGENERATE_FUNCTION(AInternetLink,-1,execStringToIpAddr);
AUTOGENERATE_FUNCTION(AInternetLink,-1,execIpAddrToString);
AUTOGENERATE_FUNCTION(AInternetLink,-1,execGetLastError);
AUTOGENERATE_FUNCTION(AInternetLink,-1,execResolve);
AUTOGENERATE_FUNCTION(AInternetLink,-1,execParseURL);
AUTOGENERATE_FUNCTION(AInternetLink,-1,execIsDataPending);
AUTOGENERATE_FUNCTION(ATcpLink,-1,execReadBinary);
AUTOGENERATE_FUNCTION(ATcpLink,-1,execReadText);
AUTOGENERATE_FUNCTION(ATcpLink,-1,execSendBinary);
AUTOGENERATE_FUNCTION(ATcpLink,-1,execSendText);
AUTOGENERATE_FUNCTION(ATcpLink,-1,execIsConnected);
AUTOGENERATE_FUNCTION(ATcpLink,-1,execClose);
AUTOGENERATE_FUNCTION(ATcpLink,-1,execOpen);
AUTOGENERATE_FUNCTION(ATcpLink,-1,execListen);
AUTOGENERATE_FUNCTION(ATcpLink,-1,execBindPort);
AUTOGENERATE_FUNCTION(AMasterServerUplink,-1,execLogStatLine);
AUTOGENERATE_FUNCTION(AMasterServerUplink,-1,execForceGameStateRefresh);
AUTOGENERATE_FUNCTION(AMasterServerUplink,-1,execReconnect);
AUTOGENERATE_FUNCTION(AMasterServerLink,-1,execPoll);
AUTOGENERATE_FUNCTION(AMasterServerClient,-1,execLaunchAutoUpdate);
AUTOGENERATE_FUNCTION(AMasterServerClient,-1,execStop);
AUTOGENERATE_FUNCTION(AMasterServerClient,-1,execStartQuery);
AUTOGENERATE_FUNCTION(AServerQueryClient,-1,execNetworkError);
AUTOGENERATE_FUNCTION(AServerQueryClient,-1,execCancelPings);
AUTOGENERATE_FUNCTION(AServerQueryClient,-1,execPingServer);

#ifndef NAMES_ONLY
#undef AUTOGENERATE_NAME
#undef AUTOGENERATE_FUNCTION
#endif

#if SUPPORTS_PRAGMA_PACK
#pragma pack (pop)
#endif


#ifdef NATIVE_DEFS_ONLY
DECLARE_NATIVE_TYPE(IpDrv,AUdpLink);
DECLARE_NATIVE_TYPE(IpDrv,AInternetLink);
DECLARE_NATIVE_TYPE(IpDrv,ATcpLink);
DECLARE_NATIVE_TYPE(IpDrv,AMasterServerGameStats);
DECLARE_NATIVE_TYPE(IpDrv,AMasterServerUplink);
DECLARE_NATIVE_TYPE(IpDrv,AMasterServerLink);
DECLARE_NATIVE_TYPE(IpDrv,AMasterServerClient);
DECLARE_NATIVE_TYPE(IpDrv,AServerQueryClient);

#define AUTO_INITIALIZE_REGISTRANTS_IPDRV \
	AUdpLink::StaticClass(); \
	GNativeLookupFuncs[Lookup++] = &FindIpDrvAUdpLinkNative; \
	AInternetLink::StaticClass(); \
	GNativeLookupFuncs[Lookup++] = &FindIpDrvAInternetLinkNative; \
	UDecompressCommandlet::StaticClass(); \
	UCompressCommandlet::StaticClass(); \
	UTcpNetDriver::StaticClass(); \
	UTcpipConnection::StaticClass(); \
	ATcpLink::StaticClass(); \
	GNativeLookupFuncs[Lookup++] = &FindIpDrvATcpLinkNative; \
	AMasterServerGameStats::StaticClass(); \
	AMasterServerUplink::StaticClass(); \
	GNativeLookupFuncs[Lookup++] = &FindIpDrvAMasterServerUplinkNative; \
	AMasterServerLink::StaticClass(); \
	GNativeLookupFuncs[Lookup++] = &FindIpDrvAMasterServerLinkNative; \
	AMasterServerClient::StaticClass(); \
	GNativeLookupFuncs[Lookup++] = &FindIpDrvAMasterServerClientNative; \
	AServerQueryClient::StaticClass(); \
	GNativeLookupFuncs[Lookup++] = &FindIpDrvAServerQueryClientNative; \
	UHTTPDownload::StaticClass(); \

#endif // NATIVE_DEFS_ONLY

#ifdef NATIVES_ONLY
NATIVE_INFO(AUdpLink) GIpDrvAUdpLinkNatives[] = 
{ 
	MAP_NATIVE(AUdpLink,execReadBinary)
	MAP_NATIVE(AUdpLink,execReadText)
	MAP_NATIVE(AUdpLink,execSendBinary)
	MAP_NATIVE(AUdpLink,execSendText)
	MAP_NATIVE(AUdpLink,execBindPort)
	{NULL,NULL}
};
IMPLEMENT_NATIVE_HANDLER(IpDrv,AUdpLink);

NATIVE_INFO(AInternetLink) GIpDrvAInternetLinkNatives[] = 
{ 
	MAP_NATIVE(AInternetLink,execGetLocalIP)
	MAP_NATIVE(AInternetLink,execGameSpyGameName)
	MAP_NATIVE(AInternetLink,execGameSpyValidate)
	MAP_NATIVE(AInternetLink,execStringToIpAddr)
	MAP_NATIVE(AInternetLink,execIpAddrToString)
	MAP_NATIVE(AInternetLink,execGetLastError)
	MAP_NATIVE(AInternetLink,execResolve)
	MAP_NATIVE(AInternetLink,execParseURL)
	MAP_NATIVE(AInternetLink,execIsDataPending)
	{NULL,NULL}
};
IMPLEMENT_NATIVE_HANDLER(IpDrv,AInternetLink);

NATIVE_INFO(ATcpLink) GIpDrvATcpLinkNatives[] = 
{ 
	MAP_NATIVE(ATcpLink,execReadBinary)
	MAP_NATIVE(ATcpLink,execReadText)
	MAP_NATIVE(ATcpLink,execSendBinary)
	MAP_NATIVE(ATcpLink,execSendText)
	MAP_NATIVE(ATcpLink,execIsConnected)
	MAP_NATIVE(ATcpLink,execClose)
	MAP_NATIVE(ATcpLink,execOpen)
	MAP_NATIVE(ATcpLink,execListen)
	MAP_NATIVE(ATcpLink,execBindPort)
	{NULL,NULL}
};
IMPLEMENT_NATIVE_HANDLER(IpDrv,ATcpLink);

NATIVE_INFO(AMasterServerUplink) GIpDrvAMasterServerUplinkNatives[] = 
{ 
	MAP_NATIVE(AMasterServerUplink,execLogStatLine)
	MAP_NATIVE(AMasterServerUplink,execForceGameStateRefresh)
	MAP_NATIVE(AMasterServerUplink,execReconnect)
	{NULL,NULL}
};
IMPLEMENT_NATIVE_HANDLER(IpDrv,AMasterServerUplink);

NATIVE_INFO(AMasterServerLink) GIpDrvAMasterServerLinkNatives[] = 
{ 
	MAP_NATIVE(AMasterServerLink,execPoll)
	{NULL,NULL}
};
IMPLEMENT_NATIVE_HANDLER(IpDrv,AMasterServerLink);

NATIVE_INFO(AMasterServerClient) GIpDrvAMasterServerClientNatives[] = 
{ 
	MAP_NATIVE(AMasterServerClient,execLaunchAutoUpdate)
	MAP_NATIVE(AMasterServerClient,execStop)
	MAP_NATIVE(AMasterServerClient,execStartQuery)
	{NULL,NULL}
};
IMPLEMENT_NATIVE_HANDLER(IpDrv,AMasterServerClient);

NATIVE_INFO(AServerQueryClient) GIpDrvAServerQueryClientNatives[] = 
{ 
	MAP_NATIVE(AServerQueryClient,execNetworkError)
	MAP_NATIVE(AServerQueryClient,execCancelPings)
	MAP_NATIVE(AServerQueryClient,execPingServer)
	{NULL,NULL}
};
IMPLEMENT_NATIVE_HANDLER(IpDrv,AServerQueryClient);

#endif // NATIVES_ONLY

#ifdef VERIFY_CLASS_SIZES
VERIFY_CLASS_SIZE_NODIE(AUdpLink)
VERIFY_CLASS_SIZE_NODIE(AInternetLink)
VERIFY_CLASS_SIZE_NODIE(ATcpLink)
VERIFY_CLASS_SIZE_NODIE(AMasterServerGameStats)
VERIFY_CLASS_SIZE_NODIE(AMasterServerUplink)
VERIFY_CLASS_SIZE_NODIE(AMasterServerLink)
VERIFY_CLASS_SIZE_NODIE(AMasterServerClient)
VERIFY_CLASS_SIZE_NODIE(AServerQueryClient)
#endif // VERIFY_CLASS_SIZES
