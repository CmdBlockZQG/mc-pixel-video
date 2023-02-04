const net = require('net')

// const sleep = ms => new Promise(resolve => setTimeout(resolve, ms))

module.exports = class RCON {
    write(type, payload) {
        const id = this.cnt
        this.cnt++
        const len = Buffer.byteLength(payload, 'ascii')
        const buf = Buffer.allocUnsafe(14 + length)

        buf.writeInt32LE(10 + len, 0)
        buf.writeInt32LE(id, 4)
        buf.writeInt32LE(type, 8)
        buf.write(payload, 12, 'ascii')
        buf.writeInt16LE(0, 12 + length)

        this.socket.write(buf)
    }

    connect(host='127.0.0.1', port=25575, passwd='') {
        return new Promise(resolve => {
            this.cnt = 0
            this.socket = net.connect({ host: host, port: port })
                .once('connect', () => {
                    this.write(3, passwd)
                })
                .on('data', resolve)
        })
    }

    send(s) {
        this.write(2, s)
    }
}