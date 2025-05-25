# 📖 Description

The Internet Relay Chat is a 1988 text-based chat protocol designed for instant messaging. 
This project is about creating our own IRC server and connecting it with an IRC client and test it. We learned how socket communication works between server and client.

This project was done by two students : [6jeanne6](https://github.com/6jeanne6) and [Christellaa](https://github.com/Christellaa)

**Main features are:**

- Being able to authenticate, set a nickname, an username, join a channel, send and receive private messages using a client.

- All messages sent from one client in a channel has to be forwarded to every other client in the channel.

- Have `operators` (like administrator having permissions) and regular users.

- Commands for channel operators:
  
  - `KICK` - Eject a client from a channel
  - `INVITE` - Invite a client to a channel
  - `TOPIC` - Change the scope of who can set the channel's topic
  - `MODE` - Change the channel's mode

    - `i`: Set/remove Invite-only channel
    - `t`: Set/remove the restrictions of the TOPIC command to channel operators
    - `k`: Set/remove the channel key (password)
    - `o`: Give/take channel operator privilege
    - `l`: Set/remove the user limit to channel

• File transfer

• Bot

# ⚙️ Installation

1. Clone the git repository

```
git clone https://github.com/Christellaa/irc ft_irc
```

2. Go to ft_irc directory

```
cd ft_irc
```

3. Compile

```
make
```

# 💬 Usage

To launch the server, open a terminal:

```
./ircserv 1111 password
```

and open another terminal:
```
irssi
/connect localhost 1111 password
```

Here are different commands you can test:
```
/nick client1
/msg client2 hello
/join channel
/mode +i
/mode -i
/mode +t
/mode +k password
/mode +o client2
/mode +l 3
/part
/dcc send client2 file_name
/dcc accept client1 file_name
In a channel : !help
```
