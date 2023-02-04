const readline = require('readline')
const fs = require('fs')

const rl = readline.createInterface({
    input: process.stdin
})

const RCON = require('./rcon')
const rcon = new RCON()

rcon.connect('127.0.0.1', 25575, '123456')
    .then(() => {
        console.log('connected')
    })

let interval = 100
let timer = null
let currentFrame = 0
let frameCount = 0

rl.on('line', (s) => {
    if (s.indexOf('init') === 0) {
        frameCount = parseInt(s.split(' ')[1])
        console.log(`初始化 总帧数${frameCount}`)
        rcon.send('/gamerule maxCommandChainLength 2147483647')
        drawFrame(0)
    } else if (s.indexOf('interval') === 0) {
        interval = parseInt(s.split(' ')[1])
        console.log(`帧间隔时间设置为${interval}ms`)
    } else if (s.indexOf('play') === 0) {
        currentFrame = 0;
        timer = setInterval(() => {
            drawFrame(currentFrame)
            currentFrame++
            if (currentFrame === frameCount) {
                clearTimeout(timer)
            }
        }, interval)
    } else if (s.indexOf('pause') === 0) {
        clearTimeout(timer)
    } else if (s.indexOf('resume') === 0) {
        timer = setInterval(() => {
            drawFrame(currentFrame)
            currentFrame++
            if (currentFrame === frameCount) {
                clearTimeout(timer)
            }
        }, interval)
    }
})

function drawFrame(i) {
    const fileRL = readline.createInterface({
        input: fs.createReadStream(`../functions_player/frame${i}.mcfunction`),
        crlfDelay: Infinity
    })
    fileRL.on('line', (cmd) => {
        // console.log(cmd)
        rcon.send('/' + cmd)
    })
}