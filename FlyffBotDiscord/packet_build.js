String.prototype.replaceAt=function(index, replacement) {
    return this.substr(0, index) + replacement+ this.substr(index + replacement.length);
}

class PacketType
{
    constructor() {
        this.SEND_BUFFER = 1
        this.AUTHENTICATE = 2
        this.PUT_CHAT_MESSAGE = 3
        this.PUT_COMMAND = 4

    }
};

class PacketAuthenticate
{
    constructor() {
        this.SERVER_FLYFF = 0
        this.BOT_DISCORD = 1
        this.CLIENT_EXTERN = 2
    }
}

class PacketBuilder
{
    constructor() {
        this.offset = 0
        this.packet = 0
        this.length = 0
        this.type = 0
    }

    readInt() {
        let buffer = new String()
        this.offset += 8
        let index = this.offset - 1
        for (let i = 0; i < 4; ++i) {
            let r = this.packet[index]
            let l = this.packet[index -1]
            buffer += l
            buffer += r
            index -= 2
        }
        let value = parseInt(buffer, 16)
        return value
    }

    readString() {
        let length = this.readInt()
        let buffer = new String()
        let start = this.offset
        this.offset += (length * 2)

        for (let i = 0; i < length; ++i) {
            let hex = new String()
            hex += this.packet[start]
            hex += this.packet[start + 1]

            let val = parseInt(hex, 16)
            let char = String.fromCharCode(val)

            buffer += char
            start += 2
        }
        return buffer
    }

    writeInt(number) {
        let nbStr = number.toString(16)
        let numberHexa = new String()
        let numberZero = 8

        numberZero -= nbStr.length
        if (numberZero % 2 != 0) {
            numberZero -= 1
            numberHexa += "0"
        }

        numberHexa += nbStr
        for (let i = 0; i < numberZero; ++i) {
            numberHexa += "0"
        }

        this.packet += numberHexa
        this.length += 4

    }

    writeString(str) {
        let len = str.length
        this.writeInt(len)

        let hexStr =  new String()
        for (let i = 0; i < str.length; ++i) {
            let char = str[i]
            let hex = char.charCodeAt(0).toString(16)
            hexStr += hex
        }
        this.packet += hexStr
        this.length += str.length
    }

    writeSizePacket() {
        let nbStr = this.length.toString(16)
        let numberHexa = new String()
        let newP = new String()
        let numberZero = 8

        numberZero -= nbStr.length
        if (numberZero % 2 != 0) {
            numberZero -= 1
            numberHexa += "0"
        }

        numberHexa += nbStr
        for (let i = 0; i < numberZero; ++i) {
            numberHexa += "0"
        }

        newP += numberHexa
        newP += this.packet
        this.packet = newP
        this.length += 4
    }

    getData() {
        let packetHex = new String()
        let l = 0
        let r = 0
        let strHex = 0
        let dec = 0

        this.writeSizePacket()

        let buffer = new Buffer.alloc(this.length)

        let y = 0
        for (let i = 0; i < this.packet.length; ++i) {
            l = this.packet[i]
            r = this.packet[i + 1]
            strHex = l + r
            dec = parseInt(strHex, 16)
            buffer[y] = dec
            i += 1
            y += 1
        }
        this.packet = new String()
        this.length = 0
        return buffer
    }

    initPacketType(packetType) {
        this.packet = new String()
        this.length = 0
        this.offset = 0
        this.writeInt(packetType)
    }

    parse(data) {
        this.packet = data.toString('hex')
        this.length = this.readInt();
        this.type = this.readInt();
    }
}


module.exports = {
    PacketBuilder,
    PacketType,
    PacketAuthenticate
}
