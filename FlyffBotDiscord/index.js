const Discord = require('discord.js')
const client = new Discord.Client()
const config = require('./config.json')
const net = require('net')
const packet_builder = require('./packet_build.js')
const message_handler = require('./message_handler.js')
const cmduser = require('./command_user.js')

const msgH = new message_handler.MessageHandler()
msgH.builder = new packet_builder.PacketBuilder()
msgH.packetType = new packet_builder.PacketType()
msgH.config = config

const packetType = new packet_builder.PacketType()
const packetAuth = new packet_builder.PacketAuthenticate()

const command_user = new cmduser.CommandUser();

const nclient  = new net.Socket();

nclient.connect({
  port:config.port
});

nclient.on('connect', function () {
    console.log('Client: connection established with server');

    console.log('---------client details -----------------');
    var address = nclient.address();
    var port = address.port;
    var family = address.family;
    var ipaddr = address.address;
    console.log('Client is listening at port' + port);
    console.log('Client ip :' + ipaddr);
    console.log('Client is IP4/IP6 : ' + family);

    // authentification
    msgH.builder.initPacketType(packetType.AUTHENTICATE)
    msgH.builder.writeInt(1234)
    msgH.builder.writeInt(packetAuth.BOT_DISCORD)
    msgH.builder.writeString("admin")
    let buffer = msgH.builder.getData()
    nclient.write(buffer)
});

nclient.on('data', (data) => {
    console.log(data)
    msgH.builder.parse(data)
    msgH.onMsg()
})


client.on('ready', () => {
    channels = client.channels
    channels.fetch(config.channels.general)
        .then(channel => {msgH.channel_general = channel})
        .catch(console.error)
    channels.fetch(config.channels.shout)
        .then(channel => {msgH.channel_shout = channel})
        .catch(console.error)
    channels.fetch(config.channels.command)
        .then(channel => {msgH.channel_command = channel})
        .catch(console.error)
});

client.on('message', message => {
    if (message.author.id != config.id_dev_bot) {
        if (message.channel.id == msgH.channel_shout) {
            msgH.builder.initPacketType(packetType.PUT_CHAT_MESSAGE)
            msgH.builder.writeInt(1)
            msgH.builder.writeString(message.author.username)
            msgH.builder.writeString(message.toString())
            let buffer = msgH.builder.getData()
            nclient.write(buffer)
        }
        else if (message.channel.id == msgH.channel_command) {
            console.log(command_user)
            if (command_user.isCommand(message.toString()) === true) {
                msgH.builder.initPacketType(packetType.PUT_COMMAND)
                msgH.builder.writeString(message.author.username)
                msgH.builder.writeString(message.toString())
                let buffer = msgH.builder.getData()
                nclient.write(buffer)    
            }
        }
    }
});

client.login(config.token);
