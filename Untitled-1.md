me@ans:~/Desktop/ft_irc$ nc localhost 6666
:42_IRC NOTICE AUTH :*** Looking up your hostname . . .
:42_IRC NOTICE AUTH :*** Found your hostname
nick

>>>>>>>>>>>>>>>>>

me@ans:~/Desktop/ft_irc$ ./ircserv 6666 123
building socket ...  created
making socket reusable ...  Finished.
making socket non bloking ...  Finished.
binding socket . . .  Finished.
Making socket lisening. . .  Finished.
Server is running . . . 
waiting for accept new connections
New User Added !
 Receiving msg . . . recv  [nick]
=====> [NICK]<======
Segmentation fault (core dumped)






me@ans:~/Desktop/ft_irc$ nc irc.libera.chat 6667
:tantalum.libera.chat NOTICE * :*** Checking Ident
:tantalum.libera.chat NOTICE * :*** Looking up your hostname...
:tantalum.libera.chat NOTICE * :*** Couldn't look up your hostname
:tantalum.libera.chat NOTICE * :*** No Ident response
PASS 1234
NICK ans
USER ans 0 * ans
:tantalum.libera.chat 001 ans :Welcome to the Libera.Chat Internet Relay Chat Network ans
:tantalum.libera.chat 002 ans :Your host is tantalum.libera.chat[93.158.237.2/6667], running version solanum-1.0-dev
:tantalum.libera.chat 003 ans :This server was created Sat Nov 18 2023 at 03:37:54 UTC
:tantalum.libera.chat 004 ans tantalum.libera.chat solanum-1.0-dev DGIMQRSZaghilopsuwz CFILMPQRSTbcefgijklmnopqrstuvz bkloveqjfI
:tantalum.libera.chat 005 ans ACCOUNTEXTBAN=a WHOX MONITOR=100 SAFELIST ELIST=CMNTU ETRACE FNC CALLERID=g KNOCK CHANTYPES=# EXCEPTS INVEX :are supported by this server
:tantalum.libera.chat 005 ans CHANMODES=eIbq,k,flj,CFLMPQRSTcgimnprstuz CHANLIMIT=#:250 PREFIX=(ov)@+ MAXLIST=bqeI:100 MODES=4 NETWORK=Libera.Chat STATUSMSG=@+ CASEMAPPING=rfc1459 NICKLEN=16 MAXNICKLEN=16 CHANNELLEN=50 TOPICLEN=390 :are supported by this server
:tantalum.libera.chat 005 ans DEAF=D TARGMAX=NAMES:1,LIST:1,KICK:1,WHOIS:1,PRIVMSG:4,NOTICE:4,ACCEPT:,MONITOR: EXTBAN=$,agjrxz :are supported by this server
:tantalum.libera.chat 251 ans :There are 73 users and 35615 invisible on 27 servers
:tantalum.libera.chat 252 ans 42 :IRC Operators online
:tantalum.libera.chat 253 ans 112 :unknown connection(s)
:tantalum.libera.chat 254 ans 22930 :channels formed
:tantalum.libera.chat 255 ans :I have 2408 clients and 1 servers
:tantalum.libera.chat 265 ans 2408 2963 :Current local users 2408, max 2963
:tantalum.libera.chat 266 ans 35688 37286 :Current global users 35688, max 37286
:tantalum.libera.chat 250 ans :Highest connection count: 2964 (2963 clients) (478896 connections received)
:tantalum.libera.chat 375 ans :- tantalum.libera.chat Message of the Day - 
:tantalum.libera.chat 372 ans :- This server provided by Hyperfilter (https://hyperfilter.com)
:tantalum.libera.chat 372 ans :-  
:tantalum.libera.chat 372 ans :-  
:tantalum.libera.chat 372 ans :- Welcome to Libera Chat, the IRC network for
:tantalum.libera.chat 372 ans :- free & open-source software and peer directed projects.
:tantalum.libera.chat 372 ans :-  
:tantalum.libera.chat 372 ans :- Use of Libera Chat is governed by our network policies.
:tantalum.libera.chat 372 ans :-  
:tantalum.libera.chat 372 ans :- To reduce network abuses we perform open proxy checks
:tantalum.libera.chat 372 ans :- on hosts at connection time.
:tantalum.libera.chat 372 ans :-  
:tantalum.libera.chat 372 ans :- Please visit us in #libera for questions and support.
:tantalum.libera.chat 372 ans :-  
:tantalum.libera.chat 372 ans :- Website and documentation:  https://libera.chat
:tantalum.libera.chat 372 ans :- Webchat:                    https://web.libera.chat
:tantalum.libera.chat 372 ans :- Network policies:           https://libera.chat/policies
:tantalum.libera.chat 372 ans :- Email:                      support@libera.chat
:tantalum.libera.chat 376 ans :End of /MOTD command.
:ans MODE ans :+iw
:NickServ!NickServ@services.libera.chat NOTICE ans :ans is not a registered nickname.
join ff
:tantalum.libera.chat 403 ans ff :No such channel
join #ansc 
:ans!~ans@197.146.74.114 JOIN #ansc
:tantalum.libera.chat MODE #ansc +Cnst
:tantalum.libera.chat 353 ans @ #ansc :@ans
:tantalum.libera.chat 366 ans #ansc :End of /NAMES list.
join aa
:tantalum.libera.chat 403 ans aa :No such channel
join aa aa aa
:tantalum.libera.chat 403 ans aa :No such channel
join #aa kk ff
:ans!~ans@197.146.74.114 JOIN #aa
:tantalum.libera.chat MODE #aa +Cnst
:tantalum.libera.chat 353 ans @ #aa :@ans
:tantalum.libera.chat 366 ans #aa :End of /NAMES list.
:services. MODE #aa -o ans   
:tantalum.libera.chat NOTICE #aa :*** Notice -- TS for #aa changed from 1712136822 to 1623864597
:services. MODE #aa +p-C 
:ChanServ!ChanServ@services.libera.chat JOIN #aa
:services. MODE #aa +o ChanServ 