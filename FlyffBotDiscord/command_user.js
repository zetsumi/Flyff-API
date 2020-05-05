class CommandUser
{
    constructor() {
        this.cmd = new String()
        this.list_cmd = [
            "!sys "
        ]
    }

    isCommand(command) {
        if (command.includes("!") === false && command.indexOf("!") !== 0) {
            return false
        }
        for (let it of this.list_cmd) {
            if (command.includes(it) === true) {
                return true
            }
        }
        return false
    }
}

module.exports =  {
    CommandUser
}
