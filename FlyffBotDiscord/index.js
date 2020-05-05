// Package NodeJs
const package_discord = require('discord.js')
const net = require('net')

// Import files
const config = require('./config.json')
const packet_builder = require('./packet_build.js')
const message_handler = require('./message_handler.js')
const cmduser = require('./command_user.js')

// Initialize
const discord = new package_discord.Client()
const packetType = new packet_builder.PacketType()
const packetAuth = new packet_builder.PacketAuthenticate()
const command_user = new cmduser.CommandUser();
const server  = new net.Socket();
const msgH = new message_handler.MessageHandler()
msgH.builder = new packet_builder.PacketBuilder()
msgH.packetType = new packet_builder.PacketType()
msgH.config = config


server.on('connect', function () {
    console.log('Client: connection established with server');
    console.log('---------client details -----------------');
    let address = server.address();
    let port = address.port;
    let family = address.family;
    let ipaddr = address.address;
    console.log(`Client is listening at port ${port}`);
    console.log(`Client ip : ${ipaddr}`);
    console.log(`Client is IP4/IP6 : ${family}`);

    // authentification
    msgH.builder.initPacketType(packetType.AUTHENTICATE)
    msgH.builder.writeInt(1234)
    msgH.builder.writeInt(packetAuth.BOT_DISCORD)
    msgH.builder.writeString("admin")
    let buffer = msgH.builder.getData()
    server.write(buffer)
});

server.on('data', (data) => {
    // reception des packets
    msgH.builder.parse(data)
    msgH.onMsg()
})

discord.on('ready', () => {
    channels = discord.channels
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

discord.on('message', message => {
    if (message.author.id != config.id_dev_bot) { // Si ce n'est pas le bot discord
        if (message.channel.id == msgH.channel_shout) { // Channel Shout
            // On envoie le message a APIServer
            msgH.builder.initPacketType(packetType.PUT_CHAT_MESSAGE)
            msgH.builder.writeInt(1)
            msgH.builder.writeString(message.author.username)
            msgH.builder.writeString(message.toString())
            let buffer = msgH.builder.getData()
            server.write(buffer)
        } else if (message.channel.id == msgH.channel_command) { // Channel Command
            if (command_user.isCommand(message.toString()) === true) { // Command valid
                // On envoie la commande a APIServer
                msgH.builder.initPacketType(packetType.PUT_COMMAND)
                msgH.builder.writeString(message.author.username)
                msgH.builder.writeString(message.toString())
                let buffer = msgH.builder.getData()
                server.write(buffer)
                // modifie le message sur discord
                message.delete()
                message.channel.send(`Commande effectuée par ${message.author.username} : "${message.toString()}"`)
            }
        }
    }
});


// Run application
discord.login(config.token);
server.connect({port:config.port});
  
