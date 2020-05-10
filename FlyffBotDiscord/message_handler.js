class   MessageHandler {

    constructor() {
        this.channel_general = 0
        this.channel_shout = 0
        this.channel_command = 0
        this.builder = 0
        this.packetType = 0
        this.config = 0
    }

    onMsg() {
        while (this.builder.offset < this.builder.length) {
            switch(this.builder.type) {
                case this.packetType.SEND_BUFFER : {
                    console.log(`free style packet send`)
                    break;
                }
                case this.packetType.AUTHENTICATE: {
                    console.log(`authenticate`)
                    this.onAuthenticate()
                }break;
                case  this.packetType.PUT_CHAT_MESSAGE: {
                    console.log(`Put Chat Message`)
                    this.onPutGeneral()
                } break;
                case  this.packetType.PUT_COMMAND: {
                    console.log(`Put Chat Command`)
                    this.onPutCommand()
                }
                default : {
                    console.log(`unknow${this.builder.type}`)
                }
            }
            if (this.builder.offset < this.builder.length) {
                let sizePacket = this.readInt();
                this.builder.type = this.readInt();
            }
        }
        this.builder.offset = 0
    }

    onAuthenticate() {
        let key = this.builder.readInt()
        let type = this.builder.readInt()
        let login = this.readString()

        if (key != this.config.auth.key) {
            console.log(`invalid key [${key}]`)
        }
        if (this.config.login.Contains(login) === false) {
            console.log(`invalid login [${login}]`)
        }
    }

    onPutGeneral() {
        let typeChat = this.builder.readInt()
        let playerName = this.builder.readString()
        let message = this.builder.readString()

        if (typeChat == 0)
            this.channel_general.send(`${playerName}: ${message}`)
        else if (typeChat == 1)
        {
            message = message.replace('/s', '')
            message = message.replace('/shout', '')
            this.channel_shout.send(`${playerName}: ${message}`)
        }
    }

    onPutCommand() {
        let playerName = this.builder.readString()
        let command = this.builder.readString()
        this.channel_command.send(`${playerName}: ${command}`)
    }

}

module.exports = {
    MessageHandler
}